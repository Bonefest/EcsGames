#include "SystemContainer.h"

void SystemContainer::updateSystems(entt::registry& registry, entt::dispatcher& dispatcher, float delta) {
    for(auto& systemInfo : _systems) {
        if(systemInfo.second.enabled)
            systemInfo.second.system->update(registry, dispatcher, delta);
    }
}

uint32_t SystemContainer::addSystem(shared_ptr<ISystem> system, uint32_t tag) {
    auto systemIter = _systems.find(tag);
    if(systemIter != _systems.end()) {
        while( (systemIter = _systems.find(_newSystemID)) != _systems.end()) {
            _newSystemID++;
        }

        tag = _newSystemID;
    }
    _systems[tag] = SystemInfo{system, true};
}

void SystemContainer::removeSystem(uint32_t tag) {
    if(_systems.find(tag) != _systems.end()) {
        _systems.erase(tag);
    }
}

void SystemContainer::setEnabledSystem(uint32_t tag, bool enabled) {
    auto systemIter = _systems.find(tag);
    if(systemIter != _systems.end()) {
        if(_systems[tag].enabled != enabled) {
            if(_systems[tag].enabled == false) _systems[tag].system->onEnable();
            else _systems[tag].system->onDisable();

            _systems[tag].enabled = enabled;
        }
    }
}

shared_ptr<ISystem> SystemContainer::findSystem(uint32_t tag) {
    auto systemIter = _systems.find(tag);
    if(systemIter != _systems.end())
        return systemIter->second.system;

    return nullptr;
}

bool SystemContainer::hasSystem(uint32_t tag) {
    return _systems.find(tag) != _systems.end();
}

void SystemContainer::setEnabledAllSystems(bool enabled) {
    for(auto systemInfo : _systems) {
        setEnabledSystem(systemInfo.first, enabled);
    }
}

//
//void SystemContainer::onEnableSystemEvent(const EnableSystemEvent& event) {
//    setEnabledSystem(event.tag, event.enabled);
//}

