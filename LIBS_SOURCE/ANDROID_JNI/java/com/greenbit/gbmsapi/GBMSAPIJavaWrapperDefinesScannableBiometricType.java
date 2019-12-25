package com.greenbit.gbmsapi;

import java.util.ArrayList;

public class GBMSAPIJavaWrapperDefinesScannableBiometricType {
    public static final int GBMSAPI_SBT_NO_OBJECT_TYPE = 0;
    public static final int GBMSAPI_SBT_ROLL_SINGLE_FINGER = 0x00000001;
    public static final int GBMSAPI_SBT_FLAT_SINGLE_FINGER = 0x00000002;
    public static final int GBMSAPI_SBT_FLAT_SLAP_4 = 0x00000004;
    public static final int GBMSAPI_SBT_FLAT_SLAP_2 = 0x00000008;
    public static final int GBMSAPI_SBT_FLAT_THUMBS_2 = 0x00000010;
    public static final int GBMSAPI_SBT_FLAT_LOWER_HALF_PALM = 0x00000020;
    public static final int GBMSAPI_SBT_FLAT_UPPER_HALF_PALM = 0x00000040;
    public static final int GBMSAPI_SBT_FLAT_WRITER_PALM = 0x00000080;
    public static final int GBMSAPI_SBT_FLAT_INDEXES_2 = 0x00000100;
    public static final int GBMSAPI_SBT_ROLLED_THENAR = 0x00000200;
    public static final int GBMSAPI_SBT_ROLLED_JOINT = 0x00000400;
    public static final int GBMSAPI_SBT_PLAIN_JOINT_LEFT_SIDE = 0x00000800;
    public static final int GBMSAPI_SBT_PLAIN_JOINT_RIGHT_SIDE = 0x00001000;
    public static final int GBMSAPI_SBT_ROLLED_JOINT_CENTER = 0x00002000;
    public static final int GBMSAPI_SBT_ROLLED_TIP = 0x00004000;
    public static final int GBMSAPI_SBT_ROLLED_UP = 0x00008000;
    public static final int GBMSAPI_SBT_ROLLED_DOWN = 0x00010000;
    public static final int GBMSAPI_SBT_ROLLED_HYPOTHENAR = 0x00020000;
    public static final int GBMSAPI_SBT_PHOTO = 0x80000000;

    public static final String GBMSAPI_SBT_STRING_NO_OBJECT_TYPE = "INVALID";
    public static final String GBMSAPI_SBT_STRING_ROLL_SINGLE_FINGER = "ROLL_SINGLE_FINGER";
    public static final String GBMSAPI_SBT_STRING_FLAT_SINGLE_FINGER = "FLAT_SINGLE_FINGER";
    public static final String GBMSAPI_SBT_STRING_FLAT_SLAP_4 = "FLAT_SLAP_4";
    public static final String GBMSAPI_SBT_STRING_FLAT_SLAP_2 = "FLAT_SLAP_2";
    public static final String GBMSAPI_SBT_STRING_FLAT_THUMBS_2 = "FLAT_THUMBS_2";
    public static final String GBMSAPI_SBT_STRING_FLAT_LOWER_HALF_PALM = "FLAT_LOWER_HALF_PALM";
    public static final String GBMSAPI_SBT_STRING_FLAT_UPPER_HALF_PALM = "FLAT_UPPER_HALF_PALM";
    public static final String GBMSAPI_SBT_STRING_FLAT_WRITER_PALM = "FLAT_WRITER_PALM";
    public static final String GBMSAPI_SBT_STRING_FLAT_INDEXES_2 = "FLAT_INDEXES_2";
    public static final String GBMSAPI_SBT_STRING_ROLLED_THENAR = "ROLLED_THENAR";
    public static final String GBMSAPI_SBT_STRING_ROLLED_JOINT = "ROLLED_JOINT";
    public static final String GBMSAPI_SBT_STRING_PLAIN_JOINT_LEFT_SIDE = "PLAIN_JOINT_LEFT_SIDE";
    public static final String GBMSAPI_SBT_STRING_PLAIN_JOINT_RIGHT_SIDE = "PLAIN_JOINT_RIGHT_SIDE";
    public static final String GBMSAPI_SBT_STRING_ROLLED_JOINT_CENTER = "ROLLED_JOINT_CENTER";
    public static final String GBMSAPI_SBT_STRING_ROLLED_TIP = "ROLLED_TIP";
    public static final String GBMSAPI_SBT_STRING_ROLLED_UP = "ROLLED_UP";
    public static final String GBMSAPI_SBT_STRING_ROLLED_DOWN = "ROLLED_DOWN";
    public static final String GBMSAPI_SBT_STRING_ROLLED_HYPOTHENAR = "ROLLED_HYPOTHENAR";
    public static final String GBMSAPI_SBT_STRING_PHOTO = "PHOTO";

