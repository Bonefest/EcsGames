#ifndef _GRIDRENDERINGSYSTEM_H_
#define _GRIDRENDERINGSYSTEM_H_

#include "cocos2d.h"
#include "ui/UIScrollView.h"

#include "../Dependencies/entt.hpp"

#include "System.h"
#include "../Components/Components.h"

#include "../DrawTextureNode.h"

#include "../helper.h"

#include <list>

USING_NS_CC;
using std::list;

class GridRenderingView: public ISystem {
public:
    GridRenderingView(Vec2 position, Size viewportSize, Size worldSize) {
        _container = cocos2d::ui::ScrollView::create();
        _container->setContentSize(viewportSize);
        _container->setInnerContainerSize(worldSize);
        _container->setPosition(position);
        _container->setDirection(cocos2d::ui::ScrollView::Direction::BOTH);
        _container->setScrollBarEnabled(false);

        cocos2d::Director::getInstance()->getRunningScene()->addChild(_container);

        _drawer = DrawTextureNode::createDrawer();
        _container->addChild(_drawer);

    }

    ~GridRenderingView() {
        _drawer->removeFromParentAndCleanup(true);
        _container->removeFromParentAndCleanup(true);
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

            const GameSettings& settings = registry.ctx<GameSettings>();
            WorldData& data = registry.ctx<WorldData>();


            for(uint16_t y = 0; y < Constants::MAP_HEIGHT; ++y) {
                for(uint16_t x = 0; x < Constants::MAP_WIDTH; ++x) {

                    drawCell(registry, data.floor[y][x], x, y, 0, controllableComponent, playerCell);;

                    for(auto item : data.items[y][x]) {
                        drawCell(registry, item, x, y, 1, controllableComponent, playerCell);
                    }

                    for(auto object : data.objects[y][x]) {
                        drawCell(registry, object, x, y, 2, controllableComponent, playerCell);
                    }

                    drawCell(registry, data.creatures[y][x], x, y, 3, controllableComponent, playerCell);
                }
            }
        }
    }


private:
    void drawCell(entt::registry& registry, entt::entity cell,
                  Coordinate x, Coordinate y, Coordinate z,
                  Controllable& controllableComponent, Cell& playerCell) {

        if(cell == entt::null) return;

        GameSettings& settings = registry.ctx<GameSettings>();
        WorldData& data = registry.ctx<WorldData>();

        string frameName = "";

        Color4B finalColor = settings.ambientColor;

        auto lightsView = registry.view<Cell, Light>();

        if(isVisible(registry, x, y, playerCell.x, playerCell.y, 100, data)) {

            Drawable& drawableComponent = registry.get<Drawable>(cell);

            Vec3 totalLightColor = Vec3::ZERO;
            lightsView.each([&](entt::entity light, Cell& lightCellComponent, Light& lightComponent) {
                if(isVisible(registry, x, y, lightCellComponent.x, lightCellComponent.y, lightComponent.radius, data)) {
                    float distanceToLight = Vec2(x, y).distance(Vec2(lightCellComponent.x, lightCellComponent.y));

                    float k = (1.0 - distanceToLight / float(lightComponent.radius)); //light power is unused!
                    totalLightColor.x += lightComponent.intensity.r * k;
                    totalLightColor.y += lightComponent.intensity.g * k;
                    totalLightColor.z += lightComponent.intensity.b * k;
                }
            });

            totalLightColor = totalLightColor * 255;
            finalColor.r = std::min<int>(totalLightColor.x + finalColor.r, 255);
            finalColor.g = std::min<int>(totalLightColor.y + finalColor.g, 255);
            finalColor.b = std::min<int>(totalLightColor.z + finalColor.b, 255);

            frameName = drawableComponent.animations.getCurrentFrame();

            controllableComponent.discoveredBlocks[y][x] = frameName;

        } else {
            frameName = controllableComponent.discoveredBlocks[y][x];
        }

       _drawer->drawSpriteFrame(frameName, Vec2(x, y) * settings.cellSize, z, Size(settings.cellSize,settings.cellSize), finalColor);
    }

    cocos2d::ui::ScrollView* _container;
    DrawTextureNode* _drawer;

};



#endif // _GRIDRENDERINGSYSTEM_H_
