//
//  SCGame.cpp
//  nirgal
//
//  Created by Timothy Raveling on 12/8/17.
//
//

#include "SCGame.h"
USING_NS_CC;

#pragma mark - Game logic

void SCGame::tick(float time) {
    
    // Run the astronauts
    for (AstronautSprite *os : this->astronautSprites) {
        
        // Process astronaut logic and update the sprite if needed
        if (os->astronaut->tick(&this->map, time)) {
            os->updatePosition();
        }
    }
    
    // Update the map
    this->map.tick(time);
}

#pragma mark - Map interface

void SCGame::generateSpriteObject(MapObject *ob) {
    
    // TODO: Check for extant sprites
    
    // Add the sprite
    string frame_name = ob->spriteName();
    Sprite *sprite = Sprite::createWithSpriteFrameName(frame_name);
    Coord pos = ob->apparentPosition();
    sprite->setPosition(pos.x, pos.y);
    
    if (ob->orientation == orient90)
        sprite->setRotation(90);
    
    if (ob->orientation == orient180)
        sprite->setRotation(180);
    
    if (ob->orientation == orient270)
        sprite->setRotation(270);
    
    this->mapLayer->addChild(sprite);
    
    // Create the sprite object
    ObjectSprite *os = new ObjectSprite();
    os->sprite = sprite;
    os->object = ob;
    this->objectSprites.push_back(os);
}

void SCGame::generateSpriteAstronaut(Astronaut *astronaut) {
    
    // TODO: Check for extant sprites
    
    // Link the astronaut game object
    AstronautSprite *os = new AstronautSprite;
    os->astronaut = astronaut;
    
    // Create the map sprite and selector
    os->mapAstronautSprite = Sprite::createWithSpriteFrameName(astronaut->sprite());
    os->mapAstronautSelector = Sprite::createWithSpriteFrameName("astronaut_selected.png");
    os->mapAstronautSelector->setPosition(32, 32);
    os->mapAstronautSprite->addChild(os->mapAstronautSelector);
    os->updatePosition();
    
    // Now create the roster sprite and selector
    os->rosterAstronautSprite = Sprite::createWithSpriteFrameName(astronaut->sprite());
    os->rosterAstronautSelector = Sprite::createWithSpriteFrameName("astronaut_selected.png");
    os->rosterAstronautSelector->setPosition(32, 32);
    os->rosterAstronautSprite->addChild(os->rosterAstronautSelector);
    
    // Add the astronaut to the map layer
    this->mapLayer->addChild(os->mapAstronautSprite);
    
    // Add it to the astronaut sprite list
    this->astronautSprites.push_back(os);
}

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
    
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // 1. Set up the map layer
    // -----------------------
    
    this->mapLayer = cocos2d::Layer::create();
    
    
    this->centerMapOn(this->map.startX, this->map.startY);
    this->addChild(this->mapLayer);
    
    // Load hardcoded spritesheets
    SpriteFrameCache* cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("ui/ui.plist");
    
    // Load content sprite sheets
    for (string spritesheet : DataStore::singleton()->spriteSheets) {
        
        printf("Loading spritesheet: %s\n",spritesheet.c_str());
        cache->addSpriteFramesWithFile(spritesheet);
    }
    
    // Lay out the lander in the terrain data
    
    
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
    
    // Now we're going to generate the lander object sprites
    for (MapObject *ob : this->map.objects) {
        this->generateSpriteObject(ob);
    }
    
    // Now generate the astronaut sprites
    for (Astronaut *astronaut : this->map.astronauts) {
        this->generateSpriteAstronaut(astronaut);
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
    
    // 2. Now set up the UI Layer
    // --------------------------
    
    this->uiLayer = cocos2d::Layer::create();
    
    this->uiLayer->setPosition(0,0);
    this->addChild(this->uiLayer);
    
    // Lay out the roster sprites
    float roster_y = visibleSize.height - 64;
    float left = 32 + 10;
    
    for (AstronautSprite *as : this->astronautSprites) {
        
        // Set roster position
        as->rosterAstronautSprite->setPosition(left, roster_y);
        
        // Add it to the UI panel and deselect it by default
        as->setSelected(false);
        this->uiLayer->addChild(as->rosterAstronautSprite);
        
        // Decrement y
        roster_y -= (64 + 10);
    }
    
    // 3. Set up game logic and inputs
    // -------------------------------

    // This is a looping node
    this->scheduleUpdate();
    
    // Get keyboard input
    auto eventListener = EventListenerKeyboard::create();
    
    eventListener->onKeyPressed = CC_CALLBACK_2(SCGame::keyPressed, this);
    eventListener->onKeyReleased = CC_CALLBACK_2(SCGame::keyReleased, this);
    
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener,this);
    
    // Get mouse / touch input
    
    auto touchListener = EventListenerMouse::create();
    
    touchListener->onMouseDown = CC_CALLBACK_1(SCGame::onMouseDown, this);
    touchListener->onMouseUp = CC_CALLBACK_1(SCGame::onMouseUp, this);
    touchListener->onMouseMove = CC_CALLBACK_1(SCGame::onMouseMove, this);
    touchListener->onMouseScroll = CC_CALLBACK_1(SCGame::onMouseScroll, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    return true;
}

