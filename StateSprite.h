#ifndef STATESPRITE_H_INCLUDED
#define STATESPRITE_H_INCLUDED

#include "cocos2d.h"

#include <map>
#include <string>

using std::map;
using std::string;

USING_NS_CC;

enum State {
    NORMAL, CLOSED, RAGE
};

class StateSprite: public Sprite {
public:
    CREATE_FUNC(StateSprite);
    static StateSprite* createSprite(const string& normalFrame) {
        StateSprite* sprite = StateSprite::create();
        sprite->initWithSpriteFrameName(normalFrame);
        sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

        sprite->addState(NORMAL, normalFrame);
        sprite->setState(NORMAL);

        return sprite;
    }

    void addState(State state, const string& frameName) {
        _states[state] = frameName;
    }

    void setState(State state) {
        auto stateIter = _states.find(state);
        if(stateIter != _states.end()) {
            setSpriteFrame(stateIter->second);
            _currentState = state;
        }
    }

private:
    map<State, string> _states;
    State _currentState;
};

#endif // STATESPRITE_H_INCLUDED
