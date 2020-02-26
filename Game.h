#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "cocos2d.h"
#include "entityx/entityx.h"

#include <list>

//При нажатии пробела змея разделяется на две - вторая двигается в сторону, куда мы двигались.

const double STEP_FREQ = 0.1;

enum Direction {
    LEFT, UP, RIGHT, DOWN
};

enum KeyEventType {
    PRESSED, RELEASED
};

enum PowerupType {
    LENGTH, SPEED
};

struct Drawable {
    explicit Drawable(cocos2d::Color4F clr): color(clr) { }
    cocos2d::Color4F color;
};

struct Body {
    explicit Body(cocos2d::Vec2 pos, cocos2d::Size bodySize): position(pos), size(bodySize) { }

    cocos2d::Vec2 position;
    cocos2d::Size size;
};

struct Powerup {
    Powerup(PowerupType powerupType): type(powerupType) { }

    PowerupType type;
};

struct SnakePart {
    explicit SnakePart() { }
    entityx::Entity nextPart;
};

struct Player {
    explicit Player(Direction playerDirection): previousDirection(playerDirection), direction(playerDirection) { score = 0; }

    Direction direction;
    Direction previousDirection;

    int score;
};


struct KeyEvent {
public:
    explicit KeyEvent(cocos2d::EventKeyboard::KeyCode pressedKey, KeyEventType eventType) {
        key = pressedKey;
        type = eventType;
    }

    cocos2d::EventKeyboard::KeyCode key;
    KeyEventType type;
};

struct CreateSnakePartEvent {
public:
    //CreateSnakePartEvent(Entity playerID);
    explicit CreateSnakePartEvent() { }
};

struct CollisionEvent {
public:
    CollisionEvent(entityx::Entity entityA, entityx::Entity entityB): first(entityA), second(entityB) { }

    entityx::Entity first;
    entityx::Entity second;
};

typedef std::pair<cocos2d::EventKeyboard::KeyCode, KeyEventType> KeyTypePair;

class InputHandleSystem: public entityx::System<InputHandleSystem> {
public:
    explicit InputHandleSystem(cocos2d::EventDispatcher* dispatcher, cocos2d::Scene* scene) {
        cocos2d::EventListenerKeyboard* klistener = cocos2d::EventListenerKeyboard::create();
        klistener->onKeyPressed = CC_CALLBACK_2(InputHandleSystem::onKeyPressed, this);
        klistener->onKeyReleased = CC_CALLBACK_2(InputHandleSystem::onKeyReleased, this);

        dispatcher->addEventListenerWithSceneGraphPriority(klistener, scene);
    }

    void update(entityx::EntityManager& em, entityx::EventManager& events, entityx::TimeDelta delta) {
        for(KeyTypePair event : _unreleasedEvents) {
            events.emit<KeyEvent>(event.first, event.second);
        }

        _unreleasedEvents.clear();
    }

    void onKeyPressed(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event* event) {
        _unreleasedEvents.push_back(KeyTypePair(key, PRESSED));
    }

    void onKeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event* event) {
        _unreleasedEvents.push_back(KeyTypePair(key, RELEASED));
    }
private:
    std::list<KeyTypePair> _unreleasedEvents;
};

class ControllerSystem: public entityx::System<ControllerSystem>, public entityx::Receiver<ControllerSystem> {
public:
    void configure(entityx::EventManager& manager) {
        manager.subscribe<KeyEvent>(*this);
    }

    void update(entityx::EntityManager& em, entityx::EventManager& events, entityx::TimeDelta delta) {
        entityx::ComponentHandle<Player> playerComponent;
        for(entityx::Entity entity : em.entities_with_components(playerComponent)) {
            for(KeyEvent event : _receivedEvents) {
                if(event.key == cocos2d::EventKeyboard::KeyCode::KEY_SPACE && event.type == PRESSED) {
                    events.emit<CreateSnakePartEvent>();
                }
                if(event.type == PRESSED) {
                    setPlayerDirection(playerComponent, event.key);
                }
            }

            _receivedEvents.clear();
        }
    }

    void receive(const KeyEvent& event) {
        _receivedEvents.push_back(event);
    }

private:
    void setPlayerDirection(entityx::ComponentHandle<Player>& player, cocos2d::EventKeyboard::KeyCode key) {
        if(key == cocos2d::EventKeyboard::KeyCode::KEY_A && player->previousDirection != RIGHT) {
            player->direction = LEFT;
        }
        if(key == cocos2d::EventKeyboard::KeyCode::KEY_D && player->previousDirection != LEFT) {
            player->direction = RIGHT;
        }
        if(key == cocos2d::EventKeyboard::KeyCode::KEY_W && player->previousDirection != DOWN) {
            player->direction = UP;
        }
        if(key == cocos2d::EventKeyboard::KeyCode::KEY_S && player->previousDirection != UP) {
            player->direction = DOWN;
        }
    }

