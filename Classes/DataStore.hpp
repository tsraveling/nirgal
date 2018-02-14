//
//  DataStore.hpp
//  nirgal
//
//  Created by Timothy Raveling on 12/27/17.
//

#ifndef DataStore_hpp
#define DataStore_hpp

#include <stdio.h>
#include <vector>
#include "ObjectDesign.hpp"
#include "AstronautDesign.hpp"
#include "yaml-cpp/yaml.h"

using namespace std;

class DataStore {
    
public:
    static void populateData();
    static DataStore* singleton();
    
    static ObjectDesign *objectDesignForTag(string tag);
    
    vector<ObjectDesign*> designs;
    vector<string> spriteSheets;
    
private:
    
    ObjectDesign *objectDesignForNode(YAML::Node node);
    AstronautDesign *astronautDesignForNode(YAML::Node node);
    void onboardNodeFile(YAML::Node node);
};

#endif /* DataStore_hpp */
