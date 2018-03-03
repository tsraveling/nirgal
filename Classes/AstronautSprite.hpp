//
//  AstronautSprite.hpp
//  nirgal
//
//  Created by Timothy Raveling on 3/3/18.
//

#ifndef AstronautSprite_hpp
#define AstronautSprite_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "Astronaut.hpp"

using namespace std;

class AstronautSprite {
    
public:
    
    Astronaut *astronaut;
    
    cocos2d::Sprite *mapAstronautSprite;
    cocos2d::Sprite *mapAstronautSelector;
    
    cocos2d::Sprite *rosterAstronautSprite;
    cocos2d::Sprite *rosterAstronautSelector;
    
    bool isSelected;
    
    AstronautSprite();
    void setPosition(Coord pos);
    void updatePosition();
    void updateSprite();
    void setSelected(bool selected);
};

#endif /* AstronautSprite_hpp */
