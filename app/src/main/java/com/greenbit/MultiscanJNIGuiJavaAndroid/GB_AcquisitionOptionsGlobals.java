package com.greenbit.MultiscanJNIGuiJavaAndroid;
import android.content.Intent;
import androidx.appcompat.app.AppCompatActivity;

import com.greenbit.ansinistitl.Gban2000JavaWrapperLibrary;
import com.greenbit.bozorth.BozorthJavaWrapperLibrary;
import com.greenbit.gbfinimg.GbfinimgJavaWrapperLibrary;
import com.greenbit.gbfir.GbfirJavaWrapperLibrary;
import com.greenbit.gbfrsw.GbfrswJavaWrapperDefinesSpeedPrecisionTradeoff;
import com.greenbit.gbfrsw.GbfrswJavaWrapperLibrary;
import com.greenbit.gbmsapi.GBMSAPIJavaWrapperDefinesAcquisitionOptions;
import com.greenbit.gbmsapi.GBMSAPIJavaWrapperDefinesScannableBiometricType;
import com.greenbit.gbmsapi.GBMSAPIJavaWrapperDefinesScannerAcquisitionAreas;
import com.greenbit.gbmsapi.GBMSAPIJavaWrapperLibrary;
import com.greenbit.gbnfiq.GbNfiqJavaWrapperLibrary;
import com.greenbit.gbnfiq2.GbNfiq2JavaWrapperLibrary;
import com.greenbit.jpeg.GbjpegJavaWrapperLibrary;
import com.greenbit.lfs.LfsJavaWrapperLibrary;
import com.greenbit.wsq.WsqJavaWrapperLibrary;

public class GB_AcquisitionOptionsGlobals {
    /////////////////////////////////////
    // GENERAL UTILITY
    /////////////////////////////////////
    public static void CreateDialogNeutral(String title, AppCompatActivity act)
    {
//        AlertDialog.Builder builder = new AlertDialog.Builder(act);
//        builder.setNegativeButton("Ok",null);
//        AlertDialog dialog = builder.create();
//        dialog.setTitle(title);
//        dialog.show();
		LogWindow.LogStr = title;
		Intent intent = new Intent(act, LogWindow.class);
		act.startActivity(intent);
    }

    /////////////////////////////////////
    // GBMSAPI
    /////////////////////////////////////
    public static GBMSAPIJavaWrapperLibrary GBMSAPI_Jw;
    public static GbExampleGrayScaleBitmapClass acquiredFrame;
    public static boolean acquiredFrameValid = false;

    public static int ObjTypeToAcquire = GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_NO_OBJECT_TYPE;
    private static int AcquisitionOptions = 0;
    public static int ScanArea = GBMSAPIJavaWrapperDefinesScannerAcquisitionAreas.GBMSAPI_SA_FULL_FRAME;
    public static int DeviceID = 0;
    public static void ResetAcquisitionOptions()
    {
        AcquisitionOptions = GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_AUTOCAPTURE;
    }

    public static void SetAcquisitionFlatSingleOptionsParameter(int acqOpt)
    {
        int mask = GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_AUTOCAPTURE|
                GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_FLAT_SINGLE_FINGER_ON_ROLL_AREA |
                GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_FULL_RES_PREVIEW;
        acqOpt &= mask;
        AcquisitionOptions &= ~mask;
        AcquisitionOptions |= acqOpt;
    }

    public static int GetAcquisitionFlatSingleOptionsParameter()
    {
        int mask = GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_AUTOCAPTURE|
                GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_FLAT_SINGLE_FINGER_ON_ROLL_AREA |
                GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_FULL_RES_PREVIEW;
      //  int ValToRet = AcquisitionOptions & mask;
        int ValToRet = mask;
        return ValToRet;
    }

//    public static int GetAcquisitionFlatSingleOptionsParameter()
//    {
//        int mask = GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_AUTOCAPTURE|
//                GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_FLAT_SINGLE_FINGER_ON_ROLL_AREA; // change this other line
//        int ValToRet = mask; // here the line to change
//        return ValToRet;
//    }

