#ifndef DIALOGVIEW_H_INCLUDED
#define DIALOGVIEW_H_INCLUDED

#include "cocos2d.h"
#include "ui/UIText.h"

#include "System.h"
#include "../common.h"
#include "../helper.h"
#include "../ui/Scrollbar.h"

#include "../Dialog.h"

class DialogView: public ISystem {
public:
    DialogView() {
        auto runningScene = cocos2d::Director::getInstance()->getRunningScene();
        Size _visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

        _replicaScrollbar = Scrollbar::createScrollbar(runningScene, Scrollbar::Direction::VERTICAL);
        _replicaScrollbar->setPosition(_visibleSize * 0.1f);
        _replicaScrollbar->setBackGroundColor(Color3B::BLACK);
        _replicaScrollbar->setBackGroundColorOpacity(127);
        _replicaScrollbar->setBackGroundColorType(Scrollbar::BackGroundColorType::SOLID);
        _replicaScrollbar->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        _replicaScrollbar->setScrollBarEnabled(false);
        _replicaScrollbar->setContentSize(Size(_visibleSize.width * 0.3f, _visibleSize.height * 0.8f));

        _responseScrollbar = Scrollbar::createScrollbar(runningScene, Scrollbar::Direction::VERTICAL);
        _responseScrollbar->setPosition(Vec2(_visibleSize.width * 0.4f + 16, _visibleSize.height * 0.1f));
        _responseScrollbar->setBackGroundColor(Color3B::BLACK);
        _responseScrollbar->setBackGroundColorOpacity(127);
        _responseScrollbar->setBackGroundColorType(Scrollbar::BackGroundColorType::SOLID);
        _responseScrollbar->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        _responseScrollbar->setScrollBarEnabled(false);
        _responseScrollbar->setContentSize(Size(_visibleSize.width * 0.5f, _visibleSize.height * 0.4f));

        _responseText = cocos2d::ui::Text::create("", Constants::StandardFontName, 14.0f);
        _responseText->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        _responseText->setPosition(Vec2(20, _responseScrollbar->getContentSize().height - 20));
        _responseScrollbar->addChild(_responseText);

        runningScene->addChild(_replicaScrollbar);
        runningScene->addChild(_responseScrollbar);

        _currentDialogID = -1;
    }

    ~DialogView() {
        _replicaScrollbar->removeFromParentAndCleanup(true);
        _responseScrollbar->removeFromParentAndCleanup(true);
    }

    virtual void update(entt::registry& registry, entt::dispatcher& dispatcher, float delta) {

    }

    void onDialogChanged(entt::registry& registry, entt::dispatcher& dispatcher) {
        //??? How about event listener

        DialogManager& dialogManager = registry.ctx<DialogManager>();
        initReplicasText(dialogManager);
        _currentDialogID = dialogManager.getCurrentDialogID();

        std::size_t activeIndex = dialogManager.getCurrentDialog().getCurrentReplicaIndex();
        for(std::size_t i = 0; i < _currentReplicasText.size(); ++i) {
            if(i == activeIndex) _currentReplicasText[i]->setOpacity(255);
            else _currentReplicasText[i]->setOpacity(127);
        }
    }

private:
    void initReplicasText(DialogManager& manager) {
        removeAllText();

        Vec2 textPosition = Vec2(20, _replicaScrollbar->getContentSize().height - 20);

        float scrollWidth = _replicaScrollbar->getContentSize().width;

        for(auto replica : manager.getCurrentDialog().getAvailableReplicas()) {
            Text text = replica->getReplicaText();

            cocos2d::ui::Text* uitext = cocos2d::ui::Text::create(cutMessage(text.text, 14.0f, scrollWidth - 20.0f),
                                                                  Constants::StandardFontName,
                                                                  14.0f);

            uitext->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);

            uitext->setOpacity(GLubyte(127));
            uitext->setPosition(textPosition);

            textPosition.y += -uitext->getContentSize().height - 20;

            _replicaScrollbar->addChild(uitext);

            _currentReplicasText.push_back(uitext);
        }

        Size currentResponseSize = _responseScrollbar->getContentSize();

        Text text = manager.getCurrentText();
        _responseText->setString(cutMessage(text.text, _responseText->getFontSize(), _responseScrollbar->getContentSize().width - 40));
        _responseText->setColor(text.textColor);

        _responseScrollbar->setInnerContainerSize(Size(currentResponseSize.width, std::max(currentResponseSize.height, _responseText->getContentSize().height + 40)));
        _responseText->setPosition(Vec2(20, _responseScrollbar->getInnerContainerSize().height - 20));
    }

    void removeAllText() {
        for(auto text : _currentReplicasText) {
            text->removeFromParentAndCleanup(true);
        }

        _currentReplicasText.clear();
    }

    Scrollbar* _replicaScrollbar;
    Scrollbar* _responseScrollbar;

    vector<cocos2d::ui::Text*> _currentReplicasText;

    cocos2d::ui::Text* _responseText;

    uint32_t _currentDialogID;
};


#endif // DIALOGVIEW_H_INCLUDED
