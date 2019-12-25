package com.greenbit.gbmsapi;

import java.util.ArrayList;

public class GBMSAPIJavaWrapperDefinesDeviceFeatures {
    public static final int GBMSAPI_DF_MULTIPLE_USB_CONNECTIONS_ALLOWED = 0x00000001;
    public static final int GBMSAPI_DF_AUTO_CAPTURE_BLOCKING = 0x00000002;
    public static final int GBMSAPI_DF_1000DPI_RESOLUTION = 0x00000004;
    public static final int GBMSAPI_DF_IMAGE_ROTATION = 0x00000008;
    public static final int GBMSAPI_DF_PERMANENT_USER_DATA_STORAGE = 0x00000010;
    public static final int GBMSAPI_DF_FRAME_RATE_SETTING = 0x00000020;
    public static final int GBMSAPI_DF_FW_INFO_RETRIEVE = 0x00000040;
    public static final int GBMSAPI_DF_ROLL_AREA_IQS = 0x00000080;
    public static final int GBMSAPI_DF_ROLL_AREA_GA = 0x00000100;
    public static final int GBMSAPI_DF_DRY_SKIN_IMG_ENHANCE = 0x00000200;
    public static final int GBMSAPI_DF_ROLL_OBJECT_STRIPE = 0x00000400;
    public static final int GBMSAPI_DF_UPDATE_BACKGROUND_IMAGE = 0x00000800;
    public static final int GBMSAPI_DF_ENABLE_BLOCK_ROLL_COMPOSITION = 0x00001000;
    public static final int GBMSAPI_DF_USB_3_0_SUPPORT = 0x00002000;
    public static final int GBMSAPI_DF_HW_ANTIFAKE = 0x00004000;
    public static final int GBMSAPI_DF_SW_ANTIFAKE = 0x00008000;
    public static final int GBMSAPI_DF_HEATER = 0x00010000;

    public static final String GBMSAPI_DF_STRING_MULTIPLE_USB_CONNECTIONS_ALLOWED = "Multiple USB Connections";
    public static final String GBMSAPI_DF_STRING_AUTO_CAPTURE_BLOCKING = "Auto-Capture Blocking";
    public static final String GBMSAPI_DF_STRING_1000DPI_RESOLUTION = "1000 DPI Resolution";
    public static final String GBMSAPI_DF_STRING_IMAGE_ROTATION = "ImageRotation";
    public static final String GBMSAPI_DF_STRING_PERMANENT_USER_DATA_STORAGE = "Eeprom User Data Storage";
    public static final String GBMSAPI_DF_STRING_FRAME_RATE_SETTING = "Frame Rate Setting";
    public static final String GBMSAPI_DF_STRING_FW_INFO_RETRIEVE = "Firmware Info Retrieve";
    public static final String GBMSAPI_DF_STRING_ROLL_AREA_IQS = "Roll Area IQS";
    public static final String GBMSAPI_DF_STRING_ROLL_AREA_GA = "Roll Area GA";
    public static final String GBMSAPI_DF_STRING_DRY_SKIN_IMG_ENHANCE = "Dry Skin Image Enhance";
    public static final String GBMSAPI_DF_STRING_ROLL_OBJECT_STRIPE = "Roll Object Stripe";
    public static final String GBMSAPI_DF_STRING_UPDATE_BACKGROUND_IMAGE = "Update Background Image";
    public static final String GBMSAPI_DF_STRING_ENABLE_BLOCK_ROLL_COMPOSITION = "Roll Composition Block";
    public static final String GBMSAPI_DF_STRING_USB_3_0_SUPPORT = "Usb 3.0 Support";
    public static final String GBMSAPI_DF_STRING_HW_ANTIFAKE = "Hardware Antifake";
    public static final String GBMSAPI_DF_STRING_SW_ANTIFAKE = "Software Antifake";
    public static final String GBMSAPI_DF_STRING_HEATER = "Heater";

    public static final ArrayList<String> DeviceFeaturesToStringList(int mask) {
        ArrayList<String> ValToRet = new ArrayList<String>();

        if ((mask & GBMSAPI_DF_MULTIPLE_USB_CONNECTIONS_ALLOWED) != 0)
            ValToRet.add(GBMSAPI_DF_STRING_MULTIPLE_USB_CONNECTIONS_ALLOWED);
        if ((mask & GBMSAPI_DF_AUTO_CAPTURE_BLOCKING) != 0)
            ValToRet.add(GBMSAPI_DF_STRING_AUTO_CAPTURE_BLOCKING);
        if ((mask & GBMSAPI_DF_1000DPI_RESOLUTION) != 0)
            ValToRet.add(GBMSAPI_DF_STRING_1000DPI_RESOLUTION);
        if ((mask & GBMSAPI_DF_IMAGE_ROTATION) != 0) ValToRet.add(GBMSAPI_DF_STRING_IMAGE_ROTATION);
        if ((mask & GBMSAPI_DF_PERMANENT_USER_DATA_STORAGE) != 0)
            ValToRet.add(GBMSAPI_DF_STRING_PERMANENT_USER_DATA_STORAGE);
        if ((mask & GBMSAPI_DF_FRAME_RATE_SETTING) != 0)
            ValToRet.add(GBMSAPI_DF_STRING_FRAME_RATE_SETTING);
        if ((mask & GBMSAPI_DF_FW_INFO_RETRIEVE) != 0)
            ValToRet.add(GBMSAPI_DF_STRING_FW_INFO_RETRIEVE);
        if ((mask & GBMSAPI_DF_ROLL_AREA_IQS) != 0) ValToRet.add(GBMSAPI_DF_STRING_ROLL_AREA_IQS);
        if ((mask & GBMSAPI_DF_ROLL_AREA_GA) != 0) ValToRet.add(GBMSAPI_DF_STRING_ROLL_AREA_GA);
        if ((mask & GBMSAPI_DF_DRY_SKIN_IMG_ENHANCE) != 0)
            ValToRet.add(GBMSAPI_DF_STRING_DRY_SKIN_IMG_ENHANCE);
        if ((mask & GBMSAPI_DF_ROLL_OBJECT_STRIPE) != 0)
            ValToRet.add(GBMSAPI_DF_STRING_ROLL_OBJECT_STRIPE);
        if ((mask & GBMSAPI_DF_UPDATE_BACKGROUND_IMAGE) != 0)
            ValToRet.add(GBMSAPI_DF_STRING_UPDATE_BACKGROUND_IMAGE);
        if ((mask & GBMSAPI_DF_ENABLE_BLOCK_ROLL_COMPOSITION) != 0)
            ValToRet.add(GBMSAPI_DF_STRING_ENABLE_BLOCK_ROLL_COMPOSITION);
        if ((mask & GBMSAPI_DF_USB_3_0_SUPPORT) != 0)
            ValToRet.add(GBMSAPI_DF_STRING_USB_3_0_SUPPORT);
        if ((mask & GBMSAPI_DF_HW_ANTIFAKE) != 0) ValToRet.add(GBMSAPI_DF_STRING_HW_ANTIFAKE);
        if ((mask & GBMSAPI_DF_SW_ANTIFAKE) != 0) ValToRet.add(GBMSAPI_DF_STRING_SW_ANTIFAKE);
        if ((mask & GBMSAPI_DF_HEATER) != 0) ValToRet.add(GBMSAPI_DF_STRING_HEATER);

        return ValToRet;
    }
}
