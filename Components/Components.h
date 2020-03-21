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
	explicit Drawable(Sprite* psprite): sprite(psprite) { }
	Sprite* sprite;
};

struct Cell {
    explicit Cell(int16_t px = 0, int16_t py = 0): x(px), y(py) { }

    uint16_t x;
    uint16_t y;

    string name;
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
};

struct Controllable {

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

};

struct WorldData {
    vector<vector<vector<entt::entity>>> objects;
    vector<vector<entt::entity>> creatures;
};

#endif // COMPONENTS_H_INCLUDED
