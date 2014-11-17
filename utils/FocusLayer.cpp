//
//  FocusLayer.cpp
//
//  Created by 조 중욱 on 2014. 11. 17..
//
//

#include "FocusLayer.h"


USING_NS_CC;

FocusLayer::FocusLayer():
_rectIdx(0),
_circleIdx(0)
{
}

FocusLayer::~FocusLayer()
{
}

bool FocusLayer::init()
{
    if (LayerColor::initWithColor(Color4B(0, 0, 0, 128)))
    {
        
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = CC_CALLBACK_2(FocusLayer::onTouchBegan, this);
        listener->onTouchEnded = CC_CALLBACK_2(FocusLayer::onTouchEnded, this);
        
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
        
		
        return true;
    }
    return false;
}

void FocusLayer::addRect(const Vec2& pos, const Size& size, onTouchCallback callback)
{
    Vec2 rc[4];
    rc[0] = Vec2(-size.width/2, -size.height/2);
    rc[1] = Vec2(size.width/2, -size.height/2);
    rc[2] = Vec2(size.width/2, size.height/2);
    rc[3] = Vec2(-size.width/2, size.height/2);
    
    auto rcNode = DrawNode::create();
    rcNode->setPosition(pos);
    rcNode->drawPolygon(rc, 4, Color4F(1, 1, 1, 1), 0, Color4F(1, 1, 1, 1));
    rcNode->setTag(_rectIdx);
    addChild(rcNode);
    
    BlendFunc blend;
    blend.src = GL_DST_COLOR;
    blend.dst = GL_ONE;
    rcNode->setBlendFunc(blend);
    
    rectData data;
    data._pos = pos;
    data._rect = Rect(pos.x - size.width/2, pos.y - size.height/2, size.width, size.height);
    data._callback = callback;
    data._node = rcNode;
    
    _rectList.insert(std::make_pair(_rectIdx++, data));
    
    rcNode->setScale(0);
    rcNode->runAction(EaseBackOut::create(ScaleTo::create(0.4f, 1.0f)));
}

void FocusLayer::addCircle(const Vec2& pos, float radius, onTouchCallback callback)
{
    auto node = DrawNode::create();
    node->setPosition(pos);
    node->drawDot(Vec2::ZERO, radius, Color4F(1, 1, 1, 1));
    node->setTag(_circleIdx);
    addChild(node);
    node->retain();
    
    BlendFunc blend;
    blend.src = GL_DST_COLOR;
    blend.dst = GL_ONE;
    node->setBlendFunc(blend);
    
    circleData data;
    data._pos = pos;
    data._radius = radius;
    data._callback = callback;
    data._node = node;
    
    _circleList.insert(std::make_pair(_circleIdx++, data));
    
    node->setScale(0);
    node->runAction(EaseBackOut::create(ScaleTo::create(0.4f, 1.0f)));
}

void FocusLayer::removeRect(const unsigned int idx)
{
    auto iter = _rectList.find(idx);
    if(iter != _rectList.end()) {
        auto node = iter->second._node;
        _rectList.erase(idx);
        node->runAction(Sequence::create(ScaleTo::create(0.25f, 1.2f),
                                         RemoveSelf::create(),
                                         NULL));
    }
}

void FocusLayer::removeCircle(const unsigned int idx)
{
    auto iter = _circleList.find(idx);
    if(iter != _circleList.end()) {
        auto node = iter->second._node;
        _circleList.erase(idx);
        node->runAction(Sequence::create(ScaleTo::create(0.25f, 1.2f),
                                         RemoveSelf::create(),
                                         NULL));
    }
}

bool FocusLayer::onTouchBegan(Touch* t, Event* e)
{
    auto pt = t->getLocation();
    
    for(auto rc : _rectList) {
        if(rc.second._rect.containsPoint(pt)) {
            log("rect touch");
            if(rc.second._callback)
                rc.second._callback(rc.first);
            
            return false;
        }
    }
    
    for(auto circle : _circleList) {
        Vec2 pos = circle.second._pos;
        float radius = circle.second._radius;
        
        float distance = pos.distance(pt);
        if(distance < radius) {
            log("circle touch");
            if(circle.second._callback)
                circle.second._callback(circle.first);
            
            return false;
        }
    }
    
    return true;
}
void FocusLayer::onTouchEnded(Touch* t, Event* e)
{
    
}