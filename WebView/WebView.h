//
//  WebView.h
//
//  Created by 조 중욱 on 2014. 10. 28..
//
//

#ifndef __utils__WebView__
#define __utils__WebView__

namespace ur { namespace utils {

class WebView;

class WebViewDelegate {
public:
    virtual void callbackFromJS(WebView* webview, const std::string& message){};
    virtual void onDidStartLoad(WebView* webview){};
    virtual void onDidFinishLoad(WebView* webview){};
    virtual void onError(const char* error){};
};

class WebView
{
public:
	WebView(void* obj);
    virtual ~WebView(void) {}
    
    static WebView* create(const cocos2d::Size& size);
    
    void loadUrl(const char* url);
    void evaluateJS(const char* js);
    void setVisibility(bool enable);
    void close();
    
    void setPosition(cocos2d::Vec2 vPos);
    void setContentsSize(const cocos2d::Size& size);
    void setOpacity(GLubyte opacity);
    
    void onRequestCallback(const char* message);
    void onViewLoadStart();
    void onViewLoadFinish();
    void onViewError(const char* error);
    
    void setDelegate(WebViewDelegate* delegate) { _delegate = delegate; }
    
protected:
    void*               _webView;
    WebViewDelegate*    _delegate;
	
};
    
}}


#endif