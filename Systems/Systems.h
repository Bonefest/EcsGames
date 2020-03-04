#ifndef SYSTEMS_H_INCLUDED
#define SYSTEMS_H_INCLUDED

#include "cocos2d.h"
#include "Dependencies/entt.hpp"
#include "../Events/Events.h"
#include "../Components/Components.h"

#include <memory>
#include <vector>
#include <map>

using std::map;
using std::vector;
using std::shared_ptr;
USING_NS_CC;

Vec2 randomPoint(Vec2 maximalPoint) {
    return Vec2(random(0.0f, maximalPoint.x),
                random(0.0f, maximalPoint.y));
}

float toRad(float degrees) {
    return (degrees * M_PI) / 180.0f;
}

float toDeg(float radians) {
    return (radians * 180.0f) / M_PI;
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
                    vertecies.push_back(20.0f * Vec2(std::cos(toRad(fi)) + random(0.0f, 0.25f),
                                                     std::sin(toRad(fi)) + random(0.0f, 0.25f)));
                }

                registry.assign<DrawableShape>(newMeteorite, vertecies, Color4F::BLACK, Color4F::WHITE);
                registry.assign<Transform>(newMeteorite,
                                           randomPoint(Director::getInstance()->getVisibleSize()),
                                           scale, 0.0f);
                registry.assign<Meteorite>(newMeteorite);

                std::transform(vertecies.begin(), vertecies.end(), vertecies.begin(), [scale](Vec2& position){ return position * scale; });

                PhysicsBody* body = PhysicsBody::createPolygon(vertecies.data(), vertecies.size(), PhysicsMaterial(1.0f, 0.5f, 0.0f), Vec2::ZERO);
                body->setContactTestBitmask(0xFFFFFFFF);
                body->setTag(int(newMeteorite));
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


        auto shipView = registry.view<Ship>();
        for(auto ship : shipView) {

            Ship& shipComponent = shipView.get<Ship>(ship);

            vector<entt::entity> outsideBullets;

            auto bulletsView = registry.view<Bullet, Transform>();

            for(auto bullet : bulletsView) {
                Transform& transform = bulletsView.get<Transform>(bullet);

               if(transform.position.x <= -64.0f || transform.position.x >= _visibleSize.width + 64.0f ||
                  transform.position.y <= -64.0f || transform.position.y >= _visibleSize.height + 64.0f) {
                    outsideBullets.push_back(bullet);
                    shipComponent.ammo++;
                }
            }

            for(auto& bullet : outsideBullets) {
                registry.destroy(bullet);
            }
        }
    }

private:
    Size _visibleSize;
};

class PlayerControllSystem : public ISystem {
public:
    //vector<pair<hashed_string, configuration>> playersData;
    explicit PlayerControllSystem(entt::hashed_string playerTag, entt::dispatcher& dispatcher /*, KeyConfig keyConfig */) {
        _playerTag = playerTag;

        dispatcher.sink<KeyPressedEvent>().connect<&PlayerControllSystem::onKeyPressed>(*this);
        dispatcher.sink<KeyReleasedEvent>().connect<&PlayerControllSystem::onKeyReleased>(*this);
    }

    virtual void update(entt::registry& registry, float delta) {
        auto view = registry.view<entt::tag<"player"_hs>, Transform, Physics, Ship>();

        for(auto player : view) {
            if(registry.valid(player)) {
                Transform& transformComponent = registry.get<Transform>(player);
                Physics& physicsComponent = registry.get<Physics>(player);
                Ship& shipComponent = registry.get<Ship>(player);

                Vec2 bodyVelocity = physicsComponent.physicsBody->getVelocity();
                Vec2 velocity = Vec2::ZERO;
                Vec2 opposite = Vec2::ZERO;

                if(_pressedKeys[EventKeyboard::KeyCode::KEY_A]) transformComponent.angle += delta * 5;
                if(_pressedKeys[EventKeyboard::KeyCode::KEY_D]) transformComponent.angle -= delta * 5;
                if(_pressedKeys[EventKeyboard::KeyCode::KEY_W]) {
                    velocity.x = std::cos(transformComponent.angle) * shipComponent.speed;
                    velocity.y = std::sin(transformComponent.angle) * shipComponent.speed;
                } else {
                    if(abs(bodyVelocity.x) > 0.001) opposite.x = -bodyVelocity.x / abs(bodyVelocity.x) * delta * shipComponent.speed * 2;
                    if(abs(bodyVelocity.y) > 0.001) opposite.y = -bodyVelocity.y / abs(bodyVelocity.y) * delta * shipComponent.speed * 2;

                }

                physicsComponent.physicsBody->getOwner()->setRotation(toDeg(-transformComponent.angle));

                physicsComponent.physicsBody->setVelocity(velocity +  bodyVelocity + opposite);

                if(_pressedKeys[EventKeyboard::KeyCode::KEY_SPACE]){
                        _pressedKeys[EventKeyboard::KeyCode::KEY_SPACE] = false;
                        onPlayerShooted(player, registry);
                }

            }
        }
    }

