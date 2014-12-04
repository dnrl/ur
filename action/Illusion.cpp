//
//  Illusion.cpp
//  DragonVillage3
//
//  Created by 조 중욱 on 2014. 11. 7..
//
//

#include "Illusion.h"

USING_NS_CC;
namespace ur { namespace action {
Illusion* Illusion::create(float duration, int count, float opacity)
{
    auto illusion = new (std::nothrow) Illusion();
    illusion->initWithDuration(duration, count, opacity);
    illusion->autorelease();
    return illusion;
}

bool Illusion::initWithDuration(float duration, int count, float opacity)
{
    if(ActionInterval::initWithDuration(duration)) {
        _duration = duration;
        _count = count;
        _opacity = opacity;
        
        return true;
    }
    return false;
}

Illusion* Illusion::clone() const
{
    return create(_duration, _count, _opacity);
}

Illusion* Illusion::reverse() const
{
    return clone();
}

void Illusion::startWithTarget(cocos2d::Node *target)
{
    ActionInterval::startWithTarget(target);
    _time = 0.0f;
}

void Illusion::update(float time)
{
    if (_target && ! isDone())    {
        float slice = 1.0f / _count;
        
        if(_time < time) {
            _time += slice;
            createAfterImage();
        }
    }
}

void Illusion::stop()
{
    _target = nullptr;
    ActionInterval::stop();
}

void Illusion::createAfterImage()
{
    Size size = Director::getInstance()->getWinSize();
    Size pixelSize = Director::getInstance()->getWinSizeInPixels();
    
    Vec2 rtBegin = _target->getBoundingBox().origin;
    Rect fullRect = Rect(0, 0, size.width, size.height);
    Rect fullViewport = Rect(0, 0, pixelSize.width, pixelSize.height);
    
    rtBegin = _target->getParent()->convertToWorldSpace(rtBegin);
    
    float width = _target->getBoundingBox().size.width;
    float height = _target->getBoundingBox().size.height;
    
    auto rt = RenderTexture::create(width, height, Texture2D::PixelFormat::RGBA8888);
    rt->setKeepMatrix(true);
    rt->setVirtualViewport(rtBegin - Vec2(width/2, 0), fullRect, fullViewport);
    rt->begin();
//    rt->beginWithClear(1, 0, 0, 1);
    _target->visit();
    rt->end();
    
    auto ret = Sprite::createWithTexture(rt->getSprite()->getTexture());
    ret->setPosition(Vec2(_target->getBoundingBox().getMidX() - width/2, _target->getBoundingBox().getMidY()));
    ret->setFlippedY(true);
    ret->setOpacity(_opacity);
    ret->setColor(Color3B::BLUE);
    _target->getParent()->addChild(ret, _target->getLocalZOrder()-1);
    
    ret->runAction(Sequence::create(DelayTime::create(0.5f),
                                    FadeTo::create(0.5f, 0),
                                    RemoveSelf::create(),
                                    NULL));
}
}}