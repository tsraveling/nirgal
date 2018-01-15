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

using namespace std;

#define GAME_LOOP_INTERVAL  1.0f
#define kMouseLeft          0
#define kMouseRight         1

// Input

typedef pair<MapObject*, cocos2d::Sprite*> ObjectSprite;
typedef pair<Astronaut*, cocos2d::Sprite*> AstronautSprite;

class SCGame : public cocos2d::Layer
{
public:
    
    bool keyHeldDown[256];
    
    // Variables
    Map map;                                        // Contains the map
    vector<ObjectSprite> objectSpritePairs;
    vector<AstronautSprite> astronautSpritePairs;
    vector<AstronautSprite> astronautRosterPairs;
    
    // UI elements
    cocos2d::Sprite *tileSprites[MAP_XS][MAP_YS];   // Created sprites will be stored here for map update purposes
    cocos2d::Sprite *overlaySprites[MAP_XS][MAP_YS];
    cocos2d::Sprite *wallSprites[MAP_XS * 2][MAP_YS * 2];
    cocos2d::Layer *mapLayer;
    cocos2d::Layer *uiLayer;
    
    // Map interface functions
    void addMapObject(ObjectType t, int x, int y, ObjectOrientation orientation);
    void generateObjectSprites();
    void generateSpriteTupleForObject(MapObject *ob);
    
    // Astronaut display functinos
    void generateAstronautSprites();
    void generateSpriteTupleForAstronaut(Astronaut *astronaut);
    void updateSpriteTuple(AstronautSprite *os);
    
    // Functions
    void tick(float time);                                    // Performs game logic
    
    // UI Functions
    void updateAstronautSelection();
    void centerMapOn(int x, int y);
    
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
