#ifndef COMPONENTS_H_INCLUDED
#define COMPONENTS_H_INCLUDED

#include "cocos2d.h"

USING_NS_CC;

enum CellStatus {
    ALIVE, DEAD
};

struct Transform {
    Transform(Vec2 pos, Size sze): position(pos), size(sze) { }

    Vec2 position;
    Size size;
};

struct Renderable {
    Renderable(Color4F clr): color(clr) { }

    Color4F color;
};

struct Cell {
    Cell(CellStatus cstatus): status(cstatus) { }

    CellStatus status;
};


#endif // COMPONENTS_H_INCLUDED
