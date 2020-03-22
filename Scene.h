#ifndef __SCENE_H_
#define __SCENE_H_

#include "cocos2d.h"
#include "ui/UIScrollView.h"

#include "Dependencies/entt.hpp"

#include "Systems/GridRenderingSystem.h"
#include "Systems/ControllSystem.h"
#include "Systems/HUDSystem.h"
#include "SystemManager.h"

#include "EntityFactory.h"
#include "InputHandler.h"

#include <memory>

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


        initWorldContainer();
        initContext();
        initSystems();

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
        _dispatcher->setKeyType(EventKeyboard::KeyCode::KEY_O, OPEN);
        _dispatcher->setKeyType(EventKeyboard::KeyCode::KEY_E, USE);
        _dispatcher->setKeyType(EventKeyboard::KeyCode::KEY_A, ATTACK);



        ////////////////////////////////////////////////////////////////////
        entt::registry& registry = _systemManager.getRegistry();

        StateSprite* sprite = StateSprite::createSprite("Wall.png");
        //_worldContainer->addChild(sprite);

        entt::entity player = registry.create();
        registry.assign<Controllable>(player);
        registry.assign<Drawable>(player, "Wall.png");
        registry.assign<Cell>(player, 3, 3, 0);

        WorldData& wd = registry.ctx<WorldData>();
        wd.creatures[3][3] = player;

        scheduleUpdate();
        return true;
    }
    void update(float delta) {

        _systemManager.update(delta);

    }


private:
    void initWorldContainer() {
        _worldContainer = cocos2d::ui::ScrollView::create();
        _worldContainer->setContentSize(Size(_visibleSize.width * 0.5f, _visibleSize.height * 0.8f));
        _worldContainer->setInnerContainerSize(Size(24 * 32, 24 * 32));
        _worldContainer->setPosition(_visibleSize * 0.1f);
        _worldContainer->setDirection(cocos2d::ui::ScrollView::Direction::BOTH);
        _worldContainer->setScrollBarEnabled(false);
        addChild(_worldContainer);
    }

    void initContext() {
        auto& registry = _systemManager.getRegistry();

        registry.set<EntityFactory>(registry, _worldContainer);
        registry.set<GameSettings>(32.0f, 24, 24);
        registry.set<WorldData>();
    }

    void initSystems() {
        _systemManager.addSystem(make_shared<GridRenderingSystem>(_worldContainer));
        _systemManager.addSystem(make_shared<HUDSystem>(this, _worldContainer));
        _systemManager.addSystem(make_shared<ControllSystem>(_systemManager.getDispatcher()));
    }

    void generateMap() {
        auto& registry = _systemManager.getRegistry();
        EntityFactory& factory = registry.ctx<EntityFactory>();
        WorldData& data = registry.ctx<WorldData>();

        for(int y = 0; y < 24; y++) {
            data.objects.emplace_back();
            data.creatures.emplace_back();

            for(int x = 0; x < 24; x++) {
                entt::entity floor = factory.createEntity("floor");
                assert(registry.valid(floor));

                Cell& cellComponent = registry.get<Cell>(floor);
                cellComponent.x = x;
                cellComponent.y = y;

                data.objects[y].emplace_back();
                data.objects[y][x].push_back(floor);
                data.creatures[y].emplace_back();
                data.creatures[y][x] = entt::null;
            }
        }
    }

    SystemManager _systemManager;

    cocos2d::ui::ScrollView* _worldContainer;
    shared_ptr<InputHandler> _handler;
    shared_ptr<ConfigurableKeyDispatcher> _dispatcher;

    Size _visibleSize;
};

#endif // __SCENE_H_
