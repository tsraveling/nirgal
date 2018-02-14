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
};

#endif /* ObjectDesign_hpp */
