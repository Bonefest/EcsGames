#ifndef __SCENE_H_
#define __SCENE_H_

#include "cocos2d.h"

#include "Dependencies/entt.hpp"
#include "Systems/Systems.h"

#include <memory>

class MainScene: public cocos2d::Scene {
public:
    CREATE_FUNC(MainScene);

    static MainScene* createScene() {
        return MainScene::create();
    }

    bool init() {
        if(!cocos2d::Scene::initWithPhysics()) return false;

        DrawNode* drawer = DrawNode::create();
        addChild(drawer);

        _application = std::make_shared<BaseApplication>();
        _application->addSystem(std::make_shared<DrawingSystem>(drawer));
        _application->addSystem(std::make_shared<MeteorSpawnSystem>(20, 2.0f));
        _application->addSystem(std::make_shared<PhysicsSystem>());

        _application->getRegistry().on_construct<Physics>().connect<&MainScene::physicalComponentConnectionListener>(this);
        _application->getRegistry().on_destroy<Physics>().connect<&MainScene::physicalComponentDisconnectionListener>(this);

        //getPhysicsWorld()->setDebugDrawMask(0xFFFF);

        scheduleUpdate();
        return true;
    }

    void update(float delta) {
        _application->update(delta);
    }

private:

    void physicalComponentConnectionListener(entt::registry& registry, entt::entity entity) {
        Physics& physicsComponent = registry.get<Physics>(entity);
        Node* bodyContainer = Node::create();
        bodyContainer->addComponent(physicsComponent.physicsBody);
        addChild(bodyContainer);
    }

    void physicalComponentDisconnectionListener(entt::registry& registry, entt::entity entity) {
        Physics& physicsComponent = registry.get<Physics>(entity);
        physicsComponent.physicsBody->getOwner()->removeFromParentAndCleanup(true);
    }

    std::shared_ptr<BaseApplication> _application;
};

#endif // __SCENE_H_
