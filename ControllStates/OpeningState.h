#ifndef OPENINGSTATE_H_INCLUDED
#define OPENINGSTATE_H_INCLUDED

#include "ControllState.h"

class OpeningControllState : public ControllState {
public:
    OpeningControllState(SystemContainer& container): _container(container) { }
    virtual shared_ptr<Command> handleInputEvent(IStateOwner* owner,
                                                 entt::registry& registry,
                                                 entt::dispatcher& dispatcher,
                                                 const UnprocessedKeyActionEvent& event);

    virtual void onEnter(IStateOwner* owner, entt::registry& registry, entt::dispatcher& dispatcher);

private:
    SystemContainer& _container;
};

#endif // OPENINGSTATE_H_INCLUDED
