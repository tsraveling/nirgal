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

string MapObject::spriteName() {
    
    auto design = DataStore::designFor(this->type);
    return design->baseSpriteName;
}

Coord MapObject::apparentPosition() {
    
    Coord loc = Coord();
    loc.x = (float(this->x) * 64) + 32;
    loc.y = (float(this->y) * 64) + 32;
    
    auto design = DataStore::designFor(this->type);
    if (design->width > 1 || design->height > 1) {
        
        float x_offset = (design->width - 1) * 32;
        float y_offset = (design->height - 1) * 32;
        
        switch (this->orientation) {
            case orientNormal:
                loc.x += x_offset;
                loc.y -= y_offset;
                break;
            case orient90:
                loc.y -= x_offset;
                loc.x -= y_offset;
                break;
            case orient180:
                loc.x -= x_offset;
                loc.y += y_offset;
                break;
            case orient270:
                loc.y += x_offset;
                loc.x += y_offset;
                break;
        }
    }
    
    return loc;
}
