#ifndef _GRIDRENDERINGSYSTEM_H_
#define _GRIDRENDERINGSYSTEM_H_

#include "cocos2d.h"

#include "../Dependencies/entt.hpp"

#include "System.h"
#include "../Components/Components.h"

USING_NS_CC;

class GridRenderingSystem: public ISystem {
public:
    virtual void update(entt::registry& registry, entt::dispatcher& dispatcher, float delta) {
        auto cellsView = registry.view<Cell, Drawable>();
        const GameSettings& settings = registry.ctx<GameSettings>();
        cellsView.each([&](entt::entity entity, Cell& cellComponent, Drawable& drawableComponent) {
            drawableComponent.sprite->setPosition(Vec2(cellComponent.x, cellComponent.y) * settings.cellSize);
            drawableComponent.sprite->setContentSize(Size(settings.cellSize, settings.cellSize));
        });
    }
};

#endif // _GRIDRENDERINGSYSTEM_H_
