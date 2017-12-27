//
//  DataStore.cpp
//  nirgal
//
//  Created by Timothy Raveling on 12/27/17.
//

#include "DataStore.hpp"

ObjectDesign *objectDesigns[kNumberOfObjectTypes];

ObjectDesign* DataStore::designFor(ObjectType type) {
    
    return objectDesigns[int(type)];
}

void DataStore::populateData() {
    
    for (int i=0; i<kNumberOfObjectTypes; i++) {
        
        auto design = new ObjectDesign();
        
        // Most objects are 1x1
        design->width = 1;
        design->height = 1;
        
        switch (ObjectType(i)) {
                
            case crashCouch:
                design->name = "Crash Couch";
                design->baseSpriteName = "crashcouch.png";
                design->height = 2;
                break;
                
            case landerCrate:
                design->name = "Cargo Container";
                design->baseSpriteName = "crate.png";
                break;
                
            case waterStation:
                design->name = "Water Station";
                design->baseSpriteName = "h2o.png";
                break;
                
            case landingComputer:
                design->name = "Landing Computer";
                design->baseSpriteName = "landingcomputer.png";
                break;
                
            case landingStrut:
                design->name = "Landing Strut";
                design->baseSpriteName = "landingstrut.png";
                break;
                
            case atmosphereTank:
                design->name = "Atmosphere Cannister";
                design->baseSpriteName = "o2.png";
                break;
                
            case smallFoodStation:
                design->name = "Small Food Station";
                design->baseSpriteName = "smallfood.png";
                break;
                
            case landerSolarPanel:
                design->name = "Small Solar Panel";
                design->baseSpriteName = "solarpanel.png";
                break;
                
            case suitPort:
                design->name = "Suitport";
                design->baseSpriteName = "suitport.png";
                break;
                
            default:
                break;
        }
        
        objectDesigns[i] = design;
    }
}
