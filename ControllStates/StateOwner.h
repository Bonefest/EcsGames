#ifndef STATEOWNER_H_INCLUDED
#define STATEOWNER_H_INCLUDED

#include "ControllState.h"
#include "../SystemContainer.h"

class ControllState;
class SystemContainer;

class IStateOwner {
public:
    IStateOwner();
    void setState(shared_ptr<ControllState> state, entt::registry& registry, entt::dispatcher& dispatcher);
    SystemContainer& getViewContainer() { return *_viewContainer; }
protected:
    shared_ptr<ControllState> _currentState;
    shared_ptr<SystemContainer> _viewContainer;
};


#endif // STATEOWNER_H_INCLUDED
