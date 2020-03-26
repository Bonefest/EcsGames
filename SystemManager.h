#ifndef SYSTEMMANAGER_H_INCLUDED
#define SYSTEMMANAGER_H_INCLUDED

#include <vector>

#include "SystemContainer.h"

#include "Dependencies/entt.hpp"
#include "Systems/System.h"


using std::vector;


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
