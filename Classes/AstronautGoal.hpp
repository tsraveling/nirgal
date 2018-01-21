//
//  AstronautGoal.hpp
//  nirgal-mobile
//
//  Created by Timothy Raveling on 1/15/18.
//

#ifndef AstronautGoal_hpp
#define AstronautGoal_hpp

#include <stdio.h>
#include "AstronautRoute.hpp"

using namespace std;

enum AstronautGoalType : int {
    
    goalMovement = 0
};

class AstronautGoal {
    
public:
    
    AstronautGoalType type;
    int x,y;
    
    // Constructor
    AstronautGoal(AstronautGoalType type);
    
    // Destructor
    ~AstronautGoal();
};

#endif /* AstronautGoal_hpp */