    public static ArrayList<String> ScannableTypesToStringList(int mask) {
        ArrayList<String> ValToRet = new ArrayList<String>();

        if ((mask & GBMSAPI_SBT_ROLL_SINGLE_FINGER) != 0) ValToRet.add(GBMSAPI_SBT_STRING_ROLL_SINGLE_FINGER);
        if ((mask & GBMSAPI_SBT_FLAT_SINGLE_FINGER) != 0) ValToRet.add(GBMSAPI_SBT_STRING_FLAT_SINGLE_FINGER);
        if ((mask & GBMSAPI_SBT_FLAT_SLAP_4) != 0) ValToRet.add(GBMSAPI_SBT_STRING_FLAT_SLAP_4);
        if ((mask & GBMSAPI_SBT_FLAT_SLAP_2) != 0) ValToRet.add(GBMSAPI_SBT_STRING_FLAT_SLAP_2);
        if ((mask & GBMSAPI_SBT_FLAT_THUMBS_2) != 0) ValToRet.add(GBMSAPI_SBT_STRING_FLAT_THUMBS_2);
        if ((mask & GBMSAPI_SBT_FLAT_LOWER_HALF_PALM) != 0) ValToRet.add(GBMSAPI_SBT_STRING_FLAT_LOWER_HALF_PALM);
        if ((mask & GBMSAPI_SBT_FLAT_UPPER_HALF_PALM) != 0) ValToRet.add(GBMSAPI_SBT_STRING_FLAT_UPPER_HALF_PALM);
        if ((mask & GBMSAPI_SBT_FLAT_WRITER_PALM) != 0) ValToRet.add(GBMSAPI_SBT_STRING_FLAT_WRITER_PALM);
        if ((mask & GBMSAPI_SBT_FLAT_INDEXES_2) != 0) ValToRet.add(GBMSAPI_SBT_STRING_FLAT_INDEXES_2);
        if ((mask & GBMSAPI_SBT_ROLLED_THENAR) != 0) ValToRet.add(GBMSAPI_SBT_STRING_ROLLED_THENAR);
        if ((mask & GBMSAPI_SBT_ROLLED_JOINT) != 0) ValToRet.add(GBMSAPI_SBT_STRING_ROLLED_JOINT);
        if ((mask & GBMSAPI_SBT_PLAIN_JOINT_LEFT_SIDE) != 0) ValToRet.add(GBMSAPI_SBT_STRING_PLAIN_JOINT_LEFT_SIDE);
        if ((mask & GBMSAPI_SBT_PLAIN_JOINT_RIGHT_SIDE) != 0)
            ValToRet.add(GBMSAPI_SBT_STRING_PLAIN_JOINT_RIGHT_SIDE);
        if ((mask & GBMSAPI_SBT_ROLLED_JOINT_CENTER) != 0) ValToRet.add(GBMSAPI_SBT_STRING_ROLLED_JOINT_CENTER);
        if ((mask & GBMSAPI_SBT_ROLLED_TIP) != 0) ValToRet.add(GBMSAPI_SBT_STRING_ROLLED_TIP);
        if ((mask & GBMSAPI_SBT_ROLLED_UP) != 0) ValToRet.add(GBMSAPI_SBT_STRING_ROLLED_UP);
        if ((mask & GBMSAPI_SBT_ROLLED_DOWN) != 0) ValToRet.add(GBMSAPI_SBT_STRING_ROLLED_DOWN);
        if ((mask & GBMSAPI_SBT_ROLLED_HYPOTHENAR) != 0) ValToRet.add(GBMSAPI_SBT_STRING_ROLLED_HYPOTHENAR);
        if ((mask & GBMSAPI_SBT_PHOTO) != 0) ValToRet.add(GBMSAPI_SBT_STRING_PHOTO);

        return ValToRet;
    }

