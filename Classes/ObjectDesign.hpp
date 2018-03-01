//
//  ObjectDesign.hpp
//  nirgal
//
//  Created by Timothy Raveling on 12/27/17.
//

#ifndef ObjectDesign_hpp
#define ObjectDesign_hpp

#include <stdio.h>
#include <vector>
#include "ObjectInteraction.hpp"
#include "yaml-cpp/yaml.h"

using namespace std;

class ObjectDesign
{
public:
    
    // Variables
    string key;
    int height, width;
    string name;
    string baseSpriteName;
    
    vector<ObjectInteraction> interactions;
    
    ObjectDesign();
    
    static ObjectDesign *objectDesignForNode(YAML::Node node);
};

#endif /* ObjectDesign_hpp */
