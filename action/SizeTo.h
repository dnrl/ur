//
//  SizeTo.h
//
//  Created by 조 중욱 on 2015. 1. 13..
//
//

#ifndef __DragonVillage3__SizeTo__
#define __DragonVillage3__SizeTo__

#include <cocos2d.h>

namespace ur { namespace action {
class SizeTo :
public cocos2d::ActionInterval
{
public:
    SizeTo(void) {}
    virtual ~SizeTo(void) {}
    
    static SizeTo* create(float duration, const cocos2d::Size& to);
    static SizeTo* create(float duration, const cocos2d::Size& from, const cocos2d::Size& to);
    
    virtual SizeTo* clone() const override;
    virtual SizeTo* reverse() const override;
    virtual void startWithTarget(cocos2d::Node *target) override;
    virtual void update(float time) override;
    
    bool initWithDuration(float duration, const cocos2d::Size& from, const cocos2d::Size& to);
    
private:
    cocos2d::Size           _fromSize;
    cocos2d::Size           _toSize;
	
public:
	
};
}}

#endif