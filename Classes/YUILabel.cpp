//
//  YUILabel.cpp
//  nirgal-mobile
//
//  Created by Timothy Raveling on 2/8/18.
//

#include "YUILabel.hpp"

YUILabel::YUILabel(string text, float xs, float ys) : YUIView(xs, ys) {
    
    // At this point the base YUIView has already been created, so let's build the label
    
    this->contentLabel = cocos2d::Label::createWithTTF(text, "fonts/Socket.ttf", 18);
    this->contentLabel->setPosition(0,0);
    this->contentLabel->enableWrap(true);
    this->setBackgroundColor(cocos2d::Color4B(0,0,0,0));
    this->addChild(this->contentLabel);
}

void YUILabel::resetFrame() {
    
    // Do the main reset
    YUIView::resetFrame();
    
    // Reset the position and size of the label
    this->contentLabel->setPosition(this->getContentSize().width * 0.5,this->getContentSize().height * 0.5);
    this->contentLabel->setHeight(this->getContentSize().height);
    this->contentLabel->setWidth(this->getContentSize().width);
    this->contentLabel->setMaxLineWidth(this->getContentSize().width);
    this->contentLabel->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::TOP);
}
