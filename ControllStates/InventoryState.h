#ifndef INVENTORYSTATE_H_INCLUDED
#define INVENTORYSTATE_H_INCLUDED

#include "ControllState.h"
#include "../Systems/ItemInfoView.h"
#include "../Systems/ItemListView.h"

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
    void initItems(entt::registry& registry);

    shared_ptr<ItemInfoView> _itemInfoView;
    shared_ptr<ItemListView> _itemListView;

    vector<string> _itemsNames;
    vector<entt::entity> _items;
};


#endif // INVENTORYSTATE_H_INCLUDED
