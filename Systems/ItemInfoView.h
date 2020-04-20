#ifndef ITEMINFOVIEW_H_INCLUDED
#define ITEMINFOVIEW_H_INCLUDED

#include <vector>
#include "cocos2d.h"

using std::vector;
using namespace cocos2d::StringUtils;

#include "System.h"
#include "../helper.h"
#include "../common.h"
#include "../Item.h"
#include "../AutosizeContainer.h"
#include "../Components/Components.h"

#include "ui/Scrollbar.h"
#include "ui/UIText.h"

#include "TextGenerator.h"

class ItemInfoView: public ISystem {
public:
    ItemInfoView() {
        Size visibleSize = Director::getInstance()->getVisibleSize();

        auto runningScene = cocos2d::Director::getInstance()->getRunningScene();
        _scrollbar = Scrollbar::createScrollbar(runningScene, Scrollbar::Direction::VERTICAL);
        _scrollbar->setPosition(Vec2(visibleSize.width * 0.5f + 16.0f, visibleSize.height * 0.1f));
        _scrollbar->setContentSize(Size(visibleSize.width * 0.4f, visibleSize.height * 0.8f));
        _scrollbar->setBackGroundColorOpacity(127);
        _scrollbar->setBackGroundColor(Color3B::BLACK);
        _scrollbar->setBackGroundColorType(Scrollbar::BackGroundColorType::SOLID);
        _scrollbar->setAutoAlignEnabled(true);
        _scrollbar->setAlignOffset(10.0f);
        _scrollbar->setMargin(20.0f, 20.0f, 20.0f, 20.0f);
        runningScene->addChild(_scrollbar);
    }

    ~ItemInfoView() {
        _scrollbar->removeFromParentAndCleanup(true);
    }

    void update(entt::registry& registry, entt::dispatcher& dispatcher, float delta) {

    }

    void setItem(entt::registry& registry, entt::entity item) {
        if(registry.valid(item) && registry.has<ItemComponent>(item)) {
            _scrollbar->removeAllChildren();

            clearText();

            ItemComponent& itemComponent = registry.get<ItemComponent>(item);
            Drawable& drawableComponent = registry.get<Drawable>(item);

            AutosizeContainer* container = AutosizeContainer::createContainer();

            cocos2d::Sprite* itemSprite = Sprite::createWithSpriteFrameName(drawableComponent.animations.getCurrentFrame());
            itemSprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            container->addChild(itemSprite);

            cocos2d::ui::Text* itemTitle = cocos2d::ui::Text::create(cutMessage(itemComponent.name, 14.0f, _scrollbar->getCropSize().width),
                                                                     Constants::StandardFontName, 14.0f);
            itemTitle->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            itemTitle->setPositionX(48.0f);
            container->addChild(itemTitle);

            _scrollbar->addChild(container);

            vector<string> information;
            information.push_back(itemComponent.description);
            information.push_back(format("Weight $$$color_gray$$$%.2f$$$color_white$$$ pounds\nVolume $$$color_gray$$$%.2f$$$color_white$$$ gallons", itemComponent.weight, itemComponent.volume));

            vector<string> generatedText = generateAttributesInformation(itemComponent.scalarAttributes, "%s $$$color_green$$$%+.2f", "%s $$$color_red$$$%+.2f");
            information.insert(information.end(), generatedText.begin(), generatedText.end());
            generatedText = generateAttributesInformation(itemComponent.percentAttributes, "%s $$$color_green$$$%+.2f%%", "%s $$$color_red$$$%+.2f%%");
            information.insert(information.end(), generatedText.begin(), generatedText.end());

            information.push_back("\nLIMITATIONS");

            generatedText = generateAttributesInformation(itemComponent.attributesLimitations, "%s $$$color_red$$$%+.2f $$$color_white$$$and more", "%s $$$color_red$$$%+.2f $$$color_white$$$and more");
            information.insert(information.end(), generatedText.begin(), generatedText.end());

            TextGenerator generator;

            for(auto text : information) {
               _scrollbar->addChild(generator.generateText(registry, text, 12.0f, _scrollbar->getContentSize().width));
            }
        }
    }

private:
    void clearText() {
        for(auto text : _currentInformationCont) {
            text->removeFromParentAndCleanup(true);
        }
        _currentInformationCont.clear();
    }

