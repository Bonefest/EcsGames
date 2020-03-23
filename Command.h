#ifndef COMMAND_H_INCLUDED
#define COMMAND_H_INCLUDED

#include "cocos2d.h"

USING_NS_CC;

class Command {
public:
    virtual ~Command() { }
    virtual void execute() = 0;
};

class MoveCommand: public Command {
public:
    MoveCommand(entt::registry& registry, entt::entity entity, Vec2 direction): _registry(registry),
                                                                                _receiver(entity),
                                                                                _direction(direction) { }

    virtual void execute() {
        //if cell is passable -> erase from current position -> add to new position -> create message "Moving to {NORTH; NORTH-EAST and so on}
        //else create message "Moving impossible cause {get name of cell}"

        WorldData& data = _registry.ctx<WorldData>();
        if(_registry.valid(_receiver)) {
            Cell& cellComponent = _registry.get<Cell>(_receiver);
            uint16_t newX = cellComponent.x + uint16_t(_direction.x);
            uint16_t newY = cellComponent.y + uint16_t(_direction.y);

            if(isValidDirection(newX, newY)) {
                if(!_registry.valid(data.creatures[newY][newX])) { //&& data.objects[newY][newX] is passable
                    data.creatures[cellComponent.y][cellComponent.x] = entt::null;

                    cellComponent.x = newX;
                    cellComponent.y = newY;

                    data.creatures[newY][newX] = _receiver;

                    //trigger "EntityMovedEvent"
                }
            }
        }
    }

private:
    bool isValidDirection(uint16_t x, uint16_t y) {
        GameSettings& gameSettings = _registry.ctx<GameSettings>();
        return (x >= 0 && y >=0 && x < gameSettings.gridWidth && y < gameSettings.gridHeight);
    }

    entt::registry& _registry;
    entt::entity _receiver;
    Vec2 _direction;
};

class NullCommand: public Command {
public:
    virtual void execute() { }
};

#endif // COMMAND_H_INCLUDED
