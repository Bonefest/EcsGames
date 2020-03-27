#include "ClosingState.h"
#include "NormalState.h"

#include "../Events/Events.h"

#include "../helper.h"

void ClosingControllState::onEnter(IStateOwner* owner, entt::registry& registry, entt::dispatcher& dispatcher) {
    dispatcher.trigger<HintMessageEvent>(Text{"<Open mode>\nChoose closing direction", Color3B::WHITE}, 3.0f);
}

shared_ptr<Command> ClosingControllState::handleInputEvent(IStateOwner* owner,
                                                          entt::registry& registry,
                                                          entt::dispatcher& dispatcher,
                                                          const UnprocessedKeyActionEvent& event) {

    entt::entity player = entt::null;

    auto playerView = registry.view<Controllable>();
    for(auto entity : playerView) {
        player = entity;
        break;
    }

    shared_ptr<Command> command = make_shared<NullCommand>();

    if(player != entt::null) {

        if(isDirectionKeyType(event.keyType)) {
            command = make_shared<CloseCommand>(player, directionKeyTypeToVector(event.keyType));
        }

    }

    owner->setState(make_shared<NormalControllState>(), registry, dispatcher);
    return command;
}
