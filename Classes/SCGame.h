//
//  SCGame.h
//  nirgal
//
//  Created by Timothy Raveling on 12/8/17.
//
//

#ifndef __SCGame__
#define __SCGame__

#include "cocos2d.h"
#include "Map.h"

#define GAME_LOOP_INTERVAL  1.0f

// Input

class SCGame : public cocos2d::Layer
{
public:
    
    bool keyHeldDown[256];
    
    // Variables
    Map map;                                        // Contains the map
    float timeElapsed;                              // Contains time elapsed for the update(_) function
    
    // UI elements
    cocos2d::Sprite *tileSprites[MAP_XS][MAP_YS];   // Created sprites will be stored here for map update purposes
    cocos2d::Sprite *wallSprites[MAP_XS * 2][MAP_YS * 2];
    cocos2d::Layer *mapLayer;
    
    // Functions
    void tick();                                    // Performs game logic
    
    // Input
    void keyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
    void keyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
    
    // Infrastructure
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    void update(float) override;
    CREATE_FUNC(SCGame);
    
private:
    
    // Map management functions
    void updateMapContents();
    
};

#endif /* defined(__SCGame__) */