    public static void SetAcquisitionSlapOptionsParameter(int acqOpt)
    {
        int mask = GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_AUTOCAPTURE|
                GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_FULL_RES_PREVIEW;
        acqOpt &= mask;
        AcquisitionOptions &= ~mask;
        AcquisitionOptions |= acqOpt;
    }

    public static int GetAcquisitionSlapOptionsParameter()
    {
        int mask = GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_AUTOCAPTURE|
                GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_FULL_RES_PREVIEW;
        int ValToRet = AcquisitionOptions & mask;
        return ValToRet;
    }

    public static void SetAcquisitionPalmOptionsParameter(int acqOpt)
    {
        int mask = GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_AUTOCAPTURE|
                GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_FULL_RES_PREVIEW;
        acqOpt &= mask;
        AcquisitionOptions &= ~mask;
        AcquisitionOptions |= acqOpt;
    }

    public static int GetAcquisitionPalmOptionsParameter()
    {
        int mask =GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_AUTOCAPTURE|
                GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_FULL_RES_PREVIEW;
        int ValToRet = AcquisitionOptions & mask;
        return ValToRet;
    }

    public static void SetAcquisitionRollOptionsParameter(int acqOpt)
    {
        int mask = GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_NO_ROLL_PREVIEW |
                GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_MANUAL_ROLL_PREVIEW_STOP |
                GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_ROLL_PREVIEW_TYPE;

        acqOpt &= mask;
        /**************************
         * clean roll acquisition
         * options
         */
        if ((acqOpt & GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_NO_ROLL_PREVIEW) != 0)
        {
            acqOpt &= ~(GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_MANUAL_ROLL_PREVIEW_STOP);
            acqOpt &= ~(GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_ROLL_PREVIEW_TYPE);
        }
        else if ((acqOpt & GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_MANUAL_ROLL_PREVIEW_STOP) != 0)
        {
            acqOpt &= ~(GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_ROLL_PREVIEW_TYPE);
        }
        AcquisitionOptions &= ~mask;
        AcquisitionOptions |= acqOpt;
    }

    public static int GetAcquisitionRollOptionsParameter()
    {
        int ValToRet = AcquisitionOptions & (
                GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_NO_ROLL_PREVIEW |
                        GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_MANUAL_ROLL_PREVIEW_STOP |
                        GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_ROLL_PREVIEW_TYPE
        );
        /**************************
         * clean roll acquisition
         * options
         */
        if ((ValToRet & GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_NO_ROLL_PREVIEW) != 0)
        {
            ValToRet &= ~(GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_MANUAL_ROLL_PREVIEW_STOP);
            ValToRet &= ~(GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_ROLL_PREVIEW_TYPE);
        }
        else if ((ValToRet & GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_MANUAL_ROLL_PREVIEW_STOP) != 0)
        {
            ValToRet &= ~(GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_ROLL_PREVIEW_TYPE);
        }
        return ValToRet;
    }

