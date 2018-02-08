//
//  YUIView.hpp
//  nirgal-mobile
//
//  Created by Timothy Raveling on 2/8/18.
//

#ifndef YUIView_hpp
#define YUIView_hpp

#include <stdio.h>
#include "cocos2d.h"

using namespace std;

class YUIView : public cocos2d::LayerColor {
    
public:
    
    // Variables
    
    float leftAnchor, rightAnchor, topAnchor, bottomAnchor;
    float defaultWidth, defaultHeight;
    
    // Constructor
    
    YUIView(float xs, float ys);
    
    // Functions
    
    void setBackgroundColor(const cocos2d::Color4B& color);
    
    void anchorLeft(float anchor);
    void anchorRight(float anchor);
    void anchorTop(float anchor);
    void anchorBottom(float anchor);
    
    void addSubview(YUIView *view);
    void removeSubview(YUIView *subview);
    virtual void resetFrame();
};

#endif /* YUIView_hpp */
