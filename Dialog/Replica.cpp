#include "Replica.h"
#include "Dialog.h"
#include "Dialog/DialogDatabase.h"

CloseDialogReplica::CloseDialogReplica(Text replicaText) {
    setReplicaText(replicaText);
}

void CloseDialogReplica::useReplica(entt::registry& registry, entt::dispatcher& dispatcher, entt::entity replicaOwner) {

}

SwitchDialogReplica::SwitchDialogReplica(uint32_t dialogID, Text switchText, Text replicaText): _dialogID(dialogID),
                                                                                                _switchText(switchText)
                                                                                                 {
    setReplicaText(replicaText);
}

void SwitchDialogReplica::useReplica(entt::registry& registry, entt::dispatcher& dispatcher, entt::entity replicaOwner) {
    DialogInfo& dialogInfo = registry.ctx<DialogInfo>();
    dialogInfo.dialog = registry.ctx<DialogDatabase>().getDialog(_dialogID);
}