    std::list<KeyEvent> _receivedEvents;
};

class SnakePartSpawner: public entityx::System<SnakePartSpawner>, public entityx::Receiver<SnakePartSpawner> {
public:
    SnakePartSpawner() {
        _uncreatedParts = 0;
    }

    void configure(entityx::EventManager& manager) {
        manager.subscribe<CreateSnakePartEvent>(*this);
    }

    void update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta delta) {
        if(_uncreatedParts > 0) {
            entityx::ComponentHandle<Player> playerComponent;
            entityx::ComponentHandle<SnakePart> snakePartComponent;
            entityx::ComponentHandle<Body> bodyComponent;

            entities.entities_with_components(playerComponent, snakePartComponent, bodyComponent).begin();

            while(snakePartComponent->nextPart.valid()) {
                bodyComponent = snakePartComponent->nextPart.component<Body>();
                snakePartComponent = snakePartComponent->nextPart.component<SnakePart>();
            }

            for(int i = 0;i < _uncreatedParts; ++i) {
                entityx::Entity part = entities.create();

                part.assign<SnakePart>();
                part.assign<Body>(bodyComponent->position, bodyComponent->size);
                part.assign<Drawable>(cocos2d::Color4F::WHITE);

                snakePartComponent->nextPart = part;

                snakePartComponent = part.component<SnakePart>();
            }

            cocos2d::log("HERE");

            _uncreatedParts = 0;
        }
    }

    void receive(const CreateSnakePartEvent& event) {
        _uncreatedParts++;
    }

private:
    int _uncreatedParts;
};

class SnakeMovingSystem: public entityx::System<SnakeMovingSystem> {
public:
    explicit SnakeMovingSystem() {
        _elapsedTime = 0.0;
    }

    void update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta delta) {
        _elapsedTime += delta;

        if(_elapsedTime >= STEP_FREQ) {

            entityx::ComponentHandle<Player> playerComponent;
            entityx::ComponentHandle<SnakePart> snakePartComponent;
            entityx::ComponentHandle<Body> bodyComponent;

            for(entityx::Entity entity : entities.entities_with_components(playerComponent, snakePartComponent, bodyComponent)) {
                cocos2d::Vec2 previousLocation = bodyComponent->position;

                if(playerComponent->previousDirection == LEFT) {
                    bodyComponent->position += cocos2d::Vec2(-bodyComponent->size.width, 0);
                } else if(playerComponent->previousDirection == RIGHT) {
                    bodyComponent->position += cocos2d::Vec2(bodyComponent->size.width, 0);
                } else if(playerComponent->previousDirection == UP) {
                    bodyComponent->position += cocos2d::Vec2(0, bodyComponent->size.height);
                } else if(playerComponent->previousDirection == DOWN) {
                    bodyComponent->position += cocos2d::Vec2(0, -bodyComponent->size.height);
                }

                for(entityx::Entity tail = snakePartComponent->nextPart; tail.valid(); tail = snakePartComponent->nextPart) {
                    snakePartComponent = entities.component<SnakePart>(tail.id());
                    bodyComponent = entities.component<Body>(tail.id());

                    cocos2d::Vec2 temp = bodyComponent->position;
                    bodyComponent->position = previousLocation;
                    previousLocation = temp;
                }
            }

            playerComponent->previousDirection = playerComponent->direction;
            _elapsedTime = 0.0;
        }
    }

private:
    double _elapsedTime;
};

class DrawingSystem: public entityx::System<DrawingSystem> {
public:
    explicit DrawingSystem(cocos2d::DrawNode* node) {
        _drawer = node;
    }

    void update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta delta) {
        _drawer->clear();

        entityx::ComponentHandle<Drawable> drawableComponent;
        entityx::ComponentHandle<Body> bodyComponent;

        for(entityx::Entity entity : entities.entities_with_components(drawableComponent, bodyComponent)) {
            _drawer->drawSolidRect(bodyComponent->position,
                              bodyComponent->position + cocos2d::Vec2(bodyComponent->size),
                              drawableComponent->color);
        }
    }

