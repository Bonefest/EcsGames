#ifndef __SCENE_H_
#define __SCENE_H_

#include "cocos2d.h"

#include "Dependencies/entt.hpp"
#include "Systems/Systems.h"

#include <memory>

USING_NS_CC;

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
        _application->addSystem(std::make_shared<BulletCollisionSystem>(_application->getDispatcher()));
        _application->addSystem(std::make_shared<PlayerControllSystem>("deprecated"_hs, _application->getDispatcher()));

        _application->getRegistry().on_construct<Physics>().connect<&MainScene::physicalComponentConnectionListener>(this);
        _application->getRegistry().on_destroy<Physics>().connect<&MainScene::physicalComponentDisconnectionListener>(this);

        _inputHandler = std::make_shared<InputHandler>(this, _eventDispatcher, _application->getDispatcher());
        _collisionHandler = std::make_shared<CollisionHandler>(this, _eventDispatcher, _application->getDispatcher());

        initPlayer();


        //getPhysicsWorld()->setDebugDrawMask(0xFFFF);

        scheduleUpdate();
        return true;
    }

    void update(float delta) {
        _application->update(delta);
    }

private:

    void initPlayer() {
        auto& registry = _application->getRegistry();

        auto player = registry.create();
        registry.assign<entt::tag<entt::hashed_string("player")>>(player);
        registry.assign<Transform>(player, Vec2(200, 200), 1.0f, 0.0f);

        vector<Vec2> verticies;
        verticies.push_back(Vec2( 0.0f, 0.0f));
        verticies.push_back(Vec2(-5.0f, 5.0f));
        verticies.push_back(Vec2( 10.0f,0.0f));
        verticies.push_back(Vec2(-5.0f,-5.0f));

        registry.assign<DrawableShape>(player, verticies, Color4F::BLACK, Color4F::WHITE);

        PhysicsBody* playerBody = cocos2d::PhysicsBody::createPolygon(verticies.data(), verticies.size(), PhysicsMaterial(1.0f, 0.0f, 1.0f), Vec2::ZERO);
        playerBody->setVelocityLimit(100.0f);
        playerBody->setAngularVelocityLimit(0.0f);
        playerBody->setContactTestBitmask(0xFFFFFFFF);

        playerBody->setGravityEnable(false);
        registry.assign<Physics>(player, playerBody);

    }

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
    std::shared_ptr<InputHandler> _inputHandler;
    std::shared_ptr<CollisionHandler> _collisionHandler;
};

#endif // __SCENE_H_
