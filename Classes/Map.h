//
//  Map.hpp
//  nirgal-mobile
//
//  Created by Timothy Raveling on 12/13/17.
//

#ifndef Map_hpp
#define Map_hpp

#include <stdio.h>
#include "cocos2d.h"

using namespace std;

#define MAP_XS 128
#define MAP_YS 128

/*
 * Terrains:
 *
 * 0. Mars ground
 * 1. Mars walls
 *
 */

class Map
{
public:
    
    int grid[MAP_XS][MAP_XS];
    cocos2d::Sprite *tiles[MAP_XS][MAP_YS];
    
    Map();
    
    string floorFrameForTile(int x, int y);
    static string frameForTile(int tile);
    
    void randomizeOne();
};

#endif /* Map_hpp */
