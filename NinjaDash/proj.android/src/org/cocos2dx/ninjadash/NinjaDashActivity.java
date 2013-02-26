package org.cocos2dx.ninjadash;

import java.io.File;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.app.Activity;
import android.os.Bundle;

public class NinjaDashActivity extends Cocos2dxActivity {
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		if(!isFolderExists("/sdcard/ninjadash"))
		{
			return;
		}
	}
	
	boolean isFolderExists(String strFolder) 
	{        
		File file = new File(strFolder);
		if (!file.exists()) 
		{
			if (file.mkdirs())
			{
				return true;            
			}
			else
			{
				return false;            
			}
		}
		return true;
	}
	
    static {
         System.loadLibrary("ninjadashcpp");
    }
}

