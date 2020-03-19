#ifndef SYSTEMS_H_INCLUDED
#define SYSTEMS_H_INCLUDED

#include "cocos2d.h"
#include "Dependencies/entt.hpp"
#include "../Events/Events.h"
#include "../Components/Components.h"
#include "Minimap.h"

#include <memory>
#include <vector>
#include <map>
#include <list>

using std::map;
using std::pair;
using std::list;
using std::vector;
using std::shared_ptr;
USING_NS_CC;

Rect worldRect(-2048, -2048, 4096, 4096);

const float HP_SIZE = 10.0f;

Rect getVisibleRect() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 cameraPosition = Director::getInstance()->getRunningScene()->getDefaultCamera()->getPosition();

    Rect visibleRect(cameraPosition.x - visibleSize.width * 0.5f,
                     cameraPosition.y - visibleSize.height * 0.5f,
                     visibleSize.width,
                     visibleSize.height);

    return visibleRect;
}

Vec2 randomPoint(Vec2 maximalPoint) {
    return Vec2(random(0.0f, maximalPoint.x),
                random(0.0f, maximalPoint.y));
}

vector<Vec2> createRectangle(Size size) {
        vector<Vec2> verticies = {
            Vec2(-size.width / 2, size.height / 2),
            Vec2( size.width / 2, size.height / 2),
            Vec2( size.width / 2,-size.height / 2),
            Vec2(-size.width / 2,-size.height / 2)
        };

        return verticies;
}

float toRad(float degrees) {
    return (degrees * M_PI) / 180.0f;
}

float toDeg(float radians) {
    return (radians * 180.0f) / M_PI;
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
//                    transform.position + Vec2(drawableShape.shapeRect.size.width * 0.5f, drawableShape.shapeRect.size.height * 0.5f) , Color4F::GREEN);
        }
    }

private:
    DrawNode* _renderer;
};

class HealthBarDrawingSystem: public ISystem {
public:
    HealthBarDrawingSystem(DrawNode* renderer) {
        _renderer = renderer;
    }
    virtual void update(entt::registry& registry, float delta) {
        auto view = registry.view<DrawableShape, Transform, Mortal>();
        view.each([&](DrawableShape& shape, Transform& transform, Mortal& mortal) {
            if(mortal.health > 0) {
                Rect srect = shape.shapeRect;
                Vec2 minMidPoint = transform.position + Vec2(0.0f, srect.getMinY() * 1.5f) * transform.scale;
                Vec2 hpCellVertPos = minMidPoint - (float(mortal.health) / 2.0f) * Vec2(HP_SIZE, 0) + Vec2(HP_SIZE * 0.5f, 0);
                for(int i = 0;i < mortal.health; ++i) {
                    _renderer->drawTriangle(hpCellVertPos - Vec2(HP_SIZE * 0.5f, 0),
                                            hpCellVertPos + Vec2(HP_SIZE * 0.5f, 0),
                                            hpCellVertPos - Vec2(0, HP_SIZE * 0.5f),
                                            Color4F::RED);
                    hpCellVertPos += Vec2(HP_SIZE, 0);
                }
            }
        });
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
                Size visibleSize = Director::getInstance()->getVisibleSize();
                auto newMeteorite = registry.create();

                float scale = random(0.5f, 1.5f);

                vector<Vec2> vertecies;
                for(float fi = 0.0f; fi < 360.0f; fi += 30.0f) {
                    vertecies.push_back(20.0f * Vec2(std::cos(toRad(fi)) + random(0.0f, 0.25f),
                                                     std::sin(toRad(fi)) + random(0.0f, 0.25f)));
                }

                registry.assign<Mortal>(newMeteorite, random(3, 5));
                registry.assign<DrawableShape>(newMeteorite, vertecies, Color4F::BLACK, Color4F::WHITE);
                registry.assign<Transform>(newMeteorite, generatePosition(), scale, 0.0f);

                std::transform(vertecies.begin(), vertecies.end(), vertecies.begin(), [scale](Vec2& position){ return position * scale; });

                PhysicsBody* body = PhysicsBody::createPolygon(vertecies.data(), vertecies.size(), PhysicsMaterial(1.0f, 0.5f, 0.0f), Vec2::ZERO);
                body->setContactTestBitmask(0xFFFFFFFF);
                body->setTag(int(newMeteorite));
                body->setGravityEnable(false);
                body->setVelocity(randomPoint(Vec2(200.0f, 200.0f)) - Vec2(100.0f, 100.0f));
                registry.assign<Physics>(newMeteorite, body);

                registry.assign<Meteorite>(newMeteorite);
                registry.assign<MinimapTarget>(newMeteorite, Shape::CIRCLE, Color4F::RED);



                _accumulate = 0.0f;
            }
        }
    }

