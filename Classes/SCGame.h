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

class SCGame : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(SCGame);
};

#endif /* defined(__SCGame__) */
