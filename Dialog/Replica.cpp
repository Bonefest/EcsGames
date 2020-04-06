#include "Replica.h"

Replica::Replica(Text title): _replicaTitle(title) { }

Text Replica::getReplicaTitle() const {
    return _replicaTitle;
}

void Replica::addReplicaAction(shared_ptr<ReplicaAction> replicaAction) {
    _replicaActionContainer.push_back(replicaAction);
}

void Replica::activateReplica(entt::registry& registry, entt::dispatcher& dispatcher) {
    for(auto replicaAction: _replicaActionContainer) {
        replicaAction->execute(registry, dispatcher);
    }
}
