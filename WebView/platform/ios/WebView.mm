//
//  WebView.mm
//
//  Created by 조 중욱 on 2014. 10. 28..
//
//

#include <cocos2d.h>
#import "../../WebView.h"
#import "platform/ios/CCEAGLView-ios.h"

@interface ViewDelegate : NSObject<UIWebViewDelegate>
{
    void *object;
}
@end

@implementation ViewDelegate
-(id)initWithDelegate:(void *)delegate
{
    self = [super init];
    object = delegate;
    return self;
}

-(BOOL)webView:(UIWebView*)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType
{
    NSString *url = [[request URL] absoluteString];
    ///cocos2dx: 로시작하는지 검사
    if([url hasPrefix:@"cocos2dx:"]){
        ur::utils::WebView *pDelegate = (ur::utils::WebView*)object;
        if(pDelegate != NULL){
            pDelegate->onRequestCallback([url cStringUsingEncoding:NSASCIIStringEncoding]);
        }
        return NO;
    }else{
        return YES;
    }
}

- (void)webViewDidStartLoad:(UIWebView *)webView
{
    ur::utils::WebView *pDelegate = (ur::utils::WebView*)object;
    if(pDelegate != NULL){
        pDelegate->onViewLoadStart();
    }
}

- (void)webViewDidFinishLoad:(UIWebView *)webView
{
    ur::utils::WebView *pDelegate = (ur::utils::WebView*)object;
    if(pDelegate != NULL){
        pDelegate->onViewLoadFinish();
    }
}

- (void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error
{
    ur::utils::WebView *pDelegate = (ur::utils::WebView*)object;
    if(pDelegate != NULL){
        pDelegate->onViewError([error.description cStringUsingEncoding:NSASCIIStringEncoding]);
    }
}

@end

namespace ur { namespace utils {

WebView::WebView(void *obj)
{
    _webView = obj;
    _delegate = nullptr;
}

WebView* WebView::create(const cocos2d::Size& size)
{
    WebView *webview = nullptr;
    
    cocos2d::Director* director = cocos2d::Director::getInstance();
    CCEAGLView *view = (CCEAGLView*)director->getOpenGLView()->getEAGLView();
    
    float scale = view.contentScaleFactor;
    
    GLfloat width = MIN(size.width/scale, view.frame.size.width);
    GLfloat height = MIN(size.height/scale, view.frame.size.height);
    
    UIWebView *uiView = [[UIWebView alloc] init];
    //lanndscape 검사
    UIInterfaceOrientation orientation = [[UIApplication sharedApplication] statusBarOrientation];
    if(UIInterfaceOrientationIsPortrait(orientation)){
        uiView.frame = CGRectMake(0, 0, height, width);
    }else{
        uiView.frame = CGRectMake(0, 0, width, height);
    }
    uiView.hidden = YES;
    [view addSubview:uiView];
    
    webview = new (std::nothrow) WebView((void*)uiView);
    uiView.delegate = (id<UIWebViewDelegate>)[[ViewDelegate alloc] initWithDelegate:(void *)webview];
    return webview;
}
    
void WebView::setPosition(cocos2d::Vec2 vPos)
{
    cocos2d::Director* director = cocos2d::Director::getInstance();
    CCEAGLView *view = (CCEAGLView*)director->getOpenGLView()->getEAGLView();
    
    float scale = view.contentScaleFactor;
    
    UIWebView *uiView = (UIWebView*)_webView;
    
    uiView.frame = CGRectMake(vPos.x/scale, view.frame.size.height - vPos.y/scale - uiView.frame.size.height,
                              uiView.frame.size.width, uiView.frame.size.height);
}
    
void WebView::setContentsSize(const cocos2d::Size& size)
{
    cocos2d::Director* director = cocos2d::Director::getInstance();
    CCEAGLView *view = (CCEAGLView*)director->getOpenGLView()->getEAGLView();
    
    float scale = view.contentScaleFactor;
    
    UIWebView *uiView = (UIWebView*)_webView;
    
    GLfloat width = MIN(size.width/scale, view.frame.size.width);
    GLfloat height = MIN(size.height/scale, view.frame.size.height);
    
    uiView.frame = CGRectMake(uiView.frame.origin.x, uiView.frame.origin.y, width, height);
}
    
void WebView::setOpacity(GLubyte opacity)
{
    UIWebView *uiView = (UIWebView*)_webView;
    uiView.alpha = opacity/255.f;
}

void WebView::loadUrl(const char *url)
{
    NSString *nsStr = NSLocalizedString([[NSString alloc] initWithUTF8String:url], @"Url");
    NSURL *nsUrl = [NSURL URLWithString:nsStr];
    NSURLRequest *request = [NSURLRequest requestWithURL:nsUrl];
    UIWebView *uiView = (UIWebView*)_webView;
    uiView.hidden = NO;
    [uiView loadRequest:request];
}

void WebView::setVisibility(bool enable)
{
    UIWebView *uiView = (UIWebView*)_webView;
    uiView.hidden = enable ? NO : YES;
}

void WebView::evaluateJS(const char* js)
{
    UIWebView *uiView = (UIWebView*)_webView;
    NSString *jsStr = NSLocalizedString([[NSString alloc] initWithUTF8String:js], @"JS");
    [uiView stringByEvaluatingJavaScriptFromString:jsStr];
}

void WebView::close()
{
    UIWebView *uiView = (UIWebView*)_webView;
    ViewDelegate *delegate = uiView.delegate;
    [delegate release];
    [uiView removeFromSuperview];
    [uiView release];
}

void WebView::onRequestCallback(const char *message)
{
    if(_delegate != nullptr){
        _delegate->callbackFromJS(this, message);
    }
}
    
void WebView::onViewLoadStart()
{
    if(_delegate != nullptr){
        _delegate->onDidStartLoad(this);
    }
}
void WebView::onViewLoadFinish()
{
    if(_delegate != nullptr){
        _delegate->onDidFinishLoad(this);
    }
}
void WebView::onViewError(const char* error)
{
    if(_delegate != nullptr){
        _delegate->onError(error);
    }
}

}}