private:
    Vec2 generatePosition() {
        Rect visibleRect = getVisibleRect();
        Vec2 position = visibleRect.origin;
        while(visibleRect.containsPoint(position)) {
            position.x = random(worldRect.getMinX(), worldRect.getMaxX());
            position.y = random(worldRect.getMinY(), worldRect.getMaxY());
        }

        return position;
    }

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
        Rect visibleRect = getVisibleRect();

        auto view = registry.view<Physics, Transform>();
        view.each([&](auto entity, Physics& physComponent, Transform& transformComponent) {
            transformComponent.angle += physComponent.physicsBody->getAngularVelocity() * delta;
            transformComponent.position += physComponent.physicsBody->getVelocity() * delta;

            if(!registry.has<Particle>(entity) && !visibleRect.containsPoint(transformComponent.position)) {
                //log("%f %f %f %f %f %f", visibleRect.origin.x, visibleRect.origin.y, visibleRect.size.width, visibleRect.size.height, transformComponent.position.x, transformComponent.position.y);
                //Если тело вне поля зрения и выходит за границы мира - мы берем наименьшую (наибольшую) точку
                //из двух: границы мира или границы камеры, в зависимости что дальше.
                if(transformComponent.position.x < worldRect.getMinX())
                    transformComponent.position.x = std::max(visibleRect.getMaxX(), worldRect.getMaxX());
                else if(transformComponent.position.x > worldRect.getMaxX())
                    transformComponent.position.x = std::min(visibleRect.getMinX(), worldRect.getMinX());

                if(transformComponent.position.y < worldRect.getMinY())
                    transformComponent.position.y = std::max(worldRect.getMaxY(), visibleRect.getMaxY());
                else if(transformComponent.position.y > worldRect.getMaxY())
                    transformComponent.position.y = std::min(visibleRect.getMinY(), worldRect.getMinY());
            } else {
                Rect extendedVisibleRect(visibleRect.getMinX() - 128,
                                         visibleRect.getMinY() - 128,
                                         visibleRect.size.width + 256,
                                         visibleRect.size.height + 256);
                Vec2 vecToCenter = transformComponent.position - Vec2(visibleRect.getMidX(), visibleRect.getMidY());
                if(transformComponent.position.x < -worldRect.size.width - visibleRect.size.width * 0.6f) {
                    transformComponent.position.x = worldRect.size.width + visibleRect.size.width * 0.5f + vecToCenter.x;
                } else if(transformComponent.position.x > worldRect.size.width + visibleRect.size.width * 0.6f) {
                    transformComponent.position.x = -worldRect.size.width - visibleRect.size.width * 0.5f + vecToCenter.x;
                }

                if(transformComponent.position.y < -worldRect.size.height - visibleRect.size.height * 0.6f) {
                    transformComponent.position.y = worldRect.size.height + visibleRect.size.height * 0.5f + vecToCenter.y;
                } else if(transformComponent.position.y > worldRect.size.height + visibleRect.size.height * 0.6f) {
                    transformComponent.position.y = -worldRect.size.height - visibleRect.size.height * 0.5f + vecToCenter.y;
                }


            }
            physComponent.physicsBody->getOwner()->setPosition(transformComponent.position);
        });
    }

private:
    Size _visibleSize;
};

class PlayerControllSystem : public ISystem {
public:
    //vector<pair<hashed_string, configuration>> playersData;
    explicit PlayerControllSystem(entt::hashed_string playerTag, entt::dispatcher& dispatcher /*, KeyConfig keyConfig */): _dispatcher(dispatcher), _timer(0.0f) {
        _playerTag = playerTag;

        dispatcher.sink<KeyPressedEvent>().connect<&PlayerControllSystem::onKeyPressed>(*this);
        dispatcher.sink<KeyReleasedEvent>().connect<&PlayerControllSystem::onKeyReleased>(*this);
    }

