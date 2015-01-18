//
//  ShaderLayer.cpp
//
//  Created by 조중욱 on 2015. 1. 18..
//
//

#include "ShaderLayer.h"

USING_NS_CC;

ShaderLayer::ShaderLayer():
_p(nullptr)
{
}

ShaderLayer::~ShaderLayer()
{
    CCLOGINFO("cocos2d: deallocing ShaderLayer");
}

ShaderLayer* ShaderLayer::create(const std::string& fsh, const std::string& vsh)
{
    auto ret = new (std::nothrow) ShaderLayer();
    if(ret && ret->init(fsh, vsh)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool ShaderLayer::init(const std::string& fsh, const std::string& vsh)
{
    if (!Layer::init()) return false;

    //ccPositionTextureColor_vert
    _p = GLProgram::createWithFilenames(vsh, fsh);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    _rt = RenderTexture::create(visibleSize.width, visibleSize.height);
    addChild(_rt);
    
    _rtSprite = Sprite::create();
    _rtSprite->setTexture(_rt->getSprite()->getTexture());
    _rtSprite->setTextureRect(Rect(0, 0,
                                   _rtSprite->getTexture()->getContentSize().width,
                                   _rtSprite->getTexture()->getContentSize().height));
    _rtSprite->setPosition(Point::ZERO);
    _rtSprite->setAnchorPoint(Point::ZERO);
    _rtSprite->setFlippedY(true);
    addChild(_rtSprite);
    
    _rtSprite->setGLProgram(_p);
    
		
    return true;
}


void ShaderLayer::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags)
{
    _rt->beginWithClear(0, 0, 0, 0);
    for (auto child : getChildren())
    {
        if (child != _rt && child != _rtSprite)
            child->visit(renderer, parentTransform, parentFlags);
    }
    _rt->end();
    
    _rtSprite->visit(renderer, parentTransform, parentFlags);
}



