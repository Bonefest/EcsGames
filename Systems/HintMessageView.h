#ifndef HINTMESSAGEVIEW_H_INCLUDED
#define HINTMESSAGEVIEW_H_INCLUDED

#include "cocos2d.h"
#include "ui/UIText.h"

#include "System.h"

#include "../Dependencies/entt.hpp"

#include "../Events/Events.h"
#include "../helper.h"

USING_NS_CC;


class HintMessageView: public ISystem {
public:
    HintMessageView(entt::dispatcher& dispatcher, Rect hintRect):   _dispatcher(dispatcher),
                                                                    _hintRect(hintRect),
                                                                    _elapsedTime(0.0f),
                                                                    _textLifetime(0.0f)  {

        dispatcher.sink<HintMessageEvent>().connect<&HintMessageView::onHintMessageEvent>(*this);

        Scene* runningScene = Director::getInstance()->getRunningScene();
        _hintText = cocos2d::ui::Text::create("", Constants::StandardFontName, 20.0f);
        _hintText->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        _hintText->setPosition(hintRect.origin + Vec2(0, hintRect.size.height * 0.4f));
        _hintText->setTextAreaSize(hintRect.size);
        _hintText->setTextVerticalAlignment(cocos2d::TextVAlignment::TOP);
        _hintText->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
        runningScene->addChild(_hintText);
    }

    ~HintMessageView() {
        _dispatcher.sink<HintMessageEvent>().disconnect(*this);
        _hintText->removeFromParentAndCleanup(true);
    }

    virtual void update(entt::registry& registry, entt::dispatcher& dispatcher, float delta) {
        _elapsedTime = std::min(_textLifetime, _elapsedTime + delta);

        float opacity = 1.0f;
        if(_textLifetime > 0.0f) {
            opacity = 1.0f - _elapsedTime / _textLifetime;
        }

        _hintText->setOpacity(GLubyte(opacity * 255));
    }

    void onHintMessageEvent(const HintMessageEvent& event) {
        _hintText->setString(cutMessage(event.hint.text, _hintText->getFontSize(), _hintRect.size.width));
        _hintText->setColor(event.hint.textColor);

        _textLifetime = event.lifetime;
        _elapsedTime = 0.0f;
    }

private:
    entt::dispatcher& _dispatcher;
    cocos2d::ui::Text* _hintText;

    Rect _hintRect;

    float _elapsedTime;
    float _textLifetime;
};

#endif // HINTMESSAGEVIEW_H_INCLUDED
