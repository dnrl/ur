//
//  SizeTo.cpp
//
//  Created by 조 중욱 on 2015. 1. 13..
//
//

#include "SizeTo.h"


USING_NS_CC;

namespace ur { namespace action {
    SizeTo* SizeTo::create(float duration, const cocos2d::Size& to)
    {
        return create(duration, Size::ZERO, to);
    }
    
    SizeTo* SizeTo::create(float duration, const cocos2d::Size& from, const cocos2d::Size& to)
    {
        auto sizeTo = new (std::nothrow) SizeTo();
        sizeTo->initWithDuration(duration, from, to);
        sizeTo->autorelease();
        return sizeTo;
    }
    
    bool SizeTo::initWithDuration(float duration, const cocos2d::Size& from, const cocos2d::Size& to)
    {
        if ( ActionInterval::initWithDuration(duration) ) {
            _fromSize = from;
            _toSize = to;
            
            return true;
        }
        return false;
    }
    
    SizeTo* SizeTo::clone(void) const
    {
        return create(_duration, _fromSize, _toSize);
    }
    
    SizeTo* SizeTo::reverse() const
    {
        return create(_duration, _toSize, _fromSize);
    }
    
    void SizeTo::startWithTarget(cocos2d::Node *target)
    {
        ActionInterval::startWithTarget(target);
        if(_fromSize.equals(Size::ZERO)) {
            _fromSize = target->getContentSize();
        }
    }
    
    void SizeTo::update(float time)
    {
        if(_target) {
            _target->setContentSize(_fromSize + (_toSize - _fromSize) * time);
        }
    }
}}