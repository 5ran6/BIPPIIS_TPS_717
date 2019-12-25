package com.greenbit.gbmsapi;

import java.util.ArrayList;

public class GBMSAPIJavaWrapperDefinesOptionalExternalEquipment {
    public static final int GBMSAPI_OED_PEDAL = 0x00000001;
    public static final int GBMSAPI_OED_SOUND = 0x00000002;
    public static final int GBMSAPI_OED_VUI_LED = 0x00000004;
    public static final int GBMSAPI_OED_VUI_LCD = 0x00000008;

    public static final String GBMSAPI_OED_STRING_PEDAL = "Pedal";
    public static final String GBMSAPI_OED_STRING_SOUND = "Buzzer";
    public static final String GBMSAPI_OED_STRING_VUI_LED = "LED Interface";
    public static final String GBMSAPI_OED_STRING_VUI_LCD = "LCD Display";

    public static final ArrayList<String> OptionalExternalEquipmentToStringList(int mask) {
        ArrayList<String> ValToRet = new ArrayList<String>();

        if ((mask & GBMSAPI_OED_PEDAL) != 0) ValToRet.add(GBMSAPI_OED_STRING_PEDAL);
        if ((mask & GBMSAPI_OED_SOUND) != 0) ValToRet.add(GBMSAPI_OED_STRING_SOUND);
        if ((mask & GBMSAPI_OED_VUI_LCD) != 0) ValToRet.add(GBMSAPI_OED_STRING_VUI_LCD);
        if ((mask & GBMSAPI_OED_VUI_LED) != 0) ValToRet.add(GBMSAPI_OED_STRING_VUI_LED);

        return ValToRet;
    }
}
