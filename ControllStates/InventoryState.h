#ifndef INVENTORYSTATE_H_INCLUDED
#define INVENTORYSTATE_H_INCLUDED

#include "ControllState.h"
#include "../Systems/ItemInfoView.h"

class InventoryControllState: public ControllState {
public:

    virtual void onEnter(IStateOwner* owner, entt::registry& registry, entt::dispatcher& dispatcher);
    virtual void update(IStateOwner* owner, entt::registry& registry, entt::dispatcher& dispatcher, float delta);
    virtual shared_ptr<Command> handleInputEvent(IStateOwner* owner,
                                                entt::registry& registry,
                                                entt::dispatcher& dispatcher,
                                                const UnprocessedKeyActionEvent& event);
    virtual void onExit(IStateOwner* owner, entt::registry& registry, entt::dispatcher& dispatcher);
private:
    shared_ptr<ItemInfoView> _itemInfoView;
};


#endif // INVENTORYSTATE_H_INCLUDED
