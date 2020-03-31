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
	T agillity;
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


	T defence;
	T magicDefence;

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
    range<float> itemLevelRange;

    string name;
    string description;
    string frameName;
};

struct ItemComponent {
    ItemType type;
    Attributes scalarAttributes;
    Attributes percentAttributes;
};

#include "Dependencies/json.hpp"
#include <fstream>
class ItemDatabase {
public:
    void load(string fileName) {
        nlohmann::json parsedItems;
        std::ifstream jsonFile(fileName);
        jsonFile >> parsedItems;

        for(auto jsonItem : parsedItems) {
            if(!jsonItem.contains("ID")) continue;

            MetaItem item;
            item.type = stringToType(jsonItem.value("ItemType", "misc"));
            item.scalarAttributesRange = parseAttributesRange(jsonItem["AttributesScalar"]);

        }
    }
    entt::entity createItem(ID itemID);
    MetaItem getItemMeta(ID itemID);

private:
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
    }

    map<ID, MetaItem> _items;
};

#endif // ITEM_H_INCLUDED
