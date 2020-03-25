#ifndef LOGRENDERINGVIEW_H_INCLUDED
#define LOGRENDERINGVIEW_H_INCLUDED

#include "cocos2d.h"
#include "ui/UIText.h"
#include "ui/UIButton.h"
#include "ui/UIScrollView.h"

#include "System.h"
#include "../common.h"
#include "../helper.h"

#include "../Dependencies/entt.hpp"

#include <deque>

USING_NS_CC;
using std::deque;

class LogRenderingView: public ISystem {
public:
    LogRenderingView(Vec2 position, Size size, entt::dispatcher& dispatcher): _position(position), _size(size), _dispatcher(dispatcher) {
        Scene* runningScene = cocos2d::Director::getInstance()->getRunningScene();

        _downButton = cocos2d::ui::Button::create("DownButton.png", "", "", cocos2d::ui::Button::TextureResType::PLIST);
        _downButton->setScale9Enabled(true);
        _downButton->setContentSize(Size(16, 16));
        _downButton->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        _downButton->setPosition(position);

        _downButton->addTouchEventListener(CC_CALLBACK_2(LogRenderingView::buttonDownListener, this));

        _upButton = cocos2d::ui::Button::create("UpButton.png", "", "", cocos2d::ui::Button::TextureResType::PLIST);
        _upButton->setScale9Enabled(true);
        _upButton->setContentSize(Size(16, 16));
        _upButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        _upButton->setPosition(position + Vec2(0, size.height));

        _upButton->addTouchEventListener(CC_CALLBACK_2(LogRenderingView::buttonUpListener, this));

        _scroller = cocos2d::ui::ScrollView::create();
        _scroller->setContentSize(Size(size.width - 24, size.height));
        _scroller->setInnerContainerSize(_scroller->getInnerContainerSize());
        _scroller->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        _scroller->setScrollBarEnabled(false);
        _scroller->setPosition(position + Vec2(24, 0));

        _renderer = DrawNode::create();

        runningScene->addChild(_downButton);
        runningScene->addChild(_upButton);
        runningScene->addChild(_scroller);
        runningScene->addChild(_renderer);

        dispatcher.sink<MessageEvent>().connect<&LogRenderingView::onMessageEvent>(*this);

    }

    ~LogRenderingView() {
        _dispatcher.sink<MessageEvent>().disconnect(*this);

        for(auto text : _scrollbarText) {
            text->removeFromParentAndCleanup(true);
        }

        _downButton->removeFromParentAndCleanup(true);
        _upButton->removeFromParentAndCleanup(true);
        _scroller->removeFromParentAndCleanup(true);
        _renderer->removeFromParentAndCleanup(true);
    }

    virtual void update(entt::registry& registry, entt::dispatcher& dispatcher, float delta) {
        _renderer->clear();
        _renderer->drawRect(_position, _position + Vec2(_size), Color4F::WHITE);

        Size scrollerContentSize = _scroller->getContentSize();
        Size scrollerInnerSize = _scroller->getInnerContainerSize();
        Vec2 scrollerInnerPos = _scroller->getInnerContainerPosition();

        float k = scrollerContentSize.height / scrollerInnerSize.height;
        float barHeight = k * scrollerContentSize.height - 16;

        Vec2 barOriginPos(_position.x, k * abs(scrollerInnerPos.y) + _position.y + 16);
        Vec2 barDestPos(_position.x + 16, k * abs(scrollerInnerPos.y) + barHeight + _position.y);

        _renderer->drawSolidRect(Vec2(_position.x, _position.y + 16), Vec2(_position.x + 16, _position.y + scrollerContentSize.height - 16), Color4F(0.556f, 0.435f, 0.372f, 1.0f));
        _renderer->drawSolidRect(barOriginPos, barDestPos, Color4F(0.717f, 0.56f, 0.482f, 1.0f));
    }

    void onMessageEvent(const MessageEvent& messageEvent) {
        if(_scrollbarText.size() > 10) {
            cocos2d::ui::Text* outText = _scrollbarText.front();
            _scrollbarText.pop_front();
            outText->removeFromParentAndCleanup(true);

        }

        cocos2d::ui::Text* newText = cocos2d::ui::Text::create(cutMessage(messageEvent.message, 12.0f, _size.width),
                                                                Constants::StandardFontName,
                                                                12.0f);
        newText->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        newText->setPosition(Vec2::ZERO);
        newText->setColor(messageEvent.textColor);

        _scroller->addChild(newText);

        _scrollbarText.push_back(newText);

        reallignText();
    }

    void buttonDownListener(Ref* ref, ui::Widget::TouchEventType event) {
        if(event == cocos2d::ui::Widget::TouchEventType::BEGAN) {
            Vec2 currentPosition = _scroller->getInnerContainerPosition();
            Size innerContainerSize = _scroller->getInnerContainerSize();
            _scroller->setInnerContainerPosition(Vec2(currentPosition.x, std::min(0.0f, currentPosition.y + 20)));
        }

    }

    void buttonUpListener(Ref* ref, ui::Widget::TouchEventType event) {
        if(event == cocos2d::ui::Widget::TouchEventType::BEGAN) {
            Vec2 currentPosition = _scroller->getInnerContainerPosition();
            Size innerContainerSize = _scroller->getInnerContainerSize();
            _scroller->setInnerContainerPosition(Vec2(currentPosition.x, std::max(-innerContainerSize.height + _scroller->getContentSize().height, currentPosition.y - 20)));
        }

    }

private:
    void reallignText() {
        if(!_scrollbarText.empty()) {
            size_t size = _scrollbarText.size();
            for(int i = int(size) - 2; i >= 0; i--) {
                _scrollbarText[i]->setPosition(_scrollbarText[i+1]->getPosition() + Vec2(0, _scrollbarText[i+1]->getContentSize().height));
            }

            Size currentInnerContainerSize = _scroller->getInnerContainerSize();
            Size contentSize = _scroller->getContentSize();
            float currentMaxHeight = _scrollbarText[0]->getBoundingBox().getMaxY();

            _scroller->setInnerContainerSize(Size(contentSize.width, std::max(contentSize.height, currentMaxHeight)));
        }
    }

    Vec2 _position;
    Size _size;

    cocos2d::ui::Button* _upButton;
    cocos2d::ui::Button* _downButton;
    cocos2d::ui::ScrollView* _scroller;
    cocos2d::DrawNode* _renderer;

    deque<cocos2d::ui::Text*> _scrollbarText;

    entt::dispatcher& _dispatcher;
};


#endif // LOGRENDERINGVIEW_H_INCLUDED
