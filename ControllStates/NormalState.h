#ifndef NORMALSTATE_H_INCLUDED
#define NORMALSTATE_H_INCLUDED

#include <memory>

#include "ControllState.h"

class NormalControllState: public ControllState {
public:
    NormalControllState(SystemContainer& container);
    virtual ~NormalControllState();

    virtual void onEnter(IStateOwner* owner, entt::registry& registry, entt::dispatcher& dispatcher);
    virtual void update(IStateOwner* owner, entt::registry& registry, entt::dispatcher& dispatcher, float delta);
    virtual shared_ptr<Command> handleInputEvent(IStateOwner* owner,
                                                entt::registry& registry,
                                                entt::dispatcher& dispatcher,
                                                const UnprocessedKeyActionEvent& event);
private:
    SystemContainer& _container;
};


#endif // NORMALSTATE_H_INCLUDED
