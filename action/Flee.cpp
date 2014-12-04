//
//  Flee.cpp
//  DragonVillage3
//
//  Created by 조 중욱 on 2014. 11. 6..
//
//

#include "Flee.h"

USING_NS_CC;

Flee::Flee()
{
}

Flee::~Flee()
{
    CC_SAFE_RELEASE_NULL(_toTarget);
}

Flee* Flee::create(cocos2d::Node* target, float maxSpeed, float radius, bool isRotation)
{
    auto ret = new (std::nothrow) Flee();
    ret->initWithTarget(target, maxSpeed, radius, isRotation);
    ret->autorelease();
    return ret;
}

bool Flee::initWithTarget(cocos2d::Node* target, float maxSpeed, float radius, bool isRotation)
{
    target->retain();
    _toTarget = target;
    _radius = radius;
    _maxSpeed = maxSpeed;
    _isRotate = isRotation;
    return true;
}

Flee* Flee::clone() const
{
    return create(_target, _maxSpeed, _radius, _isRotate);
}

Flee* Flee::reverse() const
{
    return nullptr;
}

void Flee::step(float time)
{
    if(_target && _toTarget) {
        Vec2 vArrive = Vec2::ZERO;
        float targetDistance = _target->getPosition().getDistance(_toTarget->getPosition());
        if(targetDistance < _radius) {
            Vec2 DesiredVelocity = (_target->getPosition() - _toTarget->getPosition()).getNormalized() * _maxSpeed;
            vArrive = (DesiredVelocity - _vVelocity);
            _vVelocity += vArrive;
        }
        else {
            _vVelocity = Vec2::ZERO;
        }
        
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

void Flee::stop()
{
    _target = nullptr;
    Action::stop();
}

bool Flee::isDone() const
{
    return (!_target->isRunning());
}