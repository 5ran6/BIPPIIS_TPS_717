package com.greenbit.gbmsapi;

public class GBMSAPIJavaWrapperDefinesDeviceInfoStruct {
    // constants
    public static int GBMSAPI_MAX_PLUGGED_DEVICE_NUM = 127;
    public static int GBMSAPI_MAX_SN_LENGHT = 20;
    // private fields
    private byte[] DeviceSerialNumArray;
    // public fields
    public int DeviceID;
    public String DeviceSerialNum;
    public void SetFields(int devId, String serial)
    {
        this.DeviceID = devId;
        this.DeviceSerialNum = serial;
    }

    public static int GetNumberOfAttachedDevices(GBMSAPIJavaWrapperDefinesDeviceInfoStruct[] list)
    {
        int valToRet = 0;
        for (GBMSAPIJavaWrapperDefinesDeviceInfoStruct item: list
             ) {
            if ((item.DeviceSerialNum != "") && (item.DeviceID > 0)) valToRet ++;
        }
        return  valToRet;
    }
}
