#ifndef _INPUTHANDLER_H_
#define _INPUTHANDLER_H_

#include "Dependencies/entt.hpp"
#include "cocos2d.h"

USING_NS_CC;

class InputHandler {
public:
    InputHandler(Node* scene, EventDispatcher* cocosDispatcher, entt::dispatcher& dispatcher): _dispatcher(dispatcher) {
        EventListenerKeyboard* klistener = EventListenerKeyboard::create();
        klistener->onKeyPressed = CC_CALLBACK_2(InputHandler::onKeyPressed, this);
        klistener->onKeyReleased = CC_CALLBACK_2(InputHandler::onKeyReleased, this);

        cocosDispatcher->addEventListenerWithSceneGraphPriority(klistener, scene);
    }

    void onKeyPressed(EventKeyboard::KeyCode key, Event* event) {
        _dispatcher.trigger<KeyPressedEvent>(key);
    }

    void onKeyReleased(EventKeyboard::KeyCode key, Event* event) {
        _dispatcher.trigger<KeyReleasedEvent>(key);
    }

private:
    entt::dispatcher& _dispatcher;
};


#endif // _INPUTHANDLER_H_
