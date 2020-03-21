#ifndef MENUSTATE_H_INCLUDED
#define MENUSTATE_H_INCLUDED

#include <memory>

using std::make_shared;
using std::shared_ptr;

#include "Command.h"
#include "Events/Events.h"

class MenuState;
struct UnprocessedKeyActionEvent;

class IStateOwner {
public:
    void setState(shared_ptr<MenuState> state) {
        _currentState = state;
    }

protected:
    shared_ptr<MenuState> _currentState;
};

class MenuState {
public:
    virtual ~MenuState() { }
    virtual shared_ptr<Command> generateCommand(IStateOwner* stateOwner, entt::registry& registry, const UnprocessedKeyActionEvent& event) = 0;
};

class MenuNormalState: public MenuState {
public:
    virtual ~MenuNormalState() { }
    virtual shared_ptr<Command> generateCommand(IStateOwner* stateOwner, entt::registry& registry, const UnprocessedKeyActionEvent& event) {
        entt::entity player = entt::null;
        auto view = registry.view<Controllable>();
        for(entt::entity entity : view) {
            player = entity; break;
        }

        assert(registry.valid(player) && "Unable to generate a command cause cannot find a controllable entity!");

        switch(event.keyType) {
        case MOVE_TOP_LEFT:     return make_shared<MoveCommand>(registry, player, Vec2(-1,  1));
        case MOVE_TOP:          return make_shared<MoveCommand>(registry, player, Vec2( 0,  1));
        case MOVE_TOP_RIGHT:    return make_shared<MoveCommand>(registry, player, Vec2( 1,  1));
        case MOVE_RIGHT:        return make_shared<MoveCommand>(registry, player, Vec2( 1,  0));
        case MOVE_BOTTOM_RIGHT: return make_shared<MoveCommand>(registry, player, Vec2( 1, -1));
        case MOVE_BOTTOM:       return make_shared<MoveCommand>(registry, player, Vec2( 0, -1));
        case MOVE_BOTTOM_LEFT:  return make_shared<MoveCommand>(registry, player, Vec2(-1, -1));
        case MOVE_LEFT:         return make_shared<MoveCommand>(registry, player, Vec2(-1,  0));
        }

        return make_shared<NullCommand>();
    }
};

#endif // MENUSTATE_H_INCLUDED
