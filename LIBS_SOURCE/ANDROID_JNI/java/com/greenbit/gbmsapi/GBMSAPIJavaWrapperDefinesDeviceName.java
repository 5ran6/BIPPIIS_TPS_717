package com.greenbit.gbmsapi;

public class GBMSAPIJavaWrapperDefinesDeviceName {
    public static int GBMSAPI_DN_DS84 = 1;    // DactyScan84
    public static int GBMSAPI_DN_MS500 = 2;    // MultiScan500
    public static int GBMSAPI_DN_MS1000 = 3;    // MultiScan1000
    public static int GBMSAPI_DN_VS3 = 4;    // VisaScan3
    public static int GBMSAPI_DN_PS2 = 5;    // PoliScan2
    public static int GBMSAPI_DN_DS40 = 6;    // DactyScan40
    public static int GBMSAPI_DN_DS26 = 7;    // DactyScan26 and DactyScan26i
    public static int GBMSAPI_DN_MC500 = 8;    // MultiScan500, China market versione
    public static int GBMSAPI_DN_MSC500 = 9;    // MultiScan500, China market version, FULL version
    public static int GBMSAPI_DN_DS40I = 10;    // DactyScan40i
    public static int GBMSAPI_DN_DS84C = 11; // DactyScan40c
    public static int GBMSAPI_DN_MC517 = 12; // MC517
    public static int GBMSAPI_DN_MSC517 = 13; // MSC517
    public static int GBMSAPI_DN_DS32 = 14; // DactyScan32
    public static int GBMSAPI_DN_MS527 = 15; // MS527
    public static int GBMSAPI_DN_DS84t = 16; // DS84t
    public static int GBMSAPI_DN_DSID20 = 17; // DactyID20
    public static int GBMSAPI_DN_MS527t	 = 18; // MS527t

    public static String DevIDToString(int deviceID)
    {
        if (deviceID == GBMSAPI_DN_DS84) return "DS84";
        if (deviceID == GBMSAPI_DN_MS500) return "MS500";
        if (deviceID == GBMSAPI_DN_MS1000) return "MS1000";
        if (deviceID == GBMSAPI_DN_VS3) return "VS3";
        if (deviceID == GBMSAPI_DN_PS2) return "PS2";
        if (deviceID == GBMSAPI_DN_DS40) return "DS40";
        if (deviceID == GBMSAPI_DN_DS26) return "DS26";
        if (deviceID == GBMSAPI_DN_MC500) return "MC500";
        if (deviceID == GBMSAPI_DN_MSC500) return "MSC500";
        if (deviceID == GBMSAPI_DN_DS40I) return "DS40I";
        if (deviceID == GBMSAPI_DN_DS84C) return "DS84C";
        if (deviceID == GBMSAPI_DN_MC517) return "MC517";
        if (deviceID == GBMSAPI_DN_MSC517) return "MSC517";
        if (deviceID == GBMSAPI_DN_DS32) return "DS32";
        if (deviceID == GBMSAPI_DN_MS527) return "MS527";
        if (deviceID == GBMSAPI_DN_DS84t) return "DS84t";
        if (deviceID == GBMSAPI_DN_DSID20) return "DSID20";
        if (deviceID == GBMSAPI_DN_MS527t) return "MS527T";

        return null;
    }
}
