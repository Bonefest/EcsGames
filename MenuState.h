#ifndef MENUSTATE_H_INCLUDED
#define MENUSTATE_H_INCLUDED

#include <memory>

using std::make_shared;
using std::shared_ptr;

#include "Command.h"
#include "Events/Events.h"

#include "Systems/GridRenderingSystem.h"
#include "Systems/LogRenderingView.h"
#include "Systems/HUDSystem.h"
#include "SystemManager.h"

class MenuState;
struct UnprocessedKeyActionEvent;

typedef uint32_t tag;

class IStateOwner;

class MenuState {
public:
    virtual ~MenuState() { }
    virtual void onEnter(entt::registry& registry, entt::dispatcher& dispatcher) { }
    virtual void update(entt::registry& registry, entt::dispatcher& dispatcher, float delta) { }

    //virtual void processCommand(IStateOwner* stateOwner, entt::registy, const UnprocessedKeyActionEvent& event);
    virtual shared_ptr<Command> generateCommand(IStateOwner* stateOwner, entt::registry& registry, entt::dispatcher& dispatcher, const UnprocessedKeyActionEvent& event) = 0;
};

class MenuNormalState: public MenuState {
public:
    MenuNormalState(entt::registry& registry, entt::dispatcher& dispatcher, IStateOwner* owner);
    virtual ~MenuNormalState();
    virtual void onEnter(entt::registry& registry, entt::dispatcher& dispatcher);
    virtual void update(entt::registry& registry, entt::dispatcher& dispatcher, float delta);
    virtual shared_ptr<Command> generateCommand(IStateOwner* stateOwner, entt::registry& registry, entt::dispatcher& dispatcher, const UnprocessedKeyActionEvent& event);
private:
    IStateOwner* _owner;
};


class MenuAttackState: public MenuState {
public:
    MenuAttackState(entt::registry& registry);
    virtual ~MenuAttackState();
    virtual void update(entt::registry& registry, entt::dispatcher& dispatcher, float delta);
    virtual shared_ptr<Command> generateCommand(IStateOwner* stateOwner, entt::registry& registry, entt::dispatcher& dispatcher, const UnprocessedKeyActionEvent& event);

private:
    shared_ptr<GridRenderingView> _gridView;

};


class IStateOwner {
public:
    void setState(shared_ptr<MenuState> state, entt::registry& registry, entt::dispatcher& dispatcher) {
        _currentState = state;
        _currentState->onEnter(registry, dispatcher);
    }

    SystemContainer& getViewContainer() { return _viewContainer; }
protected:
    shared_ptr<MenuState> _currentState;
    SystemContainer _viewContainer;
};


#endif // MENUSTATE_H_INCLUDED