    public static int GetScanAreaFromAcquisitionOptionsAndObject()
    {
        int ScanArea = 0;
        if (ObjTypeToAcquire  == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_ROLL_SINGLE_FINGER)
            ScanArea = GBMSAPIJavaWrapperDefinesScannerAcquisitionAreas.GBMSAPI_SA_ROLL_IQS;
        else if (ObjTypeToAcquire  == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_FLAT_SINGLE_FINGER)
        {
            if ((AcquisitionOptions & GBMSAPIJavaWrapperDefinesAcquisitionOptions.GBMSAPI_AO_FLAT_SINGLE_FINGER_ON_ROLL_AREA) != 0)
            {
                ScanArea = GBMSAPIJavaWrapperDefinesScannerAcquisitionAreas.GBMSAPI_SA_ROLL_IQS;
            }
            else
            {
                ScanArea = GBMSAPIJavaWrapperDefinesScannerAcquisitionAreas.GBMSAPI_SA_FULL_FRAME;
            }
        }
        else if (ObjTypeToAcquire  == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_FLAT_SLAP_4)
            ScanArea = GBMSAPIJavaWrapperDefinesScannerAcquisitionAreas.GBMSAPI_SA_FULL_FRAME;
        else if (ObjTypeToAcquire  == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_FLAT_SLAP_2)
            ScanArea = GBMSAPIJavaWrapperDefinesScannerAcquisitionAreas.GBMSAPI_SA_FULL_FRAME;
        else if (ObjTypeToAcquire  == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_FLAT_THUMBS_2)
            ScanArea = GBMSAPIJavaWrapperDefinesScannerAcquisitionAreas.GBMSAPI_SA_FULL_FRAME;
        else if (ObjTypeToAcquire  == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_FLAT_LOWER_HALF_PALM)
            ScanArea = GBMSAPIJavaWrapperDefinesScannerAcquisitionAreas.GBMSAPI_SA_FULL_FRAME;
        else if (ObjTypeToAcquire  == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_FLAT_UPPER_HALF_PALM)
            ScanArea = GBMSAPIJavaWrapperDefinesScannerAcquisitionAreas.GBMSAPI_SA_FULL_FRAME;
        else if (ObjTypeToAcquire  == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_FLAT_WRITER_PALM)
            ScanArea = GBMSAPIJavaWrapperDefinesScannerAcquisitionAreas.GBMSAPI_SA_FULL_FRAME;
        else if (ObjTypeToAcquire  == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_FLAT_INDEXES_2)
            ScanArea = GBMSAPIJavaWrapperDefinesScannerAcquisitionAreas.GBMSAPI_SA_FULL_FRAME;
        else if (ObjTypeToAcquire  == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_ROLLED_THENAR)
            ScanArea = GBMSAPIJavaWrapperDefinesScannerAcquisitionAreas.GBMSAPI_SA_ROLL_THENAR;
        else if (ObjTypeToAcquire  == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_ROLLED_JOINT)
            ScanArea = GBMSAPIJavaWrapperDefinesScannerAcquisitionAreas.GBMSAPI_SA_ROLL_JOINT;
        else if (ObjTypeToAcquire  == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_PLAIN_JOINT_LEFT_SIDE)
            ScanArea = GBMSAPIJavaWrapperDefinesScannerAcquisitionAreas.GBMSAPI_SA_ROLL_JOINT;
        else if (ObjTypeToAcquire  == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_PLAIN_JOINT_RIGHT_SIDE)
            ScanArea = GBMSAPIJavaWrapperDefinesScannerAcquisitionAreas.GBMSAPI_SA_ROLL_JOINT;
        else if (ObjTypeToAcquire  == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_ROLLED_JOINT_CENTER)
            ScanArea = GBMSAPIJavaWrapperDefinesScannerAcquisitionAreas.GBMSAPI_SA_ROLL_JOINT;
        else if (ObjTypeToAcquire  == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_ROLLED_TIP)
            ScanArea = GBMSAPIJavaWrapperDefinesScannerAcquisitionAreas.GBMSAPI_SA_ROLL_IQS;
        else if (ObjTypeToAcquire  == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_ROLLED_UP)
            ScanArea = GBMSAPIJavaWrapperDefinesScannerAcquisitionAreas.GBMSAPI_SA_ROLL_IQS;
        else if (ObjTypeToAcquire  == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_ROLLED_DOWN)
            ScanArea = GBMSAPIJavaWrapperDefinesScannerAcquisitionAreas.GBMSAPI_SA_ROLL_IQS;
        else if (ObjTypeToAcquire  == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_ROLLED_HYPOTHENAR)
            ScanArea = GBMSAPIJavaWrapperDefinesScannerAcquisitionAreas.GBMSAPI_SA_ROLL_THENAR;

        return ScanArea;
    }

