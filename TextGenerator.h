#ifndef TEXTGENERATOR_H_INCLUDED
#define TEXTGENERATOR_H_INCLUDED


#include "Dialog/Dialog.h"
#include "Components/Components.h"

#include "Dependencies/entt.hpp"

#include "helper.h"
#include "common.h"

#include <map>
#include <string>
#include <vector>

#include "cocos2d.h"
#include "ui/UIText.h"

using std::map;
using std::pair;
using std::string;
using std::vector;
using std::make_pair;
USING_NS_CC;

class TextGenerator {
public:
    cocos2d::ui::Text* generateText(entt::registry& registry, const string& unformattedText, float fontSize, float maxWidth) {
        string substitutedText = substitute(registry, unformattedText);


        vector<pair<string, Color3B>> parsedColorsText = parseColorsText(substitutedText);

        string wrappedText = "";
        for(auto text : parsedColorsText) {
            wrappedText += text.first;
        }

        wrappedText = cutMessage(wrappedText, fontSize, maxWidth);
        cocos2d::ui::Text* uitext = cocos2d::ui::Text::create(wrappedText, Constants::StandardFontName, fontSize);
        uitext->retain();

        Color3B currentColor;
        int currentPairIndex = -1;
        std::size_t pairStringVisibleChars = 0, wrappedStringVisibleChars = 1;
        for(std::size_t i = 0;i < wrappedText.size(); ++i) {
            if(isgraph(wrappedText[i])) {
                log("%c", wrappedText[i]);
                wrappedStringVisibleChars++;
                if(wrappedStringVisibleChars >= pairStringVisibleChars) {
                    currentPairIndex++;
                    pairStringVisibleChars = wrappedStringVisibleChars = 0;
                    for(auto sym : parsedColorsText[currentPairIndex].first) {
                        if(isgraph(sym)) pairStringVisibleChars++;
                    }
                    currentColor = parsedColorsText[currentPairIndex].second;
                }

                Sprite* sprite = uitext->getLetter(i);
                if(sprite != nullptr) {
                    sprite->setColor(currentColor);
                }
            }
        }

        return uitext;
    }

private:
    vector<pair<string, Color3B>> parseColorsText(string text) {
        vector<pair<string, Color3B>> result;

        Color3B currentColor = Color3B::WHITE;
        std::size_t previousStart = 0, start, end;

        while( previousStart < text.size() && (start = text.find("$$$", previousStart)) != string::npos &&
                                              (end = text.find("$$$", start + 3)) != string::npos) {
            string sstr = text.substr(previousStart, start - previousStart);
            if(!sstr.empty())
                result.push_back(pair<string, Color3B>{sstr, currentColor});
            currentColor = determineColor(text.substr(start + 3, end - start - 3));
            previousStart = end + 3;
        }
        result.push_back(pair<string, Color3B>{text.substr(previousStart), currentColor});

        return result;
    }

    Color3B determineColor(const string& color) {
        if(color == "color_white") return Color3B::WHITE;
        else if(color == "color_red") return Color3B::RED;
        else if(color == "color_blue") return Color3B::BLUE;
        else if(color == "color_green") return Color3B::GREEN;
        else if(color == "color_gray") return Color3B::GRAY;
        else if(color == "color_magenta") return Color3B::MAGENTA;

        try {
            uint32_t number = std::stoi(color, nullptr, 16);
            return integerToColor(number);


        } catch(std::invalid_argument invalid) {

        }

        return Color3B::WHITE;
    }

    string substitute(entt::registry& registry, const string& unformattedText) {
        string text = unformattedText;
        map<string, string> dataMap = collectData(registry);
        for(auto item : dataMap) {
            std::size_t variablePos = 0;
            while( (variablePos = text.find(item.first, variablePos)) != string::npos) {
                text.replace(variablePos, item.first.size(), item.second);
            }
        }

        return text;
    }

    map<string, string> collectData(entt::registry& registry) {
        map<string, string> data;

        auto playerView = registry.view<Controllable, Creature, Cell>();
        entt::entity player = entt::null;
        for(auto entity : playerView) {
            player = entity;
            break;
        }

        if(player != entt::null) {
            Cell& cellComponent = registry.get<Cell>(player);
            Creature& creatureComponent = registry.get<Creature>(player);

            data["$$$player_name$$$"] = cellComponent.name;
//            data["$$$player_race$$$"] = creatureComponent.race.name;
//            data["$$$player_class$$$"] = creatureComponent.class.name;
//            data["$$$player_exp"] = creatureComponent.experience;
        }

        DialogInfo& dialogInfo = registry.ctx<DialogInfo>();
        if(registry.valid(dialogInfo.dialogMember)) {
            if(registry.has<Cell>(dialogInfo.dialogMember)) {
                Cell& cellComponent = registry.get<Cell>(dialogInfo.dialogMember);

                data["$$$dialogmember_name$$$"] = cellComponent.name;
            } else if(registry.has<Creature>(dialogInfo.dialogMember)) {
                 Creature& creatureComponent = registry.get<Creature>(dialogInfo.dialogMember);

//                data["$$$dialogmember_race$$$"] = creatureComponent.race.name;
//                data["$$$dialogmember_class$$$"] = creatureComponent.class.name;
//                data["$$$dialogmember_exp$$$"] = creatureComponent.experience;
            }
        }

        return data;
    }

};


#endif // TEXTGENERATOR_H_INCLUDED
