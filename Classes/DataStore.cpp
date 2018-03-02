//
//  DataStore.cpp
//  nirgal
//
//  Created by Timothy Raveling on 12/27/17.
//

#include "DataStore.hpp"

DataStore *dataStoreSingleton;

ObjectDesign* DataStore::objectDesignForTag(string tag) {
    
    for (ObjectDesign *design : DataStore::singleton()->objectDesigns) {
        
        if (design->key == tag)
            return design;
    }
    
    return NULL;
}

AstronautDesign* astronautDesignForTag(string tag) {
    
    for (AstronautDesign *design : DataStore::singleton()->astronautDesigns) {
        
        if (design->key == tag)
            return design;
    }
    
    return NULL;
}

DataStore* DataStore::singleton() {
    return dataStoreSingleton;
}

void DataStore::populateData() {
    
    // Create the singleton
    dataStoreSingleton = new DataStore();
    
    // Get the overall content manifest, which lists all of the mods installed (for vanilla, only `core`)
    YAML::Node content = YAML::LoadFile("content/content.yaml");
    auto loads = content["load"];
    assert(loads.IsSequence());
    for (YAML::const_iterator it = loads.begin(); it != loads.end(); ++it) {
        
        // Get the load tag (e.g. "core")
        auto load_tag = it->as<string>();
        
        // Get the manifest
        YAML::Node manifest = YAML::LoadFile("content/" + load_tag + "/manifest.yaml");
        
        // Make sure it's a map
        assert(manifest.IsMap());
        
        // Now go through the subsets of the manifest and load those files
        for (YAML::const_iterator manifest_it = manifest.begin(); manifest_it != manifest.end(); ++manifest_it) {
            
            // Get the key for the subfolder
            auto manifest_node_key = manifest_it->first.as<string>();
            
            // Get the sequence of files to load
            auto manifest_subfiles = manifest_it->second.as<YAML::Node>();
            
            // Make sure it's a sequence
            assert(manifest_subfiles.IsSequence());
            
            // Iterate through the subsequence of files and load each one
            for (YAML::const_iterator subfile_it = manifest_subfiles.begin(); subfile_it != manifest_subfiles.end(); ++subfile_it) {
                
                // Get the file key
                auto subfile_key = subfile_it->as<string>();
                
                // Get the file
                auto basepath = "content/" + load_tag + "/" + manifest_node_key + "/";
                auto path = basepath + subfile_key + ".yaml";
                printf("Loading file from path: %s\n", path.c_str());
                YAML::Node content_file = YAML::LoadFile(path);
                
                // Onboard the file
                DataStore::singleton()->onboardNodeFile(content_file, basepath);
            }
        }
    }
}

void DataStore::onboardNodeFile(YAML::Node node, string path) {
    
    // Check for sprites
    auto sprites = node["sprite-sheets"];
    if (sprites != NULL) {
        assert(sprites.IsSequence());
        
        // If we have sprites and they're formatted as a sequence, grab em
        for (YAML::const_iterator sprite_it = sprites.begin(); sprite_it != sprites.end(); ++sprite_it) {
            
            auto sprite_sheet = sprite_it->as<string>();
            this->spriteSheets.push_back(path + sprite_sheet + ".plist");
        }
    }
    
    // Check for astronauts
    auto astronauts = node["astronauts"];
    if (astronauts != NULL) {
        assert(astronauts.IsMap());
        
        // Now go through the subsets of the manifest and load those files
        for (YAML::const_iterator astro_it = astronauts.begin(); astro_it != astronauts.end(); ++astro_it) {
            
            // Get the key for the astronaut
            auto astro_key = astro_it->first.as<string>();
            
            // Get the node for the astronaut's contents
            auto astro_node = astro_it->second.as<YAML::Node>();
            
            auto design = AstronautDesign::astronautDesignForNode(astro_node);
            if (design != NULL) {
                
                design->key = astro_key;
                printf("Adding astronaut: %s\n",astro_key.c_str());
                this->singleton()->astronautDesigns.push_back(design);
            }
        }
    }
    
    // Check for objects
    auto objects = node["objects"];
    if (objects != NULL) {
        assert(objects.IsMap());
        
        // Now go through the subsets of the manifest and load those files
        for (YAML::const_iterator it = objects.begin(); it != objects.end(); ++it) {
            
            // Get the key for the object design
            auto object_key = it->first.as<string>();
            
            // Get the node for the object's contents
            auto object_node = it->second.as<YAML::Node>();
            
            auto design = ObjectDesign::objectDesignForNode(object_node);
            if (design != NULL) {
                
                design->key = object_key;
                printf("Adding object: %s\n",object_key.c_str());
                this->singleton()->objectDesigns.push_back(design);
            }
        }
    }
    
    // TODO: Check for terrains
}
