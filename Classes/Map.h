//
//  Map.hpp
//  nirgal-mobile
//
//  Created by Timothy Raveling on 12/13/17.
//

#ifndef Map_hpp
#define Map_hpp

#include <stdio.h>
#include <vector>
#include "DataStore.hpp"
#include "MapObject.hpp"
#include "Astronaut.hpp"

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
    vector<MapObject*> objects;
    vector<Astronaut*> astronauts;
    int startX, startY;
    
    // Helper Variables
    char mod = 4;
    string baseTile[MAP_XS][MAP_YS];
    string overlayTile[MAP_XS][MAP_YS];
    
    // Logic Functions
    void tick(float time);
    
    // AI Functions
    bool isPassable(int x1, int y1, int x2, int y2);
    
    // Helper Functions
    void addObject(MapObject *object);
    void removeObject(MapObject *object);
    void setTile(int x, int y, TerrainType newTile);
    void regenerateTiles(int x1, int y1, int x2, int y2);
    static string frameForTile(TerrainType tile);
    
    // Initialization
    Map();
};

#endif /* Map_hpp */
