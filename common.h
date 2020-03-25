#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <string>

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

    UNRESOLVED
};


namespace Constants {

    enum ViewsTags {
        GridViewTag,
        LogViewTag,
        HintViewTag,
        InventoryViewTag,
    };

    const std::string StandardFontName = "fonts/PressStart2P.ttf";
}


#endif // COMMON_H_INCLUDED
