#ifndef REPLICA_H_INCLUDED
#define REPLICA_H_INCLUDED

#include <list>
#include <memory>

using std::list;
using std::shared_ptr;

#include "../common.h"

#include "../Dependencies/entt.hpp"

class Replica {
public:
	virtual void useReplica(entt::registry& registry, entt::dispatcher& dispatcher) = 0;

    void setReplicaText(Text text) {
        _replicaText = text;
    }
    Text getReplicaText() const {
        return _replicaText;
    }

protected:
    Text _replicaText;
};

class MultipleReplica: public Replica {
public:
    void useReplica(entt::registry& registry, entt::dispatcher& dispatcher);

    void addReplica(shared_ptr<Replica> replica);

private:
    list<shared_ptr<Replica>> _replicas;
};

class CloseDialogReplica: public Replica {
public:
    CloseDialogReplica(Text replicaText);

    void useReplica(entt::registry& registry, entt::dispatcher& dispatcher);


};

class SwitchDialogReplica: public Replica {
public:
    SwitchDialogReplica(ID dialogID, Text answerText, Text replicaText);
    void useReplica(entt::registry& registry, entt::dispatcher& dispatcher);

private:
    ID _dialogID;
    Text _answerText;
};

class SetNextDefaultDialogReplica: public Replica {
public:
    SetNextDefaultDialogReplica(ID dialogID, Text replicaText);
    void useReplica(entt::registry& registry, entt::dispatcher& dispatcher);

private:
    ID _nextDefaultDialogID;
};


#endif // REPLICA_H_INCLUDED
