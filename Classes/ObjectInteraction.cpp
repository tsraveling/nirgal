//
//  ObjectInteraction.cpp
//  nirgal
//
//  Created by Timothy Raveling on 2/12/18.
//

#include "ObjectInteraction.hpp"

/*
 *  Basic initialization
 */
ObjectInteraction::ObjectInteraction() {
    this->duration = 30;
    this->position.x = 0;
    this->position.y = 0;
    this->transportsUser = false;
}

/*
 *  Get an interaction type from a string
 */
ObjectInteraction::Type ObjectInteraction::typeForString(string key) {
    
    if (key == "suitUp")            return suitUp;
    else if (key == "suitDown")     return suitDown;
    
    return nothing;
}

/*
 *  Create an interactio based on what's in the node
 */
ObjectInteraction* ObjectInteraction::objectInteractionForNode(YAML::Node node) {
    
    ObjectInteraction *interaction = new ObjectInteraction();
    
    interaction->name = node["name"].as<string>();
    interaction->desc = node["description"].as<string>();
    interaction->duration = node["duration"].as<float>();
    
    auto coord_string = node["position"].as<string>();
    interaction->position = Coord::fromString(coord_string);
    
    auto type_string = node["type"].as<string>();
    interaction->type = ObjectInteraction::typeForString(type_string);
    
    interaction->transportsUser = false;
    auto transport_node = node["transport"];
    if (transport_node != NULL) {
        
        auto transport_string = transport_node.as<string>();
        interaction->transportPosition = Coord::fromString(transport_string);
        interaction->transportsUser = true;
    }
    
    return interaction;
}
