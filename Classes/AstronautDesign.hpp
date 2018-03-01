//
//  AstronautDesign.hpp
//  nirgal
//
//  Created by Timothy Raveling on 2/12/18.
//

#ifndef AstronautDesign_hpp
#define AstronautDesign_hpp

#include <stdio.h>
#include "yaml-cpp/yaml.h"

using namespace std;

class AstronautDesign
{
public:
    
    string key;
    string firstName, lastName;
    string unsuitedSpriteName, suitedSpriteName;
    
    static AstronautDesign *astronautDesignForNode(YAML::Node node);
};

#endif /* AstronautDesign_hpp */
