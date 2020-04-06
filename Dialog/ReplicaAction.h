#ifndef REPLICAACTION_H_INCLUDED
#define REPLICAACTION_H_INCLUDED

#include "../common.h"
#include "../Dependencies/entt.hpp"

#include <list>
#include <memory>

using std::list;
using std::shared_ptr;

class ReplicaAction {
public:
    virtual void execute(entt::registry& registry, entt::dispatcher& dispatcher) = 0;
};

class CloseDialog: public ReplicaAction {
public:
    void execute(entt::registry& registry, entt::dispatcher& dispatcher);
};

class SwitchDialog: public ReplicaAction {
public:
    SwitchDialog(ID dialogID, Text answerText);
    void execute(entt::registry& registry, entt::dispatcher& dispatcher);

private:
    ID _dialogID;
    Text _answerText;
};

class SetNextDefaultDialog: public ReplicaAction {
public:
    SetNextDefaultDialog(ID dialogID);
    void execute(entt::registry& registry, entt::dispatcher& dispatcher);

private:
    ID _nextDefaultDialogID;
};


#endif // REPLICAACTION_H_INCLUDED
