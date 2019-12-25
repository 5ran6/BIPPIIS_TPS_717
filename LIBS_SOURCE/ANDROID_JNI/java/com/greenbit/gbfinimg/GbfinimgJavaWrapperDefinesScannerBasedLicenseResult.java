package com.greenbit.gbfinimg;

public class GbfinimgJavaWrapperDefinesScannerBasedLicenseResult {
    /**
     * GBFINIMG license is granted
     */
    public static final int GBSCAN_NO_ERROR = 0;
    /**
     * An USB driver error happend during license check,
     * see UsbDriverErrorCodeDetail for more detail
     */
    public static final int GBSCAN_ERROR_USB_DRIVER = 1;
    /**
     * GBFINIMG license is NOT granted
     */
    public static final int GBSCAN_ERROR_USB_DEVICE_NOT_FOUND = 2;
}
