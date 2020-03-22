#ifndef ENTITYFACTORY_H_INCLUDED
#define ENTITYFACTORY_H_INCLUDED


class EntityFactory {
public:
    EntityFactory(entt::registry& registry, Node* container): _registry(registry), _container(container) { }

    void initFactoryWithFile(const std::string& fileName);

    entt::entity createEntity(string entityName) {
        entt::entity entity = entt::null;
        if(entityName == "door") {
            entity = _registry.create();

            StateSprite* entitySprite = StateSprite::createSprite("DoorOpen");
            entitySprite->addState(CLOSED, "DoorClosed");
            _container->addChild(entitySprite);

            _registry.assign<Drawable>(entity, "DoorOpen");
            _registry.assign<Cell>(entity);
            _registry.assign<Destroyable>(entity, 100.0f, 0.0f, 0.0f);
            _registry.assign<Lockable>(entity, 0);
        } else if(entityName == "wall") {
            entity = _registry.create();

            StateSprite* entitySprite = StateSprite::createSprite("Wall.png");
            _container->addChild(entitySprite);

            _registry.assign<Drawable>(entity, "Wall.png");
            _registry.assign<Destroyable>(entity, 100.0f, 0.0f, 100.0f);
            _registry.assign<Cell>(entity);
        } else if(entityName == "floor") {
            entity = _registry.create();

            _registry.assign<Drawable>(entity, "Floor.png");
            _registry.assign<Cell>(entity);
        }

        return entity;
    }

private:
    entt::registry& _registry;

    Node* _container;

};



#endif // ENTITYFACTORY_H_INCLUDE1D
