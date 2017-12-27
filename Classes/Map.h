//
//  Map.hpp
//  nirgal-mobile
//
//  Created by Timothy Raveling on 12/13/17.
//

#ifndef Map_hpp
#define Map_hpp

#include <stdio.h>

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

enum TerrainType : char {
    
    marsDirt = 0,
    metalFloor = 1,
    solidRock = 2
};

enum WallType : char {
    
    empty = 0,
    metalWall = 1,
    inflatedWall = 2
};

class Map
{
public:
    
    // Content Variables
    TerrainType grid[MAP_XS][MAP_XS];
    WallType wallGrid[MAP_XS * 2][MAP_YS * 2];
    
    // Helper Variables
    void setTile(int x, int y, TerrainType newTile);
    
    char mod = 4;
    
    // Logic Functions
    void randomizeOne();
    
    // Helper Functions
    string floorFrameForTile(int x, int y);
    static string frameForTile(TerrainType tile);
    
    // Initialization
    Map();
};

#endif /* Map_hpp */
