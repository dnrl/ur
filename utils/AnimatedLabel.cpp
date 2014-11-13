//
//  AnimatedLabel.cpp
//  util
//
//  Created by 조중욱 on 2014. 10. 25..
//
//

#include "AnimatedLabel.h"

USING_NS_CC;
using namespace ur::util;

AnimatedLabel::AnimatedLabel(TextHAlignment halignment, TextVAlignment valignment):
Label(nullptr, halignment, valignment),
_onAnimationEnded(nullptr),
_speed(0.025)
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
        ret->setText(text);
        ret->setWidth(size.width);
        ret->setHeight(size.height);
        
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
    scheduleUpdate();
}
void AnimatedLabel::stop()
{
    unscheduleUpdate();
    _isPlaying = false;
}

void AnimatedLabel::initText()
{
    _text = "";
    _tick = _speed;
    _maxWidth = 10;
    _currentPos = 0;
    _isPlaying = false;
}

void AnimatedLabel::setText(const std::string& text)
{
    _text = "";
    _fullText = text;
    _currentPos = 0;
    
    setString(_text);
}

void AnimatedLabel::showAllText()
{
    if(_isPlaying) stop();
    
    setString(_fullText);
}


void AnimatedLabel::clear()
{
    stop();
    _text = "";
    setString(_text);
}

void AnimatedLabel::appendString()
{
    if(_currentPos >= _fullText.length()) {
        stop();
        
        if(_onAnimationEnded)
            _onAnimationEnded();
        
        return;
    }
    
    
    if(_currNumLines * getLineHeight() > _labelHeight) {
        _text = "";
        stop();
        return;
    }
    std::string tmp = _fullText.substr(_currentPos++, 1);
    _text.append(tmp);
    
    log("%s : %d, %f", tmp.c_str(), _currNumLines, getLineHeight());
    setString(_text);
    log("%s : %d", tmp.c_str(), _currNumLines);
}

void AnimatedLabel::update(float dt)
{
    _tick -= dt;
    if(_tick < 0) {
        _tick = _speed;
        appendString();
    }
}


