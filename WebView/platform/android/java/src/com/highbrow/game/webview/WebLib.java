package com.highbrow.game.webview;

import java.util.concurrent.CountDownLatch;
import org.cocos2dx.lib.Cocos2dxActivity;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Context;
import android.os.Handler;
import android.util.Log;
import android.view.Display;
import android.view.Gravity;
import android.view.WindowManager;
import android.view.ViewGroup.LayoutParams;
import android.webkit.WebSettings;
import android.widget.FrameLayout;


@SuppressLint("SetJavaScriptEnabled") 
public class WebLib
{
	private final String TAG ="=== WebLib ===";
	private static Context sContext;
	private CustomWebView _dialog;
	private static int _sizeW, _sizeH = 0;
	
	private final int COCOS_WINSIZE_WIDTH = 1280;
	private final int COCOS_WINSIZE_HEIGHT = 720;
	
	private static int DEVICE_WINSIZE_WIDTH, DEVICE_WINSIZE_HEIGHT = 0;
	
	private long testDelegate = 0;
	
	class WebViewPluginInterface
	{
	    private final long mDelegate;

	    public WebViewPluginInterface(final long delegate)
	    {   
	        mDelegate = delegate;
	    }   

	    public void call(String message)
	    {   
	    	if(message == null){
	    		message = new String("");
	    	}
	    	nativeCalledFromJS(this.mDelegate, message);
	    }   
	}
	
	private native void nativeCalledFromJS(long delegate, String message);
	
	public WebLib( int sizeW, int sizeH )
	{
		sContext = Cocos2dxActivity.getContext();
		_sizeW = sizeW;
		_sizeH = sizeH;
		
		WindowManager wm = (WindowManager)sContext.getSystemService(Context.WINDOW_SERVICE);
		Display disp = wm.getDefaultDisplay();
		DEVICE_WINSIZE_WIDTH = disp.getWidth();
		DEVICE_WINSIZE_HEIGHT = disp.getHeight();
		
	}
	
	public void create()
	{
		final LayoutData layoutData = new LayoutData( changeValueX(_sizeW), changeValueY(_sizeH), 0, 0 );
		
		Activity mainActivity = ( Activity )( Cocos2dxActivity.getContext() );
		
		Log.e(TAG,  "=== showWeb ===" );
		
		final CountDownLatch Signal = new CountDownLatch(1);
		
		mainActivity.runOnUiThread( new Runnable()
		{
			
			@Override
			public void run() 
			{
				_dialog = new CustomWebView( Cocos2dxActivity.getContext() , "", layoutData );
				_dialog.show();
				Signal.countDown();
			}
		});
		
		try 
		{
			Signal.await();	
		}
		catch (InterruptedException e) 
		{
			
			Log.e(TAG,  "=== init === InterruptedException : " + e );
		}
		
//		handler.sendEmptyMessageDelayed( 0, 2000 );
	}

	public void setJavascriptIf(final long delegate)
	{
		testDelegate = delegate;
		Log.e( TAG ,  "======= setJavascriptIf ============" );
		if(sContext != null && _dialog.getWebView() != null)
		{
			Activity a = (Activity)sContext;
			a.runOnUiThread(new Runnable() {public void run() 
			{
				_dialog.getWebView().addJavascriptInterface(new WebViewPluginInterface(delegate), "Cocos2dx");
				WebSettings webSettings = _dialog.getWebView().getSettings();
				webSettings.setJavaScriptEnabled(true);
			}});
		}
	}
	
