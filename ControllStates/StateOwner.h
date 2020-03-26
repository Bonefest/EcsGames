#ifndef STATEOWNER_H_INCLUDED
#define STATEOWNER_H_INCLUDED

#include "ControllState.h"
#include "../SystemContainer.h"

class ControllState;

class IStateOwner {
public:
    void setState(shared_ptr<ControllState> state, entt::registry& registry, entt::dispatcher& dispatcher);
    SystemContainer& getViewContainer() { return _viewContainer; }
protected:
    shared_ptr<ControllState> _currentState;
    SystemContainer _viewContainer;
};


#endif // STATEOWNER_H_INCLUDED
