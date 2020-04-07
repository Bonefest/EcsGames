#include "InventoryState.h"

void InventoryControllState::onEnter(IStateOwner* owner, entt::registry& registry, entt::dispatcher& dispatcher) {

}

void InventoryControllState::update(IStateOwner* owner,
                                    entt::registry& registry,
                                    entt::dispatcher& dispatcher,
                                    float delta) { }

shared_ptr<Command> InventoryControllState::handleInputEvent(IStateOwner* owner,
                                                             entt::registry& registry,
                                                             entt::dispatcher& dispatcher,
                                                             const UnprocessedKeyActionEvent& event) {

    return make_shared<NullCommand>();
}