	public void destroy()
    {
    	if(sContext != null && _dialog.getWebView() != null)
    	{
    		Activity a = (Activity)sContext;
    		a.runOnUiThread(new Runnable() {public void run() 
    		{
    			_dialog.dismiss();
    		}});
    	}
    }
	
	
	public void loadURL(final String url)
    {
    	Log.e( TAG ,  "============ loadURL ============" );
    	if(sContext != null && _dialog.getWebView() != null){
    		final Activity a = (Activity)sContext;
    		a.runOnUiThread(new Runnable() {public void run() {
    			_dialog.getWebView().loadUrl(url);
    		}});
    	}
    }
    public void evaluateJS(final String js)
    {
    	Log.e( TAG ,  "============ evaluateJS ============" );
    	if(sContext != null && _dialog.getWebView() != null){
    		final Activity a = (Activity)sContext;
    		a.runOnUiThread(new Runnable() {public void run() 
    		{
    			_dialog.getWebView().loadUrl("javascript:" + js);
    		}});
    	}
    }
    //public void setMargins(int left, int top, int right, int bottom)
    public void setMargins(final int x, final int y, int width, int height)
    {
    	Log.e( TAG ,  "============ setMargins ============" );
    	if(sContext != null && _dialog.getWebView() != null){
    	// convert coordination from Cocos2dx to Android UI
    		Activity a = (Activity)sContext;
    		WindowManager wm = (WindowManager)a.getSystemService(Context.WINDOW_SERVICE);
    		Display disp = wm.getDefaultDisplay();
    		int left = x;
    		int right = disp.getWidth() - (left + width);
    		int top = disp.getHeight() - (y + height);
    		int bottom = y;
        
    		final FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(
    				LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT,
    				Gravity.NO_GRAVITY);
    		params.setMargins(left, top, right, bottom);
    		_dialog.getWebView().setLayoutParams(params);
    	}
    }
    
    public void setSize( int w, int h )
    {
    	Log.e( TAG ,  "============ setSize ============" );
    	
    	if( _dialog.getWebView() == null )
    	{
    		Log.e( TAG ,  "============ setSize WebView is Null ============" );
    		return;
    	}
    	
    	_sizeW = w;
    	_sizeH = h;
    	
    	FrameLayout.LayoutParams params = ( FrameLayout.LayoutParams ) _dialog.getWebView().getLayoutParams();
    	params.width = changeValueX(w);
    	params.height = changeValueY(h);
    	_dialog.getWebView().setLayoutParams( params );
    	
    }

    public void setPosition( final int posX, final int posY )
    {
    	if( _dialog.getWebView() == null )
    	{
    		return;
    	}
    	
    	FrameLayout.LayoutParams param = (android.widget.FrameLayout.LayoutParams) _dialog.getWebView().getLayoutParams(); 
    	param.setMargins( changeValueX( posX ) , changePosY( posY ), 0, 0 );
    	_dialog.getWebView().setLayoutParams( param );
    }
    
    public void setVisibility(final boolean visibility)
    {
    	
    	if(sContext != null && _dialog.getWebView() != null){
    		Activity a = (Activity)sContext;
    		a.runOnUiThread(new Runnable() {public void run() 
    		{
    			if (visibility) 
    			{
    				_dialog.show();
    			} 
    			else 
    			{
    				_dialog.dismiss();
    			}
    		}});
    	}
    }
    
    private int changePosY( int y )
    {
    	int result = 0;
    	
    	result = DEVICE_WINSIZE_HEIGHT - ( changeValueY(y) + changeValueY(_sizeH) ); 
    	
    	return result;
    }
    
    private int changeValueX( int x )
    {
    	float  value = ( ( float )DEVICE_WINSIZE_WIDTH * ( float )x ) / (float)COCOS_WINSIZE_WIDTH;
    	return ( int )value;
    }
    
    private int changeValueY( int y )
    {
    	float  value = ( ( float )DEVICE_WINSIZE_HEIGHT * ( float )y ) / (float)COCOS_WINSIZE_HEIGHT;
    	return ( int )value;
    }
	
//    Handler handler = new Handler()
//    {
//    	public void handleMessage(android.os.Message msg) 
//    	{
//    		nativeCalledFromJS(testDelegate, "Test CallBack" );
//    	};
//    };
}
