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
    
    enum Type : int {
        nothing = 0,
        suitUp = 1,
        suitDown = 2
    };
    
    string key;
    string name;
    string desc;
    float duration;
    Coord position;
    Coord transportPosition;
    bool transportsUser;
    Type type;
    
    ObjectInteraction();
    static ObjectInteraction *objectInteractionForNode(YAML::Node node);
    static Type typeForString(string key);
};

#endif /* ObjectInteraction_hpp */
