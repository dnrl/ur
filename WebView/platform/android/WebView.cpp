//
//  WebView.cpp
//  DragonVillage3
//
//  Created by 조 중욱 on 2014. 10. 28..
//
//

#include "../../WebView.h"
#include "jni/WebViewJni.h"

namespace ur { namespace utils {

WebView::WebView(void* obj){
    _webView = obj;
}

WebView* WebView::create(const cocos2d::Size& size){
    jobject obj = createWebViewJni();
    WebView* webview = nullptr;
    if(obj != NULL){
        webview = new WebView((void*)obj);
        setJavascriptIfJni(obj, webview);
    }
    return webview;
}

void WebView::loadUrl(const char* url){
    if(mWebView != NULL){
        loadUrlJni((jobject)mWebView, url);
    }
}

void CCWebView::evaluateJS(const char* js){
    if(mWebView != NULL){
        evaluateJSJni((jobject)mWebView, js);
    }
}

void CCWebView::setVisibility(bool enable){
    if(mWebView != NULL){
        setVisibilityJni((jobject)mWebView, enable);
    }
}

void CCWebView::setRect(int x, int y, int w, int h){
    if(mWebView != NULL){
        setRectJni((jobject)mWebView, x, y, w, h);
    }
}

void CCWebView::destroy(){
    if(mWebView != NULL){
        destroyJni((jobject)mWebView);
    }
}


void CCWebView::handleCalledFromJS(const char *message){
    CCWebViewDelegate *delegate = CCWebView::getWebViewDelegate();
    if(delegate != NULL){
        CCString *str = new CCString(message);
        str->autorelease();
        delegate->callbackFromJS(this, str);
    }
}
    
}}