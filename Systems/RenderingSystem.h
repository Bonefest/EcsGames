#ifndef _RENDERINGSYSTEM_H_
#define _RENDERINGSYSTEM_H_

#include "../Dependencies/entt.hpp"
#include "System.h"

#include "../Components/Components.h"

class RenderingSystem: public ISystem {
public:
	~RenderingSystem() { }
	virtual void update(entt::registry& registry, entt::dispatcher& dispatcher, float delta) {
		auto view = registry.view<Transform, Drawable>();
		view.each([&](entt::entity entity, Transform& transformComponent, Drawable& drawableComponent) {
            drawableComponent.sprite->setContentSize(transformComponent.size);
            drawableComponent.sprite->setPosition(transformComponent.position);
		});
	}
};

#endif

struct GameSettings {
    GameSettings(): cellWidth(32), cellHeight(32) { }

    uint16_t cellWidth;
    uint16_t cellHeight;

    uint16_t gridWidth;
    uint16_t gridHeight;

};
