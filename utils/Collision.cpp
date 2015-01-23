//
//  Collision.cpp
//  DragonVillage3
//
//  Created by 조 중욱 on 2015. 1. 20..
//
//

#include "Collision.h"
#include "PixelReadNode.h"

USING_NS_CC;

#pragma mark - 픽셀 터치 충돌
bool Collision::pixelAtPoint(Node* sprite, const Vec2& point)
{
    float contentScale	= CC_CONTENT_SCALE_FACTOR();
    float width			= sprite->getContentSize().width;
    float height		= sprite->getContentSize().height;
    
    auto rt = RenderTexture::create(width, height);
    
    Vec2 pos = sprite->getPosition();
    Vec2 anchor	= sprite->getAnchorPoint();
    float rangle = sprite->getRotation();
    float iScaleX = sprite->getScaleX();
    float iScaleY = sprite->getScaleY();
    
    Vec2 testPoint = Vec2((point.x + width/contentScale * anchor.x) * contentScale - pos.x,
                          (point.y + height/contentScale * anchor.y) * contentScale - pos.y);
    
    auto pixelReadNode = PixelReadNode::create(testPoint);
    
    rt->begin();
    
    sprite->setPosition(Vec2::ZERO);
    sprite->setAnchorPoint(Vec2::ZERO);
    sprite->setRotation(0.0f);
    sprite->setScale(1.0f, 1.0f);
    
    sprite->visit();
    pixelReadNode->visit();
    
    rt->end();
    
    Director::getInstance()->getRenderer()->render();
    
    if (pos.equals(Vec2::ZERO)) {
        sprite->setPosition(Vec2(-1, -1));
    }
    sprite->setRotation(rangle);
    sprite->setPosition(pos);
    sprite->setAnchorPoint(anchor);
    sprite->setScale(iScaleX, iScaleY);

    
    auto buffer = pixelReadNode->getPixelColor();
    
    log("R : %d, G : %d, B : %d, A : %d" , buffer.r, buffer.g, buffer.b, buffer.a);
    
    bool isCollision = (buffer.a > 0);
    
    return isCollision;
}

#pragma mark - 두 스프라이트 픽셀 충돌
bool Collision::PixelAtPixel(Node* spriteA, Node* spriteB)
{
    return true;
}