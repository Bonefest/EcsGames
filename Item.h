#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED

#include <map>
#include <vector>
#include <string>

using std::map;
using std::vector;
using std::string;

template <typename T>
struct AttributesTemplate {
    AttributesTemplate() {}

	T healthPoints;
	T magicPoints;

	T strength;
	T intellect;
	T agility;
	T vitality;
	T charisma;

	T stamina;

	T stabbingDamage;
	T cuttingDamage;
	T chopDamage;
    T magicDamage;


	T physicalCriticalChance;
	T magicalCriticalChance;
    T additionalGoldChance;


	T defense;
	T magicDefense;

    T fireDamage;
    T waterDamage;
    T earthDamage;
    T airDamage;

    T fireResistance;
    T waterResistance;
    T earthResistance;
    T airResistance;

};


template <typename T>
using range = std::pair<T,T>;

typedef AttributesTemplate<float> Attributes;
typedef AttributesTemplate<range<float>> AttributesRange;


enum ItemType {
    helmet,
    scapular,
    chestplate,
    armlet,
    glove,
    leggings,
    boot,
    cap,
    shield,

    sword,
    axe,
    spear,
    mace,
    staff,
    wand,
    bow,

    ring,
    earing,
    jewelry,
    amulet,

    container,
    misc,
};


enum QualityType {
    Awful,
    Bad,
    Normal,
    Good,
    Excellent,
    Perfect
};

typedef uint32_t ID;

//Эта информация будет храниться в таблицах и использоваться при генерации настоящего предмета, так же эта информация будет загружаться с файлов.
struct MetaItem {
    ItemType type;

    AttributesRange scalarAttributesRange;
    AttributesRange percentAttributesRange;
    vector<vector<ID>> availablePrefixes;
    vector<vector<ID>> availableSuffixes;
    range<QualityType> qualityRange; //?
    range<float> durabilityRange;
    range<float> weightRange;
    range<float> volumeRange;
    range<float> levelRange;

    string name;
    string description;
    string frameName;
};

struct ItemComponent {
    ItemType type;
    Attributes scalarAttributes;
    Attributes percentAttributes;
    QualityType qualityType;
    float durability;
    float weight;
    float volume;

    uint32_t level;

    string description;
};

#include "Dependencies/json.hpp"
#include <fstream>
class ItemDatabase {
public:
    void load(string fileName) {
        nlohmann::json parsedItems;
        std::ifstream jsonFile(fileName);
        jsonFile >> parsedItems;

        for(auto jsonItemPair : parsedItems.items()) {
            auto jsonItem = jsonItemPair.value();

            if(!jsonItem.contains("id")) continue;

            MetaItem item;
            item.type = stringToType(jsonItem.value("item_type", "misc"));
            item.scalarAttributesRange = parseAttributesRange(jsonItem["attributes_scalar"]);
            item.percentAttributesRange = parseAttributesRange(jsonItem["attributes_percent"]);
            item.durabilityRange = jsonItem.value("durability", range<float>());
            item.weightRange = jsonItem.value("weight", range<float>());
            item.volumeRange = jsonItem.value("volume", range<float>());
            item.levelRange = jsonItem.value("item_level", range<float>());

            item.name = jsonItemPair.key();
            item.description = jsonItem.value("description", "");
            item.frameName = jsonItem.value("frameName", "");

            _items[jsonItem["id"]] = item;
            cocos2d::log("here");
        }
    }

    entt::entity createItem(entt::registry& registry, ID itemID) {
        if(!hasItem(itemID)) return entt::null;

        const MetaItem& metaItem = _items[itemID];

        entt::entity item = registry.create();
        registry.assign<Drawable>(item, EntityAnimation({metaItem.frameName}, 0.0f));
        registry.assign<ItemComponent>(item);

        ItemComponent& itemComponent = registry.get<ItemComponent>(item);
        itemComponent.type = metaItem.type;

        itemComponent.scalarAttributes = generateAttributes(metaItem.scalarAttributesRange);
        itemComponent.percentAttributes = generateAttributes(metaItem.percentAttributesRange);
        itemComponent.qualityType = QualityType(cocos2d::random(int(metaItem.qualityRange.first),
                                                                int(metaItem.qualityRange.second)));
        itemComponent.durability = random(metaItem.durabilityRange);
        itemComponent.weight = random(metaItem.weightRange);
        itemComponent.volume = random(metaItem.volumeRange);
        itemComponent.level = uint32_t(round(random(metaItem.levelRange)));
        itemComponent.description = metaItem.description;   //TODO: description parsing (e.g `{item_name} has been created by immortal titan Ian.`)

        return item;
    }


    MetaItem getItemMeta(ID itemID) {
        auto itemIter = _items.find(itemID);
        if(itemIter != _items.end()) {
            return itemIter->second;
        }

        return MetaItem();
    }

