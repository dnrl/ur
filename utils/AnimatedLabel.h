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

NS_CC_BEGIN

namespace ur {
namespace util {
class AnimatedLabel:
public Label
{
public:
    AnimatedLabel(TextHAlignment halignment, TextVAlignment valignment);
    ~AnimatedLabel(void);
    
    static AnimatedLabel* create(const TTFConfig& ttfConfig, const std::string& text, const Size& size, TextHAlignment halignment, TextVAlignment valignment);
    
    void initText();
    
    virtual void update(float dt);
    
    using CallBack = std::function<void(void)>;
    
private:
    std::string                 _text;
    std::string                 _fullText;
    float                       _tick;
    int                         _maxWidth;
    int                         _currentPos;
    bool                        _isPlaying;
    
    double                      _speed;
    
    /* callback */
    CallBack                    _onAnimationEnded;
    
protected:
    void appendString();
    
    
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
    void play(CallBack onAnimationEnded = nullptr);
    
    ///애니메이션 정지
    void stop();
    
    ///현제 애니메이션중인지 검사
    bool isPlaying() const { return _isPlaying; }
};
}
}

NS_CC_END

#endif /* defined(__util__AnimatedLabel__) */
