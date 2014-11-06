//
//  Flee.h
//  DragonVillage3
//
//  Created by 조 중욱 on 2014. 11. 6..
//
//

#ifndef __DragonVillage3__Flee__
#define __DragonVillage3__Flee__

#include <cocos2d.h>

class Flee :
public cocos2d::Action
{
public:
	Flee(void);
	virtual ~Flee(void);
	
    static Flee* create(cocos2d::Node* target, float maxSpeed, float radius, bool isRotation);
    
    virtual Flee* clone() const override;
    virtual Flee* reverse() const override;
    virtual void step(float time) override;
    virtual bool isDone() const override;
    virtual void stop() override;
    
    bool initWithTarget(cocos2d::Node* target, float maxSpeed, float radius, bool isRotation);
    
protected:
    cocos2d::Vec2   _vVelocity;
    cocos2d::Vec2   _vLook;
    
    float           _maxSpeed;
    float           _radius;
    
    bool            _isRotate;
    
    cocos2d::Node*  _toTarget;
	
};


#endif