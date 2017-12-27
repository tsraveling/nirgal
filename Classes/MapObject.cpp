//
//  MapObject.cpp
//  nirgal
//
//  Created by Timothy Raveling on 12/27/17.
//

#include "MapObject.hpp"

MapObject::MapObject(ObjectType t, int x, int y, ObjectOrientation orientation) {
    
    this->type = t;
    this->x = x;
    this->y = y;
    this->orientation = orientation;
}
