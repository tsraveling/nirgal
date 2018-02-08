//
//  YUIView.cpp
//  nirgal-mobile
//
//  Created by Timothy Raveling on 2/8/18.
//

#include "YUIView.hpp"

// Constructor

YUIView::YUIView(float xs, float ys) {
    
    this->initWithColor(cocos2d::Color4B(0,0,0,160), xs, ys);
    
    this->defaultWidth = xs;
    this->defaultHeight = ys;
    
    this->topAnchor = -1;
    this->leftAnchor = -1;
    this->bottomAnchor = -1;
    this->rightAnchor = -1;
    
    this->setPosition(0, 0);
}

// Functions

void YUIView::setBackgroundColor(const cocos2d::Color4B& color) {
    this->initWithColor(color);
}

void YUIView::anchorLeft(float anchor) {
    
    this->leftAnchor = anchor;
    this->resetFrame();
}

void YUIView::anchorRight(float anchor) {
    
    this->rightAnchor = anchor;
    this->resetFrame();
}

void YUIView::anchorTop(float anchor) {
    
    this->topAnchor = anchor;
    this->resetFrame();
}

void YUIView::anchorBottom(float anchor) {
    
    this->bottomAnchor = anchor;
    this->resetFrame();
}

void YUIView::addSubview(YUIView *view) {
    
    this->addChild(view);
    view->resetFrame();
}

void YUIView::removeSubview(YUIView *subview) {
    
    this->removeChild(subview);
}

#pragma mark - Reset Frame

void YUIView::resetFrame() {
    
    float x = this->getPositionX();
    float y = this->getPositionY();
    auto size = this->getContentSize();
    
    auto parent = this->getParent();
    
    if (parent == NULL) {
        
        printf("Attempted to lay out YUIView without a parent.");
        return;
    }
    
    auto parent_size = parent->getContentSize();
    
    // Lay out the view on the HORIZONTAL axis //
    
    if (leftAnchor > -1 && rightAnchor > -1) {
        
        // If there's a left and right anchor, lay out the view in the middle
        
        x = leftAnchor;
        auto rx = parent_size.width - rightAnchor;
        size.width = rx - x;
        
    } else if (leftAnchor > -1) {
        
        // If there's only a left anchor, easy peasy, snap to frame
        
        x = leftAnchor;
        size.width = this->defaultWidth;
        
    } else if (rightAnchor > -1) {
        
        // If there's only a right anchor, same same
        auto rx = parent_size.width - rightAnchor;
        size.width = this->defaultWidth;
        x = rx - size.width;
        
    } else {
        
        // Default layout
        x = 0;
        size.width = this->defaultWidth;
    }
    
    
    // Lay out the view on the VERTICAL axis //
    
    if (topAnchor > -1 && bottomAnchor > -1) {
        
        // If there's a top and bottom anchor, lay out the view in the middle
        
        y = bottomAnchor;
        auto ry = parent_size.height - topAnchor;
        size.height = ry - y;
        
    } else if (bottomAnchor > -1) {
        
        // If there's only a bottom anchor, easy peasy, snap to frame
        
        y = bottomAnchor;
        size.height = this->defaultHeight;
        
    } else if (topAnchor > -1) {
        
        // If there's only a top anchor, same same
        auto ry = parent_size.height - topAnchor;
        size.height = this->defaultHeight;
        y = ry - size.height;
        
    } else {
        
        // Default layout
        y = 0;
        size.height = this->defaultHeight;
    }
    
    // Now actually set the view
    this->setPosition(x, y);
    this->setContentSize(size);
}
