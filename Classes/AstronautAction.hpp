//
//  AstronautAction.hpp
//  nirgal-mobile
//
//  Created by Timothy Raveling on 1/15/18.
//

#ifndef AstronautAction_hpp
#define AstronautAction_hpp

#include <stdio.h>

using namespace std;

enum AstronautActionType : int {
    
    actionMovement = 0
};

class AstronautAction {
public:
    
    AstronautActionType type;
    float timeLeft;
    int x, y;
    
    // Constructor
    AstronautAction(AstronautActionType type, float timeLeft);
    
    // Destructor
    ~AstronautAction();
};

#endif /* AstronautAction_hpp */