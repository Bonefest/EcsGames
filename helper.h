#ifndef HELPER_H_INCLUDED
#define HELPER_H_INCLUDED

#include "Components/Components.h"
#include "common.h"

void prepareQuad(SpriteFrame* frame, Color4B color, Rect contentSize, cocos2d::V3F_C4B_T2F_Quad* outQuad);

bool isVisible(entt::registry& registry,
               Coordinate fromX, Coordinate fromY,
               Coordinate targetX, Coordinate targetY,
               uint16_t visibleRadius,
               WorldData& data);


std::string cutMessage(const std::string& message, float fontSize, float viewWidth);

bool isValidPosition(uint16_t x, uint16_t y);

bool isDirectionKeyType(KeyType type);
Vec2 directionKeyTypeToVector(KeyType type);
std::string directionVectorToString(Vec2 direction);

#endif // HELPER_H_INCLUDED
