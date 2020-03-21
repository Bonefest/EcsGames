#ifndef CHARACTERCONTROLLSYSTEM_H_INCLUDED
#define CHARACTERCONTROLLSYSTEM_H_INCLUDED

#include "cocos2d.h"
#include <memory>
#include <vector>
#include <map>

USING_NS_CC;
using std::make_shared;
using std::vector;
using std::map;

#include "System.h"
#include "../Events/Events.h"
#include "../MenuState.h"
#include "../common.h"

class ConfigurableKeyDispatcher {
public:
    ConfigurableKeyDispatcher(entt::dispatcher& dispatcher): _dispatcher(dispatcher) {
        dispatcher.sink<KeyPressedEvent>().connect<&ConfigurableKeyDispatcher::onKeyPressedEvent>(*this);
        dispatcher.sink<KeyReleasedEvent>().connect<&ConfigurableKeyDispatcher::onKeyReleasedEvent>(*this);

        //for(KeyType, Key in Config) -> setKeyType(key, keyType);
    }

    void onKeyPressedEvent(const KeyPressedEvent& event) {
        KeyType keyType = UNRESOLVED;

        auto keyIterator = _keys.find(event.key);
        if(keyIterator != _keys.end()) {
            keyType = keyIterator->second;
        }

        _dispatcher.trigger<UnprocessedKeyActionEvent>(event.key, keyType);
    }

    void onKeyReleasedEvent(const KeyReleasedEvent& event) {

    }

    void setKeyType(EventKeyboard::KeyCode key, KeyType keyType) {
        _keys[key] = keyType;
    }

private:
    entt::dispatcher& _dispatcher;
    map<EventKeyboard::KeyCode, KeyType> _keys;
};


class ControllSystem: public ISystem, public IStateOwner {
public:
    ControllSystem(entt::dispatcher& dispatcher) {
        _currentState = make_shared<MenuNormalState>();

        dispatcher.sink<UnprocessedKeyActionEvent>().connect<&ControllSystem::onUnprocessedKeyActionEvent>(*this);
    }

    virtual void update(entt::registry& registry, entt::dispatcher& dispatcher, float delta) {
        for(UnprocessedKeyActionEvent& event : _receivedEvents) {
            //dispatcher.trigger<CommandEvent>(_currentState->generateCommand(this, registry, event));
            _currentState->generateCommand(this, registry, event)->execute();

            cocos2d::log("Unprocessed");
        }

        _receivedEvents.clear();
    }

    void onUnprocessedKeyActionEvent(const UnprocessedKeyActionEvent& event) {
        _receivedEvents.push_back(event);
    }

private:
    vector<UnprocessedKeyActionEvent> _receivedEvents;
};

/*
class ControllSystem: public ISystem {
public:
    CharacterControllSystem(entt::dispatcher& dispatcher) {
        dispatcher.sink<KeyPressedEvent>().connect<&CharacterControllSystem::onKeyPressedEvent>(*this);
        dispatcher.sink<KeyReleasedEvent>().connect<&CharacterControllSystem::onKeyReleasedEvent>(*this);
    }

    virtual void start(entt::registry& registry, entt::dispatcher& dispatcher) {
        auto view = registry.view<Controllable>();

        entt::entity player = entt::null;
        view.each([&](entt::entity entity, Controllable& controllableComponent){
            player = entity; return;
        });

        assert(registry.valid(player) && "Unable to initialize controll system: cannot find a `controllable` component.");
        _commands[EventKeyboard::KeyCode::KEY_1] = make_shared<MoveCommand>(registry, player, Vec2(-1, -1));
        _commands[EventKeyboard::KeyCode::KEY_2] = make_shared<MoveCommand>(registry, player, Vec2( 0, -1));
        _commands[EventKeyboard::KeyCode::KEY_3] = make_shared<MoveCommand>(registry, player, Vec2( 1, -1));
        _commands[EventKeyboard::KeyCode::KEY_4] = make_shared<MoveCommand>(registry, player, Vec2(-1,  0));
        _commands[EventKeyboard::KeyCode::KEY_6] = make_shared<MoveCommand>(registry, player, Vec2( 1,  0));
        _commands[EventKeyboard::KeyCode::KEY_7] = make_shared<MoveCommand>(registry, player, Vec2(-1,  1));
        _commands[EventKeyboard::KeyCode::KEY_8] = make_shared<MoveCommand>(registry, player, Vec2( 0,  1));
        _commands[EventKeyboard::KeyCode::KEY_9] = make_shared<MoveCommand>(registry, player, Vec2( 1,  1));
        //_commands[EventKeyboard::KeyCode::KEY_5] = make_shared<WaitCommand>(registry);
    }

    virtual void update(entt::registry& registry, entt::dispatcher& dispatcher, float delta) {
        for(KeyPressedEvent& event : _receivedKeyPressedEvents) {
            auto commandIter = _commands.find(event.key);
            if(commandIter != _commands.end()) {
                //dispatcher.trigger<CommandEvent>(commandFactory(commandIter->second));
                //dispatcher.trigger<RawKeyCommandPressed>(`keyType`);

                //... CommandGenerator->currentState(Normal)->onRawKeyPressed
                //void onRawKeyPressed(KeyType)
                //  if(KeyType == MOVE_TOP)
                //      dispatcher.trigger<CommandEvent>(make_shared<MoveCommand>(moveCommand(findCursor(), Vec2(0, 1))));
                break;
            }

        }
    }

    void onKeyPressedEvent(const KeyPressedEvent& event) {
        _receivedKeyPressedEvents.push_back(event);
    }

    void onKeyReleasedEvent(const KeyPressedEvent& event) {
        _receivedKeyReleasedEvents.push_back(event);
    }
private:
    shared_ptr<Command> commandFactory(KeyType keyType) {

    }

    vector<KeyPressedEvent> _receivedKeyPressedEvents;
    vector<KeyReleasedEvent> _receivedKeyReleasedEvents;

    map<EventKeyboard::KeyCode, shared_ptr<Command>> _commands;
    //map<KeyType, EventKeyboard::KeyCode> _commands;
};
*/

#endif // CHARACTERCONTROLLSYSTEM_H_INCLUDED
