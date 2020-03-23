#ifndef HELPER_H_INCLUDED
#define HELPER_H_INCLUDED

#include "Components/Components.h"

void prepareQuad(SpriteFrame* frame, Color4B color, Rect contentSize, cocos2d::V3F_C4B_T2F_Quad* outQuad);

bool isVisible(entt::registry& registry,
               Coordinate fromX, Coordinate fromY,
               Coordinate targetX, Coordinate targetY,
               uint16_t visibleRadius,
               WorldData& data);
#endif // HELPER_H_INCLUDED