    bool hasItem(ID itemID) {
        return _items.find(itemID) != _items.end();
    }
private:
    float random(range<float> rng) {
        return cocos2d::random(rng.first, rng.second);
    }

    Attributes generateAttributes(const AttributesRange& attrs) {
        Attributes result;
        result.healthPoints = random(attrs.healthPoints);
        result.magicPoints =  random(attrs.magicPoints);
        result.strength = random(attrs.strength);
        result.intellect = random(attrs.intellect);
        result.agility = random(attrs.agility);
        result.vitality = random(attrs.vitality);
        result.charisma = random(attrs.charisma);
        result.stamina = random(attrs.stamina);

        result.stabbingDamage = random(attrs.stabbingDamage);
        result.cuttingDamage = random(attrs.cuttingDamage);
        result.chopDamage = random(attrs.chopDamage);
        result.magicDamage = random(attrs.magicDamage);

        result.physicalCriticalChance = random(attrs.physicalCriticalChance);
        result.magicalCriticalChance = random(attrs.magicalCriticalChance);
        result.additionalGoldChance = random(attrs.additionalGoldChance);

        result.defense = random(attrs.defense);
        result.magicDefense = random(attrs.magicDefense);

        result.fireDamage = random(attrs.fireDamage);
        result.waterDamage = random(attrs.waterDamage);
        result.earthDamage = random(attrs.earthDamage);
        result.airDamage = random(attrs.airDamage);

        result.fireResistance = random(attrs.fireResistance);
        result.waterResistance = random(attrs.waterResistance);
        result.earthResistance = random(attrs.earthResistance);
        result.airResistance = random(attrs.airResistance);

        return result;
    }

    ItemType stringToType(string itemType) {
        if(itemType == "helmet") return helmet;
        if(itemType == "scapular") return scapular;
        if(itemType == "chestplate") return chestplate;
        if(itemType == "armlet") return armlet;
        if(itemType == "glove") return glove;
        if(itemType == "leggings") return leggings;
        if(itemType == "boot") return boot;
        if(itemType == "cap") return cap;
        if(itemType == "shield") return shield;
        if(itemType == "sword") return sword;
        if(itemType == "axe") return axe;
        if(itemType == "spear") return spear;
        if(itemType == "mace") return mace;
        if(itemType == "staff") return staff;
        if(itemType == "wand") return wand;
        if(itemType == "bow") return bow;
        if(itemType == "ring") return ring;
        if(itemType == "earing") return earing;
        if(itemType == "jewelry") return jewelry;
        if(itemType == "amulet") return amulet;
        if(itemType == "container") return container;

        return misc;
    }

    AttributesRange parseAttributesRange(nlohmann::json& attributes) {
        AttributesRange attrRange;
        attrRange.healthPoints = attributes.value("health_points", range<float>());
        attrRange.magicPoints = attributes.value("magic_points", range<float>());

        attrRange.strength = attributes.value("strength", range<float>());
        attrRange.intellect = attributes.value("intellect", range<float>());
        attrRange.agility = attributes.value("agility", range<float>());
        attrRange.vitality = attributes.value("vitality", range<float>());
        attrRange.charisma = attributes.value("charisma", range<float>());
        attrRange.stamina = attributes.value("stamina", range<float>());

        attrRange.stabbingDamage = attributes.value("stabbing_damage", range<float>());
        attrRange.cuttingDamage = attributes.value("cutting_damage", range<float>());
        attrRange.chopDamage = attributes.value("chop_damage", range<float>());
        attrRange.magicDamage = attributes.value("magic_damage", range<float>());

        attrRange.physicalCriticalChance = attributes.value("physical_critical_chance", range<float>());
        attrRange.magicalCriticalChance = attributes.value("magical_critical_chance", range<float>());
        attrRange.additionalGoldChance = attributes.value("additional_gold_chance", range<float>());

        attrRange.defense = attributes.value("defense", range<float>());
        attrRange.magicDefense = attributes.value("magic_defense", range<float>());

        attrRange.fireDamage = attributes.value("fire_damage", range<float>());
        attrRange.waterDamage = attributes.value("water_damage", range<float>());
        attrRange.earthDamage = attributes.value("earth_damage", range<float>());
        attrRange.airDamage = attributes.value("air_damage", range<float>());

        attrRange.fireResistance = attributes.value("fire_resistance", range<float>());
        attrRange.waterResistance = attributes.value("water_resistance", range<float>());
        attrRange.earthResistance = attributes.value("earth_resistance", range<float>());
        attrRange.airResistance = attributes.value("air_resistance", range<float>());

        return attrRange;
    }

    map<ID, MetaItem> _items;
};

#endif // ITEM_H_INCLUDED
