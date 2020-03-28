#ifndef ENTITYFACTORY_H_INCLUDED
#define ENTITYFACTORY_H_INCLUDED


class EntityFactory {
public:
    EntityFactory(entt::registry& registry): _registry(registry){ }

    void initFactoryWithFile(const std::string& fileName);

    entt::entity createEntity(string entityName) {
        entt::entity entity = entt::null;
        if(entityName == "door") {
            entity = _registry.create();

            _registry.assign<Drawable>(entity, EntityAnimation({"DoorUnlighted.png"}, 0.0f));
            _registry.assign<Cell>(entity);
            _registry.assign<Destroyable>(entity, 100.0f, 0.0f, 0.0f);
            _registry.assign<Lockable>(entity, 0);
            _registry.assign<Speakable>(entity);

            Cell& doorCellComponent = _registry.get<Cell>(entity);
            doorCellComponent.passable = false;
            doorCellComponent.transparent = false;
            doorCellComponent.name = "Wooden door";

            Speakable& doorSpeakableComponent = _registry.get<Speakable>(entity);
            doorSpeakableComponent.status.setStatusDialog(0, StatusInfo{Constants::DialogsTags::DoorDialog, Text{"Hallo, it's a greeting text", Color3B::YELLOW}});
            doorSpeakableComponent.status.setCurrentStatus(0);

            Drawable& doorDrawableComponent = _registry.get<Drawable>(entity);
            doorDrawableComponent.animations.setAnimation(Constants::AnimationsTags::OpenedAnimationTag, EntityAnimation({"DoorOpen.png"}, 0.0f));
            doorDrawableComponent.animations.setAnimation(Constants::AnimationsTags::ClosedAnimationTag, EntityAnimation({"DoorUnlighted.png"}, 0.0f));

        } else if(entityName == "wall") {
            entity = _registry.create();

            _registry.assign<Drawable>(entity, EntityAnimation({"Wall.png"}, 0.0f));
            _registry.assign<Destroyable>(entity, 100.0f, 0.0f, 100.0f);
            _registry.assign<Cell>(entity);
        } else if(entityName == "floor") {
            entity = _registry.create();

            _registry.assign<Drawable>(entity, EntityAnimation({"Floor.png"}, 0.0f));
            _registry.assign<Cell>(entity);
        }

        return entity;
    }

private:
    entt::registry& _registry;
};



#endif // ENTITYFACTORY_H_INCLUDE1D
