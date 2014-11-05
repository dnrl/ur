//
//  Java_org_cocos2dx_lib_Cocos2dxWebView.cpp
//  DragonVillage3
//
//  Created by 조 중욱 on 2014. 10. 28..
//
//

//#include "jni/JniHelper.h"
//#include <string.h>
//#include <android/log.h>
//
//#include "Director.h"

#include "WebViewJni.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include "../../../WebView.h"
#define LOG_TAG "WebViewJni"
//

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define JAVAVM    cocos2d::JniHelper::getJavaVM()

//*

using namespace std;
using namespace cocos2d;
using namespace ur::utils;

extern "C" {
    static bool getEnv(JNIEnv **env)
    {
        bool bRet = false;
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
        
        do
        {
            if (JAVAVM->GetEnv((void**)env, JNI_VERSION_1_4) != JNI_OK)
            {
                LOGD("Failed to get the environment using GetEnv()");
                break;
            }
            
            if (JAVAVM->AttachCurrentThread(env, 0) < 0)
            {
                LOGD("Failed to get the environment using AttachCurrentThread()");
                break;
            }
            
            bRet = true;
        } while (0);
#endif
        
        return bRet;
    }
    
    bool getInstanceMethodInfo(JniMethodInfo &methodinfo, jobject &obj, const char *methodName, const char *paramCode)
    {
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
        jmethodID methodID = 0;
        JNIEnv *pEnv = 0;
        bool bRet = false;
        do{
            if(!getEnv(&pEnv)){
                break;
            }
            jclass classID = pEnv->GetObjectClass(obj);
            methodID = pEnv->GetMethodID(classID, methodName, paramCode);
            if(methodID == NULL){
                log("Failed to get method id of %s", methodName);
                break;
            }
            methodinfo.classID = classID;
            methodinfo.env = pEnv;
            methodinfo.methodID = methodID;
            
            bRet = true;
        }while(0);
        return bRet;
#else
        return true;
#endif
    }
    
//    static jobject sContext = NULL;
    
    jobject createWebViewJni( int sizeW, int sizeH )
//    jobject createWebViewJni()
    {
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
        JniMethodInfo t;
        jobject ret = NULL;
        if(JniHelper::getMethodInfo(t, "com/highbrow/game/webview/WebLib", "<init>", "(II)V"))
        {
            ret = t.env->NewObject(t.classID, t.methodID, sizeW, sizeH );
            t.env->DeleteLocalRef(t.classID);
        }
        return ret;
#else
        return nullptr;
#endif
    }
    
    void setJavascriptIfJni(jobject obj, void *delegate)
    {
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
        JniMethodInfo t;
        if(getInstanceMethodInfo(t, obj, "setJavascriptIf", "(J)V"))
        {
            t.env->CallVoidMethod(obj, t.methodID, delegate);
            t.env->DeleteLocalRef(t.classID);
        }
#endif
    }
    
    void loadUrlJni(jobject obj, const char *url)
    {
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
        JniMethodInfo t;
        if(getInstanceMethodInfo(t, obj, "loadURL", "(Ljava/lang/String;)V")){
            jstring jUrl;
            if(!url){
                jUrl = t.env->NewStringUTF("");
            }else{
                jUrl = t.env->NewStringUTF(url);
            }
            t.env->CallVoidMethod(obj, t.methodID, jUrl);
            t.env->DeleteLocalRef(jUrl);
            t.env->DeleteLocalRef(t.classID);
        }
#endif
    }
    
    void evaluateJSJni(jobject obj, const char *js)
    {
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
        JniMethodInfo t;
        if(getInstanceMethodInfo(t, obj, "evaluateJS", "(Ljava/lang/String;)V")){
            jstring jJS;
            if(!js){
                jJS = t.env->NewStringUTF("");
            }else{
                jJS = t.env->NewStringUTF(js);
            }
            t.env->CallVoidMethod(obj, t.methodID, jJS);
            t.env->DeleteLocalRef(jJS);
            t.env->DeleteLocalRef(t.classID);
        }
#endif
    }
    
    void setVisibilityJni(jobject obj, bool enable)
    {
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
        JniMethodInfo t;
        if(getInstanceMethodInfo(t, obj, "setVisibility", "(Z)V")){
            t.env->CallVoidMethod(obj, t.methodID, enable);
            t.env->DeleteLocalRef(t.classID);
        }
#endif
    }
    
    void setRectJni(jobject obj, int x, int y, int w, int h)
    {
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
        JniMethodInfo t;
        if(getInstanceMethodInfo(t, obj, "setMargins", "(IIII)V")){
            t.env->CallVoidMethod(obj, t.methodID, x, y, w, h);
            t.env->DeleteLocalRef(t.classID);
        }
#endif
    }
    
    void setSize( jobject obj, int w, int h )
    {
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
        JniMethodInfo t;
        if(getInstanceMethodInfo(t, obj, "setSize", "(II)V"))
        {
            t.env->CallVoidMethod(obj, t.methodID, w, h );
            t.env->DeleteLocalRef(t.classID);
        }
#endif
    }

    void createWeb( jobject obj )
    {
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
        JniMethodInfo t;
        if(getInstanceMethodInfo(t, obj, "create", "()V"))
        {
            t.env->CallVoidMethod(obj, t.methodID );
            t.env->DeleteLocalRef(t.classID);
        }
#endif
    }

    void setPositionJni( jobject obj, int x, int y )
    {
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
        JniMethodInfo t;
        if(getInstanceMethodInfo(t, obj, "setPosition", "(II)V"))
        {
            t.env->CallVoidMethod(obj, t.methodID, x, y );
            t.env->DeleteLocalRef(t.classID);
        }
#endif
    }

    void closeJni(jobject obj)
    {
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
        JniMethodInfo t;
        if(getInstanceMethodInfo(t, obj, "destroy", "()V")){
            t.env->CallVoidMethod(obj, t.methodID);
            t.env->DeleteLocalRef(t.classID);
        }
#endif
    }
    
//#if ( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
    // from Cocos2dxWebView
//    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_gree_webview_Cocos2dxWebView_nativeCalledFromJS(JNIEnv *env, jobject obj, jlong delegate, jstring message)
    JNIEXPORT void JNICALL Java_com_highbrow_game_webview_WebLib_nativeCalledFromJS(JNIEnv *env, jobject obj, jlong delegate, jstring message)
    {
        if(delegate)
        {
            const char* str = env->GetStringUTFChars(message, 0); 
            WebView *webView = (WebView*)delegate;
            webView->onRequestCallback( str );
            env->ReleaseStringUTFChars(message, str);
        }
    }
//#endif
    
}

/*/
void showWeb( int sizeW, int sizeH, int posX, int posY, const char* url )
{
//        size.width size.height pos.x pos.y
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
        JniMethodInfo t;
        
        if( JniHelper::getMethodInfo( t, "com/highbrow/game/webview/WebLib", "showWeb", "(FFFFLjava/lang/String;)V" ) )
        {
            jstring jurl = t.env->NewStringUTF( url );
    
            t.env->CallVoidMethod( t.classID, t.methodID, sizeW, sizeH, posX, posY, jurl );
            t.env->DeleteLocalRef( t.classID );
            t.env->DeleteLocalRef( jurl );
            
        }
#endif
}

//*/