    void onKeyPressed(const KeyPressedEvent& event) {
        _pressedKeys[event.key] = true;
    }

    void onKeyReleased(const KeyReleasedEvent& event) {
        _pressedKeys[event.key] = false;
    }

private:
    void onPlayerShooted(entt::entity player, entt::registry& registry) {
        Transform& transformComponent = registry.get<Transform>(player);
        Physics& physicsComponent = registry.get<Physics>(player);
        Ship& shipComponent = registry.get<Ship>(player);

        if(shipComponent.ammo > 0) {
            entt::entity bullet = registry.create();

            //TODO : FACTORY
            vector<Vec2> bulletVerticies = {
                Vec2(-1.0f, 1.0f),
                Vec2( 1.0f, 1.0f),
                Vec2( 1.0f,-1.0f),
                Vec2(-1.0f,-1.0f)
            };

            PhysicsBody* bulletBody = PhysicsBody::createPolygon(bulletVerticies.data(), bulletVerticies.size(), PhysicsMaterial(1.0f, 0.5f, 0.0f), Vec2::ZERO);
            bulletBody->setContactTestBitmask(0xFFFFFFFF);
            bulletBody->setTag(int(bullet));
            bulletBody->setGravityEnable(false);
            bulletBody->setVelocity(physicsComponent.physicsBody->getVelocity().getNormalized() * 200);

            registry.assign<Bullet>(bullet);
            registry.assign<DrawableShape>(bullet, bulletVerticies, Color4F::WHITE, Color4F::WHITE);
            registry.assign<Transform>(bullet, transformComponent.position + Vec2(cos(transformComponent.angle), sin(transformComponent.angle)), 1.0f, transformComponent.angle);
            registry.assign<Physics>(bullet, bulletBody);

            shipComponent.ammo--;
        }
    }

    entt::hashed_string _playerTag;
    map<EventKeyboard::KeyCode, bool> _pressedKeys;

};

class BulletCollisionSystem : public ISystem {
public:
    BulletCollisionSystem(entt::dispatcher& dispatcher) {
        dispatcher.sink<CollisionBeginEvent>().connect<&BulletCollisionSystem::onCollisionBegin>(*this);
    }

    virtual void update(entt::registry& registry, float delta) {
        for(CollisionBeginEvent event : _collisionEvents) {
            if(registry.valid(event.entityA) && registry.valid(event.entityB)) {
                entt::entity bullet = entt::null, entity;
                if(registry.has<Bullet>(event.entityA)) { bullet = event.entityA; entity = event.entityB; }
                else if(registry.has<Bullet>(event.entityB)) { bullet = event.entityB; entity = event.entityA; }

                if(registry.valid(bullet) && !registry.has<entt::tag<"player"_hs>>(entity)) {
                    registry.destroy(bullet);
                    registry.destroy(entity);
                }

            }
        }

        _collisionEvents.clear();
    }

    void onCollisionBegin(const CollisionBeginEvent& event) {
        _collisionEvents.push_back(event);
    }

private:
    vector<CollisionBeginEvent> _collisionEvents;
};

class InputHandler {
public:
    InputHandler(Node* scene, EventDispatcher* dispatcher, entt::dispatcher& edispatcher):_dispatcher(edispatcher) {
        auto klistener = EventListenerKeyboard::create();

        klistener->onKeyPressed = CC_CALLBACK_2(InputHandler::onKeyPressed, this);
        klistener->onKeyReleased = CC_CALLBACK_2(InputHandler::onKeyReleased, this);

        dispatcher->addEventListenerWithSceneGraphPriority(klistener, scene);
    }

    void onKeyPressed(EventKeyboard::KeyCode key, Event* event) {
        //_dispatcher.trigger<KeyPressedEvent>(key, currentTime);
        _dispatcher.trigger<KeyPressedEvent>(key);
    }

    void onKeyReleased(EventKeyboard::KeyCode key, Event* event) {
        _dispatcher.trigger<KeyReleasedEvent>(key);
    }

private:
    entt::dispatcher& _dispatcher;
};

class CollisionHandler {
public:
    CollisionHandler(Node* scene, EventDispatcher* dispatcher, entt::dispatcher& edispatcher):_dispatcher(edispatcher) {

        auto contactListener = EventListenerPhysicsContact::create();
        contactListener->onContactBegin = CC_CALLBACK_1(CollisionHandler::onCollisionBegin, this);

        dispatcher->addEventListenerWithSceneGraphPriority(contactListener, scene);
    }

    bool onCollisionBegin(PhysicsContact& contact) {
        _dispatcher.trigger<CollisionBeginEvent>(entt::registry::entity_type(contact.getShapeA()->getBody()->getTag()),
                                                 entt::registry::entity_type(contact.getShapeB()->getBody()->getTag()));
        return true;
    }

private:
    entt::dispatcher& _dispatcher;
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
    entt::dispatcher& getDispatcher() { return _dispatcher; }

private:
    entt::registry _registry;
    entt::dispatcher _dispatcher;
    vector<shared_ptr<ISystem>> _systems;
};

#endif // SYSTEMS_H_INCLUDED
