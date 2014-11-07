//
//  Illusion.cpp
//  DragonVillage3
//
//  Created by 조 중욱 on 2014. 11. 7..
//
//

#include "Illusion.h"

USING_NS_CC;

Illusion::Illusion()
{
}

Illusion::~Illusion()
{
}

Illusion* Illusion::create(float duration, int count, float opacity)
{
    auto illusion = new Illusion();
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
            addSprite();
        }
    }
}

void Illusion::stop()
{
    _target = nullptr;
    ActionInterval::stop();
}


void Illusion::addSprite()
{
    Size size = Director::getInstance()->getWinSize();
    Size pixelSize = Director::getInstance()->getWinSizeInPixels();
    
    auto originPos = _target->getBoundingBox().origin;
    
    auto rt = RenderTexture::create(_target->getBoundingBox().size.width ,
                                    _target->getBoundingBox().size.height,
                                    Texture2D::PixelFormat::RGBA8888);
    rt->setKeepMatrix(true);
    rt->setVirtualViewport(Vec2(originPos.x, originPos.y),
                           Rect(0, 0, size.width, size.height),
                           Rect(0, 0, pixelSize.width, pixelSize.height));
    rt->begin();
    _target->visit();
    rt->end();
    
    auto ret = Sprite::createWithTexture(rt->getSprite()->getTexture());
    ret->setPosition(Vec2(_target->getBoundingBox().getMidX(), _target->getBoundingBox().getMidY()));
    ret->setFlippedY(true);
    ret->setOpacity(_opacity);
    _target->getParent()->addChild(ret, _target->getLocalZOrder()-1);
    
    ret->runAction(Sequence::create(DelayTime::create(0.3f),
                                    FadeTo::create(0.3f, 0),
                                    RemoveSelf::create(),
                                    NULL));
}