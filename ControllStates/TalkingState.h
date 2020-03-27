#ifndef TALKINGSTATE_H_INCLUDED
#define TALKINGSTATE_H_INCLUDED

#include "ControllState.h"

class TalkingControllState: public ControllState {
public:
    TalkingControllState(entt::entity partner);
    ~TalkingControllState();
    virtual void onEnter(IStateOwner* owner,
                         entt::registry& registry,
                         entt::dispatcher& dispatcher);
    virtual shared_ptr<Command> handleInputEvent(IStateOwner* owner,
                                                 entt::registry& registry,
                                                 entt::dispatcher& dispatcher,
                                                 const UnprocessedKeyActionEvent& event);

private:
    entt::entity _partner;
};

#endif // TALKINGSTATE_H_INCLUDED
