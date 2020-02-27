#ifndef SYSTEMS_H_INCLUDED
#define SYSTEMS_H_INCLUDED

#include "cocos2d.h"
#include "Dependencies/entt.hpp"
#include "../Components/Components.h"

#include <memory>
#include <vector>

using std::vector;
using std::shared_ptr;
USING_NS_CC;

Vec2 randomPoint(Vec2 maximalPoint) {
    return Vec2(random(0.0f, maximalPoint.x),
                random(0.0f, maximalPoint.y));
}

void physicalComponentDisconnectionListener(entt::registry& registry, entt::entity entity) {
    Physics& physicsComponent = registry.get<Physics>(entity);
    physicsComponent.physicsBody->release();
}


class ISystem {
public:
    virtual void update(entt::registry& registry, float delta) = 0;
};

class DrawingSystem : public ISystem {
public:
    DrawingSystem(DrawNode* renderer) {
        _renderer = renderer;
    }

    void update(entt::registry& registry, float delta) {
        _renderer->clear();

        auto view = registry.view<DrawableShape, Transform>();
        for(auto entity : view) {
            DrawableShape& drawableShape = view.get<DrawableShape>(entity);
            Transform& transform = view.get<Transform>(entity);

            vector<Vec2> transformedVertecies(drawableShape.vertecies);
            for(Vec2& vertex : transformedVertecies) {
                vertex.scale(transform.scale);
                vertex.rotate(Vec2::ZERO, transform.angle);
                vertex.add(transform.position);
            }

            _renderer->drawPolygon(transformedVertecies.data(), transformedVertecies.size(),
                                   drawableShape.fillColor, 1.0f, drawableShape.borderColor);
        }
    }

private:
    DrawNode* _renderer;
};

class MeteorSpawnSystem : public ISystem {
public:
    MeteorSpawnSystem(int maximalMeteorsCount,
                      float spawnTime) {
        _maximalCount = maximalMeteorsCount;
        _spawnTime = spawnTime;
    }

    void update(entt::registry& registry, float delta) {
        _accumulate += delta;

        //if(entt::monostate<entt::hashed_string("Level")>() == 0)
        if(_accumulate > _spawnTime) {
            auto view = registry.view<Meteorite>();

            if(view.size() < _maximalCount) {
                auto newMeteorite = registry.create();

                float scale = random(0.5f, 1.5f);

                vector<Vec2> vertecies;
                for(float fi = 0.0f; fi < 360.0f; fi += 30.0f) {
                    vertecies.push_back(20.0f * Vec2(std::cos(M_PI * fi / 180.0f) + random(0.0f, 0.25f),
                                                     std::sin(M_PI * fi / 180.0f) + random(0.0f, 0.25f)));
                }

                registry.assign<DrawableShape>(newMeteorite, vertecies, Color4F::BLACK, Color4F::WHITE);
                registry.assign<Transform>(newMeteorite,
                                           randomPoint(Director::getInstance()->getVisibleSize()),
                                           scale, 0.0f);
                registry.assign<Meteorite>(newMeteorite);

                std::transform(vertecies.begin(), vertecies.end(), vertecies.begin(), [scale](Vec2& position){ return position * scale; });

                PhysicsBody* body = PhysicsBody::createPolygon(vertecies.data(), vertecies.size(), PhysicsMaterial(1.0f, 0.5f, 0.0f), Vec2::ZERO);

                body->setGravityEnable(false);
                body->setVelocity(randomPoint(Vec2(200.0f, 200.0f)) - Vec2(100.0f, 100.0f));

                registry.assign<Physics>(newMeteorite, body);

                _accumulate = 0.0f;
            }
        }
    }

private:
    int _maximalCount;

    float _spawnTime;
    float _accumulate;

};

class PhysicsSystem: public ISystem {
public:
    PhysicsSystem() {
        _visibleSize = Director::getInstance()->getVisibleSize();
    }

    void update(entt::registry& registry, float delta) {
        auto view = registry.view<Physics, Transform>();
        view.each([&](auto entity, Physics& physComponent, Transform& transformComponent) {
            transformComponent.angle += physComponent.physicsBody->getAngularVelocity() * delta;
            transformComponent.position += physComponent.physicsBody->getVelocity() * delta;
            if(transformComponent.position.x < -64.0f)
                transformComponent.position.x = _visibleSize.width + 64.0f;
            else if(transformComponent.position.x > _visibleSize.width + 64.0f)
                transformComponent.position.x = -64.0f;

            if(transformComponent.position.y < -64.0f)
                transformComponent.position.y = _visibleSize.height + 64.0f;
            else if(transformComponent.position.y > _visibleSize.height + 64.0f)
                transformComponent.position.y = -64.0f;

            physComponent.physicsBody->getOwner()->setPosition(transformComponent.position);
        });
    }

private:
    Size _visibleSize;
};

class BaseApplication {
public:
    void addSystem(shared_ptr<ISystem> system) {
        _systems.push_back(system);
    }

    void update(float delta) {
        for(auto system : _systems) {
            system->update(_registry, delta);
        }
    }

    entt::registry& getRegistry() { return _registry; }
private:
    entt::registry _registry;
    vector<shared_ptr<ISystem>> _systems;
};

#endif // SYSTEMS_H_INCLUDED
