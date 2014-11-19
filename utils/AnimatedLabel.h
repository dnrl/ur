//
//  AnimatedLabel.h
//  util
//
//  Created by 조중욱 on 2014. 10. 25..
//
//

#ifndef __util__AnimatedLabel__
#define __util__AnimatedLabel__

#include <cocos2d.h>

namespace ur { namespace utils {
class AnimatedLabel:
public cocos2d::Label
{
public:
    AnimatedLabel(cocos2d::TextHAlignment halignment, cocos2d::TextVAlignment valignment);
    ~AnimatedLabel(void);
    
    static AnimatedLabel* create(const cocos2d::TTFConfig& ttfConfig, const std::string& text, const cocos2d::Size& size, cocos2d::TextHAlignment halignment, cocos2d::TextVAlignment valignment);
    
    void initText();
    
    using CallBack = std::function<void(void*)>;
    
private:
    std::string                 _fullText;
    std::queue<std::string>     _textQueue;
    bool                        _isPlaying;
    bool                        _isShowAllText;
    
    double                      _speed;
    
    /* callback */
    CallBack                    _onAnimationEnded;
    
public:
    ///자간 속도
    void setAnimationSpeed(double speed);
    
    ///문자열 추가
    void setText(const std::string& text);
    
    ///전체 글 한번에 보기
    void showAllText();
    
    ///화면에 출력된 문자열 지우기
    void clear();
    
    ///애니메이션 실행
    void play(CallBack onAnimationEnded);
    
    ///애니메이션 정지
    void stop();
    
    ///현제 애니메이션중인지 검사
    bool isPlaying() const { return _isPlaying; }
    bool isShowAllText() const { return _isShowAllText; }
};
}}

#endif /* defined(__util__AnimatedLabel__) */
