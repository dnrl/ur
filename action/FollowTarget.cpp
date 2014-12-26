//
//  FollowTarget.cpp
//
//  Created by 조 중욱 on 2014. 11. 6..
//
//

#include "FollowTarget.h"

USING_NS_CC;

namespace ur { namespace action {

///감속 (낮을수록 빠름)
const double DecelerationTweaker = 0.35;

FollowTarget::FollowTarget():
_toTarget(nullptr),
_isRotate(false),
_isBounding(false),
_maxSpeed(0.0f),
_vLook(Vec2::ZERO),
_vVelocity(Vec2::ZERO),
_onArriveCallback(nullptr)
{
}

FollowTarget::~FollowTarget()
{
}

FollowTarget* FollowTarget::create(cocos2d::Node* target, float maxSpeed, bool isRotation, Deceleration deceleration)
{
    return create(target, maxSpeed, isRotation, deceleration, false, nullptr);
}
    
FollowTarget* FollowTarget::create(cocos2d::Node* target, float maxSpeed, bool isRotation, Deceleration deceleration, bool isBounding, arriveCallback onArrive)
{
    auto ret = new (std::nothrow) FollowTarget();
    ret->initWithTarget(target, maxSpeed, isRotation, deceleration, isBounding, onArrive);
    ret->autorelease();
    return ret;
}

bool FollowTarget::initWithTarget(cocos2d::Node* target, float maxSpeed, bool isRotation, Deceleration deceleration, bool isBounding, arriveCallback onArrive)
{
    target->retain();
    _toTarget = target;
    
    _maxSpeed = maxSpeed;
    _isRotate = isRotation;
    
    _deceleration = deceleration;
    
    _onArriveCallback = onArrive;
    
    _isBounding = isBounding;
    
    return true;
}

FollowTarget* FollowTarget::clone() const
{
    return create(_toTarget, _maxSpeed, _isRotate, _deceleration, _isBounding, _onArriveCallback);
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
        
        if(_isBounding) {
            if(_target->getBoundingBox().intersectsRect(_toTarget->getBoundingBox())) {
                dist = 0;
                if(_onArriveCallback)
                    _onArriveCallback(_target);
                
                return;
            }
        }
        
        if(dist > 0) {
            float speed = dist / ((float)_deceleration * DecelerationTweaker);
            speed = std::min(speed, _maxSpeed);
            
            Vec2 DesiredVelocity = toTarget * speed / dist;
            vArrive = (DesiredVelocity - _vVelocity);
        }
        else {
            if(_onArriveCallback)
                _onArriveCallback(_target);
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