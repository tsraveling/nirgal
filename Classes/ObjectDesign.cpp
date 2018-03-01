//
//  ObjectDesign.cpp
//  nirgal
//
//  Created by Timothy Raveling on 12/27/17.
//

#include "ObjectDesign.hpp"

ObjectDesign::ObjectDesign() {
    this->height = 1;
    this->width = 1;
}

ObjectDesign* ObjectDesign::objectDesignForNode(YAML::Node node) {
    
    ObjectDesign *design = new ObjectDesign();
    
    design->name = node["name"].as<string>();
    design->baseSpriteName = node["sprite"].as<string>();
    
    // Get height if available (defaults to one)
    auto height = node["height"];
    if (height != NULL)
        design->height = height.as<int>();
    
    // Get width if available (defaults to one)
    auto width = node["width"];
    if (width != NULL)
        design->width = width.as<int>();
    
    return design;
}
