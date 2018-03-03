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
#include "YUIView.hpp"
#include "YUILabel.hpp"
#include "AstronautSprite.hpp"
#include "ObjectSprite.cpp"

using namespace std;

#define GAME_LOOP_INTERVAL  1.0f
#define kMouseLeft          0
#define kMouseRight         1

class SCGame : public cocos2d::Layer
{
public:
    
    bool keyHeldDown[256];
    
    // Variables
    Map map;                                        // Contains the map
    vector<ObjectSprite*> objectSprites;
    vector<AstronautSprite*> astronautSprites;
    
    // UI elements
    cocos2d::Sprite *tileSprites[MAP_XS][MAP_YS];   // Created sprites will be stored here for map update purposes
    cocos2d::Sprite *overlaySprites[MAP_XS][MAP_YS];
    cocos2d::Sprite *wallSprites[MAP_XS * 2][MAP_YS * 2];
    cocos2d::Layer *mapLayer;
    cocos2d::Layer *uiLayer;
    
    // Map interface functions
    void generateSpriteObject(MapObject *ob);
    
    // Astronaut display functinos
    void generateSpriteAstronaut(Astronaut *astronaut);
    void updateAstronautSprite(AstronautSprite *os);
    
    // Functions
    void tick(float time);                                    // Performs game logic
    
    // UI Functions
    void centerMapOn(int x, int y);
    void selectAstronaut(AstronautSprite *os);
    
    // Keyboard Input
    void keyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
    void keyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
    
    // Mouse / touch input
    int movedMouse;
    virtual bool onMouseDown(cocos2d::Event*);
    virtual void onMouseUp(cocos2d::Event*);
    virtual void onMouseMove(cocos2d::Event*);
    virtual void onMouseScroll(cocos2d::Event*);
    
    // Infrastructure
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    void update(float) override;
    CREATE_FUNC(SCGame);
    
private:
    
    
};

#endif /* defined(__SCGame__) */
