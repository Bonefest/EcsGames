#include "TalkingState.h"
#include "NormalState.h"
#include "Dialog.h"

TalkingControllState::TalkingControllState(entt::entity partner): _partner(partner) { }
TalkingControllState::~TalkingControllState() { }

void TalkingControllState::onEnter(IStateOwner* owner,
                                   entt::registry& registry,
                                   entt::dispatcher& dispatcher) {
    DialogManager& dialogManager = registry.ctx<DialogManager>();

    Speakable& speakableComponent = registry.get<Speakable>(_partner);
    StatusInfo currentStatus = speakableComponent.status.getCurrentStatusInfo();
    dialogManager.setCurrentDialog(currentStatus.dialogID);
    dialogManager.setCurrentText(currentStatus.greetingText);
    dialogManager.setDialogPartner(_partner);
}

shared_ptr<Command> TalkingControllState::handleInputEvent(IStateOwner* owner,
                                                           entt::registry& registry,
                                                           entt::dispatcher& dispatcher,
                                                           const UnprocessedKeyActionEvent& event) {
    DialogManager& dialogManager = registry.ctx<DialogManager>();

    if(event.keyType == MOVE_TOP) {
        dialogManager.getCurrentDialog().previousReplica();
    } else if(event.keyType == MOVE_BOTTOM) {
        dialogManager.getCurrentDialog().nextReplica();
    } else if(event.keyType == CANCEL) {
        owner->setState(make_shared<NormalControllState>(), registry, dispatcher);
    } else if(event.key == cocos2d::EventKeyboard::KeyCode::KEY_ENTER) {
        dialogManager.getCurrentDialog().getCurrentReplica()->useReplica(registry, dispatcher, _partner);
    }

    return make_shared<NullCommand>();
}
