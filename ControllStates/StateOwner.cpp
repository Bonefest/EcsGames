#include "StateOwner.h"

IStateOwner::IStateOwner() {
    _viewContainer = make_shared<SystemContainer>();
}

void IStateOwner::setState(shared_ptr<ControllState> state, entt::registry& registry, entt::dispatcher& dispatcher) {
    _currentState = state;
    _currentState->onEnter(this, registry, dispatcher);
}