    virtual void update(entt::registry& registry, float delta) {


        auto view = registry.view<entt::tag<"player"_hs>, Transform, Physics, Ship>();

        for(KeyPressedEvent& event : _receivedPressedKeys) {
            _pressedKeys[event.key] = true;

            if(event.key == EventKeyboard::KeyCode::KEY_SHIFT) {
                onAcceleration(registry, true);
            }
        }

        for(auto player : view) {
            if(registry.valid(player)) {
                Transform& transformComponent = registry.get<Transform>(player);
                Physics& physicsComponent = registry.get<Physics>(player);
                Ship& shipComponent = registry.get<Ship>(player);
                Mortal& mortalComponent = registry.get<Mortal>(player);

                if(registry.has<Ghost>(player)) {
                    Ghost& ghostComponent = registry.get<Ghost>(player);
                    ghostComponent.currentTime += delta;
                    if(ghostComponent.currentTime >= ghostComponent.respawnTime) {
                        DrawableShape& shapeComponent = registry.get<DrawableShape>(player);
                        shapeComponent.borderColor.a = 1.0f;

                        transformComponent.position = Vec2(Director::getInstance()->getVisibleSize() * 0.5f);
                        physicsComponent.physicsBody->setCollisionBitmask(0xFFFFFFFF);
                        mortalComponent.health = 10;

                        registry.remove<Ghost>(player);


                        _dispatcher.trigger<CreateParticlesEvent>(50, transformComponent.position, 1.0f, 2.0f, 1.0f, 200.0f, Color4F::WHITE);
                    }
                }

                if(mortalComponent.health <= 0 && !registry.has<Ghost>(player)) {
                    DrawableShape& shapeComponent = registry.get<DrawableShape>(player);
                    shapeComponent.borderColor.a = 0.25f;
                    physicsComponent.physicsBody->setCollisionBitmask(0);
                    registry.assign<Ghost>(player, 10.0f);
                }

                Vec2 bodyVelocity = physicsComponent.physicsBody->getVelocity();
                Vec2 velocity = Vec2::ZERO;
                Vec2 opposite = Vec2::ZERO;

                if(_pressedKeys[EventKeyboard::KeyCode::KEY_A]) transformComponent.angle += delta * 5;
                if(_pressedKeys[EventKeyboard::KeyCode::KEY_D]) transformComponent.angle -= delta * 5;
                if(_pressedKeys[EventKeyboard::KeyCode::KEY_W]) {
                    velocity.x = std::cos(transformComponent.angle) * shipComponent.speed;
                    velocity.y = std::sin(transformComponent.angle) * shipComponent.speed;

                    _timer += delta;
                    if(_timer > 0.1f) {
                        _dispatcher.trigger<CreateParticlesEvent>(5, transformComponent.position, 0.5f, 1.0f, 1.0f, 20.0f, Color4F::GRAY);
                        _timer = 0.0f;
                    }
                } else {
                    if(abs(bodyVelocity.x) > 0.001) opposite.x = -bodyVelocity.x / abs(bodyVelocity.x) * delta * shipComponent.speed * 2;
                    if(abs(bodyVelocity.y) > 0.001) opposite.y = -bodyVelocity.y / abs(bodyVelocity.y) * delta * shipComponent.speed * 2;

                }

                physicsComponent.physicsBody->getOwner()->setRotation(toDeg(-transformComponent.angle));

                physicsComponent.physicsBody->setVelocity(velocity +  bodyVelocity + opposite);

                if(!registry.has<Ghost>(player)) {
                    if(_pressedKeys[EventKeyboard::KeyCode::KEY_SPACE]){
                        _pressedKeys[EventKeyboard::KeyCode::KEY_SPACE] = false;
                        onPlayerShooted(player, registry);
                    }
                }

                updateCamera(transformComponent.position);
            }
        }

        for(KeyReleasedEvent& event : _receivedReleasedKeys) {
            _pressedKeys[event.key] = false;

            if(event.key == EventKeyboard::KeyCode::KEY_SHIFT) {
                onAcceleration(registry, false);
            }
        }

        _receivedPressedKeys.clear();
        _receivedReleasedKeys.clear();
    }

