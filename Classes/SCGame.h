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

class SCGame : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    Map map;
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // Game loop manager
    void update(float) override;
    
    // Game loop itself
    float timeElapsed;
    void tick();
    
    // implement the "static create()" method manually
    CREATE_FUNC(SCGame);
};

#endif /* defined(__SCGame__) */
