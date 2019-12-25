package com.greenbit.gbmsapi;

import com.greenbit.utils.GBJavaWrapperUtilBOOLForJavaToCExchange;
import com.greenbit.utils.GBJavaWrapperUtilDoubleForJavaToCExchange;
import com.greenbit.utils.GBJavaWrapperUtilIntForJavaToCExchange;

public class GBMSAPIJavaWrapperLibrary {
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("GBMSAPI_JNI");
    }

    public native String GetLastErrorString();

    public native int Load();

    public native int Unload();

    public native int GetAttachedDeviceList(GBMSAPIJavaWrapperDefinesDeviceInfoStruct[] attachedDeviceList);

    public native int SetOpenedJavaFD(int[] oJFD, int[] b_num, int[] devAdd, int numOfOJFD);

    public native int SetCurrentDevice(int deviceID, String DeviceSerialNumber);

    public native int GetCurrentDevice(GBMSAPIJavaWrapperDefinesDeviceInfoStruct currentDevice);

    public native int GetScannableTypes(GBJavaWrapperUtilIntForJavaToCExchange ScannableTypes);

    public native int GetDeviceFeatures(GBJavaWrapperUtilIntForJavaToCExchange DeviceFeatures);

    public native int GetSupportedScanAreas(GBJavaWrapperUtilIntForJavaToCExchange ScanAreas);

    public native int GetSupportedScanAreasByDevice(int DeviceID, GBJavaWrapperUtilIntForJavaToCExchange ScanAreas);

    public native int SetRollAreaStandard(int RollAreaStandard);

    public native int GetSupportedScanOptions(GBJavaWrapperUtilIntForJavaToCExchange ScanOptions);

    public native int GetOptionalExternalEquipment(GBJavaWrapperUtilIntForJavaToCExchange OptionalExternalEquipment);

    public native int GetAvailableImageInfo(int ObjectTypeToScan, GBJavaWrapperUtilIntForJavaToCExchange AvailableImageInfo);

    public native int GetImageSize(
            int ObjectTypeToScan, int OptionMask, int ScanArea,
            GBMSAPIJavaWrapperDefinesImageSize FullResImageSize,
            GBMSAPIJavaWrapperDefinesImageSize PreviewImageSize);

    public native int GetMaxImageSize(GBMSAPIJavaWrapperDefinesImageSize maxImageSize);

    public native int GetCalibrationImageSize(
            int ScanArea,
            GBMSAPIJavaWrapperDefinesImageSize CalibrationImageSize);

    public native int GetFrameRateRange(
            int DeviceID, int FrameRateOptions, int ScanArea,
            GBJavaWrapperUtilDoubleForJavaToCExchange MaxFrameRate,
            GBJavaWrapperUtilDoubleForJavaToCExchange MinFrameRate,
            GBJavaWrapperUtilDoubleForJavaToCExchange DefaultFrameRate
    );

    public native int SetDiagnosticFilterForLCD(int DiagnosticToBeFiltered);

    public native int EnableAutoCaptureBlockForDetectedFakes(boolean Flag);

    public native int SetHardwareFakeFingerDetectionThreshold(int FFDThreshold);

    public native int HardwareFakeFingerDetection(
            GBJavaWrapperUtilBOOLForJavaToCExchange FfdFlag,
            GBJavaWrapperUtilIntForJavaToCExchange FfdDiagnostic
    );

    public native int SetSoftwareFakeFingerDetectionThreshold(int FFDThreshold);

    public native int SoftwareFakeFingerDetection(
            byte[] ImageFrame, int SizeX, int SizeY,
            GBJavaWrapperUtilBOOLForJavaToCExchange FfdFlag
    );

    public native int SetSelectImageTimeout(int SelectImageTimeout);

    public native int SetLowerHalfPalmCompletenessThreshold(int CompletenessThreshold);

    public native int SetFrameRate(int ScanArea, int FrameRateOptions, double FrameRate);

    public native int EnableManualLEDControl(boolean Enable);

    public native int EnableLEDColorSettingDuringAcquisition(boolean Enable);

    public native int SetImageRotation(int ImageRotation);

    public native int EnableDrySkinImgEnhance(boolean Enable);

    public native int RollEnableRollStripeAcquisition(int ScanArea, boolean Enable);

    public native int RollEnableBlockComposition(boolean Enable);

    public native int StartAcquisition(
            int jObjToScan, int jOptionMask, int jScanArea,
            Object jObjThatImplementsCallback,
            Object jAcquisitionParameters,
            int jDisplayOptionMask,
            int jContrastLimitLCD, int jCompletenessLimitLCD);

    public native int StopAcquisition();

    public native int RollStopPreview();

    public native int SetClippingRegionSize(int ClippingRegionWidth, int ClippingRegionHeigth);

    public native int RollSetPreviewTimeout(int Timeout);

    public native int VuiLedBlinkDuringAcquisition(boolean Blink);

    public native int GetDryWetFingerAreaPercent(
            GBJavaWrapperUtilIntForJavaToCExchange Dry,
            GBJavaWrapperUtilIntForJavaToCExchange Wet
    );

    public native int GetAutoCapturePhase(
            GBJavaWrapperUtilIntForJavaToCExchange AcquisitionPhase
    );

    public native int SetAutoCaptureBlocking(boolean Blocking);

    public native int GetFingerprintContrast(GBJavaWrapperUtilIntForJavaToCExchange Contrast);

    public native int GetFingerprintSize(GBJavaWrapperUtilIntForJavaToCExchange FingerprintSize);

    public native int GetLowerHalfPalmCompleteness(GBJavaWrapperUtilIntForJavaToCExchange LowerHalfPalmCompleteness);

    public native int GetClippingRegionPosition(
            GBJavaWrapperUtilIntForJavaToCExchange ClippingRegionPosX,
            GBJavaWrapperUtilIntForJavaToCExchange ClippingRegionPosY,
            GBJavaWrapperUtilIntForJavaToCExchange ClippingRegionSizeX,
            GBJavaWrapperUtilIntForJavaToCExchange ClippingRegionSizeY
    );

    public native int RollGetCurrentStripeCoord(
            GBJavaWrapperUtilIntForJavaToCExchange LeftX,
            GBJavaWrapperUtilIntForJavaToCExchange RightX
    );

    public native int UpdateBackgroundImage(
            byte [] ImageFrame,
            GBJavaWrapperUtilIntForJavaToCExchange Diagnostic
    );

    public native int ImageFinalization(byte[] ImageFrame);

    public native int GetFrameStatistic(
            GBJavaWrapperUtilIntForJavaToCExchange AcquiredFrames,
            GBJavaWrapperUtilIntForJavaToCExchange LostFrames
    );

    public native int RollGetCompositeImageInfo(
            GBJavaWrapperUtilIntForJavaToCExchange RolledArtefactSize,
            byte[]  MarkerFrame,
            byte[]  NotWipedArtefactFrame,
            GBJavaWrapperUtilIntForJavaToCExchange CompositeFingerprintSize,
            GBJavaWrapperUtilIntForJavaToCExchange CompositeFingerprintContrast,
            GBJavaWrapperUtilIntForJavaToCExchange FlatFingerprintSize,
            GBJavaWrapperUtilIntForJavaToCExchange SizeX,
            GBJavaWrapperUtilIntForJavaToCExchange SizeY
    );

    public native int SetCalibrationImage(int ScanArea, byte [] ImageFrame);

    public native int GetCalibrationImage(int ScanArea, byte[] ImageFrame, GBJavaWrapperUtilIntForJavaToCExchange Diagnostic);

    public native int GetMultiScanAPIVersion(
            GBJavaWrapperUtilIntForJavaToCExchange Ver1,
            GBJavaWrapperUtilIntForJavaToCExchange Ver2,
            GBJavaWrapperUtilIntForJavaToCExchange Ver3,
            GBJavaWrapperUtilIntForJavaToCExchange Ver4
    );

    /*******
     *
     * @param Ver1
     * @param Ver2
     * @param Ver3
     * @param Ver4
     * @return null means error
     */
    public native String GetUnderlyingLibraryDllVersion(
            GBJavaWrapperUtilIntForJavaToCExchange Ver1,
            GBJavaWrapperUtilIntForJavaToCExchange Ver2,
            GBJavaWrapperUtilIntForJavaToCExchange Ver3,
            GBJavaWrapperUtilIntForJavaToCExchange Ver4
    );

    public native int GetUSBErrorCode(GBJavaWrapperUtilIntForJavaToCExchange USBErrorCode);

    public native int GetScannerStatistics(
            GBJavaWrapperUtilIntForJavaToCExchange Counter,
            GBJavaWrapperUtilIntForJavaToCExchange ProductionDate
    );

    public native int Sound (int Tone, int Duration, int Iterations);

    public native int GetUserDataSize(GBJavaWrapperUtilIntForJavaToCExchange UserDataSize);

    public native int ReadUserData(int Address, int Size, byte [] DataBuffer);

    public native int WriteUserData(int Address, int Size, byte [] DataBuffer);

    public native int SetFingerIndicatorVUIState(int IndicatorMask, int Color, boolean Blink);

    public native int GetUsbLinkSpeed(
            GBJavaWrapperUtilIntForJavaToCExchange UsbLinkSpeed);

    public native int GetheaterMeanTempRange(
            GBJavaWrapperUtilDoubleForJavaToCExchange Max,
            GBJavaWrapperUtilDoubleForJavaToCExchange Default,
            GBJavaWrapperUtilDoubleForJavaToCExchange Min
    );

    public native int GetHeaterMeanTemp(
            GBJavaWrapperUtilDoubleForJavaToCExchange TempC
    );

    public native int SetHeaterMeanTemp(
            double TempC
    );

    public native int CheckHeater(
            GBJavaWrapperUtilIntForJavaToCExchange Result);
}
