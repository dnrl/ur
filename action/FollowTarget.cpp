//
//  FollowTarget.cpp
//  DragonVillage3
//
//  Created by 조 중욱 on 2014. 11. 6..
//
//

#include "FollowTarget.h"

USING_NS_CC;

namespace ur { namespace action {

///감속 (낮을수록 빠름)
const double DecelerationTweaker = 0.15;

FollowTarget::FollowTarget():
_toTarget(nullptr),
_isRotate(false),
_maxSpeed(0.0f),
_vLook(Vec2::ZERO),
_vVelocity(Vec2::ZERO)
{
}

FollowTarget::~FollowTarget()
{
    CC_SAFE_RELEASE_NULL(_toTarget);
}

FollowTarget* FollowTarget::create(cocos2d::Node* target, float maxSpeed, bool isRotation, Deceleration deceleration)
{
    auto ret = new FollowTarget();
    ret->initWithTarget(target, maxSpeed, isRotation, deceleration);
    ret->autorelease();
    return ret;
}

bool FollowTarget::initWithTarget(cocos2d::Node* target, float maxSpeed, bool isRotation, Deceleration deceleration)
{
    target->retain();
    _toTarget = target;
    
    _maxSpeed = maxSpeed;
    _isRotate = isRotation;
    
    _deceleration = deceleration;
    
    return true;
}

FollowTarget* FollowTarget::clone() const
{
    auto ret = new FollowTarget();
    ret->initWithTarget(_toTarget, _maxSpeed, _isRotate, _deceleration);
    ret->autorelease();
    return ret;
}

FollowTarget* FollowTarget::reverse() const
{
    return clone();
}

void FollowTarget::step(float time)
{
    if(_target && _toTarget) {
        Vec2 vArrive = Vec2::ZERO;
        
        Vec2 toTarget = _toTarget->getPosition() - _target->getPosition();
        
        double dist = toTarget.getLength();
        if(dist > 0) {
            float speed = dist / ((float)_deceleration * DecelerationTweaker);
            
            speed = std::min(speed, _maxSpeed);
            
            Vec2 DesiredVelocity = toTarget * speed / dist;
            vArrive = (DesiredVelocity - _vVelocity);
        }
        
        _vVelocity += vArrive;
        
        Vec2 vPos = _target->getPosition();
        vPos += _vVelocity * time;
        _target->setPosition(vPos);
        
        if(_isRotate) {
            if(_vVelocity.length() > 0) {
                _vLook = _vVelocity;
                _vLook.normalize();
                float angle = _vLook.getAngle();
                _target->setRotation(90-CC_RADIANS_TO_DEGREES(angle));
            }
        }
    }
}

void FollowTarget::stop()
{
    _target = nullptr;
    Action::stop();
}

bool FollowTarget::isDone() const
{
    return (!_toTarget->isRunning());
}
    

}}