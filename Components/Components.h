#ifndef COMPONENTS_H_INCLUDED
#define COMPONENTS_H_INCLUDED

#include "Dependencies/entt.hpp"

#include "cocos2d.h"
#include <vector>

using std::vector;
USING_NS_CC;

struct Transform {
    Transform(Vec2 pos, float scle, float ang): position(pos), scale(scle), angle(ang) { }

    Vec2 position;
    float scale;
    float angle;
};

struct DrawableShape {
    DrawableShape(const vector<Vec2>& vrtecies, Color4F fllColor,Color4F brderColor): vertecies(vrtecies),
                                                                                      fillColor(fllColor),
                                                                                      borderColor(brderColor) { }
    vector<Vec2> vertecies;

    Color4F fillColor;
    Color4F borderColor;
};

struct Physics {
    Physics(PhysicsBody* body): physicsBody(body) { }
    PhysicsBody* physicsBody;
};

//struct Magnet
//struct Particle

//TODO: USING TAGS
struct Meteorite {};

#endif // COMPONENTS_H_INCLUDED
