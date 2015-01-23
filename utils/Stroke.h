//
//  Stroke.h
//  DragonVillage3
//
//  Created by 조 중욱 on 2015. 1. 21..
//
//

#ifndef __DragonVillage3__Stroke__
#define __DragonVillage3__Stroke__

#include <cocos2d.h>

NS_CC_BEGIN

class Stroke
{
public:
    Stroke(void) {}
    virtual ~Stroke(void) {}
	
    static void outline(Sprite* sprite, const Color3B& color, float size);
    static Node* outline2(Sprite* sprite, const Color3B& color, float size);
    
    static void outline3(Sprite* sprite, const Color3B& color, float size);
};

NS_CC_END

#endif