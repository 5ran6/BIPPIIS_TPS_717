package com.greenbit.MultiscanJNIGuiJavaAndroid;

import android.os.Bundle;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;
import android.widget.TextView;

public class LogWindow extends AppCompatActivity
{
	private TextView tvLog;
	public static String LogStr = "";
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_log_window);
		Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
		setSupportActionBar(toolbar);
		/*************************************
		 * frame view
		 */
		tvLog = (TextView) findViewById(R.id.tvLog);
		tvLog.setText(LogStr);
	}

}
