#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "../Dependencies/entt.hpp"

class ISystem {
public:
	virtual ~ISystem() { }
	virtual void start(entt::registry& registry, entt::dispatcher& dispatcher) { }
	virtual void update(entt::registry& registry, entt::dispatcher& dispatcher, float delta) = 0;
};

#endif
