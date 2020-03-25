#ifndef EVENTS_H_INCLUDED
#define EVENTS_H_INCLUDED


#include <string>
using std::string;

#include "cocos2d.h"
#include "../common.h"

USING_NS_CC;

struct EnableSystemEvent {
    EnableSystemEvent(uint32_t systemTag, bool systemEnabled): tag(systemTag), enabled(systemEnabled) { }

    uint32_t tag;
    bool enabled;
};

//Можно заменить одной структурой, но я решил оставить так для возможности разделения событий на 2 метода
struct KeyPressedEvent {
    explicit KeyPressedEvent(cocos2d::EventKeyboard::KeyCode keyCode): key(keyCode) { }

    cocos2d::EventKeyboard::KeyCode key;
};

struct KeyReleasedEvent {
    explicit KeyReleasedEvent(cocos2d::EventKeyboard::KeyCode keyCode): key(keyCode) { }

    cocos2d::EventKeyboard::KeyCode key;
};

struct MessageEvent {
    explicit MessageEvent(const string& actionDescription, Color3B color): message(actionDescription),
                                                                           textColor(color) { }
    uint32_t channel;
    string  message;
    Color3B textColor;
};

struct HintMessageEvent {
    explicit HintMessageEvent(string hintMessage, Color3B hintColor, float hintLifetime): hint(hintMessage),
                                                                                          textColor(hintColor),
                                                                                          lifetime(hintLifetime) { }


    string hint;
    Color3B textColor;
    float lifetime;
};

struct UnprocessedKeyActionEvent {
    explicit UnprocessedKeyActionEvent(EventKeyboard::KeyCode pressedKey, KeyType type): key(pressedKey), keyType(type) { }

    EventKeyboard::KeyCode key;
    KeyType keyType;
};

#endif // EVENTS_H_INCLUDED
