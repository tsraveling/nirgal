//
//  ObjectSprite.hpp
//  nirgal
//
//  Created by Timothy Raveling on 3/3/18.
//

#ifndef ObjectSprite_hpp
#define ObjectSprite_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "MapObject.hpp"

using namespace std;

class ObjectSprite {
    
public:
    
    cocos2d::Sprite *sprite;
    MapObject *object;
};

#endif /* ObjectSprite_hpp */
