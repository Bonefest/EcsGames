#ifndef COMPONENTS_H_INCLUDED
#define COMPONENTS_H_INCLUDED

#include "cocos2d.h"
#include "../Item.h"
#include "../common.h"
#include "../Animation.h"
#include "../StateSprite.h"
#include "../EntityRelationStatus.h"

#include "../Dependencies/entt.hpp"

#include <string>
#include <vector>
#include <array>

using std::string;
using std::vector;
using std::array;

using namespace cocos2d;

struct Drawable {
	explicit Drawable(EntityAnimation animation): color(Color4B::WHITE) {
        animations.setAnimation(Constants::AnimationsTags::BaseAnimationTag, animation);
        animations.setCurrentState(Constants::AnimationsTags::BaseAnimationTag);
	}

	Color4B color;

    AnimationsContainer animations;
};

typedef uint16_t Coordinate;

struct Cell {
    explicit Cell(int16_t px = 0, int16_t py = 0, uint16_t pz = 0): x(px), y(py), z(pz), passable(true), transparent(true) { }

    Coordinate x;
    Coordinate y;
    Coordinate z;

    bool passable;
    bool transparent;
    bool visible;

    string name;
};

struct Light {
    explicit Light(Color4F lightIntensity, float lightPower, uint32_t lightRadius): intensity(lightIntensity),
                                                                                    power(lightPower),
                                                                                    radius(lightRadius) { }
    Color4F intensity;
    float power;
    uint32_t radius;
};



struct Creature {

    Attributes baseAttributes;
    Attributes currentAttributes;

    map<ItemType, entt::entity> wornItems;
    vector<entt::entity> inventoryItems;

    float maxInventoryVolume;
    float maxInventoryWeight;

    float currentInventoryVolume;
    float currentInventoryWeight;

    uint32_t penaltyTime;
    uint32_t viewDistance;
};

struct DynamicObject {

    float movementSpeed;
    Vec2 direction;
};

struct Controllable {
    vector<vector<std::string>> discoveredBlocks;   //TODO: map<level, vector<vector<std::string>>
};

struct Destroyable {
    Destroyable(float maximalHP, float regenSpeed, float objDefence): maximalHealth(maximalHP),
                                                                      currentHealth(maximalHP),
                                                                      regenerationSpeed(regenSpeed) { }

    float maximalHealth;
    float currentHealth;

    float regenerationSpeed;

    float defense;
};

struct Lockable {

    Lockable(uint8_t level): lockLevel(level) { }

    uint8_t lockLevel;

};

struct Speakable {
    explicit Speakable() { }

    ID dialogID;
};

struct GameSettings {
    GameSettings(float size, uint16_t gwidth, uint16_t gheight, Color4B ambColor): cellSize(32.0f),
                                                                                   gridWidth(gwidth),
                                                                                   gridHeight(gheight),
                                                                                   ambientColor(ambColor) { }

    float cellSize;

    uint16_t gridWidth;
    uint16_t gridHeight;

    Color4B ambientColor;

};

template <typename T>
using gameMap = array<array<T, Constants::MAP_WIDTH>, Constants::MAP_HEIGHT>;

struct WorldData {
    gameMap<entt::entity> floor;
    gameMap<vector<entt::entity>> objects;
    gameMap<vector<entt::entity>> items;
    gameMap<entt::entity> creatures;
};

#endif // COMPONENTS_H_INCLUDED
