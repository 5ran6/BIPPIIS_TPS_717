package com.greenbit.gbmsapi;

import java.util.ArrayList;

public class GBMSAPIJavaWrapperDefinesAcquisitionOptions {
    public static final int GBMSAPI_AO_AUTOCAPTURE = 0x00000001;
    public static final int GBMSAPI_AO_NO_ROLL_PREVIEW = 0x00000002;
    public static final int GBMSAPI_AO_MANUAL_ROLL_PREVIEW_STOP = 0x00000004;
    public static final int GBMSAPI_AO_ROLL_PREVIEW_TYPE = 0x00000008;
    public static final int GBMSAPI_AO_FULL_RES_PREVIEW = 0x00000010;
    public static final int GBMSAPI_AO_FLAT_SINGLE_FINGER_ON_ROLL_AREA = 0x00000020;
    public static final int GBMSAPI_AO_HIGH_SPEED_PREVIEW = 0x00000040;
    public static final int GBMSAPI_AO_ADAPT_ROLL_AREA_POSITION = 0x00000080;
    public static final int GBMSAPI_AO_FORCE_ROLL_TO_LEFT = 0x00000100;
    public static final int GBMSAPI_AO_FORCE_ROLL_TO_RIGHT = 0x00000200;
    public static final int GBMSAPI_AO_EXTERNAL_ROLL_COMPOSITION = 0x00000400;
    
    public static final String GBMSAPI_AO_STRING_AUTOCAPTURE = "Auto-Capture";
    public static final String GBMSAPI_AO_STRING_NO_ROLL_PREVIEW = "No Roll Preview";
    public static final String GBMSAPI_AO_STRING_MANUAL_ROLL_PREVIEW_STOP = "Manual Roll Preview Stop";
    public static final String GBMSAPI_AO_STRING_ROLL_PREVIEW_TYPE = "Center Preview Type";
    public static final String GBMSAPI_AO_STRING_FULL_RES_PREVIEW = "Full Resolution Preview";
    public static final String GBMSAPI_AO_STRING_FLAT_SINGLE_FINGER_ON_ROLL_AREA = "Flat Finger On Roll Area";
    public static final String GBMSAPI_AO_STRING_HIGH_SPEED_PREVIEW = "High Speed Preview";
    public static final String GBMSAPI_AO_STRING_ADAPT_ROLL_AREA_POSITION = "Adapt Roll Area Position";
    public static final String GBMSAPI_AO_STRING_FORCE_ROLL_TO_LEFT = "Force Roll To Left";
    public static final String GBMSAPI_AO_STRING_FORCE_ROLL_TO_RIGHT = "Force Roll To Right";
    public static final String GBMSAPI_AO_STRING_EXTERNAL_ROLL_COMPOSITION = "External Roll Composition";

    public static final ArrayList<String> ScanOptionsToStringList(int mask) {
        ArrayList<String> ValToRet = new ArrayList<String>();

        if ((mask & GBMSAPI_AO_AUTOCAPTURE) != 0) ValToRet.add(GBMSAPI_AO_STRING_AUTOCAPTURE);
        if ((mask & GBMSAPI_AO_NO_ROLL_PREVIEW) != 0) ValToRet.add(GBMSAPI_AO_STRING_NO_ROLL_PREVIEW);
        if ((mask & GBMSAPI_AO_MANUAL_ROLL_PREVIEW_STOP) != 0) ValToRet.add(GBMSAPI_AO_STRING_MANUAL_ROLL_PREVIEW_STOP);
        if ((mask & GBMSAPI_AO_ROLL_PREVIEW_TYPE) != 0) ValToRet.add(GBMSAPI_AO_STRING_ROLL_PREVIEW_TYPE);
        if ((mask & GBMSAPI_AO_FULL_RES_PREVIEW) != 0) ValToRet.add(GBMSAPI_AO_STRING_FULL_RES_PREVIEW);
        if ((mask & GBMSAPI_AO_FLAT_SINGLE_FINGER_ON_ROLL_AREA) != 0) ValToRet.add(GBMSAPI_AO_STRING_FLAT_SINGLE_FINGER_ON_ROLL_AREA);
        if ((mask & GBMSAPI_AO_HIGH_SPEED_PREVIEW) != 0) ValToRet.add(GBMSAPI_AO_STRING_HIGH_SPEED_PREVIEW);
        if ((mask & GBMSAPI_AO_ADAPT_ROLL_AREA_POSITION) != 0) ValToRet.add(GBMSAPI_AO_STRING_ADAPT_ROLL_AREA_POSITION);
        if ((mask & GBMSAPI_AO_FORCE_ROLL_TO_LEFT) != 0) ValToRet.add(GBMSAPI_AO_STRING_FORCE_ROLL_TO_LEFT);
        if ((mask & GBMSAPI_AO_FORCE_ROLL_TO_RIGHT) != 0) ValToRet.add(GBMSAPI_AO_STRING_FORCE_ROLL_TO_RIGHT);
        if ((mask & GBMSAPI_AO_EXTERNAL_ROLL_COMPOSITION) != 0) ValToRet.add(GBMSAPI_AO_STRING_EXTERNAL_ROLL_COMPOSITION);

        return ValToRet;
    }
}
