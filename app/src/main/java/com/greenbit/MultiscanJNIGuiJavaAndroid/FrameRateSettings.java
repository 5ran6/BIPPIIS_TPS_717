package com.greenbit.MultiscanJNIGuiJavaAndroid;

import android.os.Bundle;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;
import android.view.View;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.TextView;

import com.greenbit.utils.GBJavaWrapperUtilDoubleForJavaToCExchange;
import com.greenbit.gbmsapi.GBMSAPIJavaWrapperDefinesFrameRateOptions;
import com.greenbit.gbmsapi.GBMSAPIJavaWrapperDefinesReturnCodes;

public class FrameRateSettings extends AppCompatActivity {
    private TextView tbLogger;
    private Button bSetFrameRate;
    private Button bSetDefaultPreviewFr;
    private TextView tbPreviewFrameRate, tbPreviewFrameRateMin, tbPreviewFrameRateMax;
    private Double PreviewMinFr,PreviewMaxFr,PreviewDefaultFr;
    private SeekBar scrollBarPreview;
    private Button bSetDefaultAcqFr;
    private TextView tbAcqFrameRate, tbAcqFrameRateMin, tbAcqFrameRateMax;
    private SeekBar scrollBarAcq;
    private Double AcquisitionMinFr,AcquisitionMaxFr,AcquisitionDefaultFr;

    private void LogFrameRateMessages(String logStr)
    {
        tbLogger.setText(logStr);
    }

    private void SetFrameRateClick()
    {
        int RetVal;
        String checkGbmsapi;
        /*************************
         * preview
         */
        RetVal = GB_AcquisitionOptionsGlobals.GBMSAPI_Jw.SetFrameRate(
                GB_AcquisitionOptionsGlobals.ScanArea,0,
                CalculateFrameRateFromProgress(PreviewMinFr, PreviewMaxFr, scrollBarPreview.getProgress())
        );
        if (RetVal != GBMSAPIJavaWrapperDefinesReturnCodes.GBMSAPI_ERROR_CODE_NO_ERROR) {
            checkGbmsapi = "Preview " + GB_AcquisitionOptionsGlobals.GBMSAPI_Jw.GetLastErrorString();
            this.setTitle(checkGbmsapi);
            EnableWidgets(false);
            return;
        }

        /***************************
         * acquisition
         */
        RetVal = GB_AcquisitionOptionsGlobals.GBMSAPI_Jw.SetFrameRate(
                GB_AcquisitionOptionsGlobals.ScanArea,
                GBMSAPIJavaWrapperDefinesFrameRateOptions.GBMSAPI_FRO_FULL_RESOLUTION_MODE,
                CalculateFrameRateFromProgress(AcquisitionMinFr, AcquisitionMaxFr, scrollBarAcq.getProgress())
        );
        if (RetVal != GBMSAPIJavaWrapperDefinesReturnCodes.GBMSAPI_ERROR_CODE_NO_ERROR) {
            checkGbmsapi = "Acquisition " + GB_AcquisitionOptionsGlobals.GBMSAPI_Jw.GetLastErrorString();
            this.setTitle(checkGbmsapi);
            EnableWidgets(false);
            return;
        }

        this.setTitle("SetFrameRate OK");
    }
    
    private void EnableWidgets(boolean enable)
    {
        bSetFrameRate.setEnabled(enable);
        tbPreviewFrameRate.setEnabled(enable);
        tbPreviewFrameRateMin.setEnabled(enable);
        tbPreviewFrameRateMax.setEnabled(enable);
        scrollBarPreview.setEnabled(enable);
        tbAcqFrameRate.setEnabled(enable);
        tbAcqFrameRateMin.setEnabled(enable);
        tbAcqFrameRateMax.setEnabled(enable);
        scrollBarAcq.setEnabled(enable);
    }

    private double CalculateFrameRateFromProgress(double min, double max, int prog)
    {
        return (min + ((max - min) * prog)/100);
    }

