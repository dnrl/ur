//
//  Shake.cpp
//
//  Created by 조 중욱 on 2014. 11. 6..
//
//

#include "Shake.h"

USING_NS_CC;

namespace ur { namespace action {

Shake* Shake::create(float duration, float strength)
{
    return create(duration, strength, strength);
}

Shake* Shake::create(float duration, float strength_x, float strength_y)
{
    auto shake = new Shake();
    shake->initWithDuration(duration, strength_x, strength_y);
    shake->autorelease();
    return shake;
}

bool Shake::initWithDuration(float duration, float strength_x, float strength_y)
{
    if ( ActionInterval::initWithDuration(duration) ) {
        _strength_x = strength_x;
        _strength_y = strength_y;
        
        return true;
    }
    return false;
}

Shake* Shake::clone(void) const
{
    auto a = new Shake();
    a->initWithDuration(_duration, _strength_x, _strength_y);
    a->autorelease();
    return a;
}

Shake* Shake::reverse() const
{
    return Shake::create(_duration, _strength_x, _strength_y);
}

float fgRangeRand(float min, float max)
{
    float rnd = ((float)rand() / (float)RAND_MAX);
    return rnd * (max - min) + min;
}

void Shake::update(float time)
{
    float randx = fgRangeRand( -_strength_x, _strength_x );
    float randy = fgRangeRand( -_strength_y, _strength_y );
    
    _target->setPosition(Point(_initial_x + randx, _initial_y + randy));
}

void Shake::stop()
{
    _target->setPosition(Point(_initial_x, _initial_y));
    
    ActionInterval::stop();
}

void Shake::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    _initial_x = target->getPosition().x;
    _initial_y = target->getPosition().y;
}
}}