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

            auto lightsView = registry.view<Cell, Light>();

            cellsView.each([&](entt::entity entity, Cell& cellComponent, Drawable& drawableComponent) {

                string frameName = "";
                Color4B finalColor = settings.ambientColor;

                if(isVisible(registry, cellComponent.x, cellComponent.y, playerCell.x, playerCell.y, 100, data)) {

                    Vec3 totalLightColor = Vec3::ZERO;
                    lightsView.each([&](entt::entity light, Cell& lightCellComponent, Light& lightComponent) {
                        if(isVisible(registry, cellComponent.x, cellComponent.y,
                                               lightCellComponent.x, lightCellComponent.y, lightComponent.radius, data)) {
                            float distanceToLight = Vec2(cellComponent.x, cellComponent.y).distance(Vec2(lightCellComponent.x, lightCellComponent.y));

                            float k = (1.0 - distanceToLight / float(lightComponent.radius)); //light power is unused!
                            log("%f %f", distanceToLight, k);
                            totalLightColor.x += lightComponent.intensity.r * k;
                            totalLightColor.y += lightComponent.intensity.g * k;
                            totalLightColor.z += lightComponent.intensity.b * k;
                        }
                    });

                    totalLightColor = totalLightColor * 255;
                    finalColor.r = std::min<int>(totalLightColor.x + finalColor.r, 255);
                    finalColor.g = std::min<int>(totalLightColor.y + finalColor.g, 255);
                    finalColor.b = std::min<int>(totalLightColor.z + finalColor.b, 255);

                    controllableComponent.discoveredBlocks[cellComponent.y][cellComponent.x] = drawableComponent.currentFrame;

                    frameName = drawableComponent.currentFrame;

                } else {
                    frameName = controllableComponent.discoveredBlocks[cellComponent.y][cellComponent.x];
                }

               _drawer->drawSpriteFrame(frameName,
                                 Vec2(cellComponent.x, cellComponent.y) * settings.cellSize,
                                 cellComponent.z,
                                 Size(settings.cellSize,settings.cellSize),
                                 finalColor);


            });

        }
    }

private:

    DrawTextureNode* _drawer;

};



#endif // _GRIDRENDERINGSYSTEM_H_
