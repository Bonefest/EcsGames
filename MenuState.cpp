#include "Systems/GridRenderingSystem.h"
#include "Systems/HintMessageView.h"
#include "Systems/HUDSystem.h"

#include "MenuState.h"

#include "common.h"

MenuNormalState::MenuNormalState(SystemContainer& container): _container(container) { }

MenuNormalState::~MenuNormalState() { }

void MenuNormalState::onEnter(entt::registry& registry, entt::dispatcher& dispatcher) {
    Size visibleSize = Director::getInstance()->getVisibleSize();

    GameSettings& settings = registry.ctx<GameSettings>();

    if(_container.findSystem(Constants::ViewsTags::GridViewTag) == nullptr) {
        _container.addSystem(make_shared<GridRenderingView>(visibleSize * 0.1f, Size(visibleSize.width * 0.5f, visibleSize.height * 0.8f),
                                                           Size(settings.gridWidth, settings.gridHeight) * settings.cellSize), Constants::ViewsTags::GridViewTag);
    }

    if(_container.findSystem(Constants::ViewsTags::LogViewTag) == nullptr) {
        _container.addSystem(make_shared<LogRenderingView>(Vec2(visibleSize.width * 0.6f, visibleSize.height * 0.1f),
                                                          Size(visibleSize.width * 0.3f, visibleSize.height * 0.5f), dispatcher), Constants::ViewsTags::LogViewTag);
    }

    if(_container.findSystem(Constants::ViewsTags::HintViewTag) == nullptr) {
        _container.addSystem(make_shared<HintMessageView>(dispatcher, Rect(visibleSize * 0.1f, visibleSize * 0.5f)));
    }


    _container.setEnabledSystem(Constants::ViewsTags::GridViewTag, true);
    _container.setEnabledSystem(Constants::ViewsTags::LogViewTag,  true);
    _container.setEnabledSystem(Constants::ViewsTags::HintViewTag, true);

    dispatcher.trigger<HintMessageEvent>("<Normal mode>", Color3B::WHITE, 3.0f);
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
            stateOwner->setState(make_shared<MenuAttackState>(_container), registry, dispatcher);
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


MenuAttackState::MenuAttackState(SystemContainer& container): _container(container) { }

MenuAttackState::~MenuAttackState() { }

void MenuAttackState::update(entt::registry& registry, entt::dispatcher& dispatcher, float delta) {

}

void MenuAttackState::onEnter(entt::registry& registry, entt::dispatcher& dispatcher) {

    _container.setEnabledSystem(Constants::ViewsTags::LogViewTag, false);

    dispatcher.trigger<HintMessageEvent>("<Attack mode>\nChoose an enemy!", Color3B::WHITE, 3.0f);
}

shared_ptr<Command> MenuAttackState::generateCommand(IStateOwner* stateOwner, entt::registry& registry, entt::dispatcher& dispatcher, const UnprocessedKeyActionEvent& event) {
    if(event.key == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE) {
        stateOwner->setState(make_shared<MenuNormalState>(_container), registry, dispatcher);
    }


    return make_shared<NullCommand>();
}
