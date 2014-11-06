//
//  Shake.h
//
//  Created by 조 중욱 on 2014. 11. 6..
//
//

#ifndef __Shake__
#define __Shake__

#include <cocos2d.h>

namespace ur { namespace action {
class Shake :
public cocos2d::ActionInterval
{
public:
    Shake(void) {};
    virtual ~Shake(void) {};
    
    static Shake* create(float duration, float strength );
    static Shake* create(float duration, float strength_x, float strength_y );
    
    virtual Shake* clone() const override;
    virtual Shake* reverse() const override;
    virtual void startWithTarget(cocos2d::Node *target) override;
    virtual void update(float time) override;
    virtual void stop() override;
    
    bool initWithDuration(float duration, float strength_x, float strength_y );
    
protected:
    float _initial_x, _initial_y;
    float _strength_x, _strength_y;
};
}}

#endif