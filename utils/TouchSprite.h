//
//  TouchSprite.h
//  util
//
//  Created by 조중욱 on 2015. 1. 17..
//
//

#ifndef __util__TouchSprite__
#define __util__TouchSprite__

#include <cocos2d.h>

NS_CC_BEGIN

class TouchSprite :
public Sprite
{
public:
	TouchSprite(void);
	virtual ~TouchSprite(void);
	
    virtual bool initWithFilePath(const std::string& filePath);
    
    using CallBack = std::function<void(TouchSprite*)>;
    
    virtual bool onTouchBegan(Touch* t, Event* e);
    virtual void onTouchEnded(Touch* t, Event* e);
    virtual void onTouchMoved(Touch* t, Event* e);
    virtual void onTouchCancelled(Touch* t, Event* e);
        
protected:
    CallBack            _touchBeginCallback;
    CallBack            _touchEndCallback;
    
    bool                _isTouchEnable;
public:
    void setTouchEnable(bool e);
};

NS_CC_END

#endif /* defined(__util__TouchSprite__) */
