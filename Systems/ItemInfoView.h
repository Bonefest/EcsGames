#ifndef ITEMINFOVIEW_H_INCLUDED
#define ITEMINFOVIEW_H_INCLUDED

#include <vector>
#include "cocos2d.h"

using std::vector;
using cocos2d::StringUtils;

#include "System.h"
#include "../helper.h"
#include "../common.h"
#include "../Components/Components.h"
#include "ui/Scrollbar.h"

class ItemInfoView: public ISystem {
public:
    ItemInfoView() {
        Size visibleSize = Director::getInstance()->getVisibleSize();

        auto runningScene = cocos2d::Director::getInstance()->getRunningScene();
        _scrollbar = Scrollbar::createScrollbar(runningScene, Scrollbar::Direction::VERTICAL);
        _scrollbar->setContentSize()
        runningScene->addChild(_scrollbar);
    }

    void update(entt::registry& registry, entt::dispatcher& dispatcher, float delta) {

    }

    void setItem(entt::registry& registry, entt::entity item) {
        if(registry.valid(item) && registry.has<ItemComponent>(item)) {
            clearText();

            vector<Text> rawText;
            ItemComponent& itemComponent = registry.get<ItemComponent>(item);
            Attributes& attrib = ItemComponent.scalarAttributes;

            if(hasValue(attrib.healthPoints)) rawText.push_back(Text{format("Health Points %f", attrib.healthPoints), Color3B::WHITE});
            if(hasValue(attrib.magicPoints)) rawText.push_back(Text{format("Magic Points %f", attrib.magicPoints), Color3B::WHITE});

            if(hasValue(attrib.strength)) rawText.push_back(Text{format("Strength %f", attrib.strength), Color3B::WHITE});
            if(hasValue(attrib.intellect)) rawText.push_back(Text{format("Intellect %f", attrib.intellect), Color3B::WHITE});
            if(hasValue(attrib.agility)) rawText.push_back(Text{format("Agility %f", attrib.agility), Color3B::WHITE});
            if(hasValue(attrib.vitality)) rawText.push_back(Text{format("Vitality %f", attrib.vitality), Color3B::WHITE});
            if(hasValue(attrib.charisma)) rawText.push_back(Text{format("Charisma %f", attrib.charisma), Color3B::WHITE});

            if(hasValue(attrib.stamina)) rawText.push_back(Text{format("Stamina %f", attrib.stamina), Color3B::WHITE});

            if(hasValue(attrib.stabbingDamage)) rawText.push_back(Text{format("Stabbing damage %f", attrib.stabbingDamage), Color3B::WHITE});
            if(hasValue(attrib.cuttingDamage)) rawText.push_back(Text{format("Cutting damage %f", attrib.cuttingDamage), Color3B::WHITE});
            if(hasValue(attrib.chopDamage)) rawText.push_back(Text{format("Chop damage %f", attrib.chopDamage), Color3B::WHITE});
            if(hasValue(attrib.magicDamage)) rawText.push_back(Text{format("Magic damage %f", attrib.magicDamage), Color3B::WHITE});

            if(hasValue(attrib.defense)) rawText.push_back(Text{format("Defense %f", attrib.defense), Color3B::WHITE});
            if(hasValue(attrib.magicDefense)) rawText.push_back(Text{format("Magic Defense %f", attrib.magicDefense), Color3B::WHITE});

            if(hasValue(attrib.fireDamage)) rawText.push_back(Text{format("Fire Damage %f", attrib.fireDamage), Color3B::WHITE});
            if(hasValue(attrib.waterDamage)) rawText.push_back(Text{format("Water Damage %f", attrib.waterDamage), Color3B::WHITE});
            if(hasValue(attrib.earthDamage)) rawText.push_back(Text{format("Earth Damage %f", attrib.earthDamage), Color3B::WHITE});
            if(hasValue(attrib.airDamage)) rawText.push_back(Text{format("Air Damage %f", attrib.airDamage), Color3B::WHITE});

            if(hasValue(attrib.fireDamage)) rawText.push_back(Text{format("Fire Resistance %f", attrib.fireResistance), Color3B::WHITE});
            if(hasValue(attrib.waterDamage)) rawText.push_back(Text{format("Water Resistance %f", attrib.waterResistance), Color3B::WHITE});
            if(hasValue(attrib.earthDamage)) rawText.push_back(Text{format("Earth Resistance %f", attrib.earthResistance), Color3B::WHITE});
            if(hasValue(attrib.airDamage)) rawText.push_back(Text{format("Air Resistance %f", attrib.airResistance), Color3B::WHITE});


            for(Text text : rawText) {
                cocos2d::ui::Text* uitext = cocos2d::ui::Text::create(cutMessage(text.text, 12.0f, ), Constants::StandardFontName, 12.0f);
                uitext->setTextColor(text.textColor);
                _scrollbar->addChild(uitext);
            }
        }
    }

private:
    void clearText() {
        for(auto text : _currentInformationCont) {
            text->removeFromParentAndCleanup(true);
        }
        _currentInformationCont->clear();
    }

    void alignInformationText() {

    }

    bool hasValue(float value) {
    return abs(value) > 0.01f;
    }

    entt::entity _currentItem;
    vector<cocos2d::ui::Text*> _currentInformationCont;
    Scrollbar* _scrollbar;

};

#endif // ITEMINFOVIEW_H_INCLUDED
