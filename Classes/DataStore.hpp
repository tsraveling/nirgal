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
    static AstronautDesign *astronautDesignForTag(string tag);
    
    vector<ObjectDesign*> objectDesigns;
    vector<AstronautDesign*> astronautDesigns;
    vector<string> spriteSheets;
    
private:
    
    void onboardNodeFile(YAML::Node node, string path);
};

#endif /* DataStore_hpp */
