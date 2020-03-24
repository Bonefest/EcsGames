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

    void draw(cocos2d::Renderer* renderer, const Mat4& transform, uint32_t flags) {


        cocos2d::SpriteFrame* frame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName("Floor.png");
        //state = cocos2d::GLProgramState::getOrCreateWithGLProgramName(cocos2d::GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP, frame->getTexture());
        cocos2d::BlendFunc func = cocos2d::BlendFunc::ALPHA_NON_PREMULTIPLIED;
        //const cocos2d::PolygonInfo& polyInfo = frame->getPolygonInfo();

        static cocos2d::PolygonInfo polyInfo = frame->getPolygonInfo();



        command.init(-1, frame->getTexture(), cocos2d::GLProgramState::getOrCreateWithGLProgramName(cocos2d::GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP, frame->getTexture()), func, polyInfo.triangles, Mat4::IDENTITY, flags);
        renderer->addCommand(&command);

    }

private:
    map<State, string> _states;
    State _currentState;

    cocos2d::TrianglesCommand command;
};

#endif // STATESPRITE_H_INCLUDED