    public static String ScannableTypesToString(int mask) 
    {
        String ValToRet = "";

        if ((mask == GBMSAPI_SBT_ROLL_SINGLE_FINGER)) ValToRet = (GBMSAPI_SBT_STRING_ROLL_SINGLE_FINGER);
        if ((mask == GBMSAPI_SBT_FLAT_SINGLE_FINGER)) ValToRet = (GBMSAPI_SBT_STRING_FLAT_SINGLE_FINGER);
        if ((mask == GBMSAPI_SBT_FLAT_SLAP_4)) ValToRet = (GBMSAPI_SBT_STRING_FLAT_SLAP_4);
        if ((mask == GBMSAPI_SBT_FLAT_SLAP_2)) ValToRet = (GBMSAPI_SBT_STRING_FLAT_SLAP_2);
        if ((mask == GBMSAPI_SBT_FLAT_THUMBS_2)) ValToRet = (GBMSAPI_SBT_STRING_FLAT_THUMBS_2);
        if ((mask == GBMSAPI_SBT_FLAT_LOWER_HALF_PALM)) ValToRet = (GBMSAPI_SBT_STRING_FLAT_LOWER_HALF_PALM);
        if ((mask == GBMSAPI_SBT_FLAT_UPPER_HALF_PALM)) ValToRet = (GBMSAPI_SBT_STRING_FLAT_UPPER_HALF_PALM);
        if ((mask == GBMSAPI_SBT_FLAT_WRITER_PALM)) ValToRet = (GBMSAPI_SBT_STRING_FLAT_WRITER_PALM);
        if ((mask == GBMSAPI_SBT_FLAT_INDEXES_2)) ValToRet = (GBMSAPI_SBT_STRING_FLAT_INDEXES_2);
        if ((mask == GBMSAPI_SBT_ROLLED_THENAR)) ValToRet = (GBMSAPI_SBT_STRING_ROLLED_THENAR);
        if ((mask == GBMSAPI_SBT_ROLLED_JOINT)) ValToRet = (GBMSAPI_SBT_STRING_ROLLED_JOINT);
        if ((mask == GBMSAPI_SBT_PLAIN_JOINT_LEFT_SIDE)) ValToRet = (GBMSAPI_SBT_STRING_PLAIN_JOINT_LEFT_SIDE);
        if ((mask == GBMSAPI_SBT_PLAIN_JOINT_RIGHT_SIDE)) ValToRet = (GBMSAPI_SBT_STRING_PLAIN_JOINT_RIGHT_SIDE);
        if ((mask == GBMSAPI_SBT_ROLLED_JOINT_CENTER)) ValToRet = (GBMSAPI_SBT_STRING_ROLLED_JOINT_CENTER);
        if ((mask == GBMSAPI_SBT_ROLLED_TIP)) ValToRet = (GBMSAPI_SBT_STRING_ROLLED_TIP);
        if ((mask == GBMSAPI_SBT_ROLLED_UP)) ValToRet = (GBMSAPI_SBT_STRING_ROLLED_UP);
        if ((mask == GBMSAPI_SBT_ROLLED_DOWN)) ValToRet = (GBMSAPI_SBT_STRING_ROLLED_DOWN);
        if ((mask == GBMSAPI_SBT_ROLLED_HYPOTHENAR)) ValToRet = (GBMSAPI_SBT_STRING_ROLLED_HYPOTHENAR);
        if ((mask == GBMSAPI_SBT_PHOTO)) ValToRet = (GBMSAPI_SBT_STRING_PHOTO);

        return ValToRet;
    }
    
