#ifndef EXAMPLESCENE_H_INCLUDED
#define EXAMPLESCENE_H_INCLUDED

#include "cocos2d.h"
#include "Game.h"

#include <memory>

class ExampleScene: public cocos2d::Scene {
public:
    CREATE_FUNC(ExampleScene);

    static ExampleScene* createScene() {
        return ExampleScene::create();
    }

    bool init() {
        if(!cocos2d::Scene::init()) return false;

        cocos2d::DrawNode* drawer = cocos2d::DrawNode::create();
        addChild(drawer);

        application = std::make_shared<Application>(drawer, _eventDispatcher, this);

        entityx::Entity player = application->entities.create();
        player.assign<Player>(UP);
        player.assign<Body>(cocos2d::Vec2(128, 128), cocos2d::Size(32, 32));
        player.assign<Drawable>(cocos2d::Color4F::WHITE);
        player.assign<SnakePart>();

        scheduleUpdate();
        return true;
    }

    void update(float delta) {
        application->update(delta);
    }

private:
    std::shared_ptr<Application> application;

};

#endif // EXAMPLESCENE_H_INCLUDED
