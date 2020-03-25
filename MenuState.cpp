#include "Systems/GridRenderingSystem.h"
#include "Systems/HUDSystem.h"

#include "MenuState.h"

#include "common.h"

MenuNormalState::MenuNormalState(entt::registry& registry, entt::dispatcher& dispatcher, IStateOwner* owner): _owner(owner) {

}

MenuNormalState::~MenuNormalState() { }

void MenuNormalState::onEnter(entt::registry& registry, entt::dispatcher& dispatcher) {
    Size visibleSize = Director::getInstance()->getVisibleSize();

    GameSettings& settings = registry.ctx<GameSettings>();

    SystemContainer& container = _owner->getViewContainer();

    if(container.findSystem(ViewsTags::GridViewTag) == nullptr) {
        container.addSystem(make_shared<GridRenderingView>(visibleSize * 0.1f, Size(visibleSize.width * 0.5f, visibleSize.height * 0.8f),
                                                           Size(settings.gridWidth, settings.gridHeight) * settings.cellSize), ViewsTags::GridViewTag);
    }

    if(container.findSystem(ViewsTags::LogViewTag) == nullptr) {
        container.addSystem(make_shared<LogRenderingView>(Vec2(visibleSize.width * 0.6f, visibleSize.height * 0.1f),
                                                          Size(visibleSize.width * 0.3f, visibleSize.height * 0.5f), dispatcher), ViewsTags::LogViewTag);
    }
}

void MenuNormalState::update(entt::registry& registry, entt::dispatcher& dispatcher, float delta) {

}

shared_ptr<Command> MenuNormalState::generateCommand(IStateOwner* stateOwner, entt::registry& registry, entt::dispatcher& dispatcher, const UnprocessedKeyActionEvent& event) {
        entt::entity player = entt::null;
        auto view = registry.view<Controllable>();
        for(entt::entity entity : view) {
            player = entity; break;
        }

        if(event.key == cocos2d::EventKeyboard::KeyCode::KEY_A) {
            stateOwner->setState(make_shared<MenuAttackState>(registry), registry, dispatcher);
        }

        dispatcher.trigger<MessageEvent>("Lorem ipsum dolor glsdlglsd lgsdlglsd glsdl glsdl gsld glsdlglsdg lsdlgdslglsdlg lsdl gsldgldslgdslgldslgdslglsdgldslgldsglsdl", Color3B::RED);

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

////////////////////////////////////////////////////////


MenuAttackState::MenuAttackState(entt::registry& registry) {
    Size visibleSize = Director::getInstance()->getVisibleSize();

    GameSettings& settings = registry.ctx<GameSettings>();

    _gridView = make_shared<GridRenderingView>(visibleSize * 0.1f,
                                               Size(visibleSize.width * 0.5f, visibleSize.height * 0.8f),
                                               Size(settings.gridWidth, settings.gridHeight) * settings.cellSize);
}

MenuAttackState::~MenuAttackState() { }

void MenuAttackState::update(entt::registry& registry, entt::dispatcher& dispatcher, float delta) {
    _gridView->update(registry, dispatcher, delta);
}

shared_ptr<Command> MenuAttackState::generateCommand(IStateOwner* stateOwner, entt::registry& registry, entt::dispatcher& dispatcher, const UnprocessedKeyActionEvent& event) {
    entt::entity player = entt::null;
    auto view = registry.view<Controllable>();
    for(entt::entity entity : view) {
        player = entity; break;
    }

//    if(event.key == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE) {
//        stateOwner->setState(make_shared<MenuNormalState>(registry));
//    }

    assert(registry.valid(player) && "Unable to generate a command cause cannot find a controllable entity!");

    return make_shared<NullCommand>();
}
