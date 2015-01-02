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
TintBlink* TintBlink::create(cocos2d::Color3B color)
{
    return create(cocos2d::Color3B::WHITE, color);
}

TintBlink* TintBlink::create(cocos2d::Color3B from, cocos2d::Color3B to)
{
    auto tint = new (std::nothrow) TintBlink();
    tint->initWithColor(from, to);
    tint->autorelease();
    return tint;
}

bool TintBlink::initWithColor(cocos2d::Color3B from, cocos2d::Color3B to)
{
    _fromColor = from;
    _toColor = to;
    _isChange = false;
    _tick = 0.0f;
    
    return true;
}

TintBlink* TintBlink::clone(void) const
{
    return create(_fromColor, _toColor);
}

TintBlink* TintBlink::reverse() const
{
    return create(_toColor, _fromColor);
}

void TintBlink::step(float time)
{
    if (!isDone())
    {
        _tick += time;
        if(_tick > 0.5f) {
            _tick = 0.0f;
            _target->setColor((!_isChange) ? _toColor : _fromColor);
            _isChange = !_isChange;
        }
    }
}

bool TintBlink::isDone() const
{
    return (!_target);
}

void TintBlink::stop()
{
    if(_target)
        _target->setColor(_fromColor);
    Action::stop();
}

}}