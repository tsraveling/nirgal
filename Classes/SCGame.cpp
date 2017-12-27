//
//  SCGame.cpp
//  nirgal
//
//  Created by Timothy Raveling on 12/8/17.
//
//

#include "SCGame.h"
USING_NS_CC;

#pragma mark - Rendering

void SCGame::updateMapContents(){
    for (int x=0; x<MAP_XS; x++) {
        for (int y=0; y<MAP_YS; y++) {
            
            auto sprite = this->tileSprites[x][y];
            if (sprite->getTag() != int(this->map.grid[x][y])) {
                
                auto frame_name = this->map.baseTile[x][y];
                sprite->setSpriteFrame(frame_name);
                sprite->setTag(int(this->map.grid[x][y]));
            }
        }
    }
}

#pragma mark - Game logic

void SCGame::tick() {
    
    this->updateMapContents();
}

#pragma mark - UI Logic

#pragma mark - Initialization

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
    
    // Set up the map layer
    this->mapLayer = cocos2d::Layer::create();
    this->mapLayer->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    this->addChild(this->mapLayer);
    
    /////////////////////////////
    // 1. Scene layout
    
    SpriteFrameCache* cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("res/terrain/terrain.plist");
    cache->addSpriteFramesWithFile("res/terrain/mars-wall.plist");
    
    // Generate the initial map layout
    this->map.regenerateTiles(0, 0, MAP_XS - 1, MAP_YS - 1);
    
    // Set up the terrain tiles
    for (int x=0; x < MAP_XS; x++) {
        for (int y=0; y < MAP_YS; y++) {
            
            // Create the base tile
            auto frame_name = this->map.baseTile[x][y];
            Sprite *sprite = Sprite::createWithSpriteFrameName(frame_name);
            sprite->setPosition((float(x)*64) + 32, (float(y)*64) + 32);
            sprite->setTag(int(this->map.grid[x][y]));
            this->mapLayer->addChild(sprite);
            this->tileSprites[x][y] = sprite;
            
            // Create the overlay tile
            auto overlay_name = this->map.overlayTile[x][y];
            Sprite *overlay = Sprite::createWithSpriteFrameName(overlay_name.length() > 0 ? overlay_name : "mars-wall-07.png");
            overlay->setPosition((float(x)*64) + 32, (float(y)*64) + 32);
            overlay->setTag(int(this->map.grid[x][y]));
            
            // Hide unused overlay segments
            if (overlay_name.length() == 0) {
                overlay->setVisible(false);
                overlay->pause();
            }
            
            this->mapLayer->addChild(overlay);
            this->overlaySprites[x][y] = overlay;
            
        }
    }
    
    // Set up the wall tiles
    for (int x=0; x < MAP_XS * 2; x++) {
        for (int y = 0; y < MAP_YS * 2; y++) {
            
            if (x % 2 != 0 || y % 2 != 0) {
                
                
                Sprite *sprite = Sprite::createWithSpriteFrameName("wall_metal_segment.png");
                sprite->setPosition((float(x)*32) + 32, (float(y)*32) + 32);
                sprite->setTag(int(this->map.grid[x][y]));
                
                // Hide unused wall segments
                if (this->map.wallGrid[x][y] == empty) {
                    sprite->setVisible(false);
                    sprite->pause();
                }
                
                if (y % 2 != 0)
                    sprite->setRotation(90);
                
                this->mapLayer->addChild(sprite);
                this->wallSprites[x][y] = sprite;
            }
        }
    }

    // This is a looping node
    this->scheduleUpdate();
    
    // Get keyboard input
    auto eventListener = EventListenerKeyboard::create();
    
    eventListener->onKeyPressed = CC_CALLBACK_2(SCGame::keyPressed, this);
    eventListener->onKeyReleased = CC_CALLBACK_2(SCGame::keyReleased, this);
    
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener,this);
    
    return true;
}

#pragma mark - Input Methods

void SCGame::keyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
    printf("Depressed %d\n", int(keyCode));
    
    if (int(keyCode) < 256) {
        this->keyHeldDown[int(keyCode)] = true;
    }
}

void SCGame::keyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
    printf("Released %d\n", int(keyCode));
    
    if (int(keyCode) < 256) {
        this->keyHeldDown[int(keyCode)] = false;
    }
}

#pragma mark - Cocos2d methods

void SCGame::update(float delta) {
    
    // Map movement
    float map_velocity = 800 * delta;
    Vec2 loc = this->mapLayer->getPosition();
    if (this->keyHeldDown[146])
        this->mapLayer->setPosition(loc.x, loc.y -= map_velocity);
    if (this->keyHeldDown[127])
        this->mapLayer->setPosition(loc.x -= map_velocity, loc.y);
    if (this->keyHeldDown[142])
        this->mapLayer->setPosition(loc.x, loc.y +=map_velocity);
    if (this->keyHeldDown[124])
        this->mapLayer->setPosition(loc.x += map_velocity, loc.y);
    
    this->timeElapsed += delta;
    while (this->timeElapsed >= GAME_LOOP_INTERVAL) {
        
        this->timeElapsed -= GAME_LOOP_INTERVAL;
        this->tick();
    }
}
