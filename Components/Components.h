#ifndef COMPONENTS_H_INCLUDED
#define COMPONENTS_H_INCLUDED

#include "Dependencies/entt.hpp"

#include "cocos2d.h"
#include <vector>

using std::vector;
USING_NS_CC;

struct Transform {
    explicit Transform(Vec2 pos, float scle, float ang): position(pos), scale(scle), angle(ang) { }

    Vec2 position;
    float scale;
    float angle;    //TODO: Округление
};

struct DrawableShape {
    explicit DrawableShape(const vector<Vec2>& vrtecies, Color4F fllColor,Color4F brderColor): vertecies(vrtecies),
                                                                                      fillColor(fllColor),
                                                                                      borderColor(brderColor)
    {
        float minx, miny;
        float maxx, maxy;
        minx = maxx = vrtecies[0].x, miny = maxy = vrtecies[0].y;

        for(Vec2 vec : vrtecies) {
            minx = std::min(minx, vec.x);
            miny = std::min(miny, vec.y);

            maxx = std::max(maxx, vec.x);
            maxy = std::max(maxy, vec.y);
        }
        log("%f %f %f %f", minx, miny, maxx, maxy);
        float midx = (maxx + minx) * 0.5f;
        float midy = (maxy + miny) * 0.5f;

        float size = std::max(maxx - minx, maxy - miny);
        shapeRect = Rect(midx, midy, size, size);
    }
    vector<Vec2> vertecies;

    Color4F fillColor;
    Color4F borderColor;
    Rect shapeRect;
};

struct Mortal {
    explicit Mortal(int hp) : health(hp) { }
    int health;
};

struct Ship {
    explicit Ship(float shipSpeed, int mxHealth, int mxAmmo): speed(shipSpeed), maxHealth(mxHealth), maxAmmo(mxAmmo), ammo(mxAmmo), score(0) { }

    float speed;

    int maxHealth;
    int health;

    int maxAmmo;
    int ammo;

    int score;
};

struct Bullet {
    Bullet(entt::entity bulletOwner): owner(bulletOwner) { }

    entt::entity owner;
};

struct Physics {
    explicit Physics(PhysicsBody* body): physicsBody(body) { }
    PhysicsBody* physicsBody;
};

struct Particle {
    explicit Particle(float particleLivingTime): livingTime(particleLivingTime), currentTime(0.0f) { }

    float livingTime;
    float currentTime;
};

//struct Magnet

//TODO: USING TAGS
struct Meteorite {
    Meteorite() { }
};

#endif // COMPONENTS_H_INCLUDED
