//
//  DataStore.cpp
//  nirgal
//
//  Created by Timothy Raveling on 12/27/17.
//

#include "DataStore.hpp"

DataStore *dataStoreSingleton;

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
                
                
            }
        }
    }
}
