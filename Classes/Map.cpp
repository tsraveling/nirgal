//
//  Map.cpp
//  nirgal-mobile
//
//  Created by Timothy Raveling on 12/13/17.
//

#include "Map.h"

Map::Map() {
    
    for (int x=0;x<MAP_XS;x++) {
        for (int y=0;y<MAP_YS;y++) {
            
            this->grid[x][y] = int(rand() % 2);
        }
    }
}

string Map::floorFrameForTile(int x, int y) {
    return Map::frameForTile(this->grid[x][y]);
}

string Map::frameForTile(int tile) {
    
    switch (tile) {
        case 0:
            return "floor.png";
            break;
        case 1:
            return "wall-00.png";
            break;
            
        default:
            break;
    }
    
    return "floor.png";
}

void Map::randomizeOne() {
    
    int x = int(rand() % MAP_XS);
    int y = int(rand() % MAP_YS);
    
    this->grid[x][y] = int(rand() % 2);
}
