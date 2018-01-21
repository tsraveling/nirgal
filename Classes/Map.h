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
#include <algorithm>
#include "DataStore.hpp"
#include "MapObject.hpp"
#include "Astronaut.hpp"

using namespace std;

#define MAP_XS 128
#define MAP_YS 128

struct MapCoord {
    
    int x, y;
    
    // We need to define custom operators for this struct in order to get the maps in the pathing system to work right
    
    friend bool operator <(const MapCoord& a, const MapCoord& b) {
        return std::tie(a.x, a.y) < std::tie(b.x,b.y);
    }
    
    friend bool operator ==(const MapCoord& a, const MapCoord& b) {
        return a.x == b.x && a.y == b.y;
    }
    
    friend bool operator !=(const MapCoord& a, const MapCoord& b) {
        return a.x != b.x || a.y != b.y;
    }
};

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
    bool isPassable(int x, int y, RoutingDirection direction);
    vector<MapCoord> getNeighbors(int x, int y);
    AstronautRoute *computeRoute(Astronaut *astronaut, int dx, int dy);
    bool tileBlocked(int x, int y);
    
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
