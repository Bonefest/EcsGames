#include "InventoryState.h"
#include "NormalState.h"
#include "common.h"

#include "../Components/Components.h"

void InventoryControllState::onEnter(IStateOwner* owner, entt::registry& registry, entt::dispatcher& dispatcher) {
    initItems(registry);

    _itemInfoView = make_shared<ItemInfoView>();
    owner->getViewContainer().setEnabledAllSystems(false);

    _itemListView = make_shared<ItemListView>();
    _itemListView->setItems(_itemsNames);


    owner->getViewContainer().addSystem(_itemInfoView, Constants::ViewsTags::ItemViewTag);
    owner->getViewContainer().addSystem(_itemListView, Constants::ViewsTags::ItemListTag);

    //TEST

    _itemInfoView->setItem(registry, registry.ctx<ItemDatabase>().createItem(registry, 1));
}

void InventoryControllState::initItems(entt::registry& registry) {
    _itemsNames.clear();
    registry.view<Controllable, Creature>().each([&](entt::entity player, Controllable& controllable, Creature& creature) {
        _items = creature.inventoryItems;
        for(auto item : _items) {
            if(registry.has<ItemComponent>(item)) {
                ItemComponent& itemComponent = registry.get<ItemComponent>(item);
                _itemsNames.push_back(itemComponent.name);
            }
        }
    });
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
    } else if(event.keyType == KeyType::MOVE_TOP) {
        if(!_itemsNames.empty()) {
            _itemListView->setActiveItemIndex(int64_t(_itemListView->getActiveItemIndex() - 1) % _itemsNames.size());
        }
    } else if(event.keyType == KeyType::MOVE_BOTTOM) {
        if(!_itemsNames.empty()) {
            _itemListView->setActiveItemIndex(int64_t(_itemListView->getActiveItemIndex() + 1) % _itemsNames.size());
        }
    } else if(event.keyType == KeyType::USE) {
        std::size_t itemIndex = _itemListView->getActiveItemIndex();
        if(itemIndex < _items.size()) {
            _itemInfoView->setItem(registry, _items[itemIndex]);
        }
    }
    return make_shared<NullCommand>();
}


void InventoryControllState::onExit(IStateOwner* owner,
                                    entt::registry& registry,
                                    entt::dispatcher& dispatcher) {

    owner->getViewContainer().removeSystem(Constants::ViewsTags::ItemViewTag);
    owner->getViewContainer().removeSystem(Constants::ViewsTags::ItemListTag);
    owner->getViewContainer().setEnabledAllSystems(true);
}
