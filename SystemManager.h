#ifndef SYSTEMMANAGER_H_INCLUDED
#define SYSTEMMANAGER_H_INCLUDED

#include <vector>
#include <memory>

#include "Dependencies/entt.hpp"
#include "Systems/System.h"

using std::vector;
using std::shared_ptr;

struct SystemInfo {
    //SystemInfo(shared_ptr<ISystem> newSystem, bool enable): system(newSystem), enabled(enable) {}
    shared_ptr<ISystem> system;
    bool enabled;
};

class SystemContainer {
public:
    SystemContainer(): _newSystemID(0) { }

    void updateSystems(entt::registry& registry, entt::dispatcher& dispatcher, float delta) {
        for(auto& systemInfo : _systems) {
            if(systemInfo.second.enabled)
                systemInfo.second.system->update(registry, dispatcher, delta);
        }
    }

    uint32_t addSystem(shared_ptr<ISystem> system, uint32_t tag = 0) {
        auto systemIter = _systems.find(tag);
        if(systemIter != _systems.end()) {
            while( (systemIter = _systems.find(_newSystemID)) != _systems.end()) {
                _newSystemID++;
            }

            tag = _newSystemID;
        }
        _systems[tag] = SystemInfo{system, true};
    }

    void removeSystem(uint32_t tag) {
        if(_systems.find(tag) != _systems.end()) {
            _systems.erase(tag);
        }
    }

    void setEnabledSystem(uint32_t tag, bool enabled) {
        auto systemIter = _systems.find(tag);
        if(systemIter != _systems.end())
            _systems[tag].enabled = enabled;
    }

    shared_ptr<ISystem> findSystem(uint32_t tag) {
        auto systemIter = _systems.find(tag);
        if(systemIter != _systems.end())
            return systemIter->second.system;

        return nullptr;
    }

    void onEnableSystemEvent(const EnableSystemEvent& event) {
        setEnabledSystem(event.tag, event.enabled);
    }

private:
    map<uint32_t, SystemInfo> _systems;
    uint32_t _newSystemID;

};

class SystemManager {
public:
    SystemManager() {
        //_dispatcher.sink<EnableSystemEvent>().connect<&SystemManager::onEnableSystemEvent>(*this);
    }

    void update(float delta) {
        _container.updateSystems(_registry, _dispatcher, delta);
    }

    SystemContainer& getContainer() { return _container; }
    entt::registry& getRegistry() { return _registry; }
    entt::dispatcher& getDispatcher() { return _dispatcher; }
private:
    entt::registry _registry;
    entt::dispatcher _dispatcher;

    SystemContainer _container;
};

#endif // SYSTEMMANAGER_H_INCLUDED
