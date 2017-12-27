//
//  MapObject.hpp
//  nirgal
//
//  Created by Timothy Raveling on 12/27/17.
//

#ifndef MapObject_hpp
#define MapObject_hpp

#include <stdio.h>
#include "ObjectDesign.hpp"

enum ObjectOrientation : int {
    up = 0,
    right = 1,
    down = 2,
    left = 3
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
};

#endif /* MapObject_hpp */
