#ifndef TALKINGPREPARINGSTATE_H_INCLUDED
#define TALKINGPREPARINGSTATE_H_INCLUDED



#include "ControllState.h"

class TalkingPreparingState: public ControllState {
public:
    virtual ~TalkingPreparingState();

    virtual void onEnter(IStateOwner* owner, entt::registry& registry, entt::dispatcher& dispatcher);
    virtual shared_ptr<Command> handleInputEvent(IStateOwner* owner,
                                                 entt::registry& registry,
                                                 entt::dispatcher& dispatcher,
                                                 const UnprocessedKeyActionEvent& event);

};


#endif // TALKINGPREPARINGSTATE_H_INCLUDED
