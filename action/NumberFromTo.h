//
//  NumberFromTo.h
//
//  Created by 조 중욱 on 2014. 10. 6..
//
//

#ifndef __DragonVillage3__NumberFromTo__
#define __DragonVillage3__NumberFromTo__

#include <cocos2d.h>

namespace ur { namespace action {

class NumberFromTo :
public cocos2d::ActionInterval
{
public:
    NumberFromTo(void){}
    virtual ~NumberFromTo(void){}
    
    bool initWithDuration(float duration, float from, float to, const std::string& format);
    
    static NumberFromTo* create(float duration, float from, float to, const std::string& format);
    
    virtual NumberFromTo* clone() const override;
    virtual NumberFromTo* reverse() const override;
    virtual void startWithTarget(cocos2d::Node *target) override;
    virtual void update(float time) override;
    
protected:
    float           _to;
    float           _from;
    
    std::string     _format;
	
protected:
	
};

}}


#endif