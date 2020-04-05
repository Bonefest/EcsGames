#ifndef TALKINGSTATE_H_INCLUDED
#define TALKINGSTATE_H_INCLUDED

#include "../Systems/DialogView.h"
#include "ControllState.h"

#include "../Dialog/DialogDatabase.h"
#include "../Dialog/Dialog.h"

class TalkingControllState: public ControllState {
public:
    TalkingControllState(entt::entity partner);
    ~TalkingControllState();

    virtual void onEnter(IStateOwner* owner,
                         entt::registry& registry,
                         entt::dispatcher& dispatcher);
    virtual void onExit(IStateOwner* owner,
                        entt::registry& registry,
                        entt::dispatcher& dispatcher);

    virtual shared_ptr<Command> handleInputEvent(IStateOwner* owner,
                                                 entt::registry& registry,
                                                 entt::dispatcher& dispatcher,
                                                 const UnprocessedKeyActionEvent& event);

private:
    entt::entity _partner;
    shared_ptr<DialogView> _dialogView;
};

#endif // TALKINGSTATE_H_INCLUDED
