#include "../ControllStates/NormalState.h"
#include "../Components/Components.h"
#include "../Events/Events.h"
#include "DialogDatabase.h"
#include "ReplicaAction.h"
#include "../Command.h"
#include "Dialog.h"


void CloseDialog::execute(entt::registry& registry, entt::dispatcher& dispatcher) {
    dispatcher.trigger<StateControllCommandEvent>(make_shared<SetStateCommand>(make_shared<NormalControllState>()));
}

SwitchDialog::SwitchDialog(ID dialogID, Text answerText): _dialogID(dialogID),
                                                          _answerText(answerText) { }

void SwitchDialog::execute(entt::registry& registry, entt::dispatcher& dispatcher) {
    DialogInfo& dialogInfo = registry.ctx<DialogInfo>();
    dialogInfo.answer = _answerText;
    dialogInfo.dialog = registry.ctx<DialogDatabase>().getDialog(_dialogID);
}

SetNextDefaultDialog::SetNextDefaultDialog(ID dialogID): _nextDefaultDialogID(dialogID) { }

void SetNextDefaultDialog::execute(entt::registry& registry, entt::dispatcher& dispatcher) {
    DialogInfo& dialogInfo = registry.ctx<DialogInfo>();
    Speakable& speakableComponent = registry.get<Speakable>(dialogInfo.dialogMember);
    speakableComponent.dialogID = _nextDefaultDialogID;
}
