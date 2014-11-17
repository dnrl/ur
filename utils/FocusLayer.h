//
//  FocusLayer.h
//
//  Created by 조 중욱 on 2014. 11. 17..
//
//

#ifndef __FocusLayer__
#define __FocusLayer__

#include <cocos2d.h>

NS_CC_BEGIN

typedef std::function<void(const unsigned int idx)> onTouchCallback;

typedef struct rectData {
    cocos2d::Vec2       _pos;
    cocos2d::Rect       _rect;
    cocos2d::DrawNode*  _node;
    onTouchCallback     _callback;
}rectData;

typedef struct circleData {
    cocos2d::Vec2       _pos;
    float               _radius;
    cocos2d::DrawNode*  _node;
    onTouchCallback     _callback;
}circleData;

class FocusLayer :
public cocos2d::LayerColor
{
public:
	FocusLayer(void);
	virtual ~FocusLayer(void);
	
    virtual bool init();
    
    CREATE_FUNC(FocusLayer);
    
private:
    std::map<unsigned int, rectData>        _rectList;
    std::map<unsigned int, circleData>      _circleList;
    
    unsigned int                        _rectIdx;
    unsigned int                        _circleIdx;
	
public:
    void addRect(const Vec2& pos, const Size& size, onTouchCallback callback);
    void addCircle(const Vec2& pos, float radius, onTouchCallback callback);
    
    void removeRect(const unsigned int idx);
    void removeCircle(const unsigned int idx);
    
    bool onTouchBegan(Touch* t, Event* e);
    void onTouchEnded(Touch* t, Event* e);
	
};

NS_CC_END

#endif