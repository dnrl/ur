//
//  Java_org_cocos2dx_lib_Cocos2dxWebView.h
//
//  Created by 조 중욱 on 2014. 10. 28..
//
//

#ifndef __Java_org_cocos2dx_lib_Cocos2dxWebView_H__
#define __Java_org_cocos2dx_lib_Cocos2dxWebView_H__

#include "cocos2d.h"
#include <jni.h>

extern "C"
{
    jobject createWebViewJni();
    void setJavascriptIfJni(jobject obj, void *delegate);
    void loadUrlJni(jobject obj, const char* url);
    void evaluateJSJni(jobject obj, const char* js);
    void setVisibilityJni(jobject obj, bool enable);
    void setPositionJni(jobject obj, int x, int y, int w, int h);
    void closeJni(jobject obj);
}
#endif
