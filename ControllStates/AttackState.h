#ifndef ATTACKSTATE_H_INCLUDED
#define ATTACKSTATE_H_INCLUDED


#include "ControllState.h"

class AttackControllState: public ControllState {
public:
    AttackControllState(SystemContainer& container);
    virtual ~AttackControllState();

    virtual void onEnter(IStateOwner* owner, entt::registry& registry, entt::dispatcher& dispatcher);
    virtual void update(IStateOwner* owner, entt::registry& registry, entt::dispatcher& dispatcher, float delta);
    virtual shared_ptr<Command> handleInputEvent(IStateOwner* owner,
                                                 entt::registry& registry,
                                                 entt::dispatcher& dispatcher,
                                                 const UnprocessedKeyActionEvent& event);

private:
    SystemContainer& _container;

};

#endif // ATTACKSTATE_H_INCLUDED
