#ifndef HUDSYSTEM_H_INCLUDED
#define HUDSYSTEM_H_INCLUDED

#include "cocos2d.h"

#include "System.h"
#include "../Events/Events.h"

#include <deque>

USING_NS_CC;

class HUDSystem: public ISystem {
public:
    HUDSystem(Node* scene, Node* container): _scene(scene), _container(container) {
        _renderer = DrawNode::create();

        scene->addChild(_renderer);
    }

    virtual void update(entt::registry& registry, entt::dispatcher& dispatcher, float delta) {
        _renderer->clear();
        _renderer->drawRect(_container->getPosition(), _container->getPosition() + Vec2(_container->getContentSize()), Color4F::WHITE);
    }

    void onGameActionEvent(const GameActionEvent& event) {
        //Messaging system
    }

private:
    Node* _scene;
    Node* _container;

    DrawNode* _renderer;

};


#endif // HUDSYSTEM_H_INCLUDED
