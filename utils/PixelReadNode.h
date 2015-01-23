//
//  PixelReadNode.h
//  DragonVillage3
//
//  Created by 조 중욱 on 2015. 1. 20..
//
//

#ifndef __DragonVillage3__PixelReadNode__
#define __DragonVillage3__PixelReadNode__

#include <cocos2d.h>

NS_CC_BEGIN

class CC_DLL PixelReadNode :
public cocos2d::Node
{
public:
	PixelReadNode(void);
	virtual ~PixelReadNode(void);
	
    virtual bool init(const cocos2d::Vec2& point);
    
    static PixelReadNode* create(const cocos2d::Vec2& point);
    
    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags);
    Color4B getPixelColor() const { return _pixelColorRead; };
    
protected:
    void onDraw();
    cocos2d::CustomCommand  _readPixelsCommand;
    cocos2d::Vec2           _readPoint;
    Color4B                _pixelColorRead;
    
public:
	
};

NS_CC_END

#endif