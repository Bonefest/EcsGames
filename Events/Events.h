#ifndef EVENTS_H_INCLUDED
#define EVENTS_H_INCLUDED

#include "cocos2d.h"

//Можно заменить одной структурой, но я решил оставить так для возможности разделения событий на 2 метода
struct KeyPressedEvent {
    explicit KeyPressedEvent(cocos2d::EventKeyboard::KeyCode keyCode): key(keyCode) { }

    cocos2d::EventKeyboard::KeyCode key;
};

struct KeyReleasedEvent {
    explicit KeyReleasedEvent(cocos2d::EventKeyboard::KeyCode keyCode): key(keyCode) { }

    cocos2d::EventKeyboard::KeyCode key;
};



#endif // EVENTS_H_INCLUDED
