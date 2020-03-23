#ifndef COMPONENTS_H_INCLUDED
#define COMPONENTS_H_INCLUDED

#include "cocos2d.h"
#include "../StateSprite.h"

#include <string>
#include <vector>

using std::string;
using std::vector;

USING_NS_CC;
struct Drawable {
	explicit Drawable(const string& frameName): currentFrame(frameName), color(Color4B::WHITE) {
        //cocos2d::SpriteFrame* frame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName("Floor.png");

	}

    string currentFrame;
	Color4B color;
};

typedef uint16_t Coordinate;

struct Cell {
    explicit Cell(int16_t px = 0, int16_t py = 0, uint16_t pz = 0): x(px), y(py), z(pz), passable(true) { }

    Coordinate x;
    Coordinate y;
    Coordinate z;

    bool passable;

    string name;
};

struct PointLight {
    explicit PointLight(Color4B lightIntensity, float lightPower, uint32_t lightRadius): intensity(lightIntensity),
                                                                                         power(lightPower),
                                                                                         radius(lightRadius) { }
    Color4B intensity;
    float power;
    uint32_t radius;
};

struct Creature {

    uint8_t level;
    uint32_t experience;

    float magic;

    float movementSpeed;
    float attackSpeed;

    uint16_t strength;
    uint16_t agillity;
    uint16_t intellect;

    uint32_t penaltyTime;
    uint32_t viewDistance;
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

struct GameSettings {
    GameSettings(float size, uint16_t gwidth, uint16_t gheight): cellSize(32.0f),
                                                                 gridWidth(gwidth),
                                                                 gridHeight(gheight) { }

    float cellSize;

    uint16_t gridWidth;
    uint16_t gridHeight;

    Color4B ambientColor;

};

struct WorldData {
    vector<vector<vector<entt::entity>>> objects;
    vector<vector<entt::entity>> creatures;
};

#endif // COMPONENTS_H_INCLUDED
