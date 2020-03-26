#include "StateOwner.h"

void IStateOwner::setState(shared_ptr<ControllState> state, entt::registry& registry, entt::dispatcher& dispatcher) {
    _currentState = state;
    _currentState->onEnter(this, registry, dispatcher);
}

