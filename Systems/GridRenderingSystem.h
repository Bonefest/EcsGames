#ifndef _GRIDRENDERINGSYSTEM_H_
#define _GRIDRENDERINGSYSTEM_H_

#include "cocos2d.h"

#include "../Dependencies/entt.hpp"

#include "System.h"
#include "../Components/Components.h"

#include "../DrawTextureNode.h"

#include "../helper.h"

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

        auto playerView = registry.view<Controllable>();
        entt::entity player = entt::null;
        for(auto entity: playerView) {
            player = entity; break;
        }

        if(player != entt::null) {

            Cell& playerCell = registry.get<Cell>(player);
            Controllable& controllableComponent = registry.get<Controllable>(player);

            auto cellsView = registry.view<Cell, Drawable>();
            const GameSettings& settings = registry.ctx<GameSettings>();
            WorldData& data = registry.ctx<WorldData>();

//            auto lightsView = registry.view<Cell, PointLight>();

            cellsView.each([&](entt::entity entity, Cell& cellComponent, Drawable& drawableComponent) {

                if(isVisible(registry, cellComponent.x, cellComponent.y, playerCell.x, playerCell.y, 5, data)) {
                    float distance = sqrt((cellComponent.x - playerCell.x)*(cellComponent.x - playerCell.x) +
                                     (cellComponent.y - playerCell.y)*(cellComponent.y - playerCell.y));

                    Color4B color = drawableComponent.color;
                    color.r -= (200 / 5) * distance;
                    color.g -= (200 / 5) * distance;
                    color.b -= (200 / 5) * distance;
                    _drawer->drawSpriteFrame(drawableComponent.currentFrame,
                                         Vec2(cellComponent.x, cellComponent.y) * settings.cellSize,
                                         cellComponent.z,
                                         Size(settings.cellSize,settings.cellSize),
                                         color);

                    controllableComponent.discoveredBlocks[cellComponent.y][cellComponent.x] = drawableComponent.currentFrame;

                } else {
                    string frameName = controllableComponent.discoveredBlocks[cellComponent.y][cellComponent.x];
                    if(frameName != "") {
                        _drawer->drawSpriteFrame(frameName, Vec2(cellComponent.x, cellComponent.y) * settings.cellSize, cellComponent.z, Size(settings.cellSize,settings.cellSize), Color4B(40, 40, 40, 255));
                    }
                }


            });

        }
    }

private:

    DrawTextureNode* _drawer;

};



#endif // _GRIDRENDERINGSYSTEM_H_