    public static int ScannableTypeFromString(String strVal)
    {
        int ValToRet = 0;
        if (strVal.equals(GBMSAPI_SBT_STRING_ROLL_SINGLE_FINGER)) ValToRet = GBMSAPI_SBT_ROLL_SINGLE_FINGER;
        if (strVal.equals(GBMSAPI_SBT_STRING_FLAT_SINGLE_FINGER)) ValToRet = GBMSAPI_SBT_FLAT_SINGLE_FINGER;
        if (strVal.equals(GBMSAPI_SBT_STRING_FLAT_SLAP_4)) ValToRet = GBMSAPI_SBT_FLAT_SLAP_4;
        if (strVal.equals(GBMSAPI_SBT_STRING_FLAT_SLAP_2)) ValToRet = GBMSAPI_SBT_FLAT_SLAP_2;
        if (strVal.equals(GBMSAPI_SBT_STRING_FLAT_THUMBS_2)) ValToRet = GBMSAPI_SBT_FLAT_THUMBS_2;
        if (strVal.equals(GBMSAPI_SBT_STRING_FLAT_LOWER_HALF_PALM)) ValToRet = GBMSAPI_SBT_FLAT_LOWER_HALF_PALM;
        if (strVal.equals(GBMSAPI_SBT_STRING_FLAT_UPPER_HALF_PALM)) ValToRet = GBMSAPI_SBT_FLAT_UPPER_HALF_PALM;
        if (strVal.equals(GBMSAPI_SBT_STRING_FLAT_WRITER_PALM)) ValToRet = GBMSAPI_SBT_FLAT_WRITER_PALM;
        if (strVal.equals(GBMSAPI_SBT_STRING_FLAT_INDEXES_2)) ValToRet = GBMSAPI_SBT_FLAT_INDEXES_2;
        if (strVal.equals(GBMSAPI_SBT_STRING_ROLLED_THENAR)) ValToRet = GBMSAPI_SBT_ROLLED_THENAR;
        if (strVal.equals(GBMSAPI_SBT_STRING_ROLLED_JOINT)) ValToRet = GBMSAPI_SBT_ROLLED_JOINT;
        if (strVal.equals(GBMSAPI_SBT_STRING_PLAIN_JOINT_LEFT_SIDE)) ValToRet = GBMSAPI_SBT_PLAIN_JOINT_LEFT_SIDE;
        if (strVal.equals(GBMSAPI_SBT_STRING_PLAIN_JOINT_RIGHT_SIDE)) ValToRet = GBMSAPI_SBT_PLAIN_JOINT_RIGHT_SIDE;
        if (strVal.equals(GBMSAPI_SBT_STRING_ROLLED_JOINT_CENTER)) ValToRet = GBMSAPI_SBT_ROLLED_JOINT_CENTER;
        if (strVal.equals(GBMSAPI_SBT_STRING_ROLLED_TIP)) ValToRet = GBMSAPI_SBT_ROLLED_TIP;
        if (strVal.equals(GBMSAPI_SBT_STRING_ROLLED_UP)) ValToRet = GBMSAPI_SBT_ROLLED_UP;
        if (strVal.equals(GBMSAPI_SBT_STRING_ROLLED_DOWN)) ValToRet = GBMSAPI_SBT_ROLLED_DOWN;
        if (strVal.equals(GBMSAPI_SBT_STRING_ROLLED_HYPOTHENAR)) ValToRet = GBMSAPI_SBT_ROLLED_HYPOTHENAR;
        if (strVal.equals(GBMSAPI_SBT_STRING_PHOTO)) ValToRet = GBMSAPI_SBT_PHOTO;
        
        return ValToRet;
    }

    public static boolean ObjToAcquireIsFlatSingle(int objToAcquire)
    {
        return ObjToAcquireIsFlatSingle(ScannableTypesToString(objToAcquire));
    }

    public static boolean ObjToAcquireIsSlapOrJoint(int objToAcquire)
    {
        return ObjToAcquireIsSlapOrJoint(ScannableTypesToString(objToAcquire));
    }

    public static boolean ObjToAcquireIsRoll(int objToAcquire)
    {
        return ObjToAcquireIsRoll(ScannableTypesToString(objToAcquire));
    }

    public static boolean ObjToAcquireIsPalm(int objToAcquire)
    {
        return ObjToAcquireIsPalm(ScannableTypesToString(objToAcquire));
    }

