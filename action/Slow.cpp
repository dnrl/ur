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
    auto slow = new (std::nothrow) Slow();
    slow->initWithDuration(duration, slowDuration);
    slow->autorelease();
    return slow;
}

bool Slow::initWithDuration(float duration, float slowDuration)
{
    auto dt = Director::getInstance()->getScheduler()->getTimeScale();
    if(ActionInterval::initWithDuration(duration * slowDuration * dt)) {
        _totalDuration = duration;
        _slowDuration = slowDuration;
        return true;
    }
    return false;
}

Slow* Slow::clone() const
{
    return create(_totalDuration, _slowDuration);
}

Slow* Slow::reverse() const
{
    return clone();
}

void Slow::startWithTarget(cocos2d::Node *target)
{
    ActionInterval::startWithTarget(target);
    _originDuration = _target->getScheduler()->getTimeScale();
}

void Slow::update(float time)
{
    _target->getScheduler()->setTimeScale(_slowDuration);
}

void Slow::stop()
{
    _target->getScheduler()->setTimeScale(_originDuration);
    _target = nullptr;
    ActionInterval::stop();
}

}}
