//
//  ScreenLog.h
//
//  Created by 조 중욱 on 2014. 9. 23..
//
//

#ifndef __ScreenLog__
#define __ScreenLog__

#include <cocos2d.h>
#include <string>
#include <vector>

enum class ScreenLogMessageType {
    FATAL,
    ERROR,
    WARNING,
    INFO,
    DEBUG,
    TRACE,
};

class screenLogMessage {
    friend class ScreenLog;
private:
    class ScreenLog*        _logLayer;
    ScreenLogMessageType    _type;
    std::string             _text;
    float                   _timestamp;
    cocos2d::Label*         _label;
    bool                    _isDirty;
    
private:
    screenLogMessage(ScreenLog* logLayer) :
    _label(nullptr),
    _logLayer(logLayer),
    _timestamp(0),
    _isDirty(true){}
    virtual ~screenLogMessage() {}
    
    void setLabelText(std::string msg);
    void createLabel();
    bool checkLabel();
};

class ScreenLog : public cocos2d::Layer
{
public:
	ScreenLog(void);
	virtual ~ScreenLog(void);
	
private:
    friend class screenLogMessage;
    
    cocos2d::TTFConfig                  _ttfConfig;
    float                               _timeout;
    std::vector<screenLogMessage*>      _vecMessages;
    pthread_mutex_t                     _contentMutex;
    
public:
    void setFontFile(std::string file);
    void setTimeoutSeconds(float seconds);
    void attachToScene(cocos2d::Scene* scene);
    
    screenLogMessage* log(ScreenLogMessageType type, const char* msg, ...);
    void setMessageText(screenLogMessage* slm, const char *p_str, ...);
    
    void update(float dt);
    void moveLabelsUp(int maxIndex);
    void clearEntries();
	
};

extern ScreenLog*   g_screenLog;

class ScopeLock {
public:
    ScopeLock(pthread_mutex_t* m) : _mutex(m) {
        pthread_mutex_lock(_mutex);
    }
    ~ScopeLock() {
        pthread_mutex_unlock(_mutex);
    }
    
    pthread_mutex_t*        _mutex;
};

class ScopeLog {
public:
    ScopeLog(std::string fn);
    ~ScopeLog();
    
    std::string     _functionName;
};

#endif