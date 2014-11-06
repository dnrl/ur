//
//  Slow.cpp
//
//  Created by 조 중욱 on 2014. 11. 6..
//
//

#include "Slow.h"

USING_NS_CC;

namespace ur { namespace action {

Slow* Slow::create(float duration, float slowDuration)
{
    auto slow = new Slow();
    slow->initWithDuration(duration, slowDuration);
    slow->autorelease();
    return slow;
}

bool Slow::initWithDuration(float duration, float slowDuration)
{
    if(ActionInterval::initWithDuration(duration)) {
        _totalDuration = duration;
        _slowDuration = slowDuration;
        return true;
    }
    return false;
}

Slow* Slow::clone() const
{
    auto slow = new Slow();
    slow->initWithDuration(_totalDuration, _slowDuration);
    slow->autorelease();
    return slow;
}

Slow* Slow::reverse() const
{
    return nullptr;
}

void Slow::startWithTarget(cocos2d::Node *target)
{
    ActionInterval::startWithTarget(target);
}

void Slow::update(float time)
{
    _target->getScheduler()->setTimeScale(_slowDuration);
}

void Slow::stop()
{
    _target->getScheduler()->setTimeScale(1.0);
    ActionInterval::stop();
}

}}