private:
    cocos2d::DrawNode* _drawer;
};

class CollisionSystem: public entityx::System<CollisionSystem> {
private:
    struct CollideData {
        entityx::Entity entity;
        Body* bodyComponent;
    };
public:
    void update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta delta) {
        entityx::ComponentHandle<Body> bodyComponent;
        //colliableComponent ...

        std::vector<CollideData> bodies;
        for(entityx::Entity entity : entities.entities_with_components(bodyComponent)) {
            bodies.push_back({entity, bodyComponent.get()});
        }

        for(int i = 0;i < bodies.size(); ++i) {
            for(int j = 0;j < bodies.size(); ++j) {
                if(bodies[i].entity == bodies[j].entity) continue;

                cocos2d::Rect rectA(bodies[i].bodyComponent->position + cocos2d::Vec2(1, 1), bodies[i].bodyComponent->size - cocos2d::Size(1, 1));
                cocos2d::Rect rectB(bodies[j].bodyComponent->position + cocos2d::Vec2(1, 1), bodies[j].bodyComponent->size - cocos2d::Size(1, 1));

                if(rectA.intersectsRect(rectB)) {

                    events.emit<CollisionEvent>(bodies[i].entity, bodies[j].entity);
                }
            }
        }
    }
};

class PowerupSpawnSystem : public entityx::System<PowerupSpawnSystem> {
public:
        PowerupSpawnSystem(int maximalCount) {
            _maximalCount = maximalCount;
        }

        void update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta delta) {
            int currentCount = 0;
            entityx::ComponentHandle<Powerup> powerupComponent;
            for(entityx::Entity entity : entities.entities_with_components(powerupComponent))
                currentCount++;

            for(int i = 0;i < _maximalCount - currentCount; ++i) {
                cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

                entityx::Entity powerup = entities.create();
                powerup.assign<Body>(cocos2d::Vec2(int(cocos2d::random(0.0f, visibleSize.width / 32)) * 32,
                                                   int(cocos2d::random(0.0f, visibleSize.height / 32)) * 32), cocos2d::Size(32, 32));
                powerup.assign<Drawable>(cocos2d::Color4F::RED);
                powerup.assign<Powerup>(LENGTH); //TODO: RANDOM
            }

        }

private:
    int _maximalCount;

};

class PowerupReactionSystem : public entityx::System<PowerupReactionSystem>, public entityx::Receiver<PowerupReactionSystem> {
public:
    void configure(entityx::EventManager& manager) {
        manager.subscribe<CollisionEvent>(*this);
    }

    void update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta delta) {
        for(CollisionEvent& event : _receivedEvents) {
            if(!event.first.valid() || !event.second.valid()) continue;

            entityx::Entity player;
            entityx::Entity powerup;


            if(event.first.has_component<Player>()) player = event.first;
            else if(event.second.has_component<Player>()) player = event.second;

            if(event.first.has_component<Powerup>()) powerup = event.first;
            else if(event.second.has_component<Powerup>()) powerup = event.second;

            if(player.valid() && powerup.valid() && player != powerup) {
                addPowerupTo(player, powerup, events);
                powerup.destroy();
            }
        }

        _receivedEvents.clear();
    }

    void receive(const CollisionEvent& event) {
        _receivedEvents.push_back(event);
    }

private:
    void addPowerupTo(entityx::Entity entity, entityx::Entity powerup, entityx::EventManager& manager) {
        entityx::ComponentHandle<Player> playerComponent = entity.component<Player>();
        entityx::ComponentHandle<Powerup> powerupComponent = powerup.component<Powerup>();

        if(powerupComponent->type == LENGTH) {
            manager.emit<CreateSnakePartEvent>();
        }
    }

    std::list<CollisionEvent> _receivedEvents;
};

class Application: public entityx::EntityX {
public:
    explicit Application(cocos2d::DrawNode* drawNode, cocos2d::EventDispatcher* dispatcher, cocos2d::Scene* scene) {
        systems.add<InputHandleSystem>(dispatcher, scene);
        systems.add<ControllerSystem>();
        systems.add<SnakeMovingSystem>();
        systems.add<DrawingSystem>(drawNode);
        systems.add<SnakePartSpawner>();

        systems.add<CollisionSystem>();
        systems.add<PowerupReactionSystem>();
        systems.add<PowerupSpawnSystem>(3);
        systems.configure();
    }

    void update(entityx::TimeDelta delta) {
        systems.update_all(delta);
    }
};

#endif // GAME_H_INCLUDED
