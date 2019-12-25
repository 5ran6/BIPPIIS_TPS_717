package com.greenbit.MultiscanJNIGuiJavaAndroid;

import android.os.Bundle;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;
import android.widget.TextView;

import com.greenbit.gbfinimg.GbfinimgJavaWrapperDefineSegmentImageDescriptor;

public class SegmentsInfoWindow extends AppCompatActivity
{
    public static int NumOfSegments, Diagnostic;
    public static GbfinimgJavaWrapperDefineSegmentImageDescriptor[] descriptors;

    private TextView Seg1BoundingBox, Seg2BoundingBox, Seg3BoundingBox, Seg4BoundingBox;
    private TextView Seg1FingerName, Seg2FingerName, Seg3FingerName, Seg4FingerName;
    private TextView Seg1QualityAndPattern, Seg2QualityAndPattern, Seg3QualityAndPattern, Seg4QualityAndPattern;

    private void FillSegmentWithDescriptor(
            TextView BB, TextView FName, TextView Qual,
            GbfinimgJavaWrapperDefineSegmentImageDescriptor desc)
    {
        if (desc == null)
        {
            BB.setText("no info");
            FName.setText("no info");
            Qual.setText("no info");
            return;
        }
        BB.setText("BBT=" + desc.BoundingBoxT + ",BBL=" + desc.BoundingBoxL +
                ",BBB=" + desc.BoundingBoxB + ",BBR=" + desc.BoundingBoxR);
        FName.setText("FingerID=" + desc.Finger);
        Qual.setText("Quality=" + desc.Quality + ",PatternComp=" + desc.PatternCompleteness +
                ",PatternVal=" + desc.PatternValidity);
    }

    private void FillDiagnostic(TextView tv, int Diag)
    {
        tv.setText("Diagnostic Value = " + Diag);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_segments_info_window);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        /************************************
         * Segment 1
         */
        Seg1BoundingBox = (TextView) findViewById(R.id.Seg1BoundingBox);
        Seg1FingerName = (TextView) findViewById(R.id.Seg1FingerName);
        Seg1QualityAndPattern = (TextView) findViewById(R.id.Seg1QualityAndPattern);

        /************************************
         * Segment 2
         */
        Seg2BoundingBox = (TextView) findViewById(R.id.Seg2BoundingBox);
        Seg2FingerName = (TextView) findViewById(R.id.Seg2FingerName);
        Seg2QualityAndPattern = (TextView) findViewById(R.id.Seg2QualityAndPattern);

        /************************************
         * Segment 1
         */
        Seg3BoundingBox = (TextView) findViewById(R.id.Seg3BoundingBox);
        Seg3FingerName = (TextView) findViewById(R.id.Seg3FingerName);
        Seg3QualityAndPattern = (TextView) findViewById(R.id.Seg3QualityAndPattern);

        /************************************
         * Segment 1
         */
        Seg4BoundingBox = (TextView) findViewById(R.id.Seg4BoundingBox);
        Seg4FingerName = (TextView) findViewById(R.id.Seg4FingerName);
        Seg4QualityAndPattern = (TextView) findViewById(R.id.Seg4QualityAndPattern);

        /*********************************************
         * SET VALUES
         */
        if (NumOfSegments <= 0)
        {
            FillSegmentWithDescriptor(Seg1BoundingBox,Seg1FingerName,Seg1QualityAndPattern,null);
            FillSegmentWithDescriptor(Seg2BoundingBox,Seg2FingerName,Seg2QualityAndPattern,null);
            FillSegmentWithDescriptor(Seg3BoundingBox,Seg3FingerName,Seg3QualityAndPattern,null);
            FillSegmentWithDescriptor(Seg4BoundingBox,Seg4FingerName,Seg4QualityAndPattern,null);
            FillDiagnostic(Seg1BoundingBox, Diagnostic);
            return;
        }
        if (NumOfSegments > 0)
        {
            FillSegmentWithDescriptor(Seg1BoundingBox,Seg1FingerName,Seg1QualityAndPattern,descriptors[0]);
        }
        if (NumOfSegments > 1)
        {
            FillSegmentWithDescriptor(Seg2BoundingBox,Seg2FingerName,Seg2QualityAndPattern,descriptors[1]);
        }
        if (NumOfSegments > 2)
        {
            FillSegmentWithDescriptor(Seg3BoundingBox,Seg3FingerName,Seg3QualityAndPattern,descriptors[2]);
        }
        if (NumOfSegments > 3)
        {
            FillSegmentWithDescriptor(Seg4BoundingBox,Seg4FingerName,Seg4QualityAndPattern,descriptors[3]);
        }
    }

}
