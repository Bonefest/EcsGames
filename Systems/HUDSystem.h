#ifndef HUDSYSTEM_H_INCLUDED
#define HUDSYSTEM_H_INCLUDED

#include "cocos2d.h"

#include "System.h"
#include "../Events/Events.h"

#include <deque>

USING_NS_CC;

class HUDSystem: public ISystem {
public:
    HUDSystem(Vec2 pos, Size size): _pos(pos), _size(size) {
        _renderer = DrawNode::create();

        Director::getInstance()->getRunningScene()->addChild(_renderer);
    }

    ~HUDSystem() {
        _renderer->removeFromParentAndCleanup(true);
    }

    virtual void update(entt::registry& registry, entt::dispatcher& dispatcher, float delta) {
        _renderer->clear();
        _renderer->drawRect(_pos, _pos + Size(_size), Color4F::WHITE);
    }


private:
    Vec2 _pos;
    Size _size;
    DrawNode* _renderer;

};


#endif // HUDSYSTEM_H_INCLUDED
