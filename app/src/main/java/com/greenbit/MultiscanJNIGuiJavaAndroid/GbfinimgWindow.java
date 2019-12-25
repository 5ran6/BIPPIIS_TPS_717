package com.greenbit.MultiscanJNIGuiJavaAndroid;

import android.content.Intent;
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

import com.greenbit.MultiscanJNIGuiJavaAndroid.utils.Tools;
import com.greenbit.ansinistitl.GBAN2011JavaWrapperDefinesApplicationProfileStruct;
import com.greenbit.ansinistitl.GBAN2011JavaWrapperDefinesCharacterEncoding;
import com.greenbit.ansinistitl.GBAN2011JavaWrapperDefinesCompressionCodes;
import com.greenbit.ansinistitl.GBAN2011JavaWrapperDefinesDateStruct;
import com.greenbit.ansinistitl.GBAN2011JavaWrapperDefinesDeviceMonitoringMode;
import com.greenbit.ansinistitl.GBAN2011JavaWrapperDefinesFAPCodes;
import com.greenbit.ansinistitl.GBAN2011JavaWrapperDefinesFingerOrSegmentQualityMetricStruct;
import com.greenbit.ansinistitl.GBAN2011JavaWrapperDefinesFingerPositions;
import com.greenbit.ansinistitl.GBAN2011JavaWrapperDefinesFingerSegmentPositionStruct;
import com.greenbit.ansinistitl.GBAN2011JavaWrapperDefinesFrictionRidgeImpressionTypes;
import com.greenbit.ansinistitl.GBAN2011JavaWrapperDefinesGmtStruct;
import com.greenbit.ansinistitl.GBAN2011JavaWrapperDefinesNistQualityMetricStruct;
import com.greenbit.ansinistitl.GBANJavaWrapperDefinesANStruct;
import com.greenbit.ansinistitl.GBANJavaWrapperDefinesAnsinistVersions;
import com.greenbit.ansinistitl.GBANJavaWrapperDefinesCompressionAlgorithmsStrings;
import com.greenbit.ansinistitl.GBANJavaWrapperDefinesFingerPositions;
import com.greenbit.ansinistitl.GBANJavaWrapperDefinesFingerSegmentsBoundsDescription;
import com.greenbit.ansinistitl.GBANJavaWrapperDefinesFingersAlternateQualityDescription;
import com.greenbit.ansinistitl.GBANJavaWrapperDefinesFingersNfiqDescription;
import com.greenbit.ansinistitl.GBANJavaWrapperDefinesPalmPositions;
import com.greenbit.ansinistitl.GBANJavaWrapperDefinesPalmQualityAlgorithms;
import com.greenbit.ansinistitl.GBANJavaWrapperDefinesRecordStruct;
import com.greenbit.ansinistitl.GBANJavaWrapperDefinesRecordTypes;
import com.greenbit.ansinistitl.GBANJavaWrapperDefinesReturnCodes;
import com.greenbit.gbfinimg.GbfinimgJavaWrapperDefineSegmentImageDescriptor;
import com.greenbit.gbfinimg.GbfinimgJavaWrapperDefinesFingerNames;
import com.greenbit.gbfinimg.GbfinimgJavaWrapperDefinesHandNames;
import com.greenbit.gbfinimg.GbfinimgJavaWrapperDefinesInputImageType;
import com.greenbit.gbfinimg.GbfinimgJavaWrapperDefinesProcessOptions;
import com.greenbit.gbfinimg.GbfinimgJavaWrapperDefinesReturnCodes;
import com.greenbit.usbPermission.IGreenbitLogger;
import com.greenbit.utils.GBJavaWrapperUtilByteArrayForJavaToCExchange;
import com.greenbit.utils.GBJavaWrapperUtilIntForJavaToCExchange;

import java.util.ArrayList;
import java.util.Calendar;

import static com.greenbit.ansinistitl.GBANJavaWrapperDefinesImpressionCodes.EFTS_14_LIVE_SCAN_PLAN;

public class GbfinimgWindow extends AppCompatActivity implements IGreenbitLogger {
    private Button bRefresh, bGbfinimgProcess;
    private Spinner comboFilesList, comboGbfinimgObjectType;
    private ArrayList<String> FilesList;
    private ImageView FrameView;
    private boolean LoggerBitmapChanged;
    private ArrayList<GbExampleGrayScaleBitmapClass> LoggerBitmapList;

    public int[] GbfinimgObjTypesArray =
            {
                    GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_NO_TYPE,
                    GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_4,
                    GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_4,
                    GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_2,
                    GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_2,
                    GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_THUMBS_2,
                    GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_SINGLE_FINGER,
                    GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_SINGLE_FINGER_FLAT,
                    GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_HALF_PALM,
                    GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_HALF_PALM,
                    GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_HALF_PALM_UPPER,
                    GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_HALF_PALM_UPPER,
                    GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_WRITER_PALM,
                    GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_WRITER_PALM,
                    GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_ROLLED_THENAR,
                    GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_ROLLED_THENAR,
                    GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_ROLLED_TIP,
                    GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_ROLLED_JOINT_FINGER_FV1,
                    GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_FLAT_JOINT_FINGER_FV2,
                    GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_FLAT_JOINT_FINGER_FV3,
                    GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_FLAT_JOINT_FINGER_FV4
            };

    private void LogBitmap(GbExampleGrayScaleBitmapClass bmp) {
        LoggerBitmapList.add(bmp);
        LoggerBitmapChanged = true;
    }

    private void InitBmpLog() {
        LoggerBitmapList = new ArrayList<GbExampleGrayScaleBitmapClass>();
        LoggerBitmapChanged = false;
    }

