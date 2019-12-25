package com.greenbit.MultiscanJNIGuiJavaAndroid;

import android.graphics.Bitmap;
import android.os.Bundle;
import android.os.Handler;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;
import android.util.DisplayMetrics;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.Spinner;

import com.greenbit.usbPermission.IGreenbitLogger;

import java.util.ArrayList;

public class WsqWindow extends AppCompatActivity implements IGreenbitLogger{
    private Button bRefresh;
    private Spinner comboFilesList;
    private ArrayList<String> FilesList;
    private ImageView FrameView;
    private boolean LoggerBitmapChanged;
    private ArrayList<GbExampleGrayScaleBitmapClass> LoggerBitmapList;

    private void LogBitmap(GbExampleGrayScaleBitmapClass bmp)
    {
        LoggerBitmapList.add(bmp);
        LoggerBitmapChanged = true;
    }

    private void InitBmpLog()
    {
        LoggerBitmapList = new ArrayList<GbExampleGrayScaleBitmapClass>();
        LoggerBitmapChanged = false;
    }

    protected void LogTimer()
    {
        if (LoggerBitmapChanged)
        {
            try
            {
                DisplayMetrics metrics = new DisplayMetrics();
                getWindowManager().getDefaultDisplay().getMetrics(metrics);
                for (GbExampleGrayScaleBitmapClass GbBmp : LoggerBitmapList)
                {
                    Bitmap bmp = GbBmp.GetBmp();
                    if (bmp != null)
                    {

                        float scaleWidth = metrics.scaledDensity / 8;

                        //set image in imageView
                        FrameView.setImageBitmap(bmp);

                        //set imageView dynamic width and height
                        FrameView.setMaxWidth((int) scaleWidth);
                        FrameView.setMaxHeight((int) scaleWidth);
                        FrameView.setMinimumWidth((int) scaleWidth);
                        FrameView.setMinimumHeight((int) scaleWidth);
                    } else
                    {
                        LogAsDialog("LogTimer: null bmp");
                    }
                }
            } catch (Exception ex)
            {
                LogAsDialog("LogBmp exc: " + ex.getMessage());
            }
            LoggerBitmapList.clear();
            LoggerBitmapChanged = false;
        }
        new Handler().postDelayed(new Runnable()
        {
            @Override
            public void run()
            {
                LogTimer();
            }
        }, 10);
    }

    protected void StartLogTimer()
    {
        new Handler().postDelayed(new Runnable()
        {
            @Override
            public void run()
            {
                LogTimer();
            }
        }, 10);
    }

    @Override
    public void LogOnScreen(String strToLog)
    {
        //GB_AcquisitionOptionsGlobals.CreateDialogNeutral(strToLog, this);
    }

    public void LogAsDialog(String logStr)
    {
        GB_AcquisitionOptionsGlobals.CreateDialogNeutral(logStr, this);
    }

    private IGreenbitLogger GetThis() {return this;}

    private void onRefresh()
    {
        FilesList.clear();
        FilesList = GbExampleGrayScaleBitmapClass.GbBmpLoadListOfWsqImages(false);

        ArrayAdapter<String> FilesListAdapter = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item, FilesList);
        FilesListAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        comboFilesList.setAdapter(FilesListAdapter);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_wsq_window);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        FilesList = new ArrayList<String>();
        InitBmpLog();

        bRefresh = (Button) findViewById(R.id.bWsqRefreshList);
        bRefresh.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View view)
            {
                onRefresh();
            }
        });
        bRefresh.setText("Refresh List");
        FrameView = (ImageView) findViewById(R.id.WsqFrameView);

        comboFilesList = (Spinner) findViewById(R.id.comboWsqFilesToVisualize);
        comboFilesList.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener()
        {

            @Override
            public void onItemSelected(AdapterView<?> arg0, View arg1,
                                       int arg2, long arg3) {
                String fname = arg0.getItemAtPosition(arg2).toString();
                GbExampleGrayScaleBitmapClass GbBmp = new GbExampleGrayScaleBitmapClass();
                try
                {
                    boolean ret = GbBmp.GbBmpFromWsqFile(fname, false, false, GetThis());
                    if (!ret)
                    {
                        throw new Exception("GbBmpFromWsqFile error");
                    }
                    else
                    {
                        LogBitmap(GbBmp);
                    }
                }
                catch (Exception ex)
                {
                    LogAsDialog("onItemSelected: " + ex.getMessage());
                }
            }

            @Override
            public void onNothingSelected(AdapterView<?> arg0)
            { }
        });
        onRefresh();
        StartLogTimer();
    }

}
