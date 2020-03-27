#include "NormalState.h"

#include "../Systems/GridRenderingSystem.h"
#include "../Systems/LogRenderingView.h"
#include "../Systems/HintMessageView.h"
#include "../Systems/HUDSystem.h"

#include "../common.h"

#include "AttackState.h"
#include "OpeningState.h"
#include "ClosingState.h"

NormalControllState::NormalControllState(SystemContainer& container): _container(container) { }

NormalControllState::~NormalControllState() { }

void NormalControllState::onEnter(IStateOwner* owner, entt::registry& registry, entt::dispatcher& dispatcher) {
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

    dispatcher.trigger<HintMessageEvent>(Text{"<Normal mode>", Color3B::WHITE}, 3.0f);
}

void NormalControllState::update(IStateOwner* owner, entt::registry& registry, entt::dispatcher& dispatcher, float delta) {

}

shared_ptr<Command> NormalControllState::handleInputEvent(IStateOwner* owner,
                                                          entt::registry& registry,
                                                          entt::dispatcher& dispatcher,
                                                          const UnprocessedKeyActionEvent& event) {
        entt::entity player = entt::null;
        auto view = registry.view<Controllable>();
        for(entt::entity entity : view) {
            player = entity; break;
        }

        if(event.key == cocos2d::EventKeyboard::KeyCode::KEY_A) {
            owner->setState(make_shared<AttackControllState>(_container), registry, dispatcher);
        }

        assert(registry.valid(player) && "Unable to generate a command cause cannot find a controllable entity!");

        switch(event.keyType) {
        case MOVE_TOP_LEFT:     return make_shared<MoveCommand>(player, Vec2(-1,  1));
        case MOVE_TOP:          return make_shared<MoveCommand>(player, Vec2( 0,  1));
        case MOVE_TOP_RIGHT:    return make_shared<MoveCommand>(player, Vec2( 1,  1));
        case MOVE_RIGHT:        return make_shared<MoveCommand>(player, Vec2( 1,  0));
        case MOVE_BOTTOM_RIGHT: return make_shared<MoveCommand>(player, Vec2( 1, -1));
        case MOVE_BOTTOM:       return make_shared<MoveCommand>(player, Vec2( 0, -1));
        case MOVE_BOTTOM_LEFT:  return make_shared<MoveCommand>(player, Vec2(-1, -1));
        case MOVE_LEFT:         return make_shared<MoveCommand>(player, Vec2(-1,  0));
        case OPEN:              owner->setState(make_shared<OpeningControllState>(_container), registry, dispatcher); break;
        case CLOSE:             owner->setState(make_shared<ClosingControllState>(_container), registry, dispatcher); break;
        }

        return make_shared<NullCommand>();
}

