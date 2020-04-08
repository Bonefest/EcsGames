#ifndef TEXTGENERATOR_H_INCLUDED
#define TEXTGENERATOR_H_INCLUDED


#include "Dialog/Dialog.h"
#include "Components/Components.h"

#include "Dependencies/entt.hpp"

#include "helper.h"
#include "common.h"

#include <map>
#include <string>

#include "cocos2d.h"
#include "ui/UIText.h"

using std::map;
using std::string;
USING_NS_CC;

class TextGenerator {

    cocos2d::ui::Text* generateText(entt::registry& registry, const string& unformattedText, float fontSize, float maxWidth) {
        string substitutedText = substitute(registry, unformattedText);
        log("%s", substitutedText->c_str());

        string wrappedText = cutMessage(clearText(substitutedText), fontSize, maxWidth);

        Color3B currentColor = Color3B::WHITE;
        cocos2d::ui::Text* text = cocos2d::ui::Text::create(wrappedText, Constants::StandardFontName, fontSize);

        std::size_t
        for(std::size_t i = 0; i < wrappedText.size(); ++i) {

        }
    }

private:
    vector<pair<string, Color3B>> parseColors(string text) {
        vector<pair<string, Color3B>> result;

        string clearedText = text;
        Color3B currentColor = Color3B::WHITE;
        std::size_t previousStart = 0, start, end;

        while( (start = clearedText.find("$$$", previousStart)) != string::npos &&
               (end = clearedText.find("$$$", start + 3)) != string::npos) {
            result.push_back(make_pair<string, Color3B>(text.substr(previousStart, start - previousStart), currentColor));

        }

        return clearedText;
    }

    vector<pair<string, Color3B>> clearText(string text) {
        string clearedText = text;
        std::size_t start = clearedText.find("$$$");
        if(start == string::npos) break;
        std::size_t end = clearedText.find("$$$", start + 3);
        while( (start = clearedText.find("$$$")) != string::npos &&
               (end = clearedText.find("$$$", start + 3)) != string::npos) {
            clearedText.erase(start, end + 3 - start);
        }

        return clearedText;
    }
    string substitute(entt::registry& registry, const string& unformattedText) {
        string text = unformattedText;
        map<string, string> dataMap = collectData(registry);
        for(auto item : dataMap) {
            std::size_t variablePos = 0;
            while( (variablePos = text.find(item->first, variablePos)) != string::npos) {
                text.replace(variablePos, item->first.size(), item->second);
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
        if(registry.valid(dialogInfo.member)) {
            if(registry.has<Cell>(dialogInfo.member)) {
                Cell& cellComponent = registry.get<Cell>(dialogInfo.member);

                data["$$$dialogmember_name$$$"] = cellComponent.name;
            } else if(registry.has<Creature>(dialogInfo.member)) {
                 Creature& creatureComponent = registry.get<Creature>(dialogInfo.member);

//                data["$$$dialogmember_race$$$"] = creatureComponent.race.name;
//                data["$$$dialogmember_class$$$"] = creatureComponent.class.name;
//                data["$$$dialogmember_exp$$$"] = creatureComponent.experience;
            }
        }

        return data;
    }

};


#endif // TEXTGENERATOR_H_INCLUDED
