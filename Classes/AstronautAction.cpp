//
//  AstronautAction.cpp
//  nirgal-mobile
//
//  Created by Timothy Raveling on 1/15/18.
//

#include "AstronautAction.hpp"

AstronautAction::AstronautAction(AstronautActionType type, float timeLeft) {
    this->type = type;
    this->timeLeft = timeLeft;
}

AstronautAction::~AstronautAction() {
    
}
