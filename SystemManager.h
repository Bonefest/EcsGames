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

class SystemManager {
public:
    SystemManager(): _newSystemID(0) {
        _dispatcher.sink<EnableSystemEvent>().connect<&SystemManager::onEnableSystemEvent>(*this);
    }

    void update(float delta) {
        for(auto& systemInfo : _systems) {
            if(systemInfo.second.enabled)
                systemInfo.second.system->update(_registry, _dispatcher, delta);
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

    void setEnabledSystem(uint32_t tag, bool enabled) {
        auto systemIter = _systems.find(tag);
        if(systemIter != _systems.end())
            _systems[tag].enabled = enabled;
    }

    void onEnableSystemEvent(const EnableSystemEvent& event) {
        setEnabledSystem(event.tag, event.enabled);
    }

    entt::registry& getRegistry() { return _registry; }
    entt::dispatcher& getDispatcher() { return _dispatcher; }
private:
    uint32_t _newSystemID;

    entt::registry _registry;
    entt::dispatcher _dispatcher;

    map<uint32_t, SystemInfo> _systems;
};

#endif // SYSTEMMANAGER_H_INCLUDED
