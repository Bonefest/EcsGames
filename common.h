#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include "cocos2d.h"
#include <string>

typedef uint32_t ID;

enum KeyType {
    MOVE_TOP_LEFT,
    MOVE_TOP,
    MOVE_TOP_RIGHT,
    MOVE_RIGHT,
    MOVE_BOTTOM_RIGHT,
    MOVE_BOTTOM,
    MOVE_BOTTOM_LEFT,
    MOVE_LEFT,

    USE,
    CANCEL,
    OPEN,
    CLOSE,
    ATTACK,
    TALK,
    INVENTORY,

    UNRESOLVED
};

namespace Constants {

    const uint16_t MAP_WIDTH  = 24;
    const uint16_t MAP_HEIGHT = 24;

    enum ViewsTags {
        GridViewTag,
        LogViewTag,
        HintViewTag,
        InventoryViewTag,
        DialogViewTag,
        ItemViewTag,
    };

    enum AnimationsTags {
        BaseAnimationTag,
        OpenedAnimationTag,
        ClosedAnimationTag,
    };


    const std::string StandardFontName = "fonts/PressStart2P.ttf";
}

struct Text {
    std::string text;
    cocos2d::Color3B textColor;
};


#endif // COMMON_H_INCLUDED