    vector<string> generateAttributesInformation(Attributes attrib, const char* goodValueFmt, const char* badValueFmt) {
        vector<string> rawText;

        if(hasValue(attrib.healthPoints)) rawText.push_back(format( ((attrib.healthPoints > 0) ? goodValueFmt : badValueFmt), "Health Points", attrib.healthPoints));
        if(hasValue(attrib.magicPoints)) rawText.push_back(format( ((attrib.magicPoints > 0) ? goodValueFmt : badValueFmt), "Magic Points", attrib.magicPoints));

        if(hasValue(attrib.strength)) rawText.push_back(format( ((attrib.strength > 0) ? goodValueFmt : badValueFmt), "Strength", attrib.strength));
        if(hasValue(attrib.intellect)) rawText.push_back(format( ((attrib.intellect > 0) ? goodValueFmt : badValueFmt), "Intellect", attrib.intellect));
        if(hasValue(attrib.agility)) rawText.push_back(format( ((attrib.agility > 0) ? goodValueFmt : badValueFmt), "Agility", attrib.agility));
        if(hasValue(attrib.vitality)) rawText.push_back(format( ((attrib.vitality > 0) ? goodValueFmt : badValueFmt), "Vitality", attrib.vitality));
        if(hasValue(attrib.charisma)) rawText.push_back(format( ((attrib.charisma > 0) ? goodValueFmt : badValueFmt), "Charisma", attrib.charisma));

        if(hasValue(attrib.stamina)) rawText.push_back(format( ((attrib.stamina > 0) ? goodValueFmt : badValueFmt), "Stamina", attrib.stamina));

        if(hasValue(attrib.stabbingDamage)) rawText.push_back(format( ((attrib.stabbingDamage > 0) ? goodValueFmt : badValueFmt), "Stabbing damage", attrib.stabbingDamage));
        if(hasValue(attrib.cuttingDamage)) rawText.push_back(format( ((attrib.cuttingDamage > 0) ? goodValueFmt : badValueFmt), "Cutting damage", attrib.cuttingDamage));
        if(hasValue(attrib.chopDamage)) rawText.push_back(format( ((attrib.chopDamage > 0) ? goodValueFmt : badValueFmt), "Chop damage", attrib.chopDamage));
        if(hasValue(attrib.magicDamage)) rawText.push_back(format( ((attrib.magicDamage > 0) ? goodValueFmt : badValueFmt), "Magic damage", attrib.magicDamage));

        if(hasValue(attrib.physicalCriticalChance)) rawText.push_back(format( ((attrib.physicalCriticalChance > 0) ? goodValueFmt : badValueFmt), "Physical Critical Chance", attrib.physicalCriticalChance));
        if(hasValue(attrib.magicalCriticalChance)) rawText.push_back(format( ((attrib.magicalCriticalChance > 0) ? goodValueFmt : badValueFmt), "Magical Critical Chance", attrib.magicalCriticalChance));
        if(hasValue(attrib.additionalGoldChance)) rawText.push_back(format( ((attrib.additionalGoldChance > 0) ? goodValueFmt : badValueFmt), "Additional Gold Chance", attrib.additionalGoldChance));


        if(hasValue(attrib.defense)) rawText.push_back(format( ((attrib.defense > 0) ? goodValueFmt : badValueFmt), "Defense", attrib.defense));
        if(hasValue(attrib.magicDefense)) rawText.push_back(format( ((attrib.magicDefense > 0) ? goodValueFmt : badValueFmt), "Magic Defense", attrib.magicDefense));

        if(hasValue(attrib.fireDamage)) rawText.push_back(format( ((attrib.fireDamage > 0) ? goodValueFmt : badValueFmt), "Fire Damage", attrib.fireDamage));
        if(hasValue(attrib.waterDamage)) rawText.push_back(format( ((attrib.waterDamage > 0) ? goodValueFmt : badValueFmt), "Water Damage", attrib.waterDamage));
        if(hasValue(attrib.earthDamage)) rawText.push_back(format( ((attrib.earthDamage > 0) ? goodValueFmt : badValueFmt), "Earth Damage", attrib.earthDamage));
        if(hasValue(attrib.airDamage)) rawText.push_back(format( ((attrib.airDamage > 0) ? goodValueFmt : badValueFmt), "Air Damage", attrib.airDamage));

        if(hasValue(attrib.fireResistance)) rawText.push_back(format( ((attrib.fireResistance > 0) ? goodValueFmt : badValueFmt), "Fire Resistance", attrib.fireResistance));
        if(hasValue(attrib.waterResistance)) rawText.push_back(format( ((attrib.waterResistance > 0) ? goodValueFmt : badValueFmt), "Water Resistance", attrib.waterResistance));
        if(hasValue(attrib.earthResistance)) rawText.push_back(format( ((attrib.earthResistance > 0) ? goodValueFmt : badValueFmt), "Earth Resistance", attrib.earthResistance));
        if(hasValue(attrib.airResistance)) rawText.push_back(format( ((attrib.airResistance > 0) ? goodValueFmt : badValueFmt), "Air Resistance", attrib.airResistance));

        return rawText;
    }

    bool hasValue(float value) {
        return abs(value) > 0.01f;
    }

    entt::entity _currentItem;
    Scrollbar* _scrollbar;
    vector<cocos2d::ui::Text*> _currentInformationCont;


};

#endif // ITEMINFOVIEW_H_INCLUDED
