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
#include "yaml-cpp/yaml.h"

using namespace std;

class DataStore {
    
public:
    static void populateData();
    static DataStore* singleton();
    
    vector<ObjectDesign*> designs;
    vector<string> spriteSheets;
    
private:
    ObjectDesign *designForNode(YAML::Node node);
    void onboardNode(YAML::Node node);
};

#endif /* DataStore_hpp */
