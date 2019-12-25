package com.greenbit.gbmsapi;

import java.util.ArrayList;

public class GBMSAPIJavaWrapperDefinesScannerAcquisitionAreas {
    public static final int GBMSAPI_SA_FULL_FRAME = 0x00000001;
    public static final int GBMSAPI_SA_ROLL_IQS = 0x00000002;
    public static final int GBMSAPI_SA_ROLL_GA = 0x00000004;
    public static final int GBMSAPI_SA_ROLL_THENAR = 0x00000008;
    public static final int GBMSAPI_SA_ROLL_JOINT = 0x00000010;
    public static final int GBMSAPI_SA_PHOTO = 0x10000000;

    public static final String GBMSAPI_SA_STRING_FULL_FRAME = "Full Frame";
    public static final String GBMSAPI_SA_STRING_ROLL_IQS = "Roll IQS";
    public static final String GBMSAPI_SA_STRING_ROLL_GA = "Roll GA";
    public static final String GBMSAPI_SA_STRING_ROLL_THENAR = "Roll Thenar";
    public static final String GBMSAPI_SA_STRING_ROLL_JOINT = "Roll Joint";
    public static final String GBMSAPI_SA_STRING_PHOTO = "Photo";

    public static final ArrayList<String> ScannerAcquisitionAreasToStringList(int mask) {
        ArrayList<String> ValToRet = new ArrayList<String>();

        if ((mask & GBMSAPI_SA_FULL_FRAME) != 0) ValToRet.add(GBMSAPI_SA_STRING_FULL_FRAME);
        if ((mask & GBMSAPI_SA_ROLL_IQS) != 0) ValToRet.add(GBMSAPI_SA_STRING_ROLL_IQS);
        if ((mask & GBMSAPI_SA_ROLL_GA) != 0) ValToRet.add(GBMSAPI_SA_STRING_ROLL_GA);
        if ((mask & GBMSAPI_SA_ROLL_THENAR) != 0) ValToRet.add(GBMSAPI_SA_STRING_ROLL_THENAR);
        if ((mask & GBMSAPI_SA_ROLL_JOINT) != 0) ValToRet.add(GBMSAPI_SA_STRING_ROLL_JOINT);
        if ((mask & GBMSAPI_SA_PHOTO) != 0) ValToRet.add(GBMSAPI_SA_STRING_PHOTO);
        return ValToRet;
    }
}
