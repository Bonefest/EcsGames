#ifndef MENUSTATE_H_INCLUDED
#define MENUSTATE_H_INCLUDED

#include <memory>

using std::make_shared;
using std::shared_ptr;

#include "StateOwner.h"

#include "../Command.h"
#include "../Events/Events.h"

struct UnprocessedKeyActionEvent;

typedef uint32_t tag;

class IStateOwner;

class ControllState {
public:
    virtual ~ControllState() { }

    virtual void onEnter(IStateOwner* owner, entt::registry& registry, entt::dispatcher& dispatcher) { }
    virtual void update(IStateOwner* owner, entt::registry& registry, entt::dispatcher& dispatcher, float delta) { }
    virtual void onExit(IStateOwner* owner, entt::registry& registry, entt::dispatcher& dispatcher) { }

    virtual shared_ptr<Command> handleInputEvent(IStateOwner* owner,
                                                 entt::registry& registry,
                                                 entt::dispatcher& dispatcher,
                                                 const UnprocessedKeyActionEvent& event) = 0;
};


#endif // MENUSTATE_H_INCLUDED
