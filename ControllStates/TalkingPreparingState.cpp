#include "TalkingPreparingState.h"

#include "../helper.h"
#include "NormalState.h"
#include "TalkingState.h"

TalkingPreparingState::~TalkingPreparingState() { }

void TalkingPreparingState::onEnter(IStateOwner* owner,
                                    entt::registry& registry,
                                    entt::dispatcher& dispatcher) {
    dispatcher.trigger<HintMessageEvent>(Text{"<Talking>\nChoose an entity to talk.", Color3B::WHITE}, 3.0f);
}

shared_ptr<Command> TalkingPreparingState::handleInputEvent(IStateOwner* owner,
                                                            entt::registry& registry,
                                                            entt::dispatcher& dispatcher,
                                                            const UnprocessedKeyActionEvent& event) {

    entt::entity player = entt::null;
    auto playerView = registry.view<Controllable>();
    for(auto entity : playerView) {
        player = entity;
        break;
    }

    if(player != entt::null) {
        if(isDirectionKeyType(event.keyType)) {


            Cell& playerCell = registry.get<Cell>(player);
            WorldData& data = registry.ctx<WorldData>();

            Vec2 direction = directionKeyTypeToVector(event.keyType);

            uint16_t newX = uint16_t(playerCell.x + direction.x);
            uint16_t newY = uint16_t(playerCell.y + direction.y);

            if(isValidPosition(newX, newY)) {
                entt::entity speakableEntity = entt::null;
                if(registry.valid(data.creatures[newY][newX]) &&
                   registry.has<Speakable>(data.creatures[newY][newX])) {
                        speakableEntity = data.creatures[newY][newX];
                }


                if(speakableEntity == entt::null) {
                    for(auto entity : data.objects[newY][newX]) {
                        if(registry.has<Speakable>(entity)) {
                            speakableEntity = entity;
                            break;
                        }
                    }
                }

                if(speakableEntity != entt::null) {
                    owner->setState(make_shared<TalkingControllState>(speakableEntity), registry, dispatcher);
                    return make_shared<NullCommand>();
                } else {
                    dispatcher.trigger<MessageEvent>(Text{"There is nothing to talk with...", Color3B::WHITE});
                }
            }

        }
    }

    owner->setState(make_shared<NormalControllState>(), registry, dispatcher);
    return make_shared<NullCommand>();
}
