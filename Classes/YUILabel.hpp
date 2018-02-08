//
//  YUILabel.hpp
//  nirgal-mobile
//
//  Created by Timothy Raveling on 2/8/18.
//

#ifndef YUILabel_hpp
#define YUILabel_hpp

#include <stdio.h>
#include "YUIView.hpp"

class YUILabel : public YUIView {
    
public:
    
    YUILabel(string text, float xs, float ys);
    virtual void resetFrame() override;
    
private:
    
    cocos2d::Label *contentLabel;
};

#endif /* YUILabel_hpp */
