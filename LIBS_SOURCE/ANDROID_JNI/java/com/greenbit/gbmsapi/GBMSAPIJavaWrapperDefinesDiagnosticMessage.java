package com.greenbit.gbmsapi;

public class GBMSAPIJavaWrapperDefinesDiagnosticMessage {
    public static int GBMSAPI_DM_SCANNER_SURFACE_NOT_NORMA = 0x00000001;
    public static int GBMSAPI_DM_SCANNER_FAILURE = 0x00000002;
    public static int GBMSAPI_DM_COMPOSITION_SLOW = 0x00000004;
    public static int GBMSAPI_DM_FLAT_FINGER_SLIDING = 0x00000008;
    public static int GBMSAPI_DM_EXT_LIGHT_TOO_STRONG = 0x00000010;
    public static int GBMSAPI_DM_FLAT_FINGER_OUT_OF_REGION_LEFT = 0x00000020;
    public static int GBMSAPI_DM_FLAT_FINGER_OUT_OF_REGION_RIGHT = 0x00000040;
    public static int GBMSAPI_DM_FLAT_FINGER_OUT_OF_REGION_TOP = 0x00000080;
    public static int GBMSAPI_DM_FLAT_FINGER_DISPLACED_DOWN = 0x00000100;
    public static int GBMSAPI_DM_FAKE_FINGER_DETECTED_AUTO_CAPTURE_BLOCKED = 0x00000200;
    public static int GBMSAPI_DM_VSROLL_IMPROPER_ROLL = 0x00020000;
    public static int GBMSAPI_DM_VSROLL_TOO_FAST_ROLL = 0x00040000;
    public static int GBMSAPI_DM_VSROLL_TOO_NARROW_ROLL = 0x00080000;
    public static int GBMSAPI_DM_VSROLL_OUTSIDE_BORDER_LEFT = 0x00100000;
    public static int GBMSAPI_DM_VSROLL_OUTSIDE_BORDER_RIGHT = 0x00200000;
    public static int GBMSAPI_DM_VSROLL_OUTSIDE_BORDER_TOP = 0x00400000;
    public static int GBMSAPI_DM_VSROLL_DISPLACED_DOWN = 0x00800000;
    public static int GBMSAPI_DM_VSROLL_ROLL_DIRECTION_RIGHT = 0x40000000;
    public static int GBMSAPI_DM_VSROLL_ROLL_DIRECTION_LEFT = 0x80000000;
    public static int GBMSAPI_DM_VSROLL_ROLL_DIRECTION_DOWN = 0x08000000;
    public static int GBMSAPI_DM_VSROLL_ROLL_DIRECTION_UP = 0x10000000;
    public static int GBMSAPI_DM_DRY_FINGER = 0x01000000;
    public static int GBMSAPI_DM_WET_FINGER = 0x02000000;
    public static int GBMSAPI_DM_TOO_SHORT_VERTICAL_ROLL = 0x20000000;

    public  static String DiagnosticToString(int diagnostic)
    {
        String valToRet = "";
        if ((diagnostic & GBMSAPI_DM_COMPOSITION_SLOW) != 0) valToRet += "Composition Slow\n";
        if ((diagnostic & GBMSAPI_DM_DRY_FINGER) != 0) valToRet += "Dry Finger\n";
        if ((diagnostic & GBMSAPI_DM_EXT_LIGHT_TOO_STRONG) != 0) valToRet += "External light Too Strong\n";
        if ((diagnostic & GBMSAPI_DM_FAKE_FINGER_DETECTED_AUTO_CAPTURE_BLOCKED) != 0) valToRet += "AutoCapture blocked: fake finger detected\n";
        if ((diagnostic & GBMSAPI_DM_FLAT_FINGER_DISPLACED_DOWN) != 0) valToRet += "Finger Displaced Down\n";
        if ((diagnostic & GBMSAPI_DM_FLAT_FINGER_OUT_OF_REGION_LEFT) != 0) valToRet += "Finger out of region left\n";
        if ((diagnostic & GBMSAPI_DM_FLAT_FINGER_OUT_OF_REGION_RIGHT) != 0) valToRet += "Finger out of region right\n";
        if ((diagnostic & GBMSAPI_DM_FLAT_FINGER_OUT_OF_REGION_TOP) != 0) valToRet += "Finger out of region top\n";
        if ((diagnostic & GBMSAPI_DM_FLAT_FINGER_SLIDING) != 0) valToRet += "Finger sliding\n";
        if ((diagnostic & GBMSAPI_DM_SCANNER_FAILURE) != 0) valToRet += "Scanner Failure\n";
        if ((diagnostic & GBMSAPI_DM_SCANNER_SURFACE_NOT_NORMA) != 0) valToRet += "Surface dirty or finger placed too early\n";
        if ((diagnostic & GBMSAPI_DM_TOO_SHORT_VERTICAL_ROLL) != 0) valToRet += "Too short vertical roll\n";
        if ((diagnostic & GBMSAPI_DM_VSROLL_DISPLACED_DOWN) != 0) valToRet += "Finger Displaced Down\n";
        if ((diagnostic & GBMSAPI_DM_VSROLL_IMPROPER_ROLL) != 0) valToRet += "Improper Roll\n";
        if ((diagnostic & GBMSAPI_DM_VSROLL_OUTSIDE_BORDER_LEFT) != 0) valToRet += "Finger out of region left\n";
        if ((diagnostic & GBMSAPI_DM_VSROLL_OUTSIDE_BORDER_RIGHT) != 0) valToRet += "Finger out of region right\n";
        if ((diagnostic & GBMSAPI_DM_VSROLL_OUTSIDE_BORDER_TOP) != 0) valToRet += "Finger out of region top\n";
        if ((diagnostic & GBMSAPI_DM_VSROLL_ROLL_DIRECTION_DOWN) != 0) valToRet += "Roll to bottom\n";
        if ((diagnostic & GBMSAPI_DM_VSROLL_ROLL_DIRECTION_LEFT) != 0) valToRet += "Roll to left\n";
        if ((diagnostic & GBMSAPI_DM_VSROLL_ROLL_DIRECTION_RIGHT) != 0) valToRet += "Roll to right\n";
        if ((diagnostic & GBMSAPI_DM_VSROLL_ROLL_DIRECTION_UP) != 0) valToRet += "Roll to top\n";
        if ((diagnostic & GBMSAPI_DM_VSROLL_TOO_FAST_ROLL) != 0) valToRet += "Too fast roll\n";
        if ((diagnostic & GBMSAPI_DM_VSROLL_TOO_NARROW_ROLL) != 0) valToRet += "Too narrow roll\n";
        if ((diagnostic & GBMSAPI_DM_WET_FINGER) != 0) valToRet += "Wet Finger\n";
        return valToRet;
    }
}
