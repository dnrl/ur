//
//  ScreenLog.cpp
//
//  Created by 조 중욱 on 2014. 9. 23..
//
//

#include "ScreenLog.h"
#include "VisibleRect.h"

USING_NS_CC;
using namespace std;

#define SCREENLOG_NUM_LINES             32
#define SCREENLOG_START_HEIGHT_PERCENT  0.2
#define SCREENLOG_PRINT_BUFFER_SIZE     8192
#define SCREENLOG_LAYER_LEVEL           1000

ScreenLog* g_screenLog = NULL;
char g_screenLogPrintBuffer[SCREENLOG_PRINT_BUFFER_SIZE];

float getTimeMillis() {
    timeval time;
    gettimeofday(&time, NULL);
    unsigned long millisecs = (time.tv_sec * 1000) + (time.tv_usec/1000);
    return (float) millisecs;
}

ScreenLog::ScreenLog()
{
    pthread_mutexattr_t Attr;
    pthread_mutexattr_init(&Attr);
    pthread_mutexattr_settype(&Attr, PTHREAD_MUTEX_RECURSIVE);
    
    pthread_mutex_init(&_contentMutex, &Attr);
    
    _timeout = 5000;//ms
    
    Director::getInstance()->getScheduler()->scheduleUpdate(this, 10000, false);
}

ScreenLog::~ScreenLog()
{
    Director::getInstance()->getScheduler()->unscheduleUpdate(this);
    
    {
        ScopeLock lock(&_contentMutex);
        for (unsigned int i = 0; i < _vecMessages.size(); i++)
            delete _vecMessages[i];
    }
    
    pthread_mutex_destroy(&_contentMutex);
}

void ScreenLog::setFontFile(string file)
{
    float screenHeightPixels = Director::getInstance()->getWinSize().height;
    float fontSize =  screenHeightPixels / (float)SCREENLOG_NUM_LINES - 1;
    
    _ttfConfig.fontFilePath = file;
    _ttfConfig.fontSize = fontSize;
}

void ScreenLog::setTimeoutSeconds(float t)
{
    _timeout = t * 1000;
}

void ScreenLog::attachToScene(cocos2d::Scene* scene)
{
    if ( getParent() )
        getParent()->removeChild(this,false);
    if ( scene )
        scene->addChild(this,SCREENLOG_LAYER_LEVEL);
}

screenLogMessage* ScreenLog::log(ScreenLogMessageType type, const char *p_str, ...)
{
    ScopeLock lock(&_contentMutex);
    
    if (! p_str )
        return NULL;
    
    va_list t_va;
    va_start (t_va, p_str);
    vsnprintf(g_screenLogPrintBuffer, SCREENLOG_PRINT_BUFFER_SIZE - 1, p_str, t_va);
    va_end (t_va);
    
    screenLogMessage *slm = new(std::nothrow) screenLogMessage(this);
    slm->_type = type;
    slm->_text = g_screenLogPrintBuffer;
    slm->_timestamp = getTimeMillis();
    _vecMessages.push_back(slm);
    
    cocos2d::log("%s", g_screenLogPrintBuffer);
    
    return slm;
}

void ScreenLog::setMessageText(screenLogMessage *slm, const char *p_str, ...)
{
    ScopeLock lock(&_contentMutex);
    
    //loop through to find matching message, in case it has already gone
    bool messageStillExists = false;
    for (int i = 0; i < _vecMessages.size(); i++) {
        if ( _vecMessages[i] == slm ) {
            messageStillExists = true;
            break;
        }
    }
    if ( ! messageStillExists )
        return;
    
    va_list t_va;
    va_start (t_va, p_str);
    vsnprintf(g_screenLogPrintBuffer, SCREENLOG_PRINT_BUFFER_SIZE - 1, p_str, t_va);
    va_end (t_va);
    
    slm->setLabelText(g_screenLogPrintBuffer);
    slm->_timestamp = getTimeMillis();
}

void ScreenLog::update(float dt)
{
    ScopeLock lock(&_contentMutex);
    
    for (int i = 0; i < _vecMessages.size(); i++) {
        screenLogMessage* slm = _vecMessages[i];
        if ( slm->checkLabel() )
            moveLabelsUp(i);
    }
    
    float now = getTimeMillis();
    int c = 0;
    for (int i = _vecMessages.size()-1; i >= 0; i--) {
        screenLogMessage *slm = _vecMessages[i];
        if (now - slm->_timestamp > _timeout || c > (2*SCREENLOG_NUM_LINES)) {
            removeChild(slm->_label,true);
            delete slm;
            _vecMessages.erase(_vecMessages.begin() + i );
        }
        c++;
    }
}

void ScreenLog::moveLabelsUp(int maxIndex)
{
    ScopeLock lock(&_contentMutex);
    
    float screenHeightPixels = Director::getInstance()->getWinSize().height;
    float fontSize =  screenHeightPixels / (float)SCREENLOG_NUM_LINES - 1;
    
    if ( maxIndex >= _vecMessages.size() )
        maxIndex = _vecMessages.size();
    
    for (int i = 0; i < maxIndex; i++) {
        screenLogMessage* slm = _vecMessages[i];
        Vec2 vPos = slm->_label->getPosition();
        vPos.y += fontSize;
        slm->_label->setPosition(vPos);
    }
}

void ScreenLog::clearEntries()
{
    ScopeLock lock(&_contentMutex);
    
    for (unsigned int i = 0; i < _vecMessages.size(); i++)
        delete _vecMessages[i];
    _vecMessages.clear();
}

void screenLogMessage::setLabelText(string msg)
{
    // can be called from other threads, delay label creation to main thread to make sure OpenGL works
    ScopeLock lock(&_logLayer->_contentMutex);
    
    _text = msg;
    _isDirty = true;
}

void screenLogMessage::createLabel()
{
    float screenHeightPixels = Director::getInstance()->getWinSize().height;
    
    _label = Label::createWithTTF(_logLayer->_ttfConfig, _text.c_str());
    _label->setAnchorPoint(Vec2(0, 0));
    
    switch (_type) {
        case ScreenLogMessageType::TRACE :
        case ScreenLogMessageType::DEBUG :      _label->setColor(Color3B(255, 255, 255)); break; // white
        case ScreenLogMessageType::INFO :       _label->setColor(Color3B( 32, 255,  32)); break; // green
        case ScreenLogMessageType::WARNING :    _label->setColor(Color3B(255, 127,  32)); break; // orange
        default:                                _label->setColor(Color3B(255,  32,  32)); break; // red
    }
    
    _label->setPosition(Vec2(VisibleRect::left().x + 2, SCREENLOG_START_HEIGHT_PERCENT * screenHeightPixels));
    _logLayer->addChild(_label);
}

bool screenLogMessage::checkLabel()
{
    if (!_label) {
        createLabel();
        _isDirty = false;
        return true;
    }
    
    if (_isDirty) {
        Vec2 originalPos = _label->getPosition();
        _logLayer->removeChild(_label,true);
        _label = NULL;
        createLabel();
        _label->setPosition(originalPos);
        _isDirty = false;
    }
    
    return false;
}

#pragma mark - ScopeLog
ScopeLog::ScopeLog(std::string fn):
_functionName(fn)
{
    g_screenLog->log(ScreenLogMessageType::TRACE, "Entered %s", _functionName.c_str());
}

ScopeLog::~ScopeLog()
{
    g_screenLog->log(ScreenLogMessageType::TRACE, "Exiting %s", _functionName.c_str());
}

