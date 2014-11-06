//
//  FollowTarget.h
//  DragonVillage3
//
//  Created by 조 중욱 on 2014. 11. 6..
//
//

#ifndef __DragonVillage3__FollowTarget__
#define __DragonVillage3__FollowTarget__

#include <cocos2d.h>

namespace ur { namespace action {

//도착시 감속
enum class Deceleration {
    fast = 1,
    normal = 2,
    slow = 3,
};

class FollowTarget :
public cocos2d::Action
{
public:
	FollowTarget(void);
	virtual ~FollowTarget(void);
    
    static FollowTarget* create(cocos2d::Node* target, float maxSpeed, bool rotationEnable, Deceleration deceleration);
    
    virtual FollowTarget* clone() const override;
    virtual FollowTarget* reverse() const override;
    virtual void step(float time) override;
    virtual bool isDone() const override;
    virtual void stop() override;
    
    bool initWithTarget(cocos2d::Node* target, float maxSpeed, bool isRotation, Deceleration deceleration);
    
protected:
    cocos2d::Vec2   _vVelocity;
    cocos2d::Vec2   _vLook;
    
    float           _maxSpeed;
    
    bool            _isRotate;
    
    cocos2d::Node*  _toTarget;
    
    Deceleration    _deceleration;
	
};
    
}}

#endif