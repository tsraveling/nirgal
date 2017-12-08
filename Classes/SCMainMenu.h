#ifndef __SCENE_MAINMENU__
#define __SCENE_MAINMENU__

#include "cocos2d.h"

class SCMainMenu : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // Navigation
    void goToGameScene(Ref *sender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(SCMainMenu);
};

#endif // __SCENE_MAINMENU__
