//
//  SCGame.cpp
//  nirgal
//
//  Created by Timothy Raveling on 12/8/17.
//
//

#include "SCGame.h"
USING_NS_CC;

#pragma mark - Public methods

Scene* SCGame::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SCGame::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SCGame::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    int xs = int(visibleSize.width);
    int ys = int(visibleSize.height);
    cocos2d::log("visible size is %d by %d",xs,ys);
    
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    /////////////////////////////
    // 1. Scene layout
    
    SpriteFrameCache* cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("res/terrain/map-test.plist");
    
    for (int x=0; x<24; x++) {
        for (int y=0; y<16; y++) {
            auto frame_name = this->map.floorFrameForTile(x, y);
            
            Sprite *sprite = Sprite::createWithSpriteFrameName(frame_name);
            sprite->setPosition((float(x)*64) + 32, (float(y)*64) + 32);
            this->addChild(sprite);
        }
    }
    
    // This is a looping node
    this->scheduleUpdate();
    
    return true;
}

void SCGame::tick() {
    
    cocos2d::log("tick.");
}

void SCGame::update(float delta) {
    
    this->timeElapsed += delta;
    while (this->timeElapsed >= GAME_LOOP_INTERVAL) {
        
        this->timeElapsed -= GAME_LOOP_INTERVAL;
        this->tick();
    }
}
