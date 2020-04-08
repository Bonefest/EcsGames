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


class ItemInfoView: public ISystem {
public:
    ItemInfoView() {
        Size visibleSize = Director::getInstance()->getVisibleSize();

        auto runningScene = cocos2d::Director::getInstance()->getRunningScene();
        _scrollbar = Scrollbar::createScrollbar(runningScene, Scrollbar::Direction::VERTICAL);
        _scrollbar->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.1f));
        _scrollbar->setContentSize(Size(visibleSize.width * 0.4f, visibleSize.height * 0.8f));
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

            vector<Text> information;
            information.push_back(Text{itemComponent.description, Color3B::WHITE});
            information.push_back(Text{format("Weight %.2f pounds\nVolume %.2f gallons", itemComponent.weight, itemComponent.volume), Color3B(220, 220, 220)});

            vector<Text> generatedText = generateAttributesInformation(itemComponent.scalarAttributes, "%s %+f", Color3B::WHITE);
            information.insert(information.end(), generatedText.begin(), generatedText.end());
            generatedText = generateAttributesInformation(itemComponent.percentAttributes, "%s %+f%%", Color3B(200, 200, 255));
            information.insert(information.end(), generatedText.begin(), generatedText.end());

            information.push_back(Text{"\nLIMITATIONS", Color3B::WHITE});

            generatedText = generateAttributesInformation(itemComponent.attributesLimitations, "%s %+f and more", Color3B(255, 200, 200));
            information.insert(information.end(), generatedText.begin(), generatedText.end());

            for(Text text : information) {
                cocos2d::ui::Text* uitext = cocos2d::ui::Text::create(cutMessage(text.text, 12.0f, _scrollbar->getContentSize().width), Constants::StandardFontName, 12.0f);
                //uitext->getLetter(1)->setColor(Color3B::YELLOW);
                uitext->setTextColor(Color4B(text.textColor));
                _scrollbar->addChild(uitext);
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

    vector<Text> generateAttributesInformation(Attributes attrib, const char* fmt, Color3B color) {
        vector<Text> rawText;

        if(hasValue(attrib.healthPoints)) rawText.push_back(Text{format(fmt, "Health Points", attrib.healthPoints), color});
        if(hasValue(attrib.magicPoints)) rawText.push_back(Text{format(fmt, "Magic Points", attrib.magicPoints), color});

        if(hasValue(attrib.strength)) rawText.push_back(Text{format(fmt, "Strength", attrib.strength), color});
        if(hasValue(attrib.intellect)) rawText.push_back(Text{format(fmt, "Intellect", attrib.intellect), color});
        if(hasValue(attrib.agility)) rawText.push_back(Text{format(fmt, "Agility", attrib.agility), color});
        if(hasValue(attrib.vitality)) rawText.push_back(Text{format(fmt, "Vitality", attrib.vitality), color});
        if(hasValue(attrib.charisma)) rawText.push_back(Text{format(fmt, "Charisma", attrib.charisma), color});

        if(hasValue(attrib.stamina)) rawText.push_back(Text{format(fmt, "Stamina", attrib.stamina), color});

        if(hasValue(attrib.stabbingDamage)) rawText.push_back(Text{format(fmt, "Stabbing damage", attrib.stabbingDamage), color});
        if(hasValue(attrib.cuttingDamage)) rawText.push_back(Text{format(fmt, "Cutting damage", attrib.cuttingDamage), color});
        if(hasValue(attrib.chopDamage)) rawText.push_back(Text{format(fmt, "Chop damage", attrib.chopDamage), color});
        if(hasValue(attrib.magicDamage)) rawText.push_back(Text{format(fmt, "Magic damage", attrib.magicDamage), color});

        if(hasValue(attrib.physicalCriticalChance)) rawText.push_back(Text{format(fmt, "Physical Critical Chance", attrib.physicalCriticalChance), color});
        if(hasValue(attrib.magicalCriticalChance)) rawText.push_back(Text{format(fmt, "Magical Critical Chance", attrib.magicalCriticalChance), color});
        if(hasValue(attrib.additionalGoldChance)) rawText.push_back(Text{format(fmt, "Additional Gold Chance", attrib.additionalGoldChance), color});


        if(hasValue(attrib.defense)) rawText.push_back(Text{format(fmt, "Defense", attrib.defense), color});
        if(hasValue(attrib.magicDefense)) rawText.push_back(Text{format(fmt, "Magic Defense", attrib.magicDefense), color});

        if(hasValue(attrib.fireDamage)) rawText.push_back(Text{format(fmt, "Fire Damage", attrib.fireDamage), color});
        if(hasValue(attrib.waterDamage)) rawText.push_back(Text{format(fmt, "Water Damage", attrib.waterDamage), color});
        if(hasValue(attrib.earthDamage)) rawText.push_back(Text{format(fmt, "Earth Damage", attrib.earthDamage), color});
        if(hasValue(attrib.airDamage)) rawText.push_back(Text{format(fmt, "Air Damage", attrib.airDamage), color});

        if(hasValue(attrib.fireDamage)) rawText.push_back(Text{format(fmt, "Fire Resistance", attrib.fireResistance), color});
        if(hasValue(attrib.waterDamage)) rawText.push_back(Text{format(fmt, "Water Resistance", attrib.waterResistance), color});
        if(hasValue(attrib.earthDamage)) rawText.push_back(Text{format(fmt, "Earth Resistance", attrib.earthResistance), color});
        if(hasValue(attrib.airDamage)) rawText.push_back(Text{format(fmt, "Air Resistance", attrib.airResistance), color});

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
