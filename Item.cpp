#include "Item.h"
#include "Components/Components.h"
#include <fstream>


void ItemDatabase::load(string fileName) {
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
        item.attributesLimitationsRange = parseAttributesRange(jsonItem["attributes_limitations"]);
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

entt::entity ItemDatabase::createItem(entt::registry& registry, ID itemID) {
    if(!hasItem(itemID)) return entt::null;

    const MetaItem& metaItem = _items[itemID];

    entt::entity item = registry.create();
    registry.assign<Drawable>(item, EntityAnimation({metaItem.frameName}, 0.0f));
    registry.assign<ItemComponent>(item);

    ItemComponent& itemComponent = registry.get<ItemComponent>(item);
    itemComponent.type = metaItem.type;

    itemComponent.scalarAttributes = generateAttributes(metaItem.scalarAttributesRange);
    itemComponent.percentAttributes = generateAttributes(metaItem.percentAttributesRange);
    itemComponent.attributesLimitations = generateAttributes(metaItem.attributesLimitationsRange);
    itemComponent.qualityType = QualityType(cocos2d::random(int(metaItem.qualityRange.first),
                                                            int(metaItem.qualityRange.second)));
    itemComponent.durability = random(metaItem.durabilityRange);
    itemComponent.weight = random(metaItem.weightRange);
    itemComponent.volume = random(metaItem.volumeRange);
    itemComponent.level = uint32_t(round(random(metaItem.levelRange)));
    itemComponent.name = metaItem.name;
    itemComponent.description = metaItem.description;   //TODO: description parsing (e.g `{item_name} has been created by immortal titan Ian.`)

    return item;
}


MetaItem ItemDatabase::getItemMeta(ID itemID) {
    auto itemIter = _items.find(itemID);
    if(itemIter != _items.end()) {
        return itemIter->second;
    }

    return MetaItem();
}

bool ItemDatabase::hasItem(ID itemID) {
    return _items.find(itemID) != _items.end();
}

float ItemDatabase::random(range<float> rng) {
    return cocos2d::random(rng.first, rng.second);
}

Attributes ItemDatabase::generateAttributes(const AttributesRange& attrs) {
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

ItemType ItemDatabase::stringToType(string itemType) {
    if(itemType == "helmet") return helmet;
    if(itemType == "scapular") return scapular;
    if(itemType == "chestplate") return chestplate;
    if(itemType == "armlet") return armlet;
    if(itemType == "glove") return glove;
    if(itemType == "leggings") return leggings;
    if(itemType == "boot") return boot;
    if(itemType == "cap") return cap;
    if(itemType == "belt") return belt;
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

AttributesRange ItemDatabase::parseAttributesRange(nlohmann::json& attributes) {
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
