//
//  Astronaut.cpp
//  nirgal-mobile
//
//  Created by Timothy Raveling on 1/12/18.
//

#include "Astronaut.hpp"
#include "Map.h"

#pragma mark - AI functions

bool Astronaut::tick(Map *map, float time) {
    
    bool needs_refresh = false;
    
    // First, check to see if we're currently doing something
    if (this->currentAction != NULL) {
        
        // If we are, decrement the duration
        this->currentAction->timeLeft -= time;
        if (this->currentAction->timeLeft <= 0) {
            
            printf("%s, concluding action\n", this->name.c_str());
            
            // Conclude the action
            switch (this->currentAction->type) {
                
                case actionMovement:                // Move the astronaut
                    
                    // Make sure the path is clear
                    if (map->isPassable(this->x, this->y, this->currentAction->direction)) {
                        
                        // Actually move the 'naut
                        switch (this->currentAction->direction) {
                            case routingUp:
                                this->y += 1;
                                break;
                            case routingRight:
                                this->x += 1;
                                break;
                            case routingDown:
                                this->y -= 1;
                                break;
                            case routingLeft:
                                this->x -= 1;
                                break;
                        }
                        
                        // Refresh the sprite
                        needs_refresh = true;
                        
                    } else {
                        
                        // TODO: replace this rough frustration delay
                        this->currentAction->timeLeft = 1.0;
                    }
                    
                    break;
            }
            
            // Remove the action, if we didn't refresh the timer on it
            if (this->currentAction->timeLeft <= 0) {
                delete(this->currentAction);
                this->currentAction = NULL;
            }
        }
    }
    
    // If the action is empty or has just been completed, select a new one from our goals
    if (this->currentAction == NULL) {
        
        // Make sure we have a goal, otherwise be idle
        if (this->immediateGoals.size() > 0) {
            
            auto goal = this->immediateGoals.front();
            switch (goal->type) {
                    
                case goalMovement:          // GOAL: Movement
                    
                    // Are we there yet?
                    if (goal->route->steps.size() == 0) {
                        
                        // If we are, then hurrah, we can be done
                        this->immediateGoals.erase(this->immediateGoals.begin());
                        
                    } else {
                        
                        // Create the step action
                        auto *action = new AstronautAction(actionMovement, this->movementSpeed());
                        
                        // Get the direction from the step
                        auto step = goal->route->steps.front();
                        action->direction = step;
                        
                        // Drop the step
                        goal->route->steps.erase(goal->route->steps.begin());
                        
                        // Perform the action
                        this->doAction(action);
                    }
                    break;
                    
                default:
                    break;
            }
        } else {
            
            // TODO: Eventually we could have the astronauts decide what to do in their free time here
        }
    }
    
    // TODO: passive astronaut stuff like health, happiness, moods, etc.
    
    return needs_refresh;
}

void Astronaut::doAction(AstronautAction *action) {
    
    // Cancel any current action
    if (this->currentAction != NULL) {
        delete(this->currentAction);
    }
    
    // Start the new one
    this->currentAction = action;
}

#pragma mark - Interface with User Input

void Astronaut::setGoal(AstronautGoal *goal) {
    
    // First clear out current goals
    this->immediateGoals.clear();
    
    // Now do this one
    this->addGoal(goal);
}

void Astronaut::addGoal(AstronautGoal *goal) {
    
    // Add the goal to the stack
    this->immediateGoals.push_back(goal);
}

#pragma mark - Stat Result Functions

float Astronaut::movementSpeed() {
    return 0.15;
}

#pragma mark - Utility functions

string Astronaut::sprite() {
    
    return this->spriteBase + (this->isSuited ? "_st.png" : "_ns.png");
}

Astronaut::Astronaut(string name, string sprite, int x, int y) {
    
    this->name = name;
    this->spriteBase = sprite;
    this->x = x;
    this->y = y;
    this->isSuited = false;
    this->currentAction = NULL;
}

Coord Astronaut::apparentPosition() {
    
    Coord loc = Coord();
    loc.x = (float(this->x) * 64) + 32;
    loc.y = (float(this->y) * 64) + 32;
    return loc;
}
