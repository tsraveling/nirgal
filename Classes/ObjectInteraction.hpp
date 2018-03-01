//
//  ObjectInteraction.hpp
//  nirgal
//
//  Created by Timothy Raveling on 2/12/18.
//

#ifndef ObjectInteraction_hpp
#define ObjectInteraction_hpp

#include <stdio.h>
#include "Coord.h"
#include "yaml-cpp/yaml.h"

using namespace std;

class ObjectInteraction
{
public:
    string key;
    string name;
    string desc;
    float duration;
    Coord position;
};

#endif /* ObjectInteraction_hpp */
