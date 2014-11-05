package com.highbrow.game.webview;

import android.annotation.SuppressLint;
import android.app.Dialog;
import android.content.Context;
import android.os.Bundle;
import android.util.Log;
//import android.util.Log;
import android.view.Display;
import android.view.Gravity;
import android.view.ViewGroup.LayoutParams;
import android.view.WindowManager;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.FrameLayout;

public class CustomWebView extends Dialog
{
	private WebView _weview;
	private LayoutData _layoutData = null;
	private Context _context = null;
	private String _url = null;
	private final String TAG = "=== CustomWebView ===";
	
	public CustomWebView(Context context) 
	{
		super( context, android.R.style.Theme_Translucent_NoTitleBar_Fullscreen );
	}
	
	public CustomWebView( Context context, String url,LayoutData layoutData )
	{
		super( context, android.R.style.Theme_Translucent_NoTitleBar_Fullscreen );
		_layoutData = layoutData;
		_context = context;
		_url = url;
	}
	
	@SuppressLint("SetJavaScriptEnabled") 
	protected void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);
		/// android layout position 값 사용을 위해서 FrameLayout을 BassLayout으로 사
		FrameLayout frameLayout = new FrameLayout( _context );
		frameLayout.setLayoutParams( new LayoutParams( LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT ) );
		setContentView( frameLayout );
		_weview = new WebView( _context );
		
		/* 스크립트 활성화 */
		_weview.getSettings().setJavaScriptEnabled( true );
		_weview.setFocusable(true);
		_weview.setFocusableInTouchMode(true);
		
//		_weview.setWebViewClient( new WebViewClient()
//		{
//			@Override
//			public boolean shouldOverrideUrlLoading(WebView view, String url) 
//			{
//				view.loadUrl(url);
//				return true;
//			};
//		});
		
		_weview.setWebChromeClient(new WebChromeClient());
		_weview.setWebViewClient(new WebViewClient());
		//mWebView.addJavascriptInterface(new WebViewPluginInterface(delegate), "Cocos2dx");
		WebSettings webSettings = _weview.getSettings();
		webSettings.setSupportZoom(false);
		//webSettings.setJavaScriptEnabled(true);
		webSettings.setPluginsEnabled(true);
		
		_weview.loadUrl( _url );
		FrameLayout.LayoutParams params = new FrameLayout.LayoutParams( _layoutData.sizeW , _layoutData.sizeH );
		params.setMargins( _layoutData.posX , getChangePositionY(), 0, 0 );
//		params.setMargins(left, top, right, bottom)
		_weview.setLayoutParams( params );
		addContentView( _weview, params);
		
		getWindow().setGravity( Gravity.BOTTOM );
		
		Log.e( TAG ,  "============= Custom WebView ============== init" );
		
	}
	
	private int getChangePositionY()
	{
		int result = 0;
		
		WindowManager wm = ( WindowManager )_context.getSystemService( Context.WINDOW_SERVICE );
		Display display = wm.getDefaultDisplay();
		
		result = display.getHeight() - ( _layoutData.posY + _layoutData.sizeH );
		
//		Log.e( TAG ,  "Win Size : " + display.getWidth() + " : " + display.getHeight() );
//		Log.e( TAG ,  "Reuslt : " + result );
		
		return result;
	}
	
	protected WebView getWebView()
	{
		return _weview;
	}
}
