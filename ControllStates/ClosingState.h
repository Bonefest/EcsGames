#ifndef CLOSESTATE_H_INCLUDED
#define CLOSESTATE_H_INCLUDED

#include "ControllState.h"

class ClosingControllState : public ControllState {
public:
    virtual shared_ptr<Command> handleInputEvent(IStateOwner* owner,
                                                 entt::registry& registry,
                                                 entt::dispatcher& dispatcher,
                                                 const UnprocessedKeyActionEvent& event);

    virtual void onEnter(IStateOwner* owner, entt::registry& registry, entt::dispatcher& dispatcher);
};

#endif // CLOSESTATE_H_INCLUDED
