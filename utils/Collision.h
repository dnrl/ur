//
//  Collision.h
//  DragonVillage3
//
//  Created by 조 중욱 on 2015. 1. 20..
//
//

#ifndef __DragonVillage3__Collision__
#define __DragonVillage3__Collision__

#include <cocos2d.h>

NS_CC_BEGIN

class Collision
{
public:
    Collision(void) {}
    virtual ~Collision(void) {}
    
    ///픽셀과 포인트 충돌
    static bool pixelAtPoint(Node* sprite, const Vec2& point);
    
    ///두 이미지의 픽셀 충돌
    static bool PixelAtPixel(Node* spriteA, Node* spriteB);
	
};

NS_CC_END

#endif