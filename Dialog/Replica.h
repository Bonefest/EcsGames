#ifndef REPLICA_H_INCLUDED
#define REPLICA_H_INCLUDED

#include <list>
#include <memory>

using std::list;
using std::shared_ptr;

#include "../common.h"
#include "ReplicaAction.h"
#include "../Dependencies/entt.hpp"


class Replica {
public:
    Replica(Text title);

    Text getReplicaTitle() const;

    void addReplicaAction(shared_ptr<ReplicaAction> replicaAction);
    void activateReplica(entt::registry& registry, entt::dispatcher& dispatcher);

private:
    Text _replicaTitle;
    list<shared_ptr<ReplicaAction>> _replicaActionContainer;
};


#endif // REPLICA_H_INCLUDED
