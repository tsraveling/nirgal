//
//  AstronautSprite.cpp
//  nirgal
//
//  Created by Timothy Raveling on 3/3/18.
//

#include "AstronautSprite.hpp"

AstronautSprite::AstronautSprite() {
    this->isSelected = false;
}

void AstronautSprite::setPosition(Coord pos) {
    
    this->mapAstronautSprite->setPosition(pos.x, pos.y);
    //this->mapAstronautSelector->setPosition(pos.x, pos.y);
}

void AstronautSprite::updatePosition() {
    this->setPosition(this->astronaut->apparentPosition());
}

void AstronautSprite::updateSprite() {
    
    // TODO: update frame if needed
}

void AstronautSprite::setSelected(bool selected) {
    
    this->isSelected = selected;
    this->rosterAstronautSelector->setVisible(selected);
    this->mapAstronautSelector->setVisible(selected);
}
