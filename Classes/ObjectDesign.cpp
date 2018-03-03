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
    
    // Get the list of interactions if there is one
    auto interactions = node["interactions"];
    if (interactions != NULL) {
        
        assert(interactions.IsMap());
        
        // Now go through the subsets of the manifest and load those files
        for (YAML::const_iterator iterator = interactions.begin(); iterator != interactions.end(); ++iterator) {
            
            // Get the key for the astronaut
            auto interaction_key = iterator->first.as<string>();
            
            // Get the node for the astronaut's contents
            auto interaction_node = iterator->second.as<YAML::Node>();
            
            ObjectInteraction* interaction = ObjectInteraction::objectInteractionForNode(interaction_node);
            
            if (interaction != NULL) {
                
                interaction->key = interaction_key;
                printf(" > Adding interaction: %s\n",interaction_key.c_str());
                design->interactions.push_back(interaction);
            }
        }
    }
    
    return design;
}
