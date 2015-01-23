//
//  TouchSprite.cpp
//  util
//
//  Created by 조중욱 on 2015. 1. 17..
//
//

#include "TouchSprite.h"
#include "PixelReadNode.h"
#include "Collision.h"

USING_NS_CC;

TouchSprite::TouchSprite():
_touchBeginCallback(nullptr),
_touchEndCallback(nullptr),
_isTouchEnable(false)
{
}

TouchSprite::~TouchSprite()
{
    CCLOGINFO("cocos2d: deallocing TouchSprite");
}

bool TouchSprite::initWithFilePath(const std::string& filePath)
{
    if (!Sprite::initWithFile(filePath)) return false;
    
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = CC_CALLBACK_2(TouchSprite::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(TouchSprite::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(TouchSprite::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(TouchSprite::onTouchCancelled, this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void TouchSprite::setTouchEnable(bool e)
{
    _isTouchEnable = e;
}


bool TouchSprite::onTouchBegan(Touch* t, Event* e)
{
    if(_isTouchEnable) {
        auto pt = t->getLocation();
        if(getBoundingBox().containsPoint(pt)) {
            if(Collision::pixelAtPoint(this, pt) ) {
                log("#### touch begin ####");
                
                if(_touchBeginCallback)
                    _touchBeginCallback(this);
                
                return true;
            }
        }
    }
    
    return false;
}

void TouchSprite::onTouchEnded(Touch* t, Event* e)
{
    if(_isTouchEnable) {
        auto pt = t->getLocation();
        if(getBoundingBox().containsPoint(pt)) {
            if(Collision::pixelAtPoint(this, pt) ) {
                log("#### touch end ####");
                
                if(_touchEndCallback)
                    _touchEndCallback(this);
            }
        }
    }
}

void TouchSprite::onTouchMoved(Touch* t, Event* e)
{
    if(_isTouchEnable) {
    }
}

void TouchSprite::onTouchCancelled(Touch* t, Event* e)
{
    
    if(_isTouchEnable) {
    }
}











