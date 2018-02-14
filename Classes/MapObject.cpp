//
//  MapObject.cpp
//  nirgal
//
//  Created by Timothy Raveling on 12/27/17.
//

#include "MapObject.hpp"

MapObject::MapObject(ObjectDesign *design, int x, int y, ObjectOrientation orientation) {
    
    this->design = design;
    this->x = x;
    this->y = y;
    this->orientation = orientation;
}

MapObject::MapObject(string design_tag, int x, int y, ObjectOrientation orientation) {
    
    this->design = DataStore::objectDesignForTag(design_tag);
    this->x = x;
    this->y = y;
    this->orientation = orientation;
}

string MapObject::spriteName() {
    
    return this->design->baseSpriteName;
}

Coord MapObject::apparentPosition() {
    
    Coord loc = Coord();
    loc.x = (float(this->x) * 64) + 32;
    loc.y = (float(this->y) * 64) + 32;
    
    if (this->design->width > 1 || this->design->height > 1) {
        
        float x_offset = (this->design->width - 1) * 32;
        float y_offset = (this->design->height - 1) * 32;
        
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
