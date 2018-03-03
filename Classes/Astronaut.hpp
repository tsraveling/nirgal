//
//  Astronaut.hpp
//  nirgal-mobile
//
//  Created by Timothy Raveling on 1/12/18.
//

#ifndef Astronaut_hpp
#define Astronaut_hpp

#include <stdio.h>
#include "AstronautGoal.hpp"
#include "AstronautAction.hpp"
#include <vector>

#include "Coord.h"

using namespace std;

class Map;

class Astronaut
{
public:
    
    int x, y;
    string name;
    bool isSuited;
    
    // AI variables
    vector<AstronautGoal*> immediateGoals;                  // This contains everything the astronaut is immediately intending to do
    float taskCounter;                                      // This processes time
    AstronautAction *currentAction;
    
    // User input interface functions
    void setGoal(AstronautGoal *goal);
    void addGoal(AstronautGoal *goal);
    
    // AI functions
    bool tick(Map *map, float time);                                  // Returns true if the sprite needs updating
    void doAction(AstronautAction *action);
    
    // Stat result functions
    float movementSpeed();
    
    // Initialization
    Astronaut(string name, string sprite, int x, int y);
    
    // Display
    string spriteBase;
    string sprite();
    Coord apparentPosition();
};

#endif /* Astronaut_hpp */
