#ifndef MENUSTATE_H_INCLUDED
#define MENUSTATE_H_INCLUDED

#include <memory>

using std::make_shared;
using std::shared_ptr;

#include "Command.h"
#include "Events/Events.h"

#include "Systems/GridRenderingSystem.h"
#include "Systems/HUDSystem.h"


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
    virtual void onEnter(entt::registry& registry, entt::dispatcher& dispatcher, float delta) { }
    virtual void update(entt::registry& registry, entt::dispatcher& dispatcher, float delta) { }

    //virtual void processCommand(IStateOwner* stateOwner, entt::registy, const UnprocessedKeyActionEvent& event);
    virtual shared_ptr<Command> generateCommand(IStateOwner* stateOwner, entt::registry& registry, const UnprocessedKeyActionEvent& event) = 0;
};

class MenuNormalState: public MenuState {
public:
    MenuNormalState(entt::registry& registry);
    virtual ~MenuNormalState();
    virtual void update(entt::registry& registry, entt::dispatcher& dispatcher, float delta);
    virtual shared_ptr<Command> generateCommand(IStateOwner* stateOwner, entt::registry& registry, const UnprocessedKeyActionEvent& event);
private:
    shared_ptr<GridRenderingView> _gridView;
    shared_ptr<HUDSystem> _hudView;
};


class MenuAttackState: public MenuState {
public:
    MenuAttackState(entt::registry& registry);
    virtual ~MenuAttackState();
    virtual void update(entt::registry& registry, entt::dispatcher& dispatcher, float delta);
    virtual shared_ptr<Command> generateCommand(IStateOwner* stateOwner, entt::registry& registry, const UnprocessedKeyActionEvent& event);

private:
    shared_ptr<GridRenderingView> _gridView;

};

#endif // MENUSTATE_H_INCLUDED
