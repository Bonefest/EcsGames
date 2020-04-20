#ifndef ITEMLISTVIEW_H_INCLUDED
#define ITEMLISTVIEW_H_INCLUDED

#include "cocos2d.h"
#include "ui/UIText.h"

#include "System.h"
#include "../common.h"
#include "../helper.h"
#include "../ui/Scrollbar.h"

#include "../Dialog/Dialog.h"

class ItemListView: public ISystem {
public:
    ItemListView() {
        _activeItemIndex = 0;

        auto runningScene = cocos2d::Director::getInstance()->getRunningScene();
        Size _visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

        _scrollbar = Scrollbar::createScrollbar(runningScene, Scrollbar::Direction::VERTICAL);
        _scrollbar->setPosition(_visibleSize * 0.1f);
        _scrollbar->setBackGroundColor(Color3B::BLACK);
        _scrollbar->setBackGroundColorOpacity(127);
        _scrollbar->setBackGroundColorType(Scrollbar::BackGroundColorType::SOLID);
        _scrollbar->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        _scrollbar->setAutoAlignEnabled(true);
        _scrollbar->setAlignOffset(20.0f);
        _scrollbar->setMargin(20.0f, 20.0f, 0.0f, 0.0f);
        _scrollbar->setScrollBarEnabled(false);
        _scrollbar->setContentSize(Size(_visibleSize.width * 0.35f, _visibleSize.height * 0.8f));

        runningScene->addChild(_scrollbar);
    }

    ~ItemListView() {
        clearText();

        _scrollbar->removeFromParentAndCleanup(true);
    }

    void update(entt::registry& registry, entt::dispatcher& dispatcher, float delta) {

    }

    void setActiveItemIndex(std::size_t index) {
        if(_activeItemIndex < _items.size()) {
            _items[_activeItemIndex]->setColor(Color3B::WHITE);
        }

        if(index < _items.size()) {
            _items[index]->setColor(Color3B::GRAY);
        }

        _activeItemIndex = index;
    }

    std::size_t getActiveItemIndex() const {
        return _activeItemIndex;
    }

    void setItems(vector<string> items) {
        clearText();

        for(auto item : items) {
            cocos2d::ui::Text* text = cocos2d::ui::Text::create(item, Constants::StandardFontName, 14.0f);
            _scrollbar->addChild(text);
            _items.push_back(text);
        }
    }

private:
    void clearText() {
        for(auto item : _items)
            item->removeFromParentAndCleanup(true);
        _items.clear();
    }

    vector<ui::Text*> _items;
    std::size_t _activeItemIndex;

    Scrollbar* _scrollbar;
};



#endif // ITEMLISTVIEW_H_INCLUDED
