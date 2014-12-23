//
//  TintBlink.cpp
//  DragonVillage3
//
//  Created by 조 중욱 on 2014. 12. 19..
//
//

#include "TintBlink.h"

USING_NS_CC;
namespace ur { namespace action {
TintBlink* TintBlink::create(float duration, cocos2d::Color3B color)
{
    return create(duration, cocos2d::Color3B::WHITE, color);
}

TintBlink* TintBlink::create(float duration, cocos2d::Color3B from, cocos2d::Color3B to)
{
    auto tint = new (std::nothrow) TintBlink();
    tint->initWithColor(duration, from, to);
    tint->autorelease();
    return tint;
}

bool TintBlink::initWithColor(float duration, cocos2d::Color3B from, cocos2d::Color3B to)
{
    if ( ActionInterval::initWithDuration(duration) )
    {
        _fromColor = from;
        _toColor = to;
        
        return true;
    }
    return false;
}

TintBlink* TintBlink::clone(void) const
{
    return create(_duration, _fromColor, _toColor);
}

TintBlink* TintBlink::reverse() const
{
    return create(_duration, _toColor, _fromColor);
}

void TintBlink::update(float time)
{
    if (_target && ! isDone())
    {
        float slice = 1.0f / _duration;
        float m = fmodf(time, slice);
        
        if(m > slice / 2) {
            _target->setColor(_toColor);
        }
        else {
            _target->setColor(_fromColor);
        }
    }
}

void TintBlink::stop()
{
    _target->setColor(_fromColor);
    ActionInterval::stop();
}

void TintBlink::startWithTarget(cocos2d::Node *target)
{
    ActionInterval::startWithTarget(target);
}
}}