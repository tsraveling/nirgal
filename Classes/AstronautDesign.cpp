//
//  AstronautDesign.cpp
//  nirgal
//
//  Created by Timothy Raveling on 2/12/18.
//

#include "AstronautDesign.hpp"

AstronautDesign* AstronautDesign::astronautDesignForNode(YAML::Node node) {
    
    AstronautDesign *design = new AstronautDesign();
    
    design->firstName = node["firstName"].as<string>();
    design->lastName = node["lastName"].as<string>();

    // Get the sprites
    auto sprites = node["sprites"].as<YAML::Node>();
    assert(sprites.IsMap());
    
    design->suitedSpriteName = sprites["suitOn"].as<string>();
    design->unsuitedSpriteName = sprites["suitOff"].as<string>();
    
    return design;
}
