#include "InventoryState.h"
#include "NormalState.h"
#include "common.h"

void InventoryControllState::onEnter(IStateOwner* owner, entt::registry& registry, entt::dispatcher& dispatcher) {
    _itemInfoView = make_shared<ItemInfoView>();
    owner->getViewContainer().setEnabledAllSystems(false);
    owner->getViewContainer().addSystem(_itemInfoView, Constants::ViewsTags::ItemViewTag);

    //TEST

    _itemInfoView->setItem(registry, registry.ctx<ItemDatabase>().createItem(registry, 1));
}

void InventoryControllState::update(IStateOwner* owner,
                                    entt::registry& registry,
                                    entt::dispatcher& dispatcher,
                                    float delta) { }

shared_ptr<Command> InventoryControllState::handleInputEvent(IStateOwner* owner,
                                                             entt::registry& registry,
                                                             entt::dispatcher& dispatcher,
                                                             const UnprocessedKeyActionEvent& event) {

    if(event.key == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE) {
        owner->setState(make_shared<NormalControllState>(), registry, dispatcher);
    }
    return make_shared<NullCommand>();
}


void InventoryControllState::onExit(IStateOwner* owner,
                                    entt::registry& registry,
                                    entt::dispatcher& dispatcher) {

    owner->getViewContainer().removeSystem(Constants::ViewsTags::ItemViewTag);
    owner->getViewContainer().setEnabledAllSystems(true);
}
