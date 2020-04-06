#ifndef COMMAND_H_INCLUDED
#define COMMAND_H_INCLUDED

#include "ControllStates/ControllState.h"
#include "ControllStates/StateOwner.h"
#include "Components/Components.h"
#include "Dependencies/entt.hpp"
#include "Events/Events.h"
#include "cocos2d.h"

#include "helper.h"

USING_NS_CC;

class ControllState;
class IStateOwner;

class Command {
public:
    virtual ~Command() { }
    virtual void execute(IStateOwner* owner, entt::registry& registry, entt::dispatcher& dispatcher) = 0;
};

class MoveCommand: public Command {
public:
    MoveCommand(entt::entity entity, Vec2 direction);

    virtual void execute(IStateOwner* owner, entt::registry& registry, entt::dispatcher& dispatcher);

private:
    entt::entity _receiver;
    Vec2 _direction;
};

class OpenCommand: public Command {
public:
    OpenCommand(entt::entity player, Vec2 direction);

    virtual void execute(IStateOwner* owner, entt::registry& registry, entt::dispatcher& dispatcher);


private:
    entt::entity _receiver;
    Vec2 _direction;
};

class CloseCommand : public Command {
public:
    CloseCommand(entt::entity player, Vec2 direction);

    virtual void execute(IStateOwner* owner, entt::registry& registry, entt::dispatcher& dispatcher);
private:
    entt::entity _receiver;
    Vec2 _direction;
};

class SetStateCommand : public Command {
public:
    SetStateCommand(shared_ptr<ControllState> state);

    void execute(IStateOwner* owner, entt::registry& registry, entt::dispatcher& dispatcher);
private:
    shared_ptr<ControllState> _state;
};

class NullCommand: public Command {
public:
    virtual void execute(IStateOwner* owner, entt::registry& registry, entt::dispatcher& dispatcher);
};
#endif // COMMAND_H_INCLUDED
