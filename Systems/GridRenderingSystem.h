#ifndef _GRIDRENDERINGSYSTEM_H_
#define _GRIDRENDERINGSYSTEM_H_

#include "cocos2d.h"

#include "../Dependencies/entt.hpp"

#include "System.h"
#include "../Components/Components.h"

#include "../DrawTextureNode.h"

#include <list>

USING_NS_CC;
using std::list;

class GridRenderingSystem: public ISystem {
public:
    GridRenderingSystem(Node* container) {
        _drawer = DrawTextureNode::createDrawer();
        container->addChild(_drawer);

    }

    virtual void update(entt::registry& registry, entt::dispatcher& dispatcher, float delta) {

        _drawer->clear();

        auto cellsView = registry.view<Cell, Drawable>();
        const GameSettings& settings = registry.ctx<GameSettings>();

        cellsView.each([&](entt::entity entity, Cell& cellComponent, Drawable& drawableComponent) {

            _drawer->drawSpriteFrame(drawableComponent.currentFrame,
                                 Vec2(cellComponent.x, cellComponent.y) * settings.cellSize,
                                 cellComponent.z,
                                 Size(settings.cellSize,settings.cellSize),
                                 drawableComponent.color);

        });

    }

private:
    DrawTextureNode* _drawer;

};



#endif // _GRIDRENDERINGSYSTEM_H_
