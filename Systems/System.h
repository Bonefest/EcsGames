#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "../Dependencies/entt.hpp"

class ISystem {
public:
	virtual ~ISystem() { }
	virtual void start(entt::registry& registry, entt::dispatcher& dispatcher) { }
	virtual void update(entt::registry& registry, entt::dispatcher& dispatcher, float delta) = 0;
	virtual void onDisable() {}
	virtual void onEnable() {}
};

#include <deque>
#include "cocos2d.h"

using std::deque;
USING_NS_CC;

class ButtonManager {
public:
    void addButton(entt::entity button);
    void removeButton(entt::entity button);

    void nextButton();
    void previousButton();

    void chooseButton(Vec2 position);

    entt::entity getCurrentButton();

private:
    deque<entt::entity> _buttons;
};

#endif
