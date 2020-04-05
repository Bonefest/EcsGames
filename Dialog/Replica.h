#ifndef REPLICA_H_INCLUDED
#define REPLICA_H_INCLUDED

#include "../common.h"

#include "../Dependencies/entt.hpp"

class Replica {
public:
	virtual void useReplica(entt::registry& registry, entt::dispatcher& dispatcher, entt::entity replicaOwner) = 0;

    void setReplicaText(Text text) {
        _replicaText = text;
    }
    Text getReplicaText() const {
        return _replicaText;
    }

protected:
    Text _replicaText;
};

class CloseDialogReplica: public Replica {
public:
    CloseDialogReplica(Text replicaText);

    void useReplica(entt::registry& registry, entt::dispatcher& dispatcher, entt::entity replicaOwner);


};

class SwitchDialogReplica: public Replica {
public:
    SwitchDialogReplica(uint32_t dialogID, Text switchText, Text replicaText);
    void useReplica(entt::registry& registry, entt::dispatcher& dispatcher, entt::entity replicaOwner);

private:
    uint32_t _dialogID;
    Text _switchText;
};


#endif // REPLICA_H_INCLUDED
