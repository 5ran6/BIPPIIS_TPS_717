package com.greenbit.MultiscanJNIGuiJavaAndroid;

import android.os.Bundle;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RadioButton;

import com.greenbit.gbfrsw.GbfrswJavaWrapperDefinesRanges;
import com.greenbit.gbfrsw.GbfrswJavaWrapperDefinesSpeedPrecisionTradeoff;

public class DactyMatchSettings extends AppCompatActivity {
    private RadioButton rbFAR_Eminus4, rbFAR_Eminus5, rbFAR_Eminus6, rbFAR_Custom;
    private EditText tbCustomMatchScore;
    private RadioButton rbSpeedPrecisionROBUST, rbSpeedPrecisionNORMAL, rbSpeedPrecisionFAST;
    private EditText tbRotationAngle;
    private Button bSaveGbfrswSettings;

    protected void SaveSettings()
    {
        /***************************
         * MATCHING SCORE
         */
        if (rbFAR_Eminus4.isChecked())
        {
            GB_AcquisitionOptionsGlobals.MatchingThreshold = 10000;
        }
        else if (rbFAR_Eminus5.isChecked())
        {
            GB_AcquisitionOptionsGlobals.MatchingThreshold = 100000;
        }
        else if (rbFAR_Eminus6.isChecked())
        {
            GB_AcquisitionOptionsGlobals.MatchingThreshold = 1000000;
        }
        else
        {
            try
            {
                double app;
                app = Double.parseDouble(tbCustomMatchScore.getText().toString());
                if ((app > GbfrswJavaWrapperDefinesRanges.GBFRSW_MAX_MATCHING_SCORE) ||
                        (app < GbfrswJavaWrapperDefinesRanges.GBFRSW_MIN_MATCHING_SCORE))
                {
                    GB_AcquisitionOptionsGlobals.CreateDialogNeutral("SaveSettings: Match threshold out of range, will be set to default", this);
                    app = 10000;
                    tbCustomMatchScore.setText("" + app);
                }
                GB_AcquisitionOptionsGlobals.MatchingThreshold = app;
            }
            catch (Exception ex)
            {
                GB_AcquisitionOptionsGlobals.CreateDialogNeutral("SaveSettings: Match threshold is not a number", this);
                tbCustomMatchScore.requestFocus();
                return;
            }
        }

        /***************************
         * SPEED VS PRECISION
         */
        if (rbSpeedPrecisionROBUST.isChecked())
        {
            GB_AcquisitionOptionsGlobals.SpeedVsPrecisionTradeoff = GbfrswJavaWrapperDefinesSpeedPrecisionTradeoff.GBFRSW_TRADEOFF_ROBUST;
        }
        else if (rbSpeedPrecisionNORMAL.isChecked())
        {
            GB_AcquisitionOptionsGlobals.SpeedVsPrecisionTradeoff = GbfrswJavaWrapperDefinesSpeedPrecisionTradeoff.GBFRSW_TRADEOFF_NORMAL;
        }
        else if (rbSpeedPrecisionFAST.isChecked())
        {
            GB_AcquisitionOptionsGlobals.SpeedVsPrecisionTradeoff = GbfrswJavaWrapperDefinesSpeedPrecisionTradeoff.GBFRSW_TRADEOFF_FAST;
        }
        else
        {
            GB_AcquisitionOptionsGlobals.SpeedVsPrecisionTradeoff = GbfrswJavaWrapperDefinesSpeedPrecisionTradeoff.GBFRSW_TRADEOFF_ROBUST;
        }

        /***************************
         * ROTATION ANGLE
         */
        try
        {
            int app = Integer.parseInt(tbRotationAngle.getText().toString());
            if ((app > GbfrswJavaWrapperDefinesRanges.GBFRSW_ROTATION_ANGLE_MAXIMAL_ACCEPTABLE) ||
                    (app < GbfrswJavaWrapperDefinesRanges.GBFRSW_ROTATION_ANGLE_MINIMAL_ACCEPTABLE))
            {
                GB_AcquisitionOptionsGlobals.CreateDialogNeutral("SaveSettings: Rotation angle out of range, will be set to default", this);
                app = 20;
                tbRotationAngle.setText("" + app);
            }
            GB_AcquisitionOptionsGlobals.MatchRotationAngle = app;
        }
        catch (Exception ex)
        {
            GB_AcquisitionOptionsGlobals.CreateDialogNeutral("SaveSettings: Rotation angle is not a number", null);
            tbRotationAngle.requestFocus();
            return;
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_dacty_match_settings);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        this.setTitle("DACTY MATCH SETTINGS");

        /***************************
         * MATCHING SCORE
         */
        rbFAR_Eminus4 = (RadioButton)findViewById(R.id.rbFAR_Eminus4);
        rbFAR_Eminus5 = (RadioButton)findViewById(R.id.rbFAR_Eminus5);
        rbFAR_Eminus6 = (RadioButton)findViewById(R.id.rbFAR_Eminus6);
        rbFAR_Custom = (RadioButton)findViewById(R.id.rbFAR_Custom);
        tbCustomMatchScore = (EditText)findViewById(R.id.tbCustomMatchScore);
        if (GB_AcquisitionOptionsGlobals.MatchingThreshold == 10000)
        {
            rbFAR_Eminus4.setChecked(true);
        }
        else if (GB_AcquisitionOptionsGlobals.MatchingThreshold == 100000)
        {
            rbFAR_Eminus5.setChecked(true);
        }
        else if (GB_AcquisitionOptionsGlobals.MatchingThreshold == 1000000)
        {
            rbFAR_Eminus6.setChecked(true);
        }
        else
        {
            rbFAR_Custom.setChecked(true);
            tbCustomMatchScore.setText("" + GB_AcquisitionOptionsGlobals.MatchingThreshold);
        }

        /***************************
         * SPEED VS PRECISION
         */
        rbSpeedPrecisionROBUST = (RadioButton)findViewById(R.id.rbSpeedPrecisionROBUST);
        rbSpeedPrecisionNORMAL = (RadioButton)findViewById(R.id.rbSpeedPrecisionNORMAL);
        rbSpeedPrecisionFAST = (RadioButton)findViewById(R.id.rbSpeedPrecisionFAST);
        if (GB_AcquisitionOptionsGlobals.SpeedVsPrecisionTradeoff == GbfrswJavaWrapperDefinesSpeedPrecisionTradeoff.GBFRSW_TRADEOFF_ROBUST)
        {
            rbSpeedPrecisionROBUST.setChecked(true);
        }
        else if (GB_AcquisitionOptionsGlobals.SpeedVsPrecisionTradeoff == GbfrswJavaWrapperDefinesSpeedPrecisionTradeoff.GBFRSW_TRADEOFF_NORMAL)
        {
            rbSpeedPrecisionNORMAL.setChecked(true);
        }
        else if (GB_AcquisitionOptionsGlobals.SpeedVsPrecisionTradeoff == GbfrswJavaWrapperDefinesSpeedPrecisionTradeoff.GBFRSW_TRADEOFF_FAST)
        {
            rbSpeedPrecisionFAST.setChecked(true);
        }
        else
        {
            rbSpeedPrecisionROBUST.setChecked(true);
        }

        /***************************
         * ROTATION ANGLE
         */
        tbRotationAngle = (EditText)findViewById(R.id.tbRotationAngle);
        tbRotationAngle.setText("" + GB_AcquisitionOptionsGlobals.MatchRotationAngle);

        /***************************
         * SAVE
         */
        bSaveGbfrswSettings = (Button) findViewById(R.id.bSaveGbfrswSettings);
        bSaveGbfrswSettings.setEnabled(true);
        bSaveGbfrswSettings.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                SaveSettings();
            }
        });
    }

}
