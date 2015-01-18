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
    
    static TouchSprite* create(const std::string& filePath);
    
    using CallBack = std::function<void(TouchSprite*)>;
	
private:
    CallBack            _touchCallback;
    
public:
    void addEvent();
    
    bool collisionCheck(const Vec2& touchPt);
	
};

NS_CC_END

#endif /* defined(__util__TouchSprite__) */