    protected void LogTimer() {
        if (LoggerBitmapChanged) {
            try {
                DisplayMetrics metrics = new DisplayMetrics();
                getWindowManager().getDefaultDisplay().getMetrics(metrics);
                for (GbExampleGrayScaleBitmapClass GbBmp : LoggerBitmapList) {
                    Bitmap bmp = GbBmp.GetBmp();
                    if (bmp != null) {

                        float scaleWidth = metrics.scaledDensity / 8;

                        //set image in imageView
                        FrameView.setImageBitmap(bmp);

                        //set imageView dynamic width and height
                        FrameView.setMaxWidth((int) scaleWidth);
                        FrameView.setMaxHeight((int) scaleWidth);
                        FrameView.setMinimumWidth((int) scaleWidth);
                        FrameView.setMinimumHeight((int) scaleWidth);
                    } else {
                        LogAsDialog("LogTimer: null bmp");
                    }
                }
            } catch (Exception ex) {
                LogAsDialog("LogBmp exc: " + ex.getMessage());
            }
            LoggerBitmapList.clear();
            LoggerBitmapChanged = false;
        }
        new Handler().postDelayed(new Runnable() {
            @Override
            public void run() {
                LogTimer();
            }
        }, 10);
    }

    protected void StartLogTimer() {
        new Handler().postDelayed(new Runnable() {
            @Override
            public void run() {
                LogTimer();
            }
        }, 10);
    }

    @Override
    public void LogOnScreen(String strToLog) {
        //GB_AcquisitionOptionsGlobals.CreateDialogNeutral(strToLog, this);
    }

    public void LogAsDialog(String logStr) {
        Tools.toast(logStr, GbfinimgWindow.this);
        //GB_AcquisitionOptionsGlobals.CreateDialogNeutral(logStr, this);
    }

    private IGreenbitLogger GetThis() {
        return this;
    }

