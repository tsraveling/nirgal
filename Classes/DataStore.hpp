//
//  DataStore.hpp
//  nirgal
//
//  Created by Timothy Raveling on 12/27/17.
//

#ifndef DataStore_hpp
#define DataStore_hpp

#include <stdio.h>
#include "ObjectDesign.hpp"

class DataStore {
    
public:
    static void populateData();
    static ObjectDesign* designFor(ObjectType type);
};

#endif /* DataStore_hpp */
