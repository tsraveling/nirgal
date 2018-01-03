//
//  MapObject.hpp
//  nirgal
//
//  Created by Timothy Raveling on 12/27/17.
//

#ifndef MapObject_hpp
#define MapObject_hpp

#include <stdio.h>
#include "DataStore.hpp"
#include "Coord.h"

enum ObjectOrientation : int {
    orientNormal = 0,
    orient90 = 1,
    orient180 = 2,
    orient270 = 3
};

class MapObject
{
public:
    
    // Item variables
    int x, y;
    ObjectOrientation orientation;
    ObjectType type;
    
    // Initialization
    MapObject(ObjectType t, int x, int y, ObjectOrientation orientation);
    
    // Interface functions
    string spriteName();
    Coord apparentPosition();
};

#endif /* MapObject_hpp */
