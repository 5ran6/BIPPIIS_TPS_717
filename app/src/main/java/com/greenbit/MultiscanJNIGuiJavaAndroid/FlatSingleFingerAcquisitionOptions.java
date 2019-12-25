package com.greenbit.MultiscanJNIGuiJavaAndroid;

import android.os.Bundle;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;
import android.widget.CompoundButton;
import android.widget.Switch;
import android.widget.Toast;

import com.greenbit.utils.GBJavaWrapperUtilIntForJavaToCExchange;
import com.greenbit.gbmsapi.GBMSAPIJavaWrapperDefinesAcquisitionOptions;
import com.greenbit.gbmsapi.GBMSAPIJavaWrapperDefinesReturnCodes;

public class FlatSingleFingerAcquisitionOptions extends AppCompatActivity {
    private Switch cbAutoCapture, cbFullResolutionPreview, cbFlatFingerOnRollArea;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_flat_acquisition_options);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        String checkGbmsapi;
        int RetVal;
        GBJavaWrapperUtilIntForJavaToCExchange scanOptions = new GBJavaWrapperUtilIntForJavaToCExchange();
        cbAutoCapture = (Switch)findViewById(R.id.switchAutoCapture);
        cbFullResolutionPreview = (Switch)findViewById(R.id.switchFullResPreview);
        cbFlatFingerOnRollArea = (Switch)findViewById(R.id.switchFlatFingerOnRollArea);
        RetVal = GB_AcquisitionOptionsGlobals.GBMSAPI_Jw.GetSupportedScanOptions(scanOptions);
        if (RetVal == GBMSAPIJavaWrapperDefinesReturnCodes.GBMSAPI_ERROR_CODE_NO_ERROR) {
            checkGbmsapi = "GetSupportedScanOptions Ok";
            this.setTitle(checkGbmsapi);
        } else {
            checkGbmsapi = GB_AcquisitionOptionsGlobals.GBMSAPI_Jw.GetLastErrorString();
            this.setTitle(checkGbmsapi);
            cbAutoCapture.setEnabled(false);
            cbFullResolutionPreview.setEnabled(false);
            cbFlatFingerOnRollArea.setEnabled(false);
            return;
        }
        cbAutoCapture.setEnabled(true);
        cbFullResolutionPreview.setEnabled(true);
        cbFlatFingerOnRollArea.setEnabled(true);

        cbAutoCapture.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                int acqOpt = GB_AcquisitionOptionsGlobals.GetAcquisitionFlatSingleOptionsParameter();
                if (isChecked) {
                    acqOpt |= GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_AUTOCAPTURE;
                    Toast popup = Toast.makeText(getBaseContext(), "Auto-Capture enabled", Toast.LENGTH_SHORT);
                    popup.show();
                } else {
                    acqOpt &= ~(GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_AUTOCAPTURE);
                    Toast popup = Toast.makeText(getBaseContext(), "Auto-Capture disabled", Toast.LENGTH_SHORT);
                    popup.show();
                }
                GB_AcquisitionOptionsGlobals.SetAcquisitionFlatSingleOptionsParameter(acqOpt);
            }
        });
        int acqOpt = GB_AcquisitionOptionsGlobals.GetAcquisitionFlatSingleOptionsParameter();
        if ((scanOptions.Get() & GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_AUTOCAPTURE) != 0)
        {
            cbAutoCapture.setEnabled(true);
            if ((acqOpt & GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_AUTOCAPTURE) != 0)
            {
                cbAutoCapture.setChecked(true);
            }
            else cbAutoCapture.setChecked(false);
        }
        else
        {
            acqOpt &= ~(GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_AUTOCAPTURE);
            cbAutoCapture.setEnabled(false);
            cbAutoCapture.setChecked(false);
        }

        cbFullResolutionPreview.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                int acqOpt = GB_AcquisitionOptionsGlobals.GetAcquisitionFlatSingleOptionsParameter();
                if (isChecked) {
                    acqOpt |= GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_FULL_RES_PREVIEW;
                    Toast popup = Toast.makeText(getBaseContext(), "Full resolution preview enabled", Toast.LENGTH_SHORT);
                    popup.show();
                } else {
                    acqOpt &= ~(GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_FULL_RES_PREVIEW);
                    Toast popup = Toast.makeText(getBaseContext(), "Full resolution preview disabled", Toast.LENGTH_SHORT);
                    popup.show();
                }
                GB_AcquisitionOptionsGlobals.SetAcquisitionFlatSingleOptionsParameter(acqOpt);
            }
        });
        if ((scanOptions.Get() & GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_FULL_RES_PREVIEW) != 0)
        {
            cbFullResolutionPreview.setEnabled(true);
            if ((acqOpt & GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_FULL_RES_PREVIEW) != 0)
            {
                cbFullResolutionPreview.setChecked(true);
            }
            else cbFullResolutionPreview.setChecked(false);
        }
        else
        {
            acqOpt &= ~(GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_FULL_RES_PREVIEW);
            cbFullResolutionPreview.setEnabled(false);
            cbFullResolutionPreview.setChecked(false);
        }

        cbFlatFingerOnRollArea.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                int acqOpt = GB_AcquisitionOptionsGlobals.GetAcquisitionFlatSingleOptionsParameter();
                if (isChecked) {
                    acqOpt |= GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_FLAT_SINGLE_FINGER_ON_ROLL_AREA;
                    Toast popup = Toast.makeText(getBaseContext(), "Flat finger on roll area", Toast.LENGTH_SHORT);
                    popup.show();
                } else {
                    acqOpt &= ~(GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_FLAT_SINGLE_FINGER_ON_ROLL_AREA);
                    Toast popup = Toast.makeText(getBaseContext(), "Flat finger on full area", Toast.LENGTH_SHORT);
                    popup.show();
                }
                GB_AcquisitionOptionsGlobals.SetAcquisitionFlatSingleOptionsParameter(acqOpt);
            }
        });
        if ((scanOptions.Get() & GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_FLAT_SINGLE_FINGER_ON_ROLL_AREA) != 0)
        {
            cbFlatFingerOnRollArea.setEnabled(true);
            if ((acqOpt & GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_FLAT_SINGLE_FINGER_ON_ROLL_AREA) != 0)
            {
                cbFlatFingerOnRollArea.setChecked(true);
            }
            else cbFlatFingerOnRollArea.setChecked(false);
        }
        else
        {
            acqOpt &= ~(GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_FLAT_SINGLE_FINGER_ON_ROLL_AREA);
            cbFlatFingerOnRollArea.setEnabled(false);
            cbFlatFingerOnRollArea.setChecked(false);
        }
        GB_AcquisitionOptionsGlobals.SetAcquisitionFlatSingleOptionsParameter(acqOpt);
    }

}