    void onKeyPressed(const KeyPressedEvent& event) {
        _receivedPressedKeys.push_back(event);
    }

    void onKeyReleased(const KeyReleasedEvent& event) {
        _receivedReleasedKeys.push_back(event);
    }

private:
    void onPlayerShooted(entt::entity player, entt::registry& registry) {
        Transform& transformComponent = registry.get<Transform>(player);
        Ship& shipComponent = registry.get<Ship>(player);

        if(shipComponent.ammo > 0) {
            entt::entity bullet = registry.create();

            //TODO : FACTORY
            vector<Vec2> bulletVerticies = createRectangle(Size(2.0f, 2.0f));

            PhysicsBody* bulletBody = PhysicsBody::createPolygon(bulletVerticies.data(), bulletVerticies.size(), PhysicsMaterial(1.0f, 0.5f, 0.0f), Vec2::ZERO);
            bulletBody->setContactTestBitmask(0xFFFFFFFF);
            bulletBody->setTag(int(bullet));
            bulletBody->setGravityEnable(false);
            bulletBody->setVelocity(Vec2(cos(transformComponent.angle), sin(transformComponent.angle)) * shipComponent.speed * 2);

            registry.assign<Bullet>(bullet, player);
            registry.assign<DrawableShape>(bullet, bulletVerticies, Color4F::WHITE, Color4F::WHITE);
            registry.assign<Transform>(bullet, transformComponent.position + Vec2(cos(-transformComponent.angle), sin(-transformComponent.angle)), 1.0f, transformComponent.angle);
            registry.assign<Physics>(bullet, bulletBody);
            registry.assign<MinimapTarget>(bullet, Shape::CIRCLE, Color4F::WHITE);

            shipComponent.ammo--;
        }
    }

    void onAcceleration(entt::registry& registry, bool pressed) {
        registry.view<Ship, Physics, Transform, DrawableShape, entt::tag<"player"_hs>>().each([&](auto player, Ship& ship, Physics& physics, Transform& transform, DrawableShape& drawable, auto passComponent) {
            if(pressed) {
                ship.speed *= 2;
                drawable.borderColor.a = 0.5f;
            }
            else {
                ship.speed /= 2;
                drawable.borderColor.a = 1.0f;

                _dispatcher.trigger<CreateParticlesEvent>(20, transform.position, 1.0f, 2.0f, 1.0f, 100.0f, Color4F::WHITE);
            }

            physics.physicsBody->setVelocityLimit(ship.speed);
        });
    }

    void updateCamera(const Vec2& playerPosition) {
        Director::getInstance()->getRunningScene()->getDefaultCamera()->setPosition(playerPosition);
    }

    entt::hashed_string _playerTag;
    entt::dispatcher& _dispatcher;
    map<EventKeyboard::KeyCode, bool> _pressedKeys;

    vector<KeyPressedEvent> _receivedPressedKeys;
    vector<KeyReleasedEvent> _receivedReleasedKeys;

    float _timer;
};

class BulletCollisionSystem : public ISystem {
public:
    BulletCollisionSystem(entt::dispatcher& dispatcher): _dispatcher(dispatcher) {
        dispatcher.sink<CollisionBeginEvent>().connect<&BulletCollisionSystem::onCollisionBegin>(*this);
    }

    virtual void update(entt::registry& registry, float delta) {
        for(CollisionBeginEvent event : _collisionEvents) {
            if(registry.valid(event.entityA) && registry.valid(event.entityB)) {

                entt::entity required, another;
                if(registry.has<Bullet>(event.entityA)) { required = event.entityA; another = event.entityB; }
                else if(registry.has<Bullet>(event.entityB)) { required = event.entityB; another = event.entityA; }

                if(registry.valid(required) && !registry.has<entt::tag<"player"_hs>>(another)) {
                    onBulletCollision(registry, required, another, event);
                    continue;
                }

                if(registry.has<entt::tag<"player"_hs>>(event.entityA)) { required = event.entityA; another = event.entityB; }
                else if(registry.has<entt::tag<"player"_hs>>(event.entityB)) { required = event.entityB; another = event.entityA; }

                if(registry.valid(required) && !registry.has<Bullet>(another)) {
                    onPlayerCollision(registry, required, another, event);
                }

            }
        }

        _collisionEvents.clear();

        vector<entt::entity> _outsideBullets;
        auto bulletsView = registry.view<Bullet>();
        bulletsView.each([&](entt::entity bullet, Bullet& bulletComponent) {
            bulletComponent.livingTime += delta;
            if(bulletComponent.livingTime > 1.0f) {
                _outsideBullets.push_back(bullet);
            }
        });

        for(auto bullet : _outsideBullets) {
            registry.destroy(bullet);
        }

    }

