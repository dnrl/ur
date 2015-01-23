//
//  Stroke.cpp
//  DragonVillage3
//
//  Created by 조 중욱 on 2015. 1. 21..
//
//

#include "Stroke.h"

USING_NS_CC;

void Stroke::outline(Sprite* sprite, const Color3B& color, float size)
{
    float width = sprite->getContentSize().width;
    float height = sprite->getContentSize().height;
    
    if(width == 0 || height == 0) return;
    
    //앵커
    Vec2 anchor = sprite->getAnchorPoint();
    
    float factor = CC_CONTENT_SCALE_FACTOR();
    
    //중심점 찾기
    Vec2 center = Vec2(width/2 + size, height/2 + size);
    
    
    //랜더 텍스쳐 생성
    auto rt = RenderTexture::create(width + (size * 2), height + (size * 2));
    rt->setPosition(center);
    //기존 스프라이트의 정보 저장
    Color3B originColor = sprite->getColor();
    Vec2 originPos = sprite->getPosition();
    BlendFunc originBlend = sprite->getBlendFunc();
    
    sprite->setBlendFunc({GL_SRC_ALPHA, GL_ONE});
    
    int drawNum = 16;
    
    rt->setClearFlags(GL_COLOR_BUFFER_BIT);
    rt->setAutoDraw(false);
    //그리기 시작
    rt->begin();
    
    sprite->setColor(color);
    
    float radian = 0.0f;
    for(auto i = 0; i < drawNum; ++i) {
        sprite->setPosition(center + Vec2(cosf(radian) * size, sinf(radian) * size));
        sprite->visit();
        radian += CC_DEGREES_TO_RADIANS(360.0f/drawNum);
    }
    
    sprite->setBlendFunc(originBlend);
    sprite->setPosition(center);
    sprite->setColor(originColor);
    sprite->visit();
    
    //그리기 종료
    rt->end();
    
    Director::getInstance()->getRenderer()->render();
    
    sprite->setPosition(originPos);
    
//    auto texture = rt->getSprite()->getTexture();
//    texture->setAntiAliasTexParameters();
    
//    sprite->setTexture(texture);
    
    sprite->addChild(rt);
    
}

Node* Stroke::outline2(Sprite* sprite, const Color3B& color, float size)
{
    SpriteBatchNode* batch = SpriteBatchNode::createWithTexture(sprite->getTexture());
    
    Texture2D* tex = batch->getTexture();
    
    auto node = Node::create();
    node->setPosition(sprite->getContentSize()/2);
    int drawNum = 16;
    float radian = 0.0f;
    for(auto i = 0; i < drawNum; ++i) {
        auto sp = Sprite::createWithTexture(tex);
        sp->setPosition(Vec2(Vec2(cosf(radian) * size, sinf(radian) * size)));
        sp->setColor(color);
        sp->setBlendFunc({GL_SRC_ALPHA, GL_ONE});
        radian += CC_DEGREES_TO_RADIANS(360.0f/drawNum);
        node->addChild(sp);
    }
    
    auto sp = Sprite::createWithTexture(tex);
    node->addChild(sp);
    
    return node;
}


void Stroke::outline3(Sprite* sprite, const Color3B& color, float size)
{
    auto fileUtiles = FileUtils::getInstance();
    auto fragmentFullPath = fileUtiles->fullPathForFilename("Shader/outline.fsh");
    auto fragSource = fileUtiles->getStringFromFile(fragmentFullPath);
    auto glprogram = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, fragSource.c_str());
    
    
    
    Vec3 colors(color.r/255, color.g/255, color.b/255);
    GLfloat radius = size;
    GLfloat threshold = 1.75;
    
    auto glstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
    glstate->setUniformVec3("u_outlineColor", colors);
    glstate->setUniformFloat("u_radius", radius);
    glstate->setUniformFloat("u_threshold", threshold);
    
    sprite->setGLProgramState(glstate);
//    auto gp = sprite->getGLProgram();
//    gp->link();
//    gp->updateUniforms();
}









