#ifndef CHARACTERCONTROLLSYSTEM_H_INCLUDED
#define CHARACTERCONTROLLSYSTEM_H_INCLUDED

#include "cocos2d.h"
#include <memory>
#include <vector>
#include <map>

using namespace cocos2d;

using std::make_shared;
using std::vector;
using std::map;


#include "System.h"
#include "../Events/Events.h"
#include "../ControllStates/NormalState.h"
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


class StateControllSystem: public ISystem, public IStateOwner {
public:
    StateControllSystem(entt::registry& registry, entt::dispatcher& dispatcher): IStateOwner() {
        setState(make_shared<NormalControllState>(), registry, dispatcher);

        dispatcher.sink<UnprocessedKeyActionEvent>().connect<&StateControllSystem::onUnprocessedKeyActionEvent>(*this);
        dispatcher.sink<StateControllCommandEvent>().connect<&StateControllSystem::onStateControllCommandEvent>(*this);
    }

    virtual void update(entt::registry& registry, entt::dispatcher& dispatcher, float delta) {
        _viewContainer->updateSystems(registry, dispatcher, delta);
        _currentState->update(this, registry, dispatcher, delta);
        for(UnprocessedKeyActionEvent& event : _receivedKeyEvents) {
            _currentState->handleInputEvent(this, registry, dispatcher, event)->execute(this, registry, dispatcher);
        }

        for(auto command : _receivedCommands) {
            command->execute(this, registry, dispatcher);
        }

        _receivedKeyEvents.clear();
    }

    void onUnprocessedKeyActionEvent(const UnprocessedKeyActionEvent& event) {
        _receivedKeyEvents.push_back(event);
    }

    void onStateControllCommandEvent(const StateControllCommandEvent& event) {
        _receivedCommands.push_back(event.command);
    }

private:
    vector<UnprocessedKeyActionEvent> _receivedKeyEvents;
    vector<shared_ptr<Command>> _receivedCommands;

};

#endif // CHARACTERCONTROLLSYSTEM_H_INCLUDED
