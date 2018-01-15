//
//  Astronaut.cpp
//  nirgal-mobile
//
//  Created by Timothy Raveling on 1/12/18.
//

#include "Astronaut.hpp"

string Astronaut::sprite() {
    
    return this->spriteBase + (this->isSuited ? "_st.png" : "_ns.png");
}

Astronaut::Astronaut(string name, string sprite, int x, int y) {
    
    this->name = name;
    this->spriteBase = sprite;
    this->x = x;
    this->y = y;
    this->isSuited = false;
}

Coord Astronaut::apparentPosition() {
    
    Coord loc = Coord();
    loc.x = (float(this->x) * 64) + 32;
    loc.y = (float(this->y) * 64) + 32;
    return loc;
}
