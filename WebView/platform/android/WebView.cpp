//
//  WebView.cpp
//  DragonVillage3
//
//  Created by 조 중욱 on 2014. 10. 28..
//
//
#include <cocos2d.h>
#include "../../WebView.h"
#include "jni/WebViewJni.h"

namespace ur { namespace utils
{

#if ( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )

WebView::WebView(void* obj)
{
    _webView = obj;
}


WebView* WebView::create(const cocos2d::Size& size)
{
    jobject obj = createWebViewJni( size.width, size.height );
//  jobject obj = createWebViewJni();
    createWeb( obj );
    
    WebView* webview = nullptr;
    if(obj != NULL)
    {
        webview = new WebView((void*)obj);
        setJavascriptIfJni(obj, webview);
    }
    return webview;
}

void WebView::loadUrl(const char* url)
{
    if(_webView != NULL)
    {
        loadUrlJni((jobject)_webView, url);
    }
}

void WebView::evaluateJS(const char* js)
{
    if(_webView != NULL)
    {
        evaluateJSJni((jobject)_webView, js);
    }
}

void WebView::setVisibility(bool enable)
{
    if(_webView != NULL)
    {
        setVisibilityJni((jobject)_webView, enable);
    }
}

void WebView::setContentsSize(const cocos2d::Size &size)
{
    if(_webView != NULL)
    {
        setRectJni((jobject)_webView, 0 , 0, size.width, size.height );
    }
}

void WebView::close()
{
    if(_webView != NULL)
    {
        closeJni((jobject)_webView);
    }
}
    
void WebView::setPosition(cocos2d::Vec2 vPos)
{
    if(_webView != NULL)
    {
        setPositionJni( (jobject)_webView,( int )vPos.x, ( int )vPos.y );
    }
}


void WebView::onRequestCallback(const char *message)
{
    WebViewDelegate *delegate = _delegate;
    if(_delegate != NULL)
    {
        std::string str = message;
        _delegate->callbackFromJS(this, str);
    }
}
    
void WebView::onViewLoadStart()
{
        
}
void WebView::onViewLoadFinish()
{
        
}
void WebView::onViewError(const char* error)
{
        
}
#endif
}}