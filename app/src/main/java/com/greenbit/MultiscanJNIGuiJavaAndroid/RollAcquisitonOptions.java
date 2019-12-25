package com.greenbit.MultiscanJNIGuiJavaAndroid;

import android.os.Bundle;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;
import android.widget.CompoundButton;
import android.widget.RadioButton;
import android.widget.Toast;

import com.greenbit.utils.GBJavaWrapperUtilIntForJavaToCExchange;
import com.greenbit.gbmsapi.GBMSAPIJavaWrapperDefinesAcquisitionOptions;
import com.greenbit.gbmsapi.GBMSAPIJavaWrapperDefinesReturnCodes;

public class RollAcquisitonOptions extends AppCompatActivity {
    private RadioButton rbSide, rbCenter, rbManual, rbNoPreview;

    private void EnableWidgets(boolean enable)
    {
        rbSide.setEnabled(enable);
        rbCenter.setEnabled(enable);
        rbManual.setEnabled(enable);
        rbNoPreview.setEnabled(enable);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_roll_acquisiton_options);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        /////////////////////////
        // init widgets
        /////////////////////////
        rbSide = (RadioButton)findViewById(R.id.rbSideRollPreview);
        rbCenter = (RadioButton)findViewById(R.id.rbCenterRollPreview);
        rbManual = (RadioButton)findViewById(R.id.rbManualRollPreviewStop);
        rbNoPreview = (RadioButton)findViewById(R.id.rbNoRollPreview);

        /////////////////////////
        // load options
        /////////////////////////
        String checkGbmsapi;
        int RetVal;

        /*********************************
         * scan options
         */
        GBJavaWrapperUtilIntForJavaToCExchange scanOptions = new GBJavaWrapperUtilIntForJavaToCExchange();
        RetVal = GB_AcquisitionOptionsGlobals.GBMSAPI_Jw.GetSupportedScanOptions(scanOptions);
        if (RetVal == GBMSAPIJavaWrapperDefinesReturnCodes.GBMSAPI_ERROR_CODE_NO_ERROR) {
            checkGbmsapi = "GetSupportedScanOptions Ok";
            this.setTitle(checkGbmsapi);
        } else {
            checkGbmsapi = GB_AcquisitionOptionsGlobals.GBMSAPI_Jw.GetLastErrorString();
            this.setTitle(checkGbmsapi);
            rbSide.setEnabled(false);
            rbCenter.setEnabled(false);
            rbManual.setEnabled(false);
            rbNoPreview.setEnabled(false);
            return;
        }

        rbSide.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                int acqOpt = GB_AcquisitionOptionsGlobals.GetAcquisitionRollOptionsParameter();
                if (isChecked) {
                    acqOpt &= ~ (GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_ROLL_PREVIEW_TYPE);
                    Toast popup = Toast.makeText(getBaseContext(), "Side Roll Preview", Toast.LENGTH_SHORT);
                    popup.show();
                }
                GB_AcquisitionOptionsGlobals.SetAcquisitionRollOptionsParameter(acqOpt);
            }
        });

        rbCenter.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                int acqOpt = GB_AcquisitionOptionsGlobals.GetAcquisitionRollOptionsParameter();
                if (isChecked) {
                    acqOpt  |= GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_ROLL_PREVIEW_TYPE;
                    Toast popup = Toast.makeText(getBaseContext(), "Center Roll Preview", Toast.LENGTH_SHORT);
                    popup.show();
                }
                else
                {
                    acqOpt &= ~ (GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_ROLL_PREVIEW_TYPE);
                }
                GB_AcquisitionOptionsGlobals.SetAcquisitionRollOptionsParameter(acqOpt);
            }
        });

        rbManual.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                int acqOpt = GB_AcquisitionOptionsGlobals.GetAcquisitionRollOptionsParameter();
                if (isChecked) {
                    acqOpt  |= GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_MANUAL_ROLL_PREVIEW_STOP;
                    Toast popup = Toast.makeText(getBaseContext(), "Manual Roll Preview Stop", Toast.LENGTH_SHORT);
                    popup.show();
                }
                else
                {
                    acqOpt &= ~ (GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_MANUAL_ROLL_PREVIEW_STOP);
                }
                GB_AcquisitionOptionsGlobals.SetAcquisitionRollOptionsParameter(acqOpt);
            }
        });

        rbNoPreview.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                int acqOpt = GB_AcquisitionOptionsGlobals.GetAcquisitionRollOptionsParameter();
                if (isChecked) {
                    acqOpt |= GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_NO_ROLL_PREVIEW;
                    Toast popup = Toast.makeText(getBaseContext(), "No Roll Preview", Toast.LENGTH_SHORT);
                    popup.show();
                }
                else
                {
                    acqOpt &= ~ (GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_NO_ROLL_PREVIEW);
                }
                GB_AcquisitionOptionsGlobals.SetAcquisitionRollOptionsParameter(acqOpt);
            }
        });

        int RollOption = GB_AcquisitionOptionsGlobals.GetAcquisitionRollOptionsParameter();

        if ((RollOption & GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_NO_ROLL_PREVIEW) != 0)
        {
            rbNoPreview.setChecked(true);
        }
        else if ((RollOption & GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_MANUAL_ROLL_PREVIEW_STOP) != 0)
        {
            rbManual.setChecked(true);
        }
        else if ((RollOption & GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_ROLL_PREVIEW_TYPE) != 0)
        {
            rbCenter.setChecked(true);
        }
        else
        {
            rbSide.setChecked(true);
        }
    }

}
