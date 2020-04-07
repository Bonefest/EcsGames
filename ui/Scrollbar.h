#ifndef SCROLLBAR_H_INCLUDED
#define SCROLLBAR_H_INCLUDED

#include "ui/UIScrollView.h"
#include "ui/UIButton.h"

class Scrollbar: public cocos2d::ui::ScrollView {
public:
    static Scrollbar* createScrollbar(Node* parent, Direction direction) {
        Scrollbar* scrollbar = new(std::nothrow) Scrollbar;
        if(scrollbar && scrollbar->initWithDirection(parent, direction)) {
            scrollbar->autorelease();
            return scrollbar;
        }

        delete scrollbar;
        scrollbar = nullptr;
        return nullptr;
    }



    bool initWithDirection(Node* parent, Direction direction) {
        if(!cocos2d::ui::ScrollView::init()) return false;

        _direction = direction;

        _leftButton = _rightButton = _upButton = _downButton = nullptr;

        _autoAlignEnabled = false;
        _alignOffset = 0.0f;
        _leftMargin = _rightMargin = _topMargin = _bottomMargin = 0.0f;

        if(direction == Direction::HORIZONTAL || direction == Direction::BOTH) {
            _leftButton = cocos2d::ui::Button::create("UpButton.png", "", "", TextureResType::PLIST);
            _leftButton->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
            _leftButton->setRotation(-90);

            _rightButton = cocos2d::ui::Button::create("UpButton.png", "", "", TextureResType::PLIST);
            _rightButton->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            _rightButton->setRotation( 90);

            _leftButton->addTouchEventListener(CC_CALLBACK_2(Scrollbar::buttonLeftListener, this));
            _rightButton->addTouchEventListener(CC_CALLBACK_2(Scrollbar::buttonRightListener, this));

            parent->addChild(_leftButton);
            parent->addChild(_rightButton);
        }
        if(direction == Direction::VERTICAL || direction == Direction::BOTH) {
            _upButton = cocos2d::ui::Button::create("UpButton.png", "", "", TextureResType::PLIST);
            _upButton->setScale9Enabled(true);
            _upButton->setContentSize(Size(16, 16));
            _upButton->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);

            _downButton = cocos2d::ui::Button::create("DownButton.png", "", "", TextureResType::PLIST);
            _downButton->setScale9Enabled(true);
            _downButton->setContentSize(Size(16, 16));
            _downButton->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);

            _upButton->addTouchEventListener(CC_CALLBACK_2(Scrollbar::buttonUpListener, this));
            _downButton->addTouchEventListener(CC_CALLBACK_2(Scrollbar::buttonDownListener, this));

            parent->addChild(_upButton);
            parent->addChild(_downButton);
        }

        _barDrawer = cocos2d::DrawNode::create();
        parent->addChild(_barDrawer);
