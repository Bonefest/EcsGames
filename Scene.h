#ifndef __SCENE_H_
#define __SCENE_H_

#include "cocos2d.h"
#include "ui/UIScrollView.h"

#include "Dependencies/entt.hpp"

#include "Systems/GridRenderingSystem.h"
#include "Systems/ControllSystem.h"
#include "Systems/HUDSystem.h"
#include "SystemManager.h"

#include "Dialog/Dialog.h"
#include "Dialog/DialogDatabase.h"

#include "EntityFactory.h"
#include "InputHandler.h"

#include <memory>

#include "Item.h"

USING_NS_CC;

using std::make_shared;
using std::shared_ptr;

class MainScene: public cocos2d::Scene {
public:
    CREATE_FUNC(MainScene);

    static MainScene* createScene() {
        return MainScene::create();
    }

    bool init() {
        if(!Scene::init()) return false;

        //getDefaultCamera()->setDefaultViewport(cocos2d::experimental::Viewport(10, 10, 200, 200));

        cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("sheet.plist");

        _visibleSize = Director::getInstance()->getVisibleSize();

        runAction(cocos2d::CallFunc::create(CC_CALLBACK_0(MainScene::onInitFinished, this)));

        scheduleUpdate();
        return true;
    }

    void onInitFinished() {

        initContext();
        initSystems();
        initDatabases();

        generateMap();

        _handler = make_shared<InputHandler>(this, _eventDispatcher, _systemManager.getDispatcher());
        _dispatcher = make_shared<ConfigurableKeyDispatcher>(_systemManager.getDispatcher());
        _dispatcher->setKeyType(EventKeyboard::KeyCode::KEY_1, MOVE_BOTTOM_LEFT);
        _dispatcher->setKeyType(EventKeyboard::KeyCode::KEY_2, MOVE_BOTTOM);
        _dispatcher->setKeyType(EventKeyboard::KeyCode::KEY_3, MOVE_BOTTOM_RIGHT);
        _dispatcher->setKeyType(EventKeyboard::KeyCode::KEY_4, MOVE_LEFT);
        _dispatcher->setKeyType(EventKeyboard::KeyCode::KEY_6, MOVE_RIGHT);
        _dispatcher->setKeyType(EventKeyboard::KeyCode::KEY_7, MOVE_TOP_LEFT);
        _dispatcher->setKeyType(EventKeyboard::KeyCode::KEY_8, MOVE_TOP);
        _dispatcher->setKeyType(EventKeyboard::KeyCode::KEY_9, MOVE_TOP_RIGHT);
        _dispatcher->setKeyType(EventKeyboard::KeyCode::KEY_ESCAPE, CANCEL);
        _dispatcher->setKeyType(EventKeyboard::KeyCode::KEY_O, OPEN);
        _dispatcher->setKeyType(EventKeyboard::KeyCode::KEY_C, CLOSE);
        _dispatcher->setKeyType(EventKeyboard::KeyCode::KEY_E, USE);
        _dispatcher->setKeyType(EventKeyboard::KeyCode::KEY_A, ATTACK);
        _dispatcher->setKeyType(EventKeyboard::KeyCode::KEY_T, TALK);
        _dispatcher->setKeyType(EventKeyboard::KeyCode::KEY_I, INVENTORY);



        ////////////////////////////////////////////////////////////////////
        entt::registry& registry = _systemManager.getRegistry();

        entt::entity player = registry.create();
        registry.assign<Controllable>(player);
        registry.assign<Drawable>(player, EntityAnimation({"Wall.png", "Floor.png"}, 1.0f));
        registry.assign<Cell>(player, 3, 3, -1);
        registry.assign<Creature>(player);

        Cell& playerCell = registry.get<Cell>(player);
        playerCell.name = "monster";

        Creature& creature = registry.get<Creature>(player);
        creature.inventoryItems.push_back(registry.ctx<ItemDatabase>().createItem(registry, 1));
        creature.inventoryItems.push_back(registry.ctx<ItemDatabase>().createItem(registry, 1));
        creature.inventoryItems.push_back(registry.ctx<ItemDatabase>().createItem(registry, 1));
        creature.inventoryItems.push_back(registry.ctx<ItemDatabase>().createItem(registry, 1));

        Controllable& controllable = registry.get<Controllable>(player);
        for(int i = 0;i < 24; ++i) {
            controllable.discoveredBlocks.emplace_back();
            for(int j = 0;j < 24; ++j) {
                controllable.discoveredBlocks[i].emplace_back();
            }
        }

        entt::entity testLight = registry.create();
        registry.assign<Cell>(testLight, 8, 5);
        registry.assign<Light>(testLight, Color4F(1.0f, 1.0f, 1.0f, 255), 1.0f, 10.0f);

        entt::entity testLight2 = registry.create();
        registry.assign<Cell>(testLight2, 14, 11);
        registry.assign<Light>(testLight2, Color4F(1.0f, 1.0f, 1.0f, 255), 1.0f, 10.0f);



        WorldData& wd = registry.ctx<WorldData>();
        wd.creatures[3][3] = player;
    }

    void update(float delta) {

        _systemManager.update(delta);

    }


private:
    void initContext() {
        auto& registry = _systemManager.getRegistry();

        registry.set<EntityFactory>(registry);
        registry.set<GameSettings>(32.0f, uint16_t(24), uint16_t(24), Color4B(40, 40, 40, 255));
        registry.set<WorldData>();
        registry.set<DialogInfo>();
        registry.set<DialogDatabase>();
        registry.set<ItemDatabase>();
    }

    void initSystems() {
        _systemManager.getContainer().addSystem(make_shared<StateControllSystem>(_systemManager.getRegistry(),
                                                                                 _systemManager.getDispatcher()));
    }

    void initDatabases() {
        ItemDatabase& itemDB = _systemManager.getRegistry().ctx<ItemDatabase>();
        itemDB.load("items_db.json");

        DialogDatabase& dialogDB = _systemManager.getRegistry().ctx<DialogDatabase>();
        dialogDB.loadDialogs("dialogs_db.json");
    }

    void generateMap() {
        auto& registry = _systemManager.getRegistry();
        EntityFactory& factory = registry.ctx<EntityFactory>();
        WorldData& data = registry.ctx<WorldData>();
        GameSettings& settings = registry.ctx<GameSettings>();

        for(int y = 0; y < Constants::MAP_HEIGHT; y++) {
            for(int x = 0; x < Constants::MAP_WIDTH; x++) {



                if(y == 3 && x > 2 && x < 18 && x != 8) {
                    entt::entity entity = entt::null;
                    entity = factory.createEntity("door");

                    Cell& cellComponent = registry.get<Cell>(entity);
                    cellComponent.x = x;
                    cellComponent.y = y;

                    data.objects[y][x].push_back(entity);
                }

                entt::entity floor = factory.createEntity("floor");
                assert(registry.valid(floor));

                Cell& cellComponent = registry.get<Cell>(floor);
                cellComponent.x = x;
                cellComponent.y = y;


                data.floor[y][x] = floor;
                data.creatures[y][x] = entt::null;
            }
        }
    }

    SystemManager _systemManager;

    shared_ptr<InputHandler> _handler;
    shared_ptr<ConfigurableKeyDispatcher> _dispatcher;

    Size _visibleSize;
};

#endif // __SCENE_H_
