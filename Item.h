#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED

#include <map>
#include <vector>
#include <string>


#include "Dependencies/entt.hpp"

#include "common.h"


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

enum class ItemSlot {
    head,
    shoulder,
    chestplate,
    left_wrist,
    right_wrist,
    left_glove,
    right_glove,
    left_hand,
    right_hand,
    leggings,
    left_boot,
    right_boot,
    cap,

    ring,
    earing,
    jewelry,

    backpack
};

enum ItemType {
    helmet,
    scapular,
    chestplate,
    belt,
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

//Эта информация будет храниться в таблицах и использоваться при генерации настоящего предмета, так же эта информация будет загружаться с файлов.
struct MetaItem {
    ItemType type;

    AttributesRange scalarAttributesRange;
    AttributesRange percentAttributesRange;

    AttributesRange attributesLimitationsRange;

    vector<vector<ID>> availablePrefixes;
    vector<vector<ID>> availableSuffixes;
    range<QualityType> qualityRange;
    range<float> durabilityRange;
    range<float> weightRange;
    range<float> volumeRange;
    range<float> levelRange;

    //Light light - У предмета может быть свечение. При надевании предмета или когда предмет лежит на земле, это свечение будет видно. Необходимо для факелов.

    string name;
    string description;
    string frameName;
};

struct ItemComponent {
    ItemType type;
    Attributes scalarAttributes;
    Attributes percentAttributes;
    Attributes attributesLimitationsRange;
    QualityType qualityType;
    float durability;
    float weight;
    float volume;

    uint32_t level;

    string description;
};


#include "Dependencies/json.hpp"

class ItemDatabase {
public:
    void load(string fileName);
    entt::entity createItem(entt::registry& registry, ID itemID);
    MetaItem getItemMeta(ID itemID);
    bool hasItem(ID itemID);
private:
    float random(range<float> rng);

    Attributes generateAttributes(const AttributesRange& attrs);

    ItemType stringToType(string itemType);
    AttributesRange parseAttributesRange(nlohmann::json& attributes);
    map<ID, MetaItem> _items;
};

#endif // ITEM_H_INCLUDED