    void onBulletCollision(entt::registry& registry, entt::entity bullet, entt::entity entity, CollisionBeginEvent event) {
        Physics& bulletPhysicsComponent = registry.get<Physics>(bullet);
        if(registry.has<Mortal>(entity)) {
            Mortal& mortalComponent = registry.get<Mortal>(entity);
            mortalComponent.health--;

            if(mortalComponent.health <= 0) {
                _dispatcher.trigger<CreateParticlesEvent>(10, event.contactPoint, 1.0f, 2.0f, 1.5f, bulletPhysicsComponent.physicsBody->getVelocity().length(), Color4F::RED);
                registry.destroy(entity);
            } else {
                _dispatcher.trigger<CreateParticlesEvent>(10, event.contactPoint, 1.0f, 1.5f, 0.8f, 40.0f, Color4F::WHITE);
            }
        }

        registry.destroy(bullet);
    }
    void onPlayerCollision(entt::registry& registry, entt::entity player, entt::entity entity, CollisionBeginEvent event) {
        Transform& transform = registry.get<Transform>(player);
        Physics& physics = registry.get<Physics>(player);
        Mortal& mortal = registry.get<Mortal>(player);

        Physics& entityPhysics = registry.get<Physics>(entity);
        if(physics.physicsBody->getCollisionBitmask() & entityPhysics.physicsBody->getCollisionBitmask()) {
            mortal.health--;
            _dispatcher.trigger<CreateParticlesEvent>(10, event.contactPoint, 1.0f, 2.0f, 1.0f, physics.physicsBody->getVelocity().length(), Color4F::YELLOW);
        }
    }

    void onCollisionBegin(const CollisionBeginEvent& event) {
        _collisionEvents.push_back(event);
    }

private:
    entt::dispatcher& _dispatcher;
    vector<CollisionBeginEvent> _collisionEvents;
};

class ParticleControlSystem : public ISystem {
public:
    ParticleControlSystem(entt::registry& registry, entt::dispatcher& dispatcher) {
        registry.on_construct<Meteorite>().connect<&ParticleControlSystem::onMeteoriteConstructed>(this);
        registry.on_destroy<Meteorite>().connect<&ParticleControlSystem::onMeteoriteDestroyed>(this);

        dispatcher.sink<CreateParticlesEvent>().connect<&ParticleControlSystem::onCreateParticlesEvent>(*this);
    }

    virtual void update(entt::registry& registry, float delta) {
        vector<entt::entity> expiredParticles;

        registry.view<DrawableShape, Particle>().each([&](auto particle, DrawableShape& drawable, Particle& p) {
            p.currentTime += delta;
            drawable.fillColor.a = 1.0f - p.currentTime / p.livingTime;
            drawable.borderColor.a = 1.0f - p.currentTime / p.livingTime;

            if(p.currentTime >= p.livingTime) {
                expiredParticles.push_back(particle);
            }
        });

       for(auto particle: expiredParticles) {
            registry.destroy(particle);
       }

       for(auto& event : _receivedEvents) {
            createParticles(registry, event);
       }

       _receivedEvents.clear();
    }

    void onMeteoriteConstructed(entt::registry& registry, entt::entity meteorite) {
        Transform& transform = registry.get<Transform>(meteorite);
        Physics& physics = registry.get<Physics>(meteorite);

        createParticles(registry, 20, transform.position, 1.0f, 5.0f, 5.0f, 40, Color4F::BLUE);
    }

