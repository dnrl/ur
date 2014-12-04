//
//  NumberFromTo.cpp
//
//  Created by 조 중욱 on 2014. 10. 6..
//
//

#include "NumberFromTo.h"

USING_NS_CC;

namespace ur { namespace action {

NumberFromTo* NumberFromTo::create(float duration, float from, float to, const std::string& format)
{
    auto fromTo = new (std::nothrow) NumberFromTo();
    fromTo->initWithDuration(duration, from, to, format);
    fromTo->autorelease();
    return fromTo;
}

bool NumberFromTo::initWithDuration(float duration, float from, float to, const std::string& format)
{
    if(ActionInterval::initWithDuration(duration)) {
        _to = to;
        _from = from;
        _format = format;
        return true;
    }
    return false;
}

NumberFromTo* NumberFromTo::clone() const
{
    auto a = new(std::nothrow) NumberFromTo();
    a->initWithDuration(_duration, _from, _to, _format);
    a->autorelease();
    return a;
}


NumberFromTo* NumberFromTo::reverse() const
{
    return NumberFromTo::create(_duration, _to, _from, _format);
}

void NumberFromTo::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
}

void NumberFromTo::update(float time)
{
    ((Label*)(_target))->setString(StringUtils::format(_format.c_str(), _from + (_to - _from) * time));
}
}}