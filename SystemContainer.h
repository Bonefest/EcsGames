#ifndef SYSTEMCONTAINER_H_INCLUDED
#define SYSTEMCONTAINER_H_INCLUDED

#include "Systems/System.h"
#include "Events/Events.h"

#include <memory>
#include <map>

using std::shared_ptr;
using std::map;

struct SystemInfo {
    shared_ptr<ISystem> system;
    bool enabled;
};

class SystemContainer {
public:
    SystemContainer(): _newSystemID(0) { }

    void updateSystems(entt::registry& registry, entt::dispatcher& dispatcher, float delta);

    uint32_t addSystem(shared_ptr<ISystem> system, uint32_t tag = 0);
    void removeSystem(uint32_t tag);

    void setEnabledSystem(uint32_t tag, bool enabled);
    shared_ptr<ISystem> findSystem(uint32_t tag);

    void onEnableSystemEvent(const EnableSystemEvent& event);

private:
    map<uint32_t, SystemInfo> _systems;
    uint32_t _newSystemID;

};

#endif // SYSTEMCONTAINER_H_INCLUDED
