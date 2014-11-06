//
//  Slow.h
//
//  Created by 조 중욱 on 2014. 11. 6..
//
//

#ifndef __DragonVillage3__Slow__
#define __DragonVillage3__Slow__

#include <cocos2d.h>

namespace ur { namespace action {

class Slow :
public cocos2d::ActionInterval
{
public:
    Slow(void) {}
    virtual ~Slow(void) {}
    
    static Slow* create(float duration, float slowDuration);
    
    virtual Slow* clone() const override;
    virtual Slow* reverse() const override;
    virtual void startWithTarget(cocos2d::Node *target) override;
    virtual void update(float time) override;
    virtual void stop() override;
    
    bool initWithDuration(float duration, float slowDuration);
    
protected:
    float           _slowDuration;
    float           _totalDuration;
};
}}

#endif