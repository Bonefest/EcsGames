#ifndef EVENTS_H_INCLUDED
#define EVENTS_H_INCLUDED


#include <string>
#include <memory>

#include "cocos2d.h"
#include "../common.h"

using namespace cocos2d;
using std::string;
using std::shared_ptr;

//
//struct EnableSystemEvent {
//    EnableSystemEvent(uint32_t systemTag, bool systemEnabled): tag(systemTag), enabled(systemEnabled) { }
//
//    uint32_t tag;
//    bool enabled;
//};

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
    explicit MessageEvent(Text messageText): message(messageText) { }
    uint32_t channel;
    Text message;
};

struct HintMessageEvent {
    explicit HintMessageEvent(Text hintText, float hintLifetime): hint(hintText),
                                                                  lifetime(hintLifetime) { }

    Text hint;
    float lifetime;
};

struct UnprocessedKeyActionEvent {
    explicit UnprocessedKeyActionEvent(EventKeyboard::KeyCode pressedKey, KeyType type): key(pressedKey), keyType(type) { }

    EventKeyboard::KeyCode key;
    KeyType keyType;
};

#include "../Command.h"

class Command;

struct StateControllCommandEvent {
    StateControllCommandEvent(shared_ptr<Command> newCommand): command(newCommand) { }

    shared_ptr<Command> command;
};

#endif // EVENTS_H_INCLUDED