    public static void SetAcquisitionOptionsForObjectType(int objType, int acqOpt)
    {
        if (GBMSAPIJavaWrapperDefinesScannableBiometricType.ObjToAcquireIsFlatSingle(objType)) SetAcquisitionFlatSingleOptionsParameter(acqOpt);
        else if (GBMSAPIJavaWrapperDefinesScannableBiometricType.ObjToAcquireIsPalm(objType)) SetAcquisitionPalmOptionsParameter(acqOpt);
        else if (GBMSAPIJavaWrapperDefinesScannableBiometricType.ObjToAcquireIsRoll(objType)) SetAcquisitionRollOptionsParameter(acqOpt);
        else if (GBMSAPIJavaWrapperDefinesScannableBiometricType.ObjToAcquireIsSlapOrJoint(objType)) SetAcquisitionSlapOptionsParameter(acqOpt);
        AcquisitionOptions = 0;
    }

    public static int GetAcquisitionOptionsForObjectType(int objType)
    {
        if (GBMSAPIJavaWrapperDefinesScannableBiometricType.ObjToAcquireIsFlatSingle(objType)) return GetAcquisitionFlatSingleOptionsParameter();
        else if (GBMSAPIJavaWrapperDefinesScannableBiometricType.ObjToAcquireIsPalm(objType)) return GetAcquisitionPalmOptionsParameter();
        else if (GBMSAPIJavaWrapperDefinesScannableBiometricType.ObjToAcquireIsRoll(objType)) return GetAcquisitionRollOptionsParameter();
        else if (GBMSAPIJavaWrapperDefinesScannableBiometricType.ObjToAcquireIsSlapOrJoint(objType)) return GetAcquisitionSlapOptionsParameter();
        return 0;
    }

    /////////////////////////////////////
    // WSQ
    /////////////////////////////////////
    public static WsqJavaWrapperLibrary WSQ_Jw;
    public static final String WsqFileName = "LastAcquiredWsq";

    /////////////////////////////////////
    // JPEG
    /////////////////////////////////////
    public static GbjpegJavaWrapperLibrary Jpeg_Jw;
    public static final String JpegFileName = "LastAcquiredJpeg";
    public static final String Jpeg2FileName = "LastAcquiredJpeg2";

    /////////////////////////////////////
    // GBFRSW (DactyMatch
    /////////////////////////////////////
    public static GbfrswJavaWrapperLibrary GBFRSW_Jw;
    public static int TemplateFileNumber = 0;
    public static double MatchingThreshold = 10000;
    public static int SpeedVsPrecisionTradeoff = GbfrswJavaWrapperDefinesSpeedPrecisionTradeoff.GBFRSW_TRADEOFF_ROBUST;
    public static int MatchRotationAngle = 50;

    public static String GetTemplateFileName(String personName)
    {
        String valToRet = "Template_" + personName + "_" + TemplateFileNumber;
        TemplateFileNumber ++;
        return valToRet;
    }

    /////////////////////////////////////
    // GBFINIMG
    /////////////////////////////////////
    public static GbfinimgJavaWrapperLibrary GBFINIMG_Jw;
    public static boolean GbfinimgLibLoaded = false;

    /////////////////////////////////////
    // ANSI NIST
    /////////////////////////////////////
    public static Gban2000JavaWrapperLibrary AN2000_Jw;
    public static boolean An2000LibLoaded = false;

    /////////////////////////////////////
    // GBFIR
    /////////////////////////////////////
    public static GbfirJavaWrapperLibrary GBFIR_Jw;
    public static boolean GbfirLibLoaded = false;

    /////////////////////////////////////
    // GBNFIQ
    /////////////////////////////////////
    public static GbNfiqJavaWrapperLibrary GBNFIQ_Jw;
    public static boolean GbNfiqLibLoaded = false;

    /////////////////////////////////////
    // GBNFIQ2
    /////////////////////////////////////
    public static GbNfiq2JavaWrapperLibrary GBNFIQ2_Jw;
    public static boolean GbNfiq2LibLoaded = false;

    /////////////////////////////////////
    // LFS
    /////////////////////////////////////
    public static LfsJavaWrapperLibrary LFS_Jw;
    public static boolean LfsLibLoaded = false;

    /////////////////////////////////////
    // BOZORTH
    /////////////////////////////////////
    public static BozorthJavaWrapperLibrary BOZORTH_Jw;
    public static boolean BozorthLibLoaded = false;
}
