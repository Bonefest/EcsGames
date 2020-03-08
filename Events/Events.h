#ifndef EVENTS_H_INCLUDED
#define EVENTS_H_INCLUDED

#include "cocos2d.h"

//Можно заменить одной структурой, но я решил оставить так для возможности разделения событий на 2 метода
struct KeyPressedEvent {
    explicit KeyPressedEvent(cocos2d::EventKeyboard::KeyCode keyCode): key(keyCode) { }

    cocos2d::EventKeyboard::KeyCode key;
};

struct KeyReleasedEvent {
    explicit KeyReleasedEvent(cocos2d::EventKeyboard::KeyCode keyCode): key(keyCode) { }

    cocos2d::EventKeyboard::KeyCode key;
};


struct CollisionBeginEvent {
    explicit CollisionBeginEvent(entt::entity enttA, entt::entity enttB, cocos2d::Vec2 physContactPoint): entityA(enttA), entityB(enttB),
                                                                                                          contactPoint(physContactPoint) { }

    entt::entity entityA;
    entt::entity entityB;

    cocos2d::Vec2 contactPoint;
};

struct CreateParticlesEvent {
    explicit CreateParticlesEvent(int count,
                                  cocos2d::Vec2 position,
                                  float minSize, float maxSize,
                                  float livingTime,
                                  float maxSpeed,
                                  cocos2d::Color4F particleColor) :

                                  particlesAmount(count), initialPosition(position),
                                  minimalSize(minSize), maximalSize(maxSize),
                                  maximalLivingTime(livingTime), maximalSpeed(maxSpeed), color(particleColor) { }

    int particlesAmount;
    cocos2d::Vec2 initialPosition;
    float minimalSize;
    float maximalSize;
    float maximalLivingTime;
    float maximalSpeed;
    cocos2d::Color4F color;
};

#endif // EVENTS_H_INCLUDED
