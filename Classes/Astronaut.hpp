//
//  Astronaut.hpp
//  nirgal-mobile
//
//  Created by Timothy Raveling on 1/12/18.
//

#ifndef Astronaut_hpp
#define Astronaut_hpp

#include <stdio.h>

#include "Coord.h"

using namespace std;

class Astronaut
{
public:
    
    int x, y;
    string name;
    bool isSuited;
    
    // Initialization
    Astronaut(string name, string sprite, int x, int y);
    
    // Display
    string spriteBase;
    string sprite();
    Coord apparentPosition();
};

#endif /* Astronaut_hpp */