    public static boolean ObjToAcquireIsFlatSingle(String objToAcquireString)
    {
        if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_FLAT_SINGLE_FINGER)
        {
            return true;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_ROLL_SINGLE_FINGER)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_FLAT_INDEXES_2)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_FLAT_LOWER_HALF_PALM)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_FLAT_SLAP_2)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_FLAT_SLAP_4)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_FLAT_THUMBS_2)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_FLAT_UPPER_HALF_PALM)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_FLAT_WRITER_PALM)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_PLAIN_JOINT_LEFT_SIDE)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_PLAIN_JOINT_RIGHT_SIDE)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_ROLLED_DOWN)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_ROLLED_HYPOTHENAR)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_ROLLED_JOINT)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_ROLLED_JOINT_CENTER)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_ROLLED_THENAR)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_ROLLED_TIP)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_ROLLED_UP)
        {
            return false;
        }
        else
        {
            return false;
        }
    }

    public static boolean ObjToAcquireIsSlapOrJoint(String objToAcquireString)
    {
        if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_FLAT_SINGLE_FINGER)
        {
            return true;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_ROLL_SINGLE_FINGER)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_FLAT_INDEXES_2)
        {
            return true;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_FLAT_LOWER_HALF_PALM)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_FLAT_SLAP_2)
        {
            return true;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_FLAT_SLAP_4)
        {
            return true;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_FLAT_THUMBS_2)
        {
            return true;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_FLAT_UPPER_HALF_PALM)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_FLAT_WRITER_PALM)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_PLAIN_JOINT_LEFT_SIDE)
        {
            return true;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_PLAIN_JOINT_RIGHT_SIDE)
        {
            return true;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_ROLLED_DOWN)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_ROLLED_HYPOTHENAR)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_ROLLED_JOINT)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_ROLLED_JOINT_CENTER)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_ROLLED_THENAR)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_ROLLED_TIP)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_ROLLED_UP)
        {
            return false;
        }
        else
        {
            return false;
        }
    }

    public static boolean ObjToAcquireIsRoll(String objToAcquireString)
    {
        if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_FLAT_SINGLE_FINGER)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_ROLL_SINGLE_FINGER)
        {
            return true;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_FLAT_INDEXES_2)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_FLAT_LOWER_HALF_PALM)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_FLAT_SLAP_2)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_FLAT_SLAP_4)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_FLAT_THUMBS_2)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_FLAT_UPPER_HALF_PALM)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_FLAT_WRITER_PALM)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_PLAIN_JOINT_LEFT_SIDE)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_PLAIN_JOINT_RIGHT_SIDE)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_ROLLED_DOWN)
        {
            return true;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_ROLLED_HYPOTHENAR)
        {
            return true;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_ROLLED_JOINT)
        {
            return true;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_ROLLED_JOINT_CENTER)
        {
            return true;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_ROLLED_THENAR)
        {
            return true;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_ROLLED_TIP)
        {
            return true;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_ROLLED_UP)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    public static boolean ObjToAcquireIsPalm(String objToAcquireString)
    {
        if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_FLAT_SINGLE_FINGER)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_ROLL_SINGLE_FINGER)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_FLAT_INDEXES_2)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_FLAT_LOWER_HALF_PALM)
        {
            return true;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_FLAT_SLAP_2)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_FLAT_SLAP_4)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_FLAT_THUMBS_2)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_FLAT_UPPER_HALF_PALM)
        {
            return true;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_FLAT_WRITER_PALM)
        {
            return true;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_PLAIN_JOINT_LEFT_SIDE)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_PLAIN_JOINT_RIGHT_SIDE)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_ROLLED_DOWN)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_ROLLED_HYPOTHENAR)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_ROLLED_JOINT)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_ROLLED_JOINT_CENTER)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_ROLLED_THENAR)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_ROLLED_TIP)
        {
            return false;
        }
        else if (objToAcquireString == GBMSAPIJavaWrapperDefinesScannableBiometricType.GBMSAPI_SBT_STRING_ROLLED_UP)
        {
            return false;
        }
        else
        {
            return false;
        }
    }
}