    private void onRefresh() {
        FilesList.clear();
        FilesList = GbExampleGrayScaleBitmapClass.GbBmpLoadListOfRawImagesWithSize();

        ArrayAdapter<String> FilesListAdapter = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item, FilesList);
        FilesListAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        comboFilesList.setAdapter(FilesListAdapter);
    }

    private void ManageGbfinimgErrors(String fName, int RetVal, boolean ShowAsDialog) {
        if (RetVal != GbfinimgJavaWrapperDefinesReturnCodes.GBFINIMG_NO_ERROR) {
            String errorToLog = fName + ": " + GB_AcquisitionOptionsGlobals.GBFINIMG_Jw.GetLastErrorString();
            if (ShowAsDialog) LogAsDialog(errorToLog);
            else LogAsDialog(errorToLog);
        }
    }

    public static int GetAN1415RecordTypeFromGbfinimgObjType(int ObjType) {
        switch (ObjType) {
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_FLAT_JOINT_FINGER_FV2:
                return GBANJavaWrapperDefinesRecordTypes.TYPE_14_ID;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_FLAT_JOINT_FINGER_FV3:
                return GBANJavaWrapperDefinesRecordTypes.TYPE_14_ID;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_FLAT_JOINT_FINGER_FV4:
                return GBANJavaWrapperDefinesRecordTypes.TYPE_14_ID;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_2:
                return GBANJavaWrapperDefinesRecordTypes.TYPE_14_ID;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_4:
                return GBANJavaWrapperDefinesRecordTypes.TYPE_14_ID;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_HALF_PALM:
                return GBANJavaWrapperDefinesRecordTypes.TYPE_15_ID;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_HALF_PALM_UPPER:
                return GBANJavaWrapperDefinesRecordTypes.TYPE_15_ID;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_ROLLED_THENAR:
                return GBANJavaWrapperDefinesRecordTypes.TYPE_15_ID;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_WRITER_PALM:
                return GBANJavaWrapperDefinesRecordTypes.TYPE_15_ID;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_NO_TYPE:
                return -1;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_2:
                return GBANJavaWrapperDefinesRecordTypes.TYPE_14_ID;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_4:
                return GBANJavaWrapperDefinesRecordTypes.TYPE_14_ID;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_HALF_PALM:
                return GBANJavaWrapperDefinesRecordTypes.TYPE_15_ID;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_HALF_PALM_UPPER:
                return GBANJavaWrapperDefinesRecordTypes.TYPE_15_ID;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_WRITER_PALM:
                return GBANJavaWrapperDefinesRecordTypes.TYPE_15_ID;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_ROLLED_THENAR:
                return GBANJavaWrapperDefinesRecordTypes.TYPE_15_ID;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_ROLLED_JOINT_FINGER_FV1:
                return GBANJavaWrapperDefinesRecordTypes.TYPE_14_ID;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_ROLLED_TIP:
                return GBANJavaWrapperDefinesRecordTypes.TYPE_14_ID;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_SINGLE_FINGER:
                return GBANJavaWrapperDefinesRecordTypes.TYPE_14_ID;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_SINGLE_FINGER_FLAT:
                return GBANJavaWrapperDefinesRecordTypes.TYPE_14_ID;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_THUMBS_2:
                return GBANJavaWrapperDefinesRecordTypes.TYPE_14_ID;
            default:
                return -1;
        }
    }

    public static String GetAN1415FingerPositionStringFromGbfinimgObjType(int ObjType) {
        switch (ObjType) {
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_FLAT_JOINT_FINGER_FV2:
                return GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_FLAT_JOINT_FINGER_FV3:
                return GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_FLAT_JOINT_FINGER_FV4:
                return GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_2:
                return GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_4:
                return GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_LEFT_4_FING;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_HALF_PALM:
                return GBANJavaWrapperDefinesPalmPositions.EFTS_15_PLP_LEFT_LOWER_PALM;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_HALF_PALM_UPPER:
                return GBANJavaWrapperDefinesPalmPositions.EFTS_15_PLP_LEFT_UPPER_PALM;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_ROLLED_THENAR:
                return GBANJavaWrapperDefinesPalmPositions.EFTS_15_PLP_LEFT_THENAR;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_WRITER_PALM:
                return GBANJavaWrapperDefinesPalmPositions.EFTS_15_PLP_LEFT_WRITER_PALM;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_NO_TYPE:
                return GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_2:
                return GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_4:
                return GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_RIGHT_4_FING;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_HALF_PALM:
                return GBANJavaWrapperDefinesPalmPositions.EFTS_15_PLP_RIGTH_LOWER_PALM;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_HALF_PALM_UPPER:
                return GBANJavaWrapperDefinesPalmPositions.EFTS_15_PLP_RIGHT_UPPER_PALM;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_WRITER_PALM:
                return GBANJavaWrapperDefinesPalmPositions.EFTS_15_PLP_RIGHT_WRITER_PALM;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_ROLLED_THENAR:
                return GBANJavaWrapperDefinesPalmPositions.EFTS_15_PLP_RIGHT_THENAR;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_ROLLED_JOINT_FINGER_FV1:
                return GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_ROLLED_TIP:
                return GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_SINGLE_FINGER:
                return GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_LEFT_INDEX;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_SINGLE_FINGER_FLAT:
                return GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_LEFT_INDEX;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_THUMBS_2:
                return GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_2_THUMBS;
            default:
                return GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_UNKNOWN;
        }
    }

    // valid for slaps only
    public static String GetANSegmentPositionFromObjTypeAndSegmentDescriptor(
            int ObjType, GbfinimgJavaWrapperDefineSegmentImageDescriptor descriptor,
            boolean isSingleFinger) {
        String ValToRet;
        if (isSingleFinger) return GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_LEFT_INDEX;
        if (descriptor.Hand == GbfinimgJavaWrapperDefinesHandNames.GBFINIMG_HAND_LEFT) {
            if (descriptor.Finger == GbfinimgJavaWrapperDefinesFingerNames.GBFINIMG_FINGER_INDEX) {
                ValToRet = GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_LEFT_INDEX;
            } else if (descriptor.Finger == GbfinimgJavaWrapperDefinesFingerNames.GBFINIMG_FINGER_LITTLE) {
                ValToRet = GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_LEFT_LITTLE;
            } else if (descriptor.Finger == GbfinimgJavaWrapperDefinesFingerNames.GBFINIMG_FINGER_MIDDLE) {
                ValToRet = GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_LEFT_MIDDLE;
            } else if (descriptor.Finger == GbfinimgJavaWrapperDefinesFingerNames.GBFINIMG_FINGER_RING) {
                ValToRet = GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_LEFT_RING;
            } else if (descriptor.Finger == GbfinimgJavaWrapperDefinesFingerNames.GBFINIMG_FINGER_THUMB) {
                ValToRet = GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_LEFT_THUMB;
            } else {
                return GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_UNKNOWN;
            }
        } else if (descriptor.Hand == GbfinimgJavaWrapperDefinesHandNames.GBFINIMG_HAND_RIGHT) {
            if (descriptor.Finger == GbfinimgJavaWrapperDefinesFingerNames.GBFINIMG_FINGER_INDEX) {
                ValToRet = GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_RIGHT_INDEX;
            } else if (descriptor.Finger == GbfinimgJavaWrapperDefinesFingerNames.GBFINIMG_FINGER_LITTLE) {
                ValToRet = GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_RIGHT_LITTLE;
            } else if (descriptor.Finger == GbfinimgJavaWrapperDefinesFingerNames.GBFINIMG_FINGER_MIDDLE) {
                ValToRet = GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_RIGHT_MIDDLE;
            } else if (descriptor.Finger == GbfinimgJavaWrapperDefinesFingerNames.GBFINIMG_FINGER_RING) {
                ValToRet = GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_RIGHT_RING;
            } else if (descriptor.Finger == GbfinimgJavaWrapperDefinesFingerNames.GBFINIMG_FINGER_THUMB) {
                ValToRet = GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_RIGHT_THUMB;
            } else {
                return GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_UNKNOWN;
            }
        } else {
            return GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_UNKNOWN;
        }
        switch (ObjType) {
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_FLAT_JOINT_FINGER_FV2:
                return GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_FLAT_JOINT_FINGER_FV3:
                return GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_FLAT_JOINT_FINGER_FV4:
                return GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_2:
                return ValToRet;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_4:
                return ValToRet;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_HALF_PALM:
                return GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_HALF_PALM_UPPER:
                return GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_ROLLED_THENAR:
                return GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_WRITER_PALM:
                return GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_NO_TYPE:
                return GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_2:
                return ValToRet;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_4:
                return ValToRet;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_HALF_PALM:
                return GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_HALF_PALM_UPPER:
                return GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_WRITER_PALM:
                return GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_ROLLED_THENAR:
                return GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_ROLLED_JOINT_FINGER_FV1:
                return GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_ROLLED_TIP:
                return GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_SINGLE_FINGER:
                return GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_SINGLE_FINGER_FLAT:
                return GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_THUMBS_2:
                return ValToRet;
            default:
                return GBANJavaWrapperDefinesFingerPositions.EFTS_14_FGP_UNKNOWN;
        }
    }

    public static int GetAN1415FingerPositionIntFromGbfinimgObjType(int ObjType) {
        switch (ObjType) {
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_FLAT_JOINT_FINGER_FV2:
                return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_FLAT_JOINT_FINGER_FV3:
                return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_FLAT_JOINT_FINGER_FV4:
                return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_2:
                return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_LEFT_INDEX_MIDDLE;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_4:
                return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_PLAIN_LEFT_SLAP_4;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_HALF_PALM:
                return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_LEFT_LOWER_PALM;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_HALF_PALM_UPPER:
                return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_LEFT_UPPER_PALM;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_ROLLED_THENAR:
                return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_LEFT_THENAR;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_WRITER_PALM:
                return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_LEFT_WRITERS_PALM;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_NO_TYPE:
                return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_2:
                return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_RIGHT_INDEX_MIDDLE;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_4:
                return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_PLAIN_RIGHT_SLAP_4;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_HALF_PALM:
                return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_RIGHT_LOWER_PALM;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_HALF_PALM_UPPER:
                return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_RIGHT_UPPER_PALM;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_WRITER_PALM:
                return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_RIGHT_WRITERS_PALM;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_ROLLED_THENAR:
                return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_RIGHT_THENAR;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_ROLLED_JOINT_FINGER_FV1:
                return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_ROLLED_TIP:
                return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_SINGLE_FINGER:
                return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_LEFT_INDEX;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_SINGLE_FINGER_FLAT:
                return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_LEFT_INDEX;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_THUMBS_2:
                return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_PLAIN_LEFT_THUMBS_2;
            default:
                return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_UNKNOWN;
        }
    }

    public static boolean GbfinimgObjIsSingleFinger(int ObjType) {
        switch (ObjType) {
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_FLAT_JOINT_FINGER_FV2:
                return true;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_FLAT_JOINT_FINGER_FV3:
                return true;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_FLAT_JOINT_FINGER_FV4:
                return true;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_ROLLED_JOINT_FINGER_FV1:
                return true;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_ROLLED_TIP:
                return true;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_SINGLE_FINGER:
                return true;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_SINGLE_FINGER_FLAT:
                return true;
            default:
                return false;
        }
    }

    public static int GetAN2011SegmentPositionFromObjTypeAndSegmentDescriptor(
            int ObjType, GbfinimgJavaWrapperDefineSegmentImageDescriptor descriptor,
            boolean isSingleFinger) {
        int ValToRet;
        if (isSingleFinger)
            return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_LEFT_INDEX;
        if (descriptor.Hand == GbfinimgJavaWrapperDefinesHandNames.GBFINIMG_HAND_LEFT) {
            if (descriptor.Finger == GbfinimgJavaWrapperDefinesFingerNames.GBFINIMG_FINGER_INDEX) {
                ValToRet = GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_LEFT_INDEX;
            } else if (descriptor.Finger == GbfinimgJavaWrapperDefinesFingerNames.GBFINIMG_FINGER_LITTLE) {
                ValToRet = GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_LEFT_LITTLE;
            } else if (descriptor.Finger == GbfinimgJavaWrapperDefinesFingerNames.GBFINIMG_FINGER_MIDDLE) {
                ValToRet = GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_LEFT_MIDDLE;
            } else if (descriptor.Finger == GbfinimgJavaWrapperDefinesFingerNames.GBFINIMG_FINGER_RING) {
                ValToRet = GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_LEFT_RING;
            } else if (descriptor.Finger == GbfinimgJavaWrapperDefinesFingerNames.GBFINIMG_FINGER_THUMB) {
                ValToRet = GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_LEFT_THUMB;
            } else {
                return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_UNKNOWN;
            }
        } else if (descriptor.Hand == GbfinimgJavaWrapperDefinesHandNames.GBFINIMG_HAND_RIGHT) {
            if (descriptor.Finger == GbfinimgJavaWrapperDefinesFingerNames.GBFINIMG_FINGER_INDEX) {
                ValToRet = GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_RIGHT_INDEX;
            } else if (descriptor.Finger == GbfinimgJavaWrapperDefinesFingerNames.GBFINIMG_FINGER_LITTLE) {
                ValToRet = GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_RIGHT_LITTLE;
            } else if (descriptor.Finger == GbfinimgJavaWrapperDefinesFingerNames.GBFINIMG_FINGER_MIDDLE) {
                ValToRet = GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_RIGHT_MIDDLE;
            } else if (descriptor.Finger == GbfinimgJavaWrapperDefinesFingerNames.GBFINIMG_FINGER_RING) {
                ValToRet = GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_RIGHT_RING;
            } else if (descriptor.Finger == GbfinimgJavaWrapperDefinesFingerNames.GBFINIMG_FINGER_THUMB) {
                ValToRet = GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_RIGHT_THUMB;
            } else {
                return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_UNKNOWN;
            }
        } else {
            return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_UNKNOWN;
        }
        switch (ObjType) {
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_FLAT_JOINT_FINGER_FV2:
                return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_FLAT_JOINT_FINGER_FV3:
                return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_FLAT_JOINT_FINGER_FV4:
                return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_2:
                return ValToRet;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_4:
                return ValToRet;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_HALF_PALM:
                return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_HALF_PALM_UPPER:
                return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_ROLLED_THENAR:
                return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_WRITER_PALM:
                return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_NO_TYPE:
                return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_2:
                return ValToRet;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_4:
                return ValToRet;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_HALF_PALM:
                return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_HALF_PALM_UPPER:
                return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_WRITER_PALM:
                return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_ROLLED_THENAR:
                return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_ROLLED_JOINT_FINGER_FV1:
                return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_ROLLED_TIP:
                return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_SINGLE_FINGER:
                return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_SINGLE_FINGER_FLAT:
                return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_UNKNOWN;
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_THUMBS_2:
                return ValToRet;
            default:
                return GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_UNKNOWN;
        }
    }

    public void SaveAN2011File(
            String fName,
            GbExampleGrayScaleBitmapClass GbBmp,
            int ObjType,
            GbfinimgJavaWrapperDefineSegmentImageDescriptor[] descriptors,
            int SegmentsNum
    ) {
        if (SegmentsNum <= 0) {
            LogAsDialog("SaveAN2011File: no input segments");
            return;
        }
        try {
            GBANJavaWrapperDefinesANStruct AnsiNistStruct;
            AnsiNistStruct = new GBANJavaWrapperDefinesANStruct();
            int RetVal;
            /*****************************
             * create ansi nist struct
             */
            GBAN2011JavaWrapperDefinesDateStruct DateStr = new GBAN2011JavaWrapperDefinesDateStruct();
            Calendar currentTime = Calendar.getInstance();
            DateStr.Build(currentTime.getTime().getYear(), currentTime.getTime().getMonth(), currentTime.getTime().getDay());
            GBAN2011JavaWrapperDefinesGmtStruct GmtStr = new GBAN2011JavaWrapperDefinesGmtStruct();
            GmtStr.Build(currentTime.getTime().getYear(), currentTime.getTime().getMonth(), currentTime.getTime().getDay(),
                    currentTime.getTime().getHours(), currentTime.getTime().getMinutes(), currentTime.getTime().getSeconds());
            GBAN2011JavaWrapperDefinesApplicationProfileStruct[] Aps = new GBAN2011JavaWrapperDefinesApplicationProfileStruct[2];
            Aps[0] = new GBAN2011JavaWrapperDefinesApplicationProfileStruct(
                    "AppProfOrg0", "AppProfName0", "AppProfVer0"
            );
            Aps[1] = new GBAN2011JavaWrapperDefinesApplicationProfileStruct(
                    "AppProfOrg1", "AppProfName1", "AppProfVer1"
            );
            RetVal = GB_AcquisitionOptionsGlobals.AN2000_Jw.CreateAnsiNist2011(
                    AnsiNistStruct,
                    "GreenbitTest",
                    DateStr,
                    1,
                    "DestAg", "OrigAg",
                    "TcId01",
                    "TcRef01",
                    0, 0,
                    "DomNameID", "DomVer",
                    GmtStr,
                    GBAN2011JavaWrapperDefinesCharacterEncoding.GB_TABLE_AN2011_4_CHAR_ENCODING_INDEX_ASCII,
                    GBAN2011JavaWrapperDefinesCharacterEncoding.GB_TABLE_AN2011_4_CHAR_ENCODING_NAME_ASCII,
                    "CharEncVer",
                    Aps,
                    "DestAgName01", "OrigAgName01"
            );
            if (RetVal != GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_NO_ERROR) {
                throw new Exception("CreateAnsiNist: " + GB_AcquisitionOptionsGlobals.AN2000_Jw.GetLastErrorString());
            }
            /*****************************
             * create Greenbit record 2
             */
            GbAndroidExampleRecord2Class Rec2 = new GbAndroidExampleRecord2Class();
            GBAN2011JavaWrapperDefinesDateStruct JohnSmithDob = new GBAN2011JavaWrapperDefinesDateStruct();
            JohnSmithDob.Build(1982, 12, 25);
            Rec2.Create(GB_AcquisitionOptionsGlobals.AN2000_Jw, -1, "John", "Smith",
                    JohnSmithDob);
            RetVal = GB_AcquisitionOptionsGlobals.AN2000_Jw.AnEngineInsertRecordIntoAnsiNistStruct(AnsiNistStruct, Rec2,
                    AnsiNistStruct.GetNumRecords(GB_AcquisitionOptionsGlobals.AN2000_Jw), 1);
            if (RetVal != GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_NO_ERROR) {
                GB_AcquisitionOptionsGlobals.AN2000_Jw.FreeAnsiNistAllocatedMemory(AnsiNistStruct);
                throw new Exception("InsertRecordIntoAnsiNistStruct: " + GB_AcquisitionOptionsGlobals.AN2000_Jw.GetLastErrorString());
            }
            /*****************************
             * create record
             */
            GBANJavaWrapperDefinesRecordStruct Record = new GBANJavaWrapperDefinesRecordStruct();
            int RecType = GetAN1415RecordTypeFromGbfinimgObjType(ObjType);
            boolean isSingleFinger = GbfinimgObjIsSingleFinger(ObjType);
            if (RecType == GBANJavaWrapperDefinesRecordTypes.TYPE_14_ID) {
                if (SegmentsNum > 0) {
                    // segments
                    GBAN2011JavaWrapperDefinesFingerSegmentPositionStruct[] seg = new GBAN2011JavaWrapperDefinesFingerSegmentPositionStruct[SegmentsNum];
                    for (int i = 0; i < SegmentsNum; i++) {
                        // segments
                        seg[i] = new GBAN2011JavaWrapperDefinesFingerSegmentPositionStruct();
                        seg[i].Build(
                                GetAN2011SegmentPositionFromObjTypeAndSegmentDescriptor(ObjType, descriptors[i], isSingleFinger),
                                descriptors[i].BoundingBoxL,
                                descriptors[i].BoundingBoxR,
                                descriptors[i].BoundingBoxT,
                                descriptors[i].BoundingBoxB
                        );
                    }
                    // NIST quality
                    GBAN2011JavaWrapperDefinesNistQualityMetricStruct[] Nqm = new GBAN2011JavaWrapperDefinesNistQualityMetricStruct[SegmentsNum];
                    for (int i = 0; i < SegmentsNum; i++) {
                        Nqm[i] = new GBAN2011JavaWrapperDefinesNistQualityMetricStruct();
                        Nqm[i].Build(
                                GetAN2011SegmentPositionFromObjTypeAndSegmentDescriptor(ObjType, descriptors[i], isSingleFinger),
                                (6 - ((descriptors[i].Quality + 20) / 20))
                        );
                        if (Nqm[i].Quality > 5) Nqm[i].Quality = 5;
                        if (Nqm[i].Quality < 1) Nqm[i].Quality = 1;
                    }
                    // call
                    RetVal = GB_AcquisitionOptionsGlobals.AN2000_Jw.CreateType14Record2011(
                            Record, -1,
                            GBAN2011JavaWrapperDefinesFrictionRidgeImpressionTypes.GB_TABLE_AN2011_7_FRIT_LS_OPT_CONTACT_PLAIN, "SourceAg",
                            DateStr,
                            GbBmp.sx, GbBmp.sy,
                            1, 500, 500,
                            GBAN2011JavaWrapperDefinesCompressionCodes.GB_TABLE_AN2011_15_CODE_UNCOMPRESSED, 8,
                            GetAN1415FingerPositionIntFromGbfinimgObjType(ObjType),
                            null, null,
                            500, 500,
                            null, "Comment", seg, Nqm, null, null,
                            null, 10, 1,
                            GBAN2011JavaWrapperDefinesDeviceMonitoringMode.GB_TABLE_AN2011_5_DMM_ASSISTED,
                            GBAN2011JavaWrapperDefinesFAPCodes.GB_TABLE_AN2011_12_FAP_45,
                            GbBmp.bytes
                    );
                    if (RetVal != GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_NO_ERROR) {
                        throw new Exception("CreateType14Record2011: " + GB_AcquisitionOptionsGlobals.AN2000_Jw.GetLastErrorString());
                    }
                }
            } else if (RecType == GBANJavaWrapperDefinesRecordTypes.TYPE_15_ID) {
                if (SegmentsNum > 0) {
                    GBAN2011JavaWrapperDefinesFingerOrSegmentQualityMetricStruct[] Pqm = new GBAN2011JavaWrapperDefinesFingerOrSegmentQualityMetricStruct[2];
                    Pqm[0] = new GBAN2011JavaWrapperDefinesFingerOrSegmentQualityMetricStruct();
                    Pqm[0].AlgorithmProductId = 12000;
                    Pqm[0].AlgorithmVendorId = 13000;
                    Pqm[0].FrictionRidgePosition = GetAN1415FingerPositionIntFromGbfinimgObjType(ObjType);
                    Pqm[0].QualityValue = 90;
                    Pqm[1] = new GBAN2011JavaWrapperDefinesFingerOrSegmentQualityMetricStruct();
                    Pqm[1].AlgorithmProductId = 12001;
                    Pqm[1].AlgorithmVendorId = 13001;
                    Pqm[1].FrictionRidgePosition = GetAN1415FingerPositionIntFromGbfinimgObjType(ObjType);
                    Pqm[1].QualityValue = 91;
                    RetVal = GB_AcquisitionOptionsGlobals.AN2000_Jw.CreateType15Record2011(
                            Record, -1,
                            GBAN2011JavaWrapperDefinesFrictionRidgeImpressionTypes.GB_TABLE_AN2011_7_FRIT_LS_UNKNOWN_PALM,
                            "SourceAg",
                            DateStr,
                            GbBmp.sx, GbBmp.sy,
                            1, 500, 500,
                            GBAN2011JavaWrapperDefinesCompressionCodes.GB_TABLE_AN2011_15_CODE_UNCOMPRESSED, 8,
                            GetAN1415FingerPositionIntFromGbfinimgObjType(ObjType),
                            500, 500,
                            null,
                            "Comment",
                            Pqm,
                            GBAN2011JavaWrapperDefinesDeviceMonitoringMode.GB_TABLE_AN2011_5_DMM_ASSISTED,
                            GbBmp.bytes
                    );
                    if (RetVal != GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_NO_ERROR) {
                        GB_AcquisitionOptionsGlobals.AN2000_Jw.FreeAnsiNistAllocatedMemory(AnsiNistStruct);
                        throw new Exception("CreateType15Record2011: " + GB_AcquisitionOptionsGlobals.AN2000_Jw.GetLastErrorString());
                    }
                }
            } else {
                GB_AcquisitionOptionsGlobals.AN2000_Jw.FreeAnsiNistAllocatedMemory(AnsiNistStruct);
                throw new Exception("Record Type not available: " + RecType);
            }

            /******************************************
             * insert record into struct
             */
            RetVal = GB_AcquisitionOptionsGlobals.AN2000_Jw.AnEngineInsertRecordIntoAnsiNistStruct(AnsiNistStruct, Record,
                    AnsiNistStruct.GetNumRecords(GB_AcquisitionOptionsGlobals.AN2000_Jw), 1);
            if (RetVal != GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_NO_ERROR) {
                GB_AcquisitionOptionsGlobals.AN2000_Jw.FreeAnsiNistAllocatedMemory(AnsiNistStruct);
                throw new Exception("InsertRecordIntoAnsiNistStruct: " + GB_AcquisitionOptionsGlobals.AN2000_Jw.GetLastErrorString());
            }
            /***********************************
             * Create AN2000 buffer and free
             * allocated memory
             */
            GBJavaWrapperUtilByteArrayForJavaToCExchange OutBuffer = new GBJavaWrapperUtilByteArrayForJavaToCExchange();
            RetVal = GB_AcquisitionOptionsGlobals.AN2000_Jw.WriteAnsiNistToBuffer(
                    AnsiNistStruct,
                    OutBuffer
            );
            GB_AcquisitionOptionsGlobals.AN2000_Jw.FreeAnsiNistAllocatedMemory(AnsiNistStruct);
            if (RetVal != GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_NO_ERROR) {
                throw new Exception("WriteAnsiNistToBuffer: " + GB_AcquisitionOptionsGlobals.AN2000_Jw.GetLastErrorString());
            }
            /***********************************
             * Save into file
             */
            GbExampleGrayScaleBitmapClass.SaveGenericBinaryFile(fName,
                    GetThis(), "An2011", OutBuffer.Get());
        } catch (Exception ex) {
            LogAsDialog("SaveAN2011File: " + ex.getMessage());
            return;
        }
    }

    public void SaveAN2000File(
            String fName,
            GbExampleGrayScaleBitmapClass GbBmp,
            int ObjType,
            GbfinimgJavaWrapperDefineSegmentImageDescriptor[] descriptors,
            int SegmentsNum
    ) {
        if (SegmentsNum <= 0) {
            LogAsDialog("SaveAN2000File: no input segments");
            return;
        }
        try {
            GBANJavaWrapperDefinesANStruct AnsiNistStruct;
            AnsiNistStruct = new GBANJavaWrapperDefinesANStruct();
            int RetVal;
            /*****************************
             * create ansi nist struct
             */
            RetVal = GB_AcquisitionOptionsGlobals.AN2000_Jw.CreateAnsiNist(
                    AnsiNistStruct,
                    GBANJavaWrapperDefinesAnsinistVersions.VERSION_0300,
                    "TransType",
                    1,
                    "DestAgency",
                    "OrigAgency",
                    "TransContID0001",
                    "TransContRef0001",
                    90, 90,
                    "GreenbitDomain"
            );
            if (RetVal != GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_NO_ERROR) {
                throw new Exception("CreateAnsiNist: " + GB_AcquisitionOptionsGlobals.AN2000_Jw.GetLastErrorString());
            }
            /*****************************
             * create record
             */
            GBANJavaWrapperDefinesRecordStruct Record = new GBANJavaWrapperDefinesRecordStruct();
            int RecType = GetAN1415RecordTypeFromGbfinimgObjType(ObjType);
            if (RecType == GBANJavaWrapperDefinesRecordTypes.TYPE_14_ID) {
                GBANJavaWrapperDefinesFingerSegmentsBoundsDescription[] SegmentsList = null;
                GBANJavaWrapperDefinesFingersNfiqDescription[] NfiqList = null;
                GBANJavaWrapperDefinesFingersAlternateQualityDescription[] SegmQualityList = null;
                GBANJavaWrapperDefinesFingersAlternateQualityDescription[] AltQualityList = null;
                boolean isSingleFinger = GbfinimgObjIsSingleFinger(ObjType);
                if (SegmentsNum > 0) {
                    SegmentsList = new GBANJavaWrapperDefinesFingerSegmentsBoundsDescription[SegmentsNum];
                    NfiqList = new GBANJavaWrapperDefinesFingersNfiqDescription[SegmentsNum];
                    SegmQualityList = new GBANJavaWrapperDefinesFingersAlternateQualityDescription[SegmentsNum];
                    AltQualityList = new GBANJavaWrapperDefinesFingersAlternateQualityDescription[SegmentsNum];
                    for (int i = 0; i < SegmentsNum; i++) {
                        // segments
                        SegmentsList[i] = new GBANJavaWrapperDefinesFingerSegmentsBoundsDescription();
                        SegmentsList[i].Build(
                                GetANSegmentPositionFromObjTypeAndSegmentDescriptor(ObjType, descriptors[i], isSingleFinger),
                                "" + descriptors[i].BoundingBoxL,
                                "" + descriptors[i].BoundingBoxR,
                                "" + descriptors[i].BoundingBoxT,
                                "" + descriptors[i].BoundingBoxB
                        );
                        // nfiq quality
                        NfiqList[i] = new GBANJavaWrapperDefinesFingersNfiqDescription();
                        NfiqList[i].Build(
                                GetANSegmentPositionFromObjTypeAndSegmentDescriptor(ObjType, descriptors[i], isSingleFinger),
                                "" + (6 - (descriptors[i].Quality) / 20)
                        );
                        // greenbit quality
                        SegmQualityList[i] = new GBANJavaWrapperDefinesFingersAlternateQualityDescription();
                        SegmQualityList[i].Build(
                                GetANSegmentPositionFromObjTypeAndSegmentDescriptor(ObjType, descriptors[i], isSingleFinger),
                                "" + descriptors[i].Quality,
                                "1251", "3728"
                        );
                        // just to test, alternate quality is placed equal to greenbit quality - 1
                        int altQual = descriptors[i].Quality;
                        if (altQual < 0) altQual = 1;
                        AltQualityList[i] = new GBANJavaWrapperDefinesFingersAlternateQualityDescription();
                        AltQualityList[i].Build(
                                GetANSegmentPositionFromObjTypeAndSegmentDescriptor(ObjType, descriptors[i], isSingleFinger),
                                "" + altQual,
                                "1251", "3729"
                        );
                    }
                    RetVal = GB_AcquisitionOptionsGlobals.AN2000_Jw.ImageToType14Record(
                            Record,
                            GbBmp.bytes, GbBmp.sx, GbBmp.sy,
                            19.5,
                            GBANJavaWrapperDefinesCompressionAlgorithmsStrings.COMP_NONE,
                            EFTS_14_LIVE_SCAN_PLAN,
                            "GreenBit Scanner", "No comment",
                            GetAN1415FingerPositionStringFromGbfinimgObjType(ObjType),
                            null,
                            SegmentsList, NfiqList, SegmQualityList, AltQualityList
                    );
                    if (RetVal != GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_NO_ERROR) {
                        GB_AcquisitionOptionsGlobals.AN2000_Jw.FreeAnsiNistAllocatedMemory(AnsiNistStruct);
                        throw new Exception("ImageToType14Record: " + GB_AcquisitionOptionsGlobals.AN2000_Jw.GetLastErrorString());
                    }
                }
            } else if (RecType == GBANJavaWrapperDefinesRecordTypes.TYPE_15_ID) {
                GBANJavaWrapperDefinesPalmQualityAlgorithms[] PalmQualitiesList = new GBANJavaWrapperDefinesPalmQualityAlgorithms[2];
                PalmQualitiesList[0] = new GBANJavaWrapperDefinesPalmQualityAlgorithms();
                PalmQualitiesList[0].Build(GetAN1415FingerPositionStringFromGbfinimgObjType(ObjType),
                        "" + descriptors[0].Quality,
                        "1251", "3728");
                PalmQualitiesList[1] = new GBANJavaWrapperDefinesPalmQualityAlgorithms();
                PalmQualitiesList[1].Build(GetAN1415FingerPositionStringFromGbfinimgObjType(ObjType),
                        "" + (6 - (descriptors[0].Quality) / 20),
                        "1251", "3729");
                RetVal = GB_AcquisitionOptionsGlobals.AN2000_Jw.ImageToType15Record(
                        Record,
                        GbBmp.bytes, GbBmp.sx, GbBmp.sy,
                        19.685,
                        GBANJavaWrapperDefinesCompressionAlgorithmsStrings.COMP_NONE,
                        EFTS_14_LIVE_SCAN_PLAN,
                        "GreenBit Scanner", "No comment",
                        GetAN1415FingerPositionStringFromGbfinimgObjType(ObjType),
                        PalmQualitiesList
                );
                if (RetVal != GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_NO_ERROR) {
                    GB_AcquisitionOptionsGlobals.AN2000_Jw.FreeAnsiNistAllocatedMemory(AnsiNistStruct);
                    throw new Exception("ImageToType15Record: " + GB_AcquisitionOptionsGlobals.AN2000_Jw.GetLastErrorString());
                }
            } else {
                GB_AcquisitionOptionsGlobals.AN2000_Jw.FreeAnsiNistAllocatedMemory(AnsiNistStruct);
                throw new Exception("Record Type not available: " + RecType);
            }
            /******************************************
             * insert record into struct
             */
            RetVal = GB_AcquisitionOptionsGlobals.AN2000_Jw.AnEngineInsertRecordIntoAnsiNistStruct(AnsiNistStruct, Record,
                    AnsiNistStruct.GetNumRecords(GB_AcquisitionOptionsGlobals.AN2000_Jw), 1);
            if (RetVal != GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_NO_ERROR) {
                GB_AcquisitionOptionsGlobals.AN2000_Jw.FreeAnsiNistAllocatedMemory(AnsiNistStruct);
                throw new Exception("InsertRecordIntoAnsiNistStruct: " + GB_AcquisitionOptionsGlobals.AN2000_Jw.GetLastErrorString());
            }
            /***********************************
             * Create AN2000 buffer and free
             * allocated memory
             */
            GBJavaWrapperUtilByteArrayForJavaToCExchange OutBuffer = new GBJavaWrapperUtilByteArrayForJavaToCExchange();
            RetVal = GB_AcquisitionOptionsGlobals.AN2000_Jw.WriteAnsiNistToBuffer(
                    AnsiNistStruct,
                    OutBuffer
            );
            GB_AcquisitionOptionsGlobals.AN2000_Jw.FreeAnsiNistAllocatedMemory(AnsiNistStruct);
            if (RetVal != GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_NO_ERROR) {
                throw new Exception("WriteAnsiNistToBuffer: " + GB_AcquisitionOptionsGlobals.AN2000_Jw.GetLastErrorString());
            }
            /***********************************
             * Save into file
             */
            GbExampleGrayScaleBitmapClass.SaveGenericBinaryFile(fName,
                    GetThis(), "An2000", OutBuffer.Get());
        } catch (Exception ex) {
            LogAsDialog("SaveAN2000File: " + ex.getMessage());
            return;
        }
    }

    protected boolean onProcess() {
        try {
            int SegmSx = 500, SegmSy = 500;
            String fname = comboFilesList.getSelectedItem().toString();
            int ObjType = GbfinimgObjTypesArray[comboGbfinimgObjectType.getSelectedItemPosition()];
            GbExampleGrayScaleBitmapClass GbBmp = new GbExampleGrayScaleBitmapClass();
            if (!GB_AcquisitionOptionsGlobals.GbfinimgLibLoaded) {
                throw new Exception("Gbfinimg library not loaded");
            }
            boolean ret = GbBmp.GbBmpFromRawFileWithSize(fname, GetThis());
            if (!ret) {
                throw new Exception("GbBmpFromRawFileWithSize error");
            }
            if (GbBmp.sx < SegmSx) SegmSx = GbBmp.sx;
            if (GbBmp.sy < SegmSy) SegmSy = GbBmp.sy;
            GbfinimgJavaWrapperDefineSegmentImageDescriptor[] descriptors = new GbfinimgJavaWrapperDefineSegmentImageDescriptor[4];
            for (int i = 0; i < 4; i++) {
                descriptors[i] = new GbfinimgJavaWrapperDefineSegmentImageDescriptor();
                descriptors[i].SegmentImage = new byte[SegmSx * SegmSy];
            }
            GBJavaWrapperUtilIntForJavaToCExchange SegmNum = new GBJavaWrapperUtilIntForJavaToCExchange(),
                    Diag = new GBJavaWrapperUtilIntForJavaToCExchange();
            int RetVal = GB_AcquisitionOptionsGlobals.GBFINIMG_Jw.ProcessImage(
                    GbBmp.bytes, GbBmp.sx, GbBmp.sy,
                    ObjType,
                    GbfinimgJavaWrapperDefinesProcessOptions.GBFINIMG_REFINE_DRY_FINGERPRINT_IMAGE |
                            GbfinimgJavaWrapperDefinesProcessOptions.GBFINIMG_REFINE_WET_FINGERPRINT_IMAGE |
                            GbfinimgJavaWrapperDefinesProcessOptions.GBFINIMG_HALO_LATENT_ELIMINATION,
                    SegmSx, SegmSy,
                    null, 0,
                    descriptors,
                    SegmNum, Diag
            );
            if (RetVal != GbfinimgJavaWrapperDefinesReturnCodes.GBFINIMG_NO_ERROR) {
                ManageGbfinimgErrors("onProcess, ProcessImage", RetVal, true);
                return false;
            }
            LogAsDialog("SegmNum=" + SegmNum.Get() + ",Diag=" + Diag.Get());
            SaveAN2000File(fname, GbBmp, ObjType, descriptors, SegmNum.Get());
            SaveAN2011File(fname, GbBmp, ObjType, descriptors, SegmNum.Get());
            SegmentsInfoWindow.NumOfSegments = SegmNum.Get();
            SegmentsInfoWindow.Diagnostic = Diag.Get();
            SegmentsInfoWindow.descriptors = descriptors;

            Intent intent = new Intent(this, SegmentsInfoWindow.class);
            startActivity(intent);
            return true;
        } catch (Exception ex) {
            LogAsDialog("onProcess: " + ex.getMessage());
            return false;
        }


    }

    private void FillObjectTypesCombo() {
        String[] GbfinimgTypesStringArray = new String[GbfinimgObjTypesArray.length];
        for (int i = 0; i < GbfinimgObjTypesArray.length; i++) {
            GbfinimgTypesStringArray[i] = GbfinimgJavaWrapperDefinesInputImageType.GetStringFromInputImageType(
                    GbfinimgObjTypesArray[i]
            );
        }
        ArrayAdapter<String> FilesListAdapter = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item, GbfinimgTypesStringArray);
        FilesListAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        comboGbfinimgObjectType.setAdapter(FilesListAdapter);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_gbfinimg_window);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        FilesList = new ArrayList<String>();
        InitBmpLog();

        /************************************
         * refresh
         */
        bRefresh = (Button) findViewById(R.id.bGbfinimgRefreshList);
        bRefresh.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                onRefresh();
            }
        });
        bRefresh.setText("Refresh List");
        /************************************
         * process
         */
        bGbfinimgProcess = (Button) findViewById(R.id.bGbfinimgProcess);
        bGbfinimgProcess.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                onProcess();
            }
        });
        bGbfinimgProcess.setText("Process");
        /*************************************
         * frame view
         */
        FrameView = (ImageView) findViewById(R.id.GbfinimgFrameView);

        /******************************
         * files list
         */
        comboFilesList = (Spinner) findViewById(R.id.comboGbfinimgFilesToVisualize);
        comboFilesList.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {

            @Override
            public void onItemSelected(AdapterView<?> arg0, View arg1,
                                       int arg2, long arg3) {
                String fname = arg0.getItemAtPosition(arg2).toString();
                GbExampleGrayScaleBitmapClass GbBmp = new GbExampleGrayScaleBitmapClass();
                try {
                    boolean ret = GbBmp.GbBmpFromRawFileWithSize(fname, GetThis());
                    if (!ret) {
                        throw new Exception("GbBmpFromRawFileWithSize error");
                    } else {
                        LogBitmap(GbBmp);
                    }
                } catch (Exception ex) {
                    LogAsDialog("onItemSelected: " + ex.getMessage());
                }
            }

            @Override
            public void onNothingSelected(AdapterView<?> arg0) {
            }
        });
        /**********************************************
         * objects to process
         */
        comboGbfinimgObjectType = (Spinner) findViewById(R.id.comboGbfinimgObjectType);
        FillObjectTypesCombo();

        onRefresh();
        StartLogTimer();
    }

}