//
//        cocos2d::EventListenerTouchOneByOne* touchListener = cocos2d::EventListenerTouchOneByOne::create();
//        touchListener->onTouchBegan = CC_CALLBACK_2(Scrollbar::touchBegan)

        scheduleUpdate();

        return true;
    }

    bool onTouchBegan(Touch* touch, Event* event) {
        bool result = ScrollView::onTouchBegan(touch, event);

        if(Rect(_position + Vec2(-16, 16), Size(16, _contentSize.height - 16)).containsPoint(touch->getLocation())) {
            _barFocused = true;
        }



        return (result || _barFocused);
    }

    void onTouchMoved(Touch* touch, Event* event) {
        ScrollView::onTouchMoved(touch, event);
        if(_barFocused) {
            float k = (touch->getLocation().y - _position.y - 16) / (_contentSize.height - 32.0f);
            float y = k * getInnerContainerSize().height;
            y = std::max(0.0f, y);
            y = std::min(getInnerContainerSize().height - getContentSize().height, y);
            setInnerContainerPosition(Vec2(getInnerContainerPosition().x, -y));
        }
    }

    void onTouchEnded(Touch* touch, Event* event) {
        ScrollView::onTouchEnded(touch, event);
        _barFocused = false;
    }

    void onTouchCancelled(Touch* touch, Event* event) {
        ScrollView::onTouchCancelled(touch, event);
        _barFocused = false;
    }

    void update(float delta) {
        _barDrawer->clear();
        _barDrawer->drawRect(_position, _position + Vec2(getContentSize()), Color4F::WHITE);

        Size scrollerContentSize = getContentSize();
        Size scrollerInnerSize = getInnerContainerSize();
        Vec2 scrollerInnerPos = getInnerContainerPosition();

        float k = scrollerContentSize.height / scrollerInnerSize.height;
        float barHeight = k * scrollerContentSize.height - 16;

        Vec2 barOriginPos(_position.x - 16.0f , k * abs(scrollerInnerPos.y) + 16 + _position.y);
        Vec2 barDestPos(_position.x, k * abs(scrollerInnerPos.y) + barHeight + _position.y);

        Color4F barColor = (_barFocused) ? Color4F(0.717f, 0.56f, 0.482f, 1.0f) : Color4F(0.717f, 0.56f, 0.482f, 0.6f);
        _barDrawer->drawSolidRect(Vec2(-16.0f + _position.x, 16.0f + _position.y), Vec2(_position.x, scrollerContentSize.height - 16 + _position.y), Color4F(0.556f, 0.435f, 0.372f, 1.0f));
        _barDrawer->drawSolidRect(barOriginPos, barDestPos, barColor);
    }

    void setContentSize(const Size& size) {
        cocos2d::ui::ScrollView::setContentSize(size);
//        if(_leftButton != nullptr) _leftButton->setNormalizedPosition(Vec2::ZERO);
//        if(_rightButton != nullptr) _rightButton->setNormalizedPosition(Vec2(1.0f, 0.0f));

        if(_direction == Direction::VERTICAL) _upButton->setPosition(_position);
        if(_direction == Direction::VERTICAL) _downButton->setPosition(_position + Vec2(0, _contentSize.height));

    }

    void removeFromParent() {
        cocos2d::ui::ScrollView::removeFromParent();
        _upButton->removeFromParent();
        _rightButton->removeFromParent();
        _barDrawer->removeFromParent();
    }

    void removeFromParentAndCleanup(bool remove) {
        cocos2d::ui::ScrollView::removeFromParentAndCleanup(remove);
        _upButton->removeFromParentAndCleanup(remove);
        _downButton->removeFromParentAndCleanup(remove);
        _barDrawer->removeFromParentAndCleanup(remove);
    }


    void buttonLeftListener(Ref* ref, ui::Widget::TouchEventType event) {
        if(event == cocos2d::ui::Widget::TouchEventType::BEGAN) {
            Vec2 currentPosition = getInnerContainerPosition();
            setInnerContainerPosition(Vec2(std::max(0.0f, currentPosition.x - 20.0f), currentPosition.y));
        }
    }

    void buttonRightListener(Ref* ref, ui::Widget::TouchEventType event) {
        if(event == cocos2d::ui::Widget::TouchEventType::BEGAN) {
            Vec2 currentPosition = getInnerContainerPosition();
            Size innerContainerSize = getInnerContainerSize();
            setInnerContainerPosition(Vec2(std::min(innerContainerSize.width - innerContainerSize.width, currentPosition.y + 20), currentPosition.y));
        }
    }

    void buttonUpListener(Ref* ref, ui::Widget::TouchEventType event) {
        //Coordinate space Y down with minus X right
        if(event == cocos2d::ui::Widget::TouchEventType::BEGAN) {
            Vec2 currentPosition = getInnerContainerPosition();
            setInnerContainerPosition(Vec2(currentPosition.x, std::min(0.0f, currentPosition.y + 20)));
        }

    }

    void buttonDownListener(Ref* ref, ui::Widget::TouchEventType event) {
        if(event == cocos2d::ui::Widget::TouchEventType::BEGAN) {
            Vec2 currentPosition = getInnerContainerPosition();
            Size innerContainerSize = getInnerContainerSize();
            setInnerContainerPosition(Vec2(currentPosition.x, std::max(-innerContainerSize.height + getContentSize().height, currentPosition.y - 20)));
        }

    }

    void setVisible(bool visible) {
        ScrollView::setVisible(visible);
        _upButton->setVisible(visible);
        _downButton->setVisible(visible);
        _barDrawer->setVisible(visible);
    }

    void setEnabled(bool enabled) {
        ScrollView::setEnabled(enabled);
        _upButton->setEnabled(enabled);
        _downButton->setEnabled(enabled);
        _barDrawer->setVisible(enabled);
    }

    void addChild(Node* child) {
        cocos2d::ui::ScrollView::addChild(child);
        alignChilds();
    }

    void setAlignOffset(float offset) {
        _alignOffset = offset;
    }

    void setMargin(float left, float top, float right, float bottom) {
        _leftMargin = left;
        _rightMargin = right;
        _topMargin = top;
        _bottomMargin;
    }

    void setAutoAlignEnabled(bool enabled) {
        _autoAlignEnabled = enabled;
    }

    bool isAutoAlignEnabled() const {
        return _autoAlignEnabled;
    }

    void alignChilds() {
        if(_autoAlignEnabled) {

            float height = _topMargin;

            auto children = getChildren();
            for(auto child : children) {
                height += child->getContentSize().height + _alignOffset;
            }

            height -= _alignOffset;
            height += _bottomMargin;

            Size innerSize = getInnerContainerSize();
            setInnerContainerSize(Size(innerSize.width,
                                       std::max(innerSize.height, height + _alignOffset)));
            float itemPositionY = getInnerContainerSize().height - _alignOffset;

            for(auto child: children) {
                child->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
                child->setPositionY(itemPositionY);
                child->setPositionX(_leftMargin);
                itemPositionY = itemPositionY - child->getContentSize().height - _alignOffset;
            }


        }
    }

    Size getCropSize() {
        return Size(Vec2(getContentSize()) - Vec2(_leftMargin + _rightMargin, _topMargin + _bottomMargin));
    }
private:

    cocos2d::DrawNode* _barDrawer;

    cocos2d::ui::Button* _upButton;
    cocos2d::ui::Button* _downButton;

    cocos2d::ui::Button* _leftButton;
    cocos2d::ui::Button* _rightButton;

    Direction _direction;

    bool _barFocused;
    bool _autoAlignEnabled;

    float _alignOffset;

    float _leftMargin;
    float _topMargin;
    float _rightMargin;
    float _bottomMargin;
};


#endif // SCROLLBAR_H_INCLUDED
