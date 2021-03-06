//
//  Illusion.h
//  DragonVillage3
//
//  Created by 조 중욱 on 2014. 11. 7..
//
//

#ifndef __DragonVillage3__Illusion__
#define __DragonVillage3__Illusion__

#include <cocos2d.h>
namespace ur { namespace action {
class Illusion :
public cocos2d::ActionInterval
{
public:
    Illusion(void) {}
    virtual ~Illusion(void) {}
    
    static Illusion* create(float duration, int count, float opacity = 100.0f);
    
    virtual Illusion* clone() const override;
    virtual Illusion* reverse() const override;
    virtual void startWithTarget(cocos2d::Node *target) override;
    virtual void update(float time) override;
    virtual void stop() override;
    
    virtual void createAfterImage();
    
    bool initWithDuration(float duration, int count, float opacity);
	
protected:
    float           _duration;
    float           _time;
    float           _count;
    float           _opacity;
    
    
	
};
}}

#endif