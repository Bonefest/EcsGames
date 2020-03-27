#ifndef OPENINGSTATE_H_INCLUDED
#define OPENINGSTATE_H_INCLUDED

#include "ControllState.h"

class OpeningControllState : public ControllState {
public:
    virtual shared_ptr<Command> handleInputEvent(IStateOwner* owner,
                                                 entt::registry& registry,
                                                 entt::dispatcher& dispatcher,
                                                 const UnprocessedKeyActionEvent& event);

    virtual void onEnter(IStateOwner* owner, entt::registry& registry, entt::dispatcher& dispatcher);
};

#endif // OPENINGSTATE_H_INCLUDED
