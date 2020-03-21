#ifndef SYSTEMMANAGER_H_INCLUDED
#define SYSTEMMANAGER_H_INCLUDED

#include <vector>
#include <memory>

#include "Dependencies/entt.hpp"
#include "Systems/System.h"

using std::vector;
using std::shared_ptr;

class SystemManager {
public:
    void update(float delta) {
        for(auto& system : _systems) {
            system->update(_registry, _dispatcher, delta);
        }
    }

    void addSystem(shared_ptr<ISystem> system) {
        _systems.push_back(system);
    }

    entt::registry& getRegistry() { return _registry; }
    entt::dispatcher& getDispatcher() { return _dispatcher; }
private:
    entt::registry _registry;
    entt::dispatcher _dispatcher;

    vector<shared_ptr<ISystem>> _systems;
};

#endif // SYSTEMMANAGER_H_INCLUDED
