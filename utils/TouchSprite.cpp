//
//  TouchSprite.cpp
//  util
//
//  Created by 조중욱 on 2015. 1. 17..
//
//

#include "TouchSprite.h"

USING_NS_CC;

TouchSprite::TouchSprite()
{
}

TouchSprite::~TouchSprite()
{
    CCLOGINFO("cocos2d: deallocing TouchSprite");
}

TouchSprite* TouchSprite::create(const std::string& filePath)
{
    auto ret = new (std::nothrow) TouchSprite();
    if(ret && ret->initWithFilePath(filePath)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool TouchSprite::initWithFilePath(const std::string& filePath)
{
    if (!Sprite::initWithFile(filePath)) return false;
    
    log("asdasd");
    
    addEvent();
    
    return true;
}


void TouchSprite::addEvent()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch* t, Event* e)->bool {
        
        if(getBoundingBox().containsPoint(t->getLocation())) {
            
            if(this->collisionCheck(t->getLocation())) {
                log("#### touch ####");
                return true;
            }
        }
        return false;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
}

bool TouchSprite::collisionCheck(const Vec2& touchPt)
{
    Size size = getContentSize();
    RenderTexture* rt = RenderTexture::create(size.width, size.height, Texture2D::PixelFormat::RGBA8888);
    
    GLubyte* pixel = new GLubyte[4];
    
    rt->beginWithClear(1, 0, 0, 1);
    
    this->visit();
    
    glReadPixels((GLint)touchPt.x, (GLint)touchPt.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
    
    rt->end();
    
    log("R : %d, G : %d, B : %d, A : %d" , pixel[0], pixel[1], pixel[2], pixel[3]);
    bool  isCollision = (pixel[0] == 0);
    
    delete[] pixel;
    return !isCollision;
}













