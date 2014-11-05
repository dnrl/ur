package com.highbrow.game.webview;

//import android.util.Log;

public class LayoutData
{
	int sizeW = 0;
	int sizeH = 0;
	int posX = 0;
	int posY = 0;
	
//	private final String TAG ="=== LayoutData ===";
	
	public LayoutData( float data_sizeW, float data_sizeH, float data_posX, float data_posY )
	{
		///Dialog Size & Position struct
		sizeW = ( int ) data_sizeW;
		sizeH = ( int ) data_sizeH;
		
		posX = ( int )data_posX;
		posY = ( int )data_posY;
		
//		Log.e( TAG ,  "LayoutData Setting " + "data_sizeW :" + data_sizeW + " data_sizeH : " + data_sizeH
//				+ " data_posX : " + data_posX + " data_posY : " + data_posY  );
	}
}
