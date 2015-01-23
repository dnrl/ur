//
//  PixelReadNode.cpp
//  DragonVillage3
//
//  Created by 조 중욱 on 2015. 1. 20..
//
//

#include "PixelReadNode.h"


USING_NS_CC;

PixelReadNode::PixelReadNode()
{
}

PixelReadNode::~PixelReadNode()
{
}

PixelReadNode* PixelReadNode::create(const cocos2d::Vec2& point)
{
    auto ret = new (std::nothrow) PixelReadNode();
    if(ret && ret->init(point)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool PixelReadNode::init(const cocos2d::Vec2& point)
{
    if(!Node::init()) return false;

    _readPoint = point;
    _pixelColorRead = Color4B(0, 0, 0, 0);
    
    return true;
}


void PixelReadNode::draw(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
    _readPixelsCommand.init(_globalZOrder);
    _readPixelsCommand.func = CC_CALLBACK_0(PixelReadNode::onDraw, this);
    renderer->addCommand(&_readPixelsCommand);
}


void PixelReadNode::onDraw()
{
    auto buffer = (Color4B*)malloc(sizeof(Color4B));
    glReadPixels(_readPoint.x, _readPoint.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    
    _pixelColorRead = Color4B(buffer->r, buffer->g, buffer->b, buffer->a);
    
    free(buffer);
}

