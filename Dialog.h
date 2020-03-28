#ifndef DIALOG_H_INCLUDED
#define DIALOG_H_INCLUDED

#include <map>
#include <vector>
#include <memory>

#include "common.h"
#include "Dependencies/entt.hpp"

using std::map;
using std::vector;
using std::shared_ptr;

class Replica {
public:
	virtual void useReplica(entt::registry& registry, entt::dispatcher& dispatcher, entt::entity replicaOwner) = 0;
	virtual Text getReplicaText() const { }
};

class SwitchDialogReplica: public Replica {
public:
    SwitchDialogReplica(uint32_t dialogID, Text switchText, Text replicaText);
    void useReplica(entt::registry& registry, entt::dispatcher& dispatcher, entt::entity replicaOwner);
    Text getReplicaText() const;
private:
    uint32_t _dialogID;
    Text _switchText;
    Text _replicaText;
};

//
//class CloseDialogReplica: public Replica {
//public:
//    CloseDialogReplica(Text replicaText): _replicaText(replicaText) { }
//
//    void useReplica(entt::registry& registry, entt::dispatcher& dispatcher, entt::entity replicaOwner) {
//        dispatcher.trigger<StateControllCommandEvent>(make_shared<ChangeStateCommand>(make_shared<NormalControllState>()));
//    }
//
//    void getReplicaText() const {
//        return _replicaText;
//    }
//};

class Dialog {
public:
    Dialog();
	void addReplica(shared_ptr<Replica> replica);
	void nextReplica();
	void previousReplica();
	std::size_t getCurrentReplicaIndex() const;
	shared_ptr<Replica> getCurrentReplica();

	const vector<shared_ptr<Replica>>& getAvailableReplicas() const;
private:
	vector<shared_ptr<Replica>> _replicas;
    std::size_t _currentReplicaIndex;
};


class DialogManager {
public:
    DialogManager();
	uint32_t setNewDialog(Dialog dialog, uint32_t dialogID);
	uint32_t getCurrentDialogID() const;

	Dialog& getDialog(uint32_t dialogID);

    bool hasDialog(uint32_t dialogID);

    void setCurrentDialog(uint32_t dialogID);

    Dialog& getCurrentDialog();

    void setDialogPartner(entt::entity partner);
    entt::entity getDialogPartner() const;

    void setCurrentText(Text text);
    Text getCurrentText() const;
private:
	map<uint32_t, Dialog> _dialogs;

	uint32_t _currentDialog;
	Text _currentText;
	entt::entity _dialogPartner;
};


#endif // DIALOG_H_INCLUDED
