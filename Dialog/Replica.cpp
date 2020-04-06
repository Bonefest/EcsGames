#include "Replica.h"
#include "Dialog.h"
#include "Dialog/DialogDatabase.h"
#include "../Components/Components.h"

void MultipleReplica::useReplica(entt::registry& registry, entt::dispatcher& dispatcher) {
    for(auto replica : _replicas) {
        replica->useReplica(registry, dispatcher);
    }
}

void MultipleReplica::addReplica(shared_ptr<Replica> replica) {
    _replicas.push_back(replica);
}

CloseDialogReplica::CloseDialogReplica(Text replicaText) {
    setReplicaText(replicaText);
}

void CloseDialogReplica::useReplica(entt::registry& registry, entt::dispatcher& dispatcher) {

}

SwitchDialogReplica::SwitchDialogReplica(uint32_t dialogID, Text answerText, Text replicaText): _dialogID(dialogID),
                                                                                                _answerText(answerText)
                                                                                                 {
    setReplicaText(replicaText);
}

void SwitchDialogReplica::useReplica(entt::registry& registry, entt::dispatcher& dispatcher) {
    DialogInfo& dialogInfo = registry.ctx<DialogInfo>();
    dialogInfo.answer = _answerText;
    dialogInfo.dialog = registry.ctx<DialogDatabase>().getDialog(_dialogID);
}

SetNextDefaultDialogReplica::SetNextDefaultDialogReplica(ID dialogID, Text replicaText): _nextDefaultDialogID(dialogID) {
    setReplicaText(replicaText);
}

void SetNextDefaultDialogReplica::useReplica(entt::registry& registry, entt::dispatcher& dispatcher) {
    DialogInfo& dialogInfo = registry.ctx<DialogInfo>();
    Speakable& speakableComponent = registry.get<Speakable>(dialogInfo.dialogMember);
    speakableComponent.dialogID = _nextDefaultDialogID;
}
