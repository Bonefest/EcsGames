#include "TalkingState.h"
#include "NormalState.h"

#include "../Dialog/Dialog.h"

TalkingControllState::TalkingControllState(entt::entity partner): _partner(partner) { }
TalkingControllState::~TalkingControllState() { }

void TalkingControllState::onEnter(IStateOwner* owner,
                                   entt::registry& registry,
                                   entt::dispatcher& dispatcher) {

    Speakable& speakableComponent = registry.get<Speakable>(_partner);
    DialogInfo& dialogInfo = registry.ctx<DialogInfo>();
    dialogInfo.dialogMember = _partner;
    dialogInfo.dialog = registry.ctx<DialogDatabase>().getDialog(speakableComponent.dialogID);
    dialogInfo.answer = dialogInfo.dialog.startText;
    if(dialogInfo.dialog.nextDialog != 0) {
        speakableComponent.dialogID = dialogInfo.dialog.nextDialog;
    }

    _dialogView = make_shared<DialogView>();
    _dialogView->onDialogChanged(registry, dispatcher);
    owner->getViewContainer().setEnabledAllSystems(false);
    owner->getViewContainer().setEnabledSystem(Constants::ViewsTags::GridViewTag, true);
    owner->getViewContainer().addSystem(_dialogView, Constants::ViewsTags::DialogViewTag);
    //addSystem EntityInformationView
}

shared_ptr<Command> TalkingControllState::handleInputEvent(IStateOwner* owner,
                                                           entt::registry& registry,
                                                           entt::dispatcher& dispatcher,
                                                           const UnprocessedKeyActionEvent& event) {
    DialogInfo& dialogInfo = registry.ctx<DialogInfo>();

    if(event.keyType == MOVE_TOP) {
        dialogInfo.currentIndex = (uint64_t(dialogInfo.currentIndex) + 1) % dialogInfo.dialog.replicas.size();
        _dialogView->onDialogChanged(registry, dispatcher);
    } else if(event.keyType == MOVE_BOTTOM) {
        dialogInfo.currentIndex = (uint64_t(dialogInfo.currentIndex) - 1) % dialogInfo.dialog.replicas.size();
        _dialogView->onDialogChanged(registry, dispatcher);
    } else if(event.keyType == CANCEL) {
        owner->setState(make_shared<NormalControllState>(), registry, dispatcher);
    } else if(event.key == cocos2d::EventKeyboard::KeyCode::KEY_ENTER) {
        dialogInfo.dialog.replicas[dialogInfo.currentIndex]->activateReplica(registry, dispatcher);
        _dialogView->onDialogChanged(registry, dispatcher);
    }

    return make_shared<NullCommand>();
}

void TalkingControllState::onExit(IStateOwner* owner,
                                  entt::registry& registry,
                                  entt::dispatcher& dispatcher) {
    owner->getViewContainer().removeSystem(Constants::ViewsTags::DialogViewTag);
    owner->getViewContainer().setEnabledAllSystems(true);
}