    private int CalculateProgressFromFrameRate(double min, double max, double fr)
    {
        if (fr < min) return 0;
        if (fr > max) return 100;
        double floatPrg = ((fr - min)/(max - min)) * 100;
        return (int)floatPrg;
    }

    private void SetFrameRateText(double min, double max, SeekBar from, TextView to, String AcqOrPrev)
    {
        double fr = CalculateFrameRateFromProgress(min, max, from.getProgress());
        to.setText(String.format(AcqOrPrev + " FR: %.2f",fr));
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_frame_rate_settings);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        
        /////////////////////////////////
        // Prepare controls
        /////////////////////////////////
        /*******************************
         * button
         */
        bSetFrameRate = (Button) findViewById(R.id.bFrameRateSet);
        bSetFrameRate.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                SetFrameRateClick();
            }
        });

        tbLogger = (TextView) findViewById(R.id.lFrameRateLogger);
        LogFrameRateMessages("");

        /*******************************
         * preview
         */
        bSetDefaultPreviewFr = (Button) findViewById(R.id.bFrameRatePreviewSetDefault);
        bSetDefaultPreviewFr.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                scrollBarPreview.setProgress(CalculateProgressFromFrameRate(PreviewMinFr,PreviewMaxFr,PreviewDefaultFr));
                SetFrameRateText(PreviewMinFr, PreviewMaxFr, scrollBarPreview, tbPreviewFrameRate, "Preview");
                SetFrameRateClick();
            }
        });
        tbPreviewFrameRate = (TextView) findViewById(R.id.lPreviewFrameRate);
        tbPreviewFrameRate.setText("Preview FR");
        tbPreviewFrameRateMin = (TextView) findViewById(R.id.lPreviewFrameRateMin);
        tbPreviewFrameRateMin.setText("CiaoMin");
        tbPreviewFrameRateMax = (TextView) findViewById(R.id.lPreviewFrameRateMax);
        tbPreviewFrameRateMax.setText("CiaoMax");

        scrollBarPreview = (SeekBar) findViewById(R.id.scrollBarPreviewFrameRate);
        scrollBarPreview.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            int progressChanged = 0;

            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser){
                SetFrameRateText(PreviewMinFr, PreviewMaxFr, scrollBarPreview, tbPreviewFrameRate, "Preview");
            }

            public void onStartTrackingTouch(SeekBar seekBar) {
                // TODO Auto-generated method stub
            }

            public void onStopTrackingTouch(SeekBar seekBar) {
            }
        });

        /**********************************
         * acquisition
         */
        bSetDefaultAcqFr = (Button) findViewById(R.id.bFrameRateAcqSetDefault);
        bSetDefaultAcqFr.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                scrollBarAcq.setProgress(CalculateProgressFromFrameRate(AcquisitionMinFr,AcquisitionMaxFr,AcquisitionDefaultFr));
                SetFrameRateText(AcquisitionMinFr, AcquisitionMaxFr, scrollBarAcq, tbAcqFrameRate, "Acquisition");
                SetFrameRateClick();
            }
        });
        tbAcqFrameRate = (TextView) findViewById(R.id.lAcqFrameRate);
        tbAcqFrameRate.setText("Acquisition FR");
        tbAcqFrameRateMin = (TextView) findViewById(R.id.lAcqFrameRateMin);
        tbAcqFrameRateMin.setText("CiaoMin");
        tbAcqFrameRateMax = (TextView) findViewById(R.id.lAcqFrameRateMax);
        tbAcqFrameRateMax.setText("CiaoMax");

        scrollBarAcq = (SeekBar) findViewById(R.id.scrollBarAcqFrameRate);
        scrollBarAcq.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            int progressChanged = 0;

            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser){
                SetFrameRateText(AcquisitionMinFr, AcquisitionMaxFr, scrollBarAcq, tbAcqFrameRate, "Acquisition");
            }

            public void onStartTrackingTouch(SeekBar seekBar) {
                // TODO Auto-generated method stub
            }

            public void onStopTrackingTouch(SeekBar seekBar) {
            }
        });

        /////////////////////////////////
        // load range
        /////////////////////////////////
        String checkGbmsapi;
        int RetVal;
        
        GBJavaWrapperUtilDoubleForJavaToCExchange MaxFrameRate = new GBJavaWrapperUtilDoubleForJavaToCExchange(),
                MinFrameRate = new GBJavaWrapperUtilDoubleForJavaToCExchange(),
                DefaultFrameRate = new GBJavaWrapperUtilDoubleForJavaToCExchange();
        /*************************
         * preview
         */
        RetVal = GB_AcquisitionOptionsGlobals.GBMSAPI_Jw.GetFrameRateRange(
                GB_AcquisitionOptionsGlobals.DeviceID,0,GB_AcquisitionOptionsGlobals.ScanArea,
                MaxFrameRate,MinFrameRate,DefaultFrameRate
        );
        if (RetVal != GBMSAPIJavaWrapperDefinesReturnCodes.GBMSAPI_ERROR_CODE_NO_ERROR) {
            checkGbmsapi = "Preview " + GB_AcquisitionOptionsGlobals.GBMSAPI_Jw.GetLastErrorString();
            this.setTitle(checkGbmsapi);
            EnableWidgets(false);
            return;
        }
        PreviewMinFr = MinFrameRate.Get();
        PreviewMaxFr = MaxFrameRate.Get();
        PreviewDefaultFr = DefaultFrameRate.Get();
        tbPreviewFrameRateMax.setText(String.format("%.2f",PreviewMaxFr));
        tbPreviewFrameRateMin.setText(String.format("%.2f",PreviewMinFr));
        scrollBarPreview.setProgress(CalculateProgressFromFrameRate(PreviewMinFr,PreviewMaxFr,PreviewDefaultFr));
        SetFrameRateText(PreviewMinFr, PreviewMaxFr, scrollBarPreview, tbPreviewFrameRate, "Preview");
        /*************************
         * acquisition
         */
        RetVal = GB_AcquisitionOptionsGlobals.GBMSAPI_Jw.GetFrameRateRange(
                GB_AcquisitionOptionsGlobals.DeviceID,
                GBMSAPIJavaWrapperDefinesFrameRateOptions.GBMSAPI_FRO_FULL_RESOLUTION_MODE,
                GB_AcquisitionOptionsGlobals.ScanArea,
                MaxFrameRate,MinFrameRate,DefaultFrameRate
        );
        if (RetVal != GBMSAPIJavaWrapperDefinesReturnCodes.GBMSAPI_ERROR_CODE_NO_ERROR) {
            checkGbmsapi = "Acquisition " + GB_AcquisitionOptionsGlobals.GBMSAPI_Jw.GetLastErrorString();
            this.setTitle(checkGbmsapi);
            EnableWidgets(false);
            return;
        }
        AcquisitionMinFr = MinFrameRate.Get();
        AcquisitionMaxFr = MaxFrameRate.Get();
        AcquisitionDefaultFr = DefaultFrameRate.Get();
        tbAcqFrameRateMax.setText(String.format("%.2f",AcquisitionMaxFr));
        tbAcqFrameRateMin.setText(String.format("%.2f",AcquisitionMinFr));
        scrollBarAcq.setProgress(CalculateProgressFromFrameRate(AcquisitionMinFr,AcquisitionMaxFr,AcquisitionDefaultFr));
        SetFrameRateText(AcquisitionMinFr, AcquisitionMaxFr, scrollBarAcq, tbAcqFrameRate, "Acquisition");


        checkGbmsapi = "GetFrameRateRange Ok";
        this.setTitle(checkGbmsapi);
        EnableWidgets(true);

    }

}
