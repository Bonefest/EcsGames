#include "AttackState.h"
#include "NormalState.h"

AttackControllState::~AttackControllState() { }

void AttackControllState::update(IStateOwner* owner, entt::registry& registry, entt::dispatcher& dispatcher, float delta) {

}

void AttackControllState::onEnter(IStateOwner* owner, entt::registry& registry, entt::dispatcher& dispatcher) {

    owner->getViewContainer().setEnabledSystem(Constants::ViewsTags::LogViewTag, false);

    dispatcher.trigger<HintMessageEvent>(Text{"<Attack mode>\nChoose an enemy!", Color3B::WHITE}, 3.0f);
}

shared_ptr<Command> AttackControllState::handleInputEvent(IStateOwner* stateOwner,
                                                          entt::registry& registry,
                                                          entt::dispatcher& dispatcher,
                                                          const UnprocessedKeyActionEvent& event) {
    if(event.key == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE) {
        stateOwner->setState(make_shared<NormalControllState>(), registry, dispatcher);
    }


    return make_shared<NullCommand>();
}
