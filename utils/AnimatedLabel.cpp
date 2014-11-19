//
//  AnimatedLabel.cpp
//  util
//
//  Created by 조중욱 on 2014. 10. 25..
//
//

#include "AnimatedLabel.h"

USING_NS_CC;

namespace ur { namespace utils {

AnimatedLabel::AnimatedLabel(TextHAlignment halignment, TextVAlignment valignment):
Label(nullptr, halignment, valignment),
_onAnimationEnded(nullptr),
_speed(0.025),
_isShowAllText(false)
{
}
AnimatedLabel::~AnimatedLabel()
{
}

AnimatedLabel* AnimatedLabel::create(const TTFConfig& ttfConfig, const std::string& text, const Size& size, TextHAlignment halignment, TextVAlignment valignment)
{
    auto ret = new AnimatedLabel(halignment, valignment);
    
    if (ret && FileUtils::getInstance()->isFileExist(ttfConfig.fontFilePath) && ret->setTTFConfig(ttfConfig))
    {
        ret->autorelease();
        ret->initText();
        ret->setDimensions(size.width, size.height);
        ret->setText(text);
        
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void AnimatedLabel::setAnimationSpeed(double speed)
{
    _speed = speed;
}

void AnimatedLabel::play(CallBack onAnimationEnded)
{
    _onAnimationEnded = onAnimationEnded;
    _isPlaying = true;
    
    for(auto i = 0; i < getStringLength(); ++i) {
        auto label = getLetter(i);
        
        if(label) {
            cocos2d::Vector<FiniteTimeAction*>  actions;
            
            actions.pushBack(DelayTime::create(_speed*i));
            actions.pushBack(Show::create());
            
            if(i == getStringLength()-1 && _onAnimationEnded != nullptr) {
                actions.pushBack(CallFunc::create([=](){
                    _onAnimationEnded(this);
                }));
            }
            
            label->runAction(Sequence::create(actions));
        }
    }
}
void AnimatedLabel::stop()
{
    _isPlaying = false;
    
    for(auto i = 0; i < getStringLength(); ++i) {
        auto label = getLetter(i);
        if(label)
            label->stopAllActions();
    }
}

void AnimatedLabel::initText()
{
    _fullText = "";
    _isPlaying = false;
}

void AnimatedLabel::setText(const std::string& text)
{
//    _fullText = text;

    setString(text);
    
    std::string tmp = _originalUTF8String;
    int i = 0;
    while (!tmp.empty()) {
        auto letter = getLetter(i);
        if(!letter) {
            _textQueue.push(tmp.substr(0, i));
            tmp = tmp.substr(i, tmp.length());
            log("%s", tmp.c_str());
            i = 0;
            removeAllChildren();
            setString(tmp);
        }
        else {
            i++;
        }
    }
    
    _textQueue.push(text);
    
    _isShowAllText = false;
    
    _fullText = _textQueue.front();
    setString(_fullText);
    _textQueue.pop();
    
    for(auto i = 0; i < getStringLength(); ++i) {
        auto label = getLetter(i);
        if(label)
            label->setVisible(false);
    }
}

void AnimatedLabel::showAllText()
{
    if(_isPlaying) stop();
    
    _isShowAllText = true;
    
    for(auto i = 0; i < getStringLength(); ++i) {
        auto label = getLetter(i);
        if(label)
            label->setVisible(true);
    }
    
    if(_onAnimationEnded)
        _onAnimationEnded(this);
}


void AnimatedLabel::clear()
{
    stop();
    _fullText = "";
    setString("");
}

}}

