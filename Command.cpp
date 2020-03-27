#include "Command.h"

MoveCommand::MoveCommand(entt::entity entity, Vec2 direction): _receiver(entity),
                                                               _direction(direction) { }

void MoveCommand::execute(IStateOwner* owner, entt::registry& registry, entt::dispatcher& dispatcher) {
    //if cell is passable -> erase from current position -> add to new position -> create message "Moving to {NORTH; NORTH-EAST and so on}
    //else create message "Moving impossible cause {get name of cell}"
    //also need a check is there a floor (and ability of a person to walk through air)

    WorldData& data = registry.ctx<WorldData>();
    if(registry.valid(_receiver)) {
        Cell& cellComponent = registry.get<Cell>(_receiver);
        uint16_t newX = cellComponent.x + uint16_t(_direction.x);
        uint16_t newY = cellComponent.y + uint16_t(_direction.y);

        if(isValidPosition(newX, newY)) {
            if(!registry.valid(data.creatures[newY][newX])) { //&& data.objects[newY][newX] is passable
                bool passable = true;
                for(auto object : data.objects[newY][newX]) {
                    Cell& objectCellComponent = registry.get<Cell>(object);
                    if(!objectCellComponent.passable) {
                        passable = false;
                        break;
                    }
                }

                if(passable) {
                    data.creatures[cellComponent.y][cellComponent.x] = entt::null;

                    cellComponent.x = newX;
                    cellComponent.y = newY;

                    data.creatures[newY][newX] = _receiver;
                }

                //trigger "EntityMovedEvent"
            }
        }
    }
}

OpenCommand::OpenCommand(entt::entity player, Vec2 direction): _receiver(player),
                                                    _direction(direction) { }

void OpenCommand::execute(IStateOwner* owner, entt::registry& registry, entt::dispatcher& dispatcher) {
    if(registry.valid(_receiver)) {
        WorldData& data = registry.ctx<WorldData>();

        Cell& cellComponent = registry.get<Cell>(_receiver);
        uint16_t newX = cellComponent.x + uint16_t(_direction.x);
        uint16_t newY = cellComponent.y + uint16_t(_direction.y);


        entt::entity doorEntity = entt::null;

        if(isValidPosition(newX, newY)) {
            for(auto entity: data.objects[newY][newX]) {
                if(registry.has<Lockable>(entity)) {
                    doorEntity = entity;
                    break;
                }
            }

            if(doorEntity != entt::null) {
                Cell& doorCellComponent = registry.get<Cell>(doorEntity);
                Drawable& doorDrawableComponent = registry.get<Drawable>(doorEntity);
                Lockable& doorLockableComponent = registry.get<Lockable>(doorEntity);

                if(doorCellComponent.passable) {
                    dispatcher.trigger<MessageEvent>(Text{StringUtils::format("<%s> is open!", doorCellComponent.name.c_str()), Color3B::WHITE});
                } else {
                    //Some logic about opening duration (calculated with player locking skill and so on)
                    //and some logic about failing to open a door (don't have a key)

                    doorCellComponent.passable = true;
                    doorCellComponent.transparent = true;
                    doorDrawableComponent.animations.setCurrentState(Constants::AnimationsTags::OpenedAnimationTag);

                    dispatcher.trigger<MessageEvent>(Text{StringUtils::format("You have opened <%s>", doorCellComponent.name.c_str()), Color3B::GREEN});
                }
            }
        }

        if(doorEntity == entt::null) {
            dispatcher.trigger<MessageEvent>(Text{"There is nothing to open!", Color3B::WHITE});
        }
    }
}

CloseCommand::CloseCommand(entt::entity player, Vec2 direction): _receiver(player),
                                                                 _direction(direction) { }

void CloseCommand::execute(IStateOwner* owner, entt::registry& registry, entt::dispatcher& dispatcher) {
    if(registry.valid(_receiver)) {
        WorldData& data = registry.ctx<WorldData>();

        Cell& cellComponent = registry.get<Cell>(_receiver);
        uint16_t newX = cellComponent.x + uint16_t(_direction.x);
        uint16_t newY = cellComponent.y + uint16_t(_direction.y);


        entt::entity doorEntity = entt::null;

        if(isValidPosition(newX, newY)) {
            for(auto entity: data.objects[newY][newX]) {
                if(registry.has<Lockable>(entity)) {
                    doorEntity = entity;
                    break;
                }
            }

            if(doorEntity != entt::null) {
                Cell& doorCellComponent = registry.get<Cell>(doorEntity);
                Drawable& doorDrawableComponent = registry.get<Drawable>(doorEntity);
                Lockable& doorLockableComponent = registry.get<Lockable>(doorEntity);

                if(!doorCellComponent.passable) {
                    dispatcher.trigger<MessageEvent>(Text{StringUtils::format("<%s> is closed!", doorCellComponent.name.c_str()), Color3B::WHITE});
                } else {
                    //Some logic about opening duration (calculated with player locking skill and so on)
                    //and some logic about failing to open a door (don't have a key)

                    doorCellComponent.passable = false;
                    doorCellComponent.transparent = false;  //if door is not initially transparent = set transparent = false else leave unchanged;
                    doorDrawableComponent.animations.setCurrentState(Constants::AnimationsTags::ClosedAnimationTag);

                    dispatcher.trigger<MessageEvent>(Text{StringUtils::format("You have closed <%s>", doorCellComponent.name.c_str()), Color3B::GREEN});
                }
            }
        }

        if(doorEntity == entt::null) {
            dispatcher.trigger<MessageEvent>(Text{"There is nothing to close!", Color3B::WHITE});
        }
    }
}

void NullCommand::execute(IStateOwner* owner, entt::registry& registry, entt::dispatcher& dispatcher) { }
