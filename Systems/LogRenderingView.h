#ifndef LOGRENDERINGVIEW_H_INCLUDED
#define LOGRENDERINGVIEW_H_INCLUDED

#include "cocos2d.h"
#include "ui/UIText.h"
#include "ui/UIButton.h"
#include "ui/UIScrollView.h"

#include "../ui/Scrollbar.h"

#include "System.h"
#include "../common.h"
#include "../helper.h"

#include "../Dependencies/entt.hpp"

#include <deque>

USING_NS_CC;
using std::deque;

class LogRenderingView: public ISystem {
public:
    LogRenderingView(Vec2 position, Size size, entt::dispatcher& dispatcher): _dispatcher(dispatcher) {
        Scene* runningScene = cocos2d::Director::getInstance()->getRunningScene();


        _scrollbar = Scrollbar::createScrollbar(runningScene, Scrollbar::Direction::VERTICAL);
        _scrollbar->setContentSize(Size(size.width, size.height));
        _scrollbar->setInnerContainerSize(_scrollbar->getInnerContainerSize());
        _scrollbar->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        _scrollbar->setScrollBarEnabled(false);
        _scrollbar->setPosition(position + Vec2(16, 0));

        runningScene->addChild(_scrollbar);

        dispatcher.sink<MessageEvent>().connect<&LogRenderingView::onMessageEvent>(*this);

    }

    ~LogRenderingView() {
        _dispatcher.sink<MessageEvent>().disconnect(*this);

        for(auto text : _scrollbarText) {
            text->removeFromParentAndCleanup(true);
        }

        _scrollbar->removeFromParentAndCleanup(true);
    }

    virtual void update(entt::registry& registry, entt::dispatcher& dispatcher, float delta) {

    }

    void onMessageEvent(const MessageEvent& messageEvent) {
        if(_scrollbarText.size() > 20) {
            cocos2d::ui::Text* outText = _scrollbarText.front();
            _scrollbarText.pop_front();
            outText->removeFromParentAndCleanup(true);

        }

        cocos2d::ui::Text* newText = cocos2d::ui::Text::create(cutMessage(messageEvent.message.text, 12.0f, _scrollbar->getContentSize().width),
                                                                          Constants::StandardFontName,
                                                                          12.0f);
        newText->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        newText->setPosition(Vec2::ZERO);
        newText->setColor(messageEvent.message.textColor);

        _scrollbar->addChild(newText);

        _scrollbarText.push_back(newText);

        reallignText();
    }

    void onEnable() {
        _scrollbar->setVisible(true);
        _scrollbar->setEnabled(true);
    }

    void onDisable() {
        _scrollbar->setVisible(false);
        _scrollbar->setEnabled(false);
    }

private:
    void reallignText() {
        if(!_scrollbarText.empty()) {
            size_t size = _scrollbarText.size();
            for(int i = int(size) - 2; i >= 0; i--) {
                _scrollbarText[i]->setPosition(_scrollbarText[i+1]->getPosition() + Vec2(0, _scrollbarText[i+1]->getContentSize().height));
            }

            Size currentInnerContainerSize = _scrollbar->getInnerContainerSize();
            Size contentSize = _scrollbar->getContentSize();
            float currentMaxHeight = _scrollbarText[0]->getBoundingBox().getMaxY();

            _scrollbar->setInnerContainerSize(Size(contentSize.width, std::max(contentSize.height, currentMaxHeight)));
        }
    }

    Scrollbar* _scrollbar;

    deque<cocos2d::ui::Text*> _scrollbarText;

    entt::dispatcher& _dispatcher;
};


#endif // LOGRENDERINGVIEW_H_INCLUDED
