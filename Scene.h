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
        if(!cocos2d::Scene::init()) return false;

        DrawNode* drawer = DrawNode::create();
        addChild(drawer);

        for(int i = 0;i < 100; ++i) {
            auto entity = registry.create();
            //registry.assign<Transform>(entity, Vec2(i, 0),
            registry.assign<Transform>(entity, Vec2(rand() % 16, rand() % 16),
                                               Size(32, 32));
            registry.assign<Cell>(entity, ALIVE);
            registry.assign<Renderable>(entity, Color4F::GREEN);


        }


        lifeIteratingSystem = std::make_shared<LifeIteratingSystem>(3, 16);
        renderingSystem = std::make_shared<RenderingSystem>(drawer);

        scheduleUpdate();
        return true;
    }

    void update(float delta) {
        renderingSystem->update(delta, registry);
        lifeIteratingSystem->update(delta, registry);
    }

private:
    entt::registry registry;

    std::shared_ptr<LifeIteratingSystem> lifeIteratingSystem;
    std::shared_ptr<RenderingSystem> renderingSystem;
};

#endif // __SCENE_H_