    void onMeteoriteDestroyed(entt::registry& registry, entt::entity meteorite) {
//        Transform& transform = registry.get<Transform>(meteorite);

//        createParticles(registry, 10 , transform.position, 1.0f, 5.0f, 5.0f, 40, Color4F::RED);
    }

    void onCreateParticlesEvent(const CreateParticlesEvent& event) {
        _receivedEvents.push_back(event);
    }
private:
    void createParticles(entt::registry& registry, int count, Vec2 position, float minSize, float maxSize, float livingTime, float maxSpeed, Color4F color) {
        for(int i = 0;i < count; ++i) {
            entt::entity particle = registry.create();

            float time = random(0.0f, livingTime);
            float angle = random(0.0, 2.0 * M_PI);
            float speed = random(0.0f, maxSpeed);
            float particleSize = random(minSize, maxSize);

            vector<Vec2> particleVerticies = createRectangle(Size(particleSize, particleSize));

            PhysicsBody* particleBody = PhysicsBody::createPolygon(particleVerticies.data(), particleVerticies.size(), PhysicsMaterial(1.0f, 0.5f, 0.0f), Vec2::ZERO);
            particleBody->setTag(int(particle));
            particleBody->setGravityEnable(false);
            particleBody->setVelocity(Vec2(cos(angle) * speed, sin(angle) * speed));

            registry.assign<DrawableShape>(particle, particleVerticies, color, color);
            registry.assign<Transform>(particle, position, 1.0f, 0.0f);
            registry.assign<Physics>(particle, particleBody);
            registry.assign<Particle>(particle, time);
        }
    }

    void createParticles(entt::registry& registry, const CreateParticlesEvent& event) {
        createParticles(registry, event.particlesAmount,
                        event.initialPosition, event.minimalSize,
                        event.maximalSize, event.maximalLivingTime,
                        event.maximalSpeed, event.color);
    }

    vector<CreateParticlesEvent> _receivedEvents;
};

class HUDSystem : public ISystem {
public:
    HUDSystem(Node* scene) {
        _visibleSize = Director::getInstance()->getVisibleSize();

        _HUDCamera = cocos2d::Camera::create();
        _HUDCamera->setPosition(Vec2(_visibleSize) * 0.5f);
        _HUDCamera->setCameraFlag(CameraFlag::USER1);

        _ammoLabel = Label::createWithTTF("/", "fonts/arial.ttf", 26);
        _scoreLabel = Label::createWithTTF(":", "fonts/arial.ttf", 26);

        _ammoLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        _ammoLabel->setPosition(Vec2(0, _visibleSize.height));
        _ammoLabel->setCameraMask((unsigned short)CameraFlag::USER1, true);

        _scoreLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        _scoreLabel->setPosition(Vec2(0.5 * _visibleSize.width, _visibleSize.height));
        _scoreLabel->setCameraMask((unsigned short)CameraFlag::USER1, true);

        scene->addChild(_HUDCamera);
        scene->addChild(_ammoLabel);
        scene->addChild(_scoreLabel);

        _minimap = std::make_shared<Minimap>(scene, Size(128, 90), Color4F(0.0f, 0.0f, 0.0f, 0.5f), Color4F::BLACK, 1.0f);
        _minimap->setBorderRect(worldRect);
        _minimap->setPosition(Vec2(_visibleSize.width - 128, _visibleSize.height - 90));
        _minimap->getRenderer()->setCameraMask((unsigned short)CameraFlag::USER1, true);
    }

    virtual void update(entt::registry& registry, float delta) {
        _minimap->draw(registry, delta);

        auto view = registry.view<Ship, entt::tag<"player"_hs>>();
        view.each([&](auto entity, Ship& shipComponent, auto passComponent) {
            _ammoLabel->setString(StringUtils::format("AMMO %d/%d", shipComponent.ammo, shipComponent.maxAmmo));
            _scoreLabel->setString(StringUtils::format("SCORE 0"));
        });
    }

private:
    Size _visibleSize;

    Label* _ammoLabel;
    Label* _scoreLabel;

    Camera* _HUDCamera;

    std::shared_ptr<Minimap> _minimap;
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
                                                 entt::registry::entity_type(contact.getShapeB()->getBody()->getTag()),
                                                 contact.getContactData()->points[0]);


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
