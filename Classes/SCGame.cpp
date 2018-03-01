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
    for (AstronautSprite os : this->astronautSpritePairs) {
        
        // Process astronaut logic and update the sprite if needed
        if (os.first->tick(&this->map, time)) {
            this->updateSpriteTuple(&os);
        }
    }
    
    // Update the map
    this->map.tick(time);
}

#pragma mark - Map interface

void SCGame::generateObjectSprites() {
    
    for (MapObject *ob : this->map.objects) {
        
        this->generateSpriteTupleForObject(ob);
    }
}

void SCGame::generateAstronautSprites() {
    
    for (Astronaut *astronaut : this->map.astronauts) {
        
        this->generateSpriteTupleForAstronaut(astronaut);
    }
}

void SCGame::generateSpriteTupleForObject(MapObject *ob) {
    
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
    
    // Create and add the tuple
    ObjectSprite os = ObjectSprite(ob, sprite);
    this->objectSpritePairs.push_back(os);
}

void SCGame::generateSpriteTupleForAstronaut(Astronaut *astronaut) {
    
    // TODO: Check for extant sprites
    
    // Add the sprite
    string frame_name = astronaut->sprite();
    Sprite *sprite = Sprite::createWithSpriteFrameName(frame_name);
    
    // Create and add the tuple
    AstronautSprite os = AstronautSprite(astronaut, sprite);
    this->astronautSpritePairs.push_back(os);
    
    // Update the sprite
    this->updateSpriteTuple(&os);
    
    // Add the astronaut to the layer
    this->mapLayer->addChild(sprite);
}

void SCGame::updateSpriteTuple(AstronautSprite *os) {
    Coord pos = os->first->apparentPosition();
    os->second->setPosition(pos.x, pos.y);
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
    
    // Load the initial sprite sheets
    
    SpriteFrameCache* cache = SpriteFrameCache::getInstance();
    /*
    cache->addSpriteFramesWithFile("res/terrain/terrain.plist");
    cache->addSpriteFramesWithFile("res/terrain/mars-wall.plist");
    cache->addSpriteFramesWithFile("res/objects/lander-assets.plist");
    cache->addSpriteFramesWithFile("res/characters/astronauts.plist");
     */
    
    return true;
    
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
    this->generateObjectSprites();
    
    // Now generate the astronaut sprites
    this->generateAstronautSprites();
    
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
    for (Astronaut *astronaut : this->map.astronauts) {
        
        // Create the roster sprite
        string frame_name = astronaut->sprite();
        Sprite *sprite = Sprite::createWithSpriteFrameName(frame_name);
        sprite->setPosition(left, roster_y);
        this->uiLayer->addChild(sprite);
        
        // Create and add the tuple
        AstronautSprite os = AstronautSprite(astronaut, sprite);
        this->astronautRosterPairs.push_back(os);
        
        // Decrement y
        roster_y -= (64 + 10);
        
        // Set up selection
        astronaut->isSelected = false;
    }
    
    this->updateAstronautSelection();
    
    // Let's try a fancy new YUIView
    this->panelView = new YUIView(100, 200);
    this->uiLayer->addChild(this->panelView);
    this->panelView->anchorLeft(30);
    this->panelView->anchorBottom(30);
    this->panelView->anchorRight(30);
    
    // Now a fancy new YUILabel!
    string text = "Lorem ipsum vel probatisim salomonis claviculae rabini hebraicim in quibus tum naturalia tum super naturalia secreta licet abditissima in promptu apparent, modò operator per necessaria et contenta faciat scia tamen oportet demonum potentia dum taxat per agantur.";
    this->panelLabel = new YUILabel(text, 100, 100);
    this->panelView->addSubview(this->panelLabel);
    this->panelLabel->anchorLeft(20);
    this->panelLabel->anchorTop(20);
    this->panelLabel->anchorRight(20);
    this->panelLabel->anchorBottom(20);
    
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

void SCGame::updateAstronautSelection() {
    
    // Update roster
    for (AstronautSprite os : this->astronautRosterPairs) {
        os.second->setColor(os.first->isSelected ? Color3B(255, 255, 255) : Color3B(200, 200, 200));
    }
    
    // Update reality
    for (AstronautSprite os : this->astronautSpritePairs) {
        os.second->setColor(os.first->isSelected ? Color3B(255, 255, 255) : Color3B(200, 200, 200));
    }
}

void SCGame::centerMapOn(int x, int y) {
    
    auto rx = float(x) * -64;
    auto ry = float(y) * -64;
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    this->mapLayer->setPosition((visibleSize.width / 2) + rx, (visibleSize.height / 2) + ry);
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
    cocos2d::log("mouseUp");
    // For now, we're just going to assume this is a mouse click. We'll deal with touch later,
    // if we ever go to tablets.
    try {
        EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);
        auto button = mouseEvent->getMouseButton();
        
        // Get the mouse location
        auto click = mouseEvent->getLocation();
        click.y = Director::getInstance()->getVisibleSize().height - click.y;
        
        if (this->movedMouse <= 10) {
            
            cocos2d::log("clicked %d", button);
            
            // CLICK HANDLER
            // -------------
            
            // Note: preference will be handled vertically. Return to intercept clicks.
            
            // 1. UI Layer
            // -----------
            
            // Roster
            
            for (AstronautSprite os : this->astronautRosterPairs) {
                
                auto rect = os.second->getBoundingBox();
                
                if (os.second->getBoundingBox().containsPoint(click)) {
                    
                    if (button == kMouseLeft) {
                        
                        // Select this astronaut
                        os.first->isSelected = true;
                        
                        // If you're not holding down ctrl or shift unselect everyone else
                        if (!(keyHeldDown[14] || keyHeldDown[12] || keyHeldDown[13])) {
                        
                            for (Astronaut *naut : this->map.astronauts) {
                                
                                if (naut != os.first)
                                    naut->isSelected = false;
                            }
                            
                            // Center on them
                            this->centerMapOn(os.first->x, os.first->y);
                            
                        }
                        
                        this->updateAstronautSelection();
                    }
                    
                    return;
                }
            }
                
                
            // 2. WORLD LAYER
            // --------------
            
            // Astronauts
            
            for (AstronautSprite os : this->astronautSpritePairs) {
                
                auto rect = os.second->getBoundingBox();
                rect.origin += this->mapLayer->getPosition(); // Adjust for map position
                
                if (rect.containsPoint(click)) {
                    
                    if (button == kMouseLeft) {
                        
                        // Reverse selection on this item
                        os.first->isSelected = true;
                        
                        // If you're not holding down ctrl or shift unselect everyone else
                        if (!(keyHeldDown[14] || keyHeldDown[12] || keyHeldDown[13])) {
                            
                            for (Astronaut *naut : this->map.astronauts) {
                                
                                if (naut != os.first)
                                    naut->isSelected = false;
                            }
                        }
                        
                        this->updateAstronautSelection();
                        
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
                
                // Assign any selected astronauts to go there
                for (Astronaut *naut : this->map.astronauts) {
                    if (naut->isSelected) {
                        
                        // Calculate path
                        auto route = this->map.computeRoute(naut, map_x, map_y);
                        
                        // Make sure there IS a path
                        if (route != NULL) {
                            
                            // Create the goal and add the route to it
                            auto goal = new AstronautGoal(goalMovement);
                            goal->route = route;
                            naut->setGoal(goal);
                            
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