#pragma mark - UI Functions

void SCGame::centerMapOn(int x, int y) {
    
    auto rx = float(x) * -64;
    auto ry = float(y) * -64;
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    this->mapLayer->setPosition((visibleSize.width / 2) + rx, (visibleSize.height / 2) + ry);
}

void SCGame::selectAstronaut(AstronautSprite *os) {
    
    // Select this astronaut
    os->setSelected(true);
    
    // If you're not holding down ctrl or shift unselect everyone else
    if (!(keyHeldDown[14] || keyHeldDown[12] || keyHeldDown[13])) {
        
        for (AstronautSprite *naut_sprite : this->astronautSprites) {
            
            if (naut_sprite != os) {
                naut_sprite->setSelected(false);
            }
        }
    }
}

#pragma mark - Keyboard Input Methods

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

#pragma mark - Mouse / Touch Input

bool SCGame::onMouseDown(Event* event)
{
    this->movedMouse = 0;
    return true;
}

void SCGame::onMouseUp(Event* event)
{
    // For now, we're just going to assume this is a mouse click. We'll deal with touch later,
    // if we ever go to tablets.
    try {
        EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);
        auto button = mouseEvent->getMouseButton();
        
        // Get the mouse location
        auto click = mouseEvent->getLocation();
        click.y = Director::getInstance()->getVisibleSize().height - click.y;
        
        if (this->movedMouse <= 10) {
            
            // CLICK HANDLER
            // -------------
            
            // Note: preference will be handled vertically. Return to intercept clicks.
            
            // 1. UI Layer
            // -----------
            
            // Roster
            
            for (AstronautSprite *os : this->astronautSprites) {
                
                auto rect = os->rosterAstronautSprite->getBoundingBox();
                
                if (os->rosterAstronautSprite->getBoundingBox().containsPoint(click)) {
                    
                    if (button == kMouseLeft) {
                        
                        this->selectAstronaut(os);
                        
                        // Center on them
                        this->centerMapOn(os->astronaut->x, os->astronaut->y);
                    }
                    
                    // Note that this means right clicks don't pass through either.
                    return;
                }
            }
                
                
            // 2. WORLD LAYER
            // --------------
            
            // Astronauts
            
            for (AstronautSprite *os : this->astronautSprites) {
                
                auto rect = os->mapAstronautSprite->getBoundingBox();
                rect.origin += this->mapLayer->getPosition(); // Adjust for map position
                
                if (rect.containsPoint(click)) {
                    
                    if (button == kMouseLeft) {
                        
                        this->selectAstronaut(os);
                        return;
                    }
                }
            }
            
            // Map interaction
            // At this point, if you're not clicking on anything specific, you're probably just clicking on the map
            
            auto map_offset = this->mapLayer->getPosition();
            auto point = click - map_offset;
            
            int map_x = int(point.x / 64);
            int map_y = int(point.y / 64);
            
            if (button == kMouseRight) {
                
                printf("Issuing click to %d, %d\n",map_x,map_y);
                
                // Assign any selected astronauts to go there
                for (AstronautSprite *ns : this->astronautSprites) {
                    
                    if (ns->isSelected) {
                        
                        printf("Issuing order to %s\n",ns->astronaut->name.c_str());
                        
                        // Calculate path
                        auto route = this->map.computeRoute(ns->astronaut, map_x, map_y);
                        
                        // Make sure there IS a path
                        if (route != NULL) {
                            
                            // Create the goal and add the route to it
                            auto goal = new AstronautGoal(goalMovement);
                            goal->route = route;
                            ns->astronaut->setGoal(goal);
                            
                        } else {
                            
                            // TODO: Astronaut should bark, "can't get there"
                        }
                        
                    }
                }
            }
            
        } else {
            cocos2d::log("mouse up");
        }
    }
    catch (std::bad_cast& e){
        // Not sure what kind of event you passed us cocos, but it was the wrong one
        return;
    }
}

void SCGame::onMouseMove(Event* event)
{
    this->movedMouse ++;
}

void SCGame::onMouseScroll(Event *event)
{
    cocos2d::log("mouse scroll");
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
    
    this->tick(delta);
}
