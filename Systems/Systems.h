#ifndef SYSTEMS_H_INCLUDED
#define SYSTEMS_H_INCLUDED

#include "Dependencies/entt.hpp"
#include "cocos2d.h"
#include "../Components/Components.h"
#include <vector>

using std::vector;

USING_NS_CC;

class RenderingSystem {
public:
    RenderingSystem(DrawNode* drawer) {
        _drawer = drawer;
    }

    void update(float delta, entt::registry& registry) {
        _drawer->clear();

        auto view = registry.view<Renderable, Transform>();

        for(auto entity : view) {
            Renderable& renderable = view.get<Renderable>(entity);
            Transform& transform = view.get<Transform>(entity);

            _drawer->drawSolidRect(transform.position * transform.size.width,
                                   transform.position * transform.size.width+ Vec2(transform.size),
                                   renderable.color);
        }
    }

private:
    DrawNode* _drawer;
};

typedef entt::entity Entity;

class LifeIteratingSystem {
public:
    LifeIteratingSystem(int minimalLifeCount, int mapSize) {
        _accumulate = 0.0f;

        _minimalLifeCount = minimalLifeCount;
        _mapSize = mapSize;

        _cellMap = vector<vector<Entity>>(mapSize);
        for(int i = 0;i < mapSize; ++i) {
            _cellMap[i] = vector<Entity>(mapSize);
            for(int j = 0;j < mapSize; ++j)
                _cellMap[i][j] = entt::null;
        }
    }

    void update(float delta, entt::registry& registry) {
        _accumulate += delta;

        if(_accumulate >= 2.0f) {

            for(int i = 0; i < _mapSize; ++i)
                for(int j = 0;j < _mapSize; ++j)
                    _cellMap[i][j] = entt::null;

            auto view = registry.view<Cell, Transform>();
            for(auto entity : view) {
                Transform& transform = view.get<Transform>(entity);
                _cellMap[int(transform.position.y)][int(transform.position.x)] = entity;
            }

            vector<Entity> deadCells;

            cocos2d::log("%d", registry.valid(getCell(-1, -1)));

            for(int y = 0; y < _mapSize; ++y) {
                for(int x = 0; x < _mapSize; ++x) {
                    int neigboursCounter = 0;
                    for(int i = -1; i < 2; ++i) {
                        for(int j = - 1; j < 2; ++j) {
                            if(i == j) continue;
                            if(registry.valid(getCell(x + i, y + j))) neigboursCounter++;
                        }
                    }
                    Entity entity = getCell(x, y);

                    if(!registry.valid(entity) && neigboursCounter == 3) {
                        auto newEntity = registry.create();
                        registry.assign<Transform>(newEntity, Vec2(x, y), Size(32, 32));
                        registry.assign<Renderable>(newEntity, Color4F::GREEN);
                        registry.assign<Cell>(newEntity, ALIVE);


                    } else if(registry.valid(entity)) {
                        if(neigboursCounter > 3 || neigboursCounter < 2) {
                            deadCells.push_back(entity);
                        }
                    }
                }
            }

            for(auto entity : deadCells) registry.destroy(entity);

            _accumulate = 0.0f;
        }
    }

private:
    Entity getCell(int x, int y) {
        if(x < 0 || x >= _mapSize || y < 0 || y >= _mapSize) return entt::null;

        return _cellMap[y][x];
    }

    int _minimalLifeCount;
    int _mapSize;

    float _accumulate;

    vector<vector<Entity>> _cellMap;


};
#endif // SYSTEMS_H_INCLUDED
