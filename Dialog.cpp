#include "Dialog.h"



SwitchDialogReplica::SwitchDialogReplica(uint32_t dialogID, Text switchText, Text replicaText): _dialogID(dialogID),
                                                                                                _switchText(switchText),
                                                                                                _replicaText(replicaText) { }

void SwitchDialogReplica::useReplica(entt::registry& registry, entt::dispatcher& dispatcher, entt::entity replicaOwner) {
    DialogManager& container = registry.ctx<DialogManager>();
    container.setCurrentDialog(_dialogID);
    container.setCurrentText(_switchText);
}

Text SwitchDialogReplica::getReplicaText() const {
    return _replicaText;
}


void Dialog::addReplica(shared_ptr<Replica> replica) {
    _replicas.push_back(replica);
}

void Dialog::nextReplica() {

    if(!_replicas.empty() && _currentReplicaIndex == _replicas.size() - 1) _currentReplicaIndex = 0;
}

void Dialog::previousReplica() {

    if(!_replicas.empty() && _currentReplicaIndex == 0)  _currentReplicaIndex = _replicas.size() - 1;
}

shared_ptr<Replica> Dialog::getCurrentReplica() {
    if(_replicas.empty()) return nullptr;

    return _replicas[_currentReplicaIndex];
}

const vector<shared_ptr<Replica>>& Dialog::getAvailableReplicas() const {
    return _replicas;
}


DialogManager::DialogManager(): _dialogPartner(entt::null) { }

uint32_t DialogManager::setNewDialog(Dialog dialog, uint32_t dialogID) {
    _dialogs[dialogID] = dialog;
}

Dialog& DialogManager::getDialog(uint32_t dialogID) {
    return _dialogs[dialogID];
}

bool DialogManager::hasDialog(uint32_t dialogID) {
    return _dialogs.find(dialogID) != _dialogs.end();
}

void DialogManager::setCurrentDialog(uint32_t dialogID) {
    if(hasDialog(dialogID)) _currentDialog = dialogID;
}

Dialog& DialogManager::getCurrentDialog() {
    return _dialogs[_currentDialog];
}

void DialogManager::setDialogPartner(entt::entity partner) {
    _dialogPartner = partner;
}

entt::entity DialogManager::getDialogPartner() const {
    return _dialogPartner;
}

void DialogManager::setCurrentText(Text text) {
    _currentText = text;
}

Text DialogManager::getCurrentText() const {
    return _currentText;
}
