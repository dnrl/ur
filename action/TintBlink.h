//
//  TintBlink.h
//  DragonVillage3
//
//  Created by 조 중욱 on 2014. 12. 19..
//
//

#ifndef __DragonVillage3__TintBlink__
#define __DragonVillage3__TintBlink__

#include <cocos2d.h>

namespace ur { namespace action {
class TintBlink :
public cocos2d::Action
{
public:
    TintBlink(void) {}
    virtual ~TintBlink(void) {}
    
    static TintBlink* create(cocos2d::Color3B color);
    static TintBlink* create(cocos2d::Color3B from, cocos2d::Color3B to);
    
    virtual TintBlink* clone() const override;
    virtual TintBlink* reverse() const override;
    virtual void step(float time) override;
    virtual bool isDone() const override;
    virtual void stop() override;
    
    bool initWithColor(cocos2d::Color3B from, cocos2d::Color3B to);
	
protected:
    cocos2d::Color3B         _fromColor;
    cocos2d::Color3B         _toColor;
    
    float                       _tick;
    bool                        _isChange;
    
};
}}

#endif