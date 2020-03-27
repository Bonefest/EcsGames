#ifndef DIALOG_H_INCLUDED
#define DIALOG_H_INCLUDED

#include "common.h"

class Replica {
	void useReplica(entt::registry& registry, entt::dispatcher& dispatcher, entt::entity replicaOwner) = 0;
	Text getReplicaText() const;
};

class SwitchDialogReplica {
public:
    SwitchDialogReplica(uint32_t dialogID, Text switchText): _dialogID(dialogID), _switchText(switchText) { }
    void useReplica(entt::registrt& registry, entt::dispatcher& dispatcher, entt::entity replicaOwner) {
        DialogManager& container = registry.ctx<DialogManager>();
        container.setCurrentDialog(dialogID);
        container.setCurrentText(_switchText);
    }

private:
    uint32_t _dialogID;
    Text _switchText;
};

class Dialog {
public:
	void addReplica(shared_ptr<Replica> replica) {
        _replicas.push_back(replica);
	}

	void nextReplica() {

        if(!_replicas.empty() && _currentReplicaIndex == _replicas.size() - 1) _currentReplicaIndex = 0;
	}

	void previousReplica() {

        if(!_replicas.empty() && _currentReplicaIndex == 0)  _currentReplicaIndex = _replicas.size() - 1;
	}

	shared_ptr<Replica> getCurrentReplica() {
        if(_replicas.empty()) return nullptr;

        return _replicas[_currentReplicaIndex];
	}

	const vector<shared_ptr<Replica>>& getAvailableReplicas() const() {
        return _replicas;
	}
private:
	vector<shared_ptr<Replica>> _replicas;
    std::size_t _currentReplicaIndex;
};


class DialogManager {
public:
	uint32_t setNewDialog(Dialog dialog, uint32_t dialogID) {
        _dialogs[dialogID] = dialog;
	}

	Dialog& getDialog(uint32_t dialogID) {
        return _dialogs[dialogID];
	}

    bool hasDialog(uint32_t dialogID) {
        return _dialogs.find(dialogID) != _dialogs.end();
    }

    void setCurrentDialog(uint32_t dialogID) {
        if(hasDialog(dialogID)) _currentDialog = dialogID;
    }

    void setCurrentText(Text text) {
        _currentText = text;
    }

    Text getCurrentText() const {
        return _currentText;
    }

private:
	map<uint32_t, Dialog> _dialogs;

	uint32_t _currentDialog;
	Text _currentText;
};


#endif // DIALOG_H_INCLUDED
