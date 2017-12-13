#include "SCMainMenu.h"
#include "SimpleAudioEngine.h"
#include "SCGame.h"

USING_NS_CC;

Scene* SCMainMenu::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SCMainMenu::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SCMainMenu::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 1. Scene layout

    auto label = Label::createWithTTF("Start game", "fonts/Socket.ttf", 24);
    auto menu_label = MenuItemLabel::create(label, CC_CALLBACK_1(SCMainMenu::goToGameScene, this));
    
    auto menu = Menu::create(menu_label, NULL);
    menu->alignItemsVerticallyWithPadding(visibleSize.height / 4);
    
    // add the label as a child to this layer
    this->addChild(menu);

    return true;
}

// Navigation
void SCMainMenu::goToGameScene(cocos2d::Ref *sender) {
    
    auto scene = SCGame::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1.0, scene));
}
