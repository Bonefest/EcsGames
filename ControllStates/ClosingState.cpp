#include "ClosingState.h"
#include "NormalState.h"

#include "../Events/Events.h"

void ClosingControllState::onEnter(IStateOwner* owner, entt::registry& registry, entt::dispatcher& dispatcher) {
    dispatcher.trigger<HintMessageEvent>("<Open mode>\nChoose closing direction", Color3B::WHITE, 3.0f);
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

        switch(event.keyType) {
        case MOVE_TOP_LEFT:    command = make_shared<CloseCommand>(player, Vec2(-1, 1)); break;
        case MOVE_TOP:         command = make_shared<CloseCommand>(player, Vec2( 0, 1)); break;
        case MOVE_TOP_RIGHT:   command = make_shared<CloseCommand>(player, Vec2( 1, 1)); break;
        case MOVE_LEFT:        command = make_shared<CloseCommand>(player, Vec2(-1, 0)); break;
        case MOVE_RIGHT:       command = make_shared<CloseCommand>(player, Vec2( 1, 0)); break;
        case MOVE_BOTTOM_LEFT: command = make_shared<CloseCommand>(player, Vec2(-1,-1)); break;
        case MOVE_BOTTOM:      command = make_shared<CloseCommand>(player, Vec2( 0,-1)); break;
        case MOVE_BOTTOM_RIGHT:command = make_shared<CloseCommand>(player, Vec2( 1,-1)); break;
        }

    }

    owner->setState(make_shared<NormalControllState>(_container), registry, dispatcher);
    return command;
}
