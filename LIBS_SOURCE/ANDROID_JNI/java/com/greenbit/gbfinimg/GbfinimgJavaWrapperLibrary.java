package com.greenbit.gbfinimg;

import com.greenbit.utils.GBJavaWrapperUtilDoubleForJavaToCExchange;
import com.greenbit.utils.GBJavaWrapperUtilIntForJavaToCExchange;

public class GbfinimgJavaWrapperLibrary {
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("GBFINIMG_JNI");
    }

    public native String GetLastErrorString();

    public native int Load();

    public native int Unload();

    /**
     *
     * @param Mask : input, @see {@link GbfinimgJavaWrapperDefinesMissingFingersMask}
     * @return @see {@link GbfinimgJavaWrapperDefinesReturnCodes}
     */
    public native int Slap4MissingFingers(
            int Mask
    );

    /**
     *
     * @param Probability :output, -1 = failure; [0,100] normal range
     * @param PatternDiagnostic : output, @see {@link GbfinimgJavaWrapperDefinesPatternDiagnosticMask}
     * @return @see {@link GbfinimgJavaWrapperDefinesReturnCodes}
     */
    public native int CheckFingerRotation180Degree(
            GBJavaWrapperUtilIntForJavaToCExchange Probability,
            GBJavaWrapperUtilIntForJavaToCExchange PatternDiagnostic
    );

    /**
     *
     * @param FingerprintNumber :output, -1 = failure; [1,4] normal range
     * @return @see {@link GbfinimgJavaWrapperDefinesReturnCodes}
     */
    public native int GetFingerprintCounter(
            GBJavaWrapperUtilIntForJavaToCExchange FingerprintNumber
    );

    /**
     *
     * @param Result : output, @see {@link GbfinimgJavaWrapperDefinesScannerBasedLicenseResult}
     * @param UsbDriverErrorCodeDetail : output, special hexadecimal codes, ask Greenbit support
     *                                 for further information
     * @return @see {@link GbfinimgJavaWrapperDefinesReturnCodes}
     */
    public native int GetLicenseInfo(
            GBJavaWrapperUtilIntForJavaToCExchange Result,
            GBJavaWrapperUtilIntForJavaToCExchange UsbDriverErrorCodeDetail
    );

    /**
     *
     * @param SegmentIndex : input, the corresponding index of the output segments of
     *                     @see ProcessImage
     * @param MissingDeltaNumber : output, [0,1,2] normal range
     * @param MissingDeltaMask : output, @see {@link GbfinimgJavaWrapperDefinesMissingDeltaMask}
     * @return @see {@link GbfinimgJavaWrapperDefinesReturnCodes}
     */
    public native int GetMissingDeltaNumber(
            int SegmentIndex,
            GBJavaWrapperUtilIntForJavaToCExchange MissingDeltaNumber,
            GBJavaWrapperUtilIntForJavaToCExchange MissingDeltaMask
    );

    /**
     *
     * @param LeftTopCornerFillingPercent : output, [0,100] normal range
     * @param RightTopCornerFillingPercent : output, [0,100] normal range
     * @return @see {@link GbfinimgJavaWrapperDefinesReturnCodes}
     */
    public native int GetRolledFingerprintTopCornerFillingPercent(
            GBJavaWrapperUtilIntForJavaToCExchange LeftTopCornerFillingPercent,
            GBJavaWrapperUtilIntForJavaToCExchange RightTopCornerFillingPercent
    );

    /**
     *
     * @param SegmentIndex : input, the corresponding index of the output segments of
     *                     @see ProcessImage
     * @param MedialJoint_BoundingBoxL
     * @param MedialJoint_BoundingBoxR
     * @param MedialJoint_BoundingBoxT
     * @param MedialJoint_BoundingBoxB
     * @param MedialJoint_Completeness
     * @param ProximalJoint_BoundingBoxL
     * @param ProximalJoint_BoundingBoxR
     * @param ProximalJoint_BoundingBoxT
     * @param ProximalJoint_BoundingBoxB
     * @param ProximalJoint_Completeness
     * @return @see {@link GbfinimgJavaWrapperDefinesReturnCodes}
     */
    public native int GetFullFingerMedialProximalJointBoundingBox(
            int SegmentIndex,
            GBJavaWrapperUtilIntForJavaToCExchange MedialJoint_BoundingBoxL,
            GBJavaWrapperUtilIntForJavaToCExchange MedialJoint_BoundingBoxR,
            GBJavaWrapperUtilIntForJavaToCExchange MedialJoint_BoundingBoxT,
            GBJavaWrapperUtilIntForJavaToCExchange MedialJoint_BoundingBoxB,
            GBJavaWrapperUtilIntForJavaToCExchange MedialJoint_Completeness,
            GBJavaWrapperUtilIntForJavaToCExchange ProximalJoint_BoundingBoxL,
            GBJavaWrapperUtilIntForJavaToCExchange ProximalJoint_BoundingBoxR,
            GBJavaWrapperUtilIntForJavaToCExchange ProximalJoint_BoundingBoxT,
            GBJavaWrapperUtilIntForJavaToCExchange ProximalJoint_BoundingBoxB,
            GBJavaWrapperUtilIntForJavaToCExchange ProximalJoint_Completeness
    );

    /**
     *
     * @param BoundingBoxL
     * @param BoundingBoxR
     * @param BoundingBoxT
     * @param BoundingBoxB
     * @param Completeness
     * @param Quality
     * @return @see {@link GbfinimgJavaWrapperDefinesReturnCodes}
     */
    public native int GetPalmPrintInterdigitalAreaBoundingBox(
            GBJavaWrapperUtilIntForJavaToCExchange BoundingBoxL,
            GBJavaWrapperUtilIntForJavaToCExchange BoundingBoxR,
            GBJavaWrapperUtilIntForJavaToCExchange BoundingBoxT,
            GBJavaWrapperUtilIntForJavaToCExchange BoundingBoxB,
            GBJavaWrapperUtilIntForJavaToCExchange Completeness,
            GBJavaWrapperUtilIntForJavaToCExchange Quality
    );

    /**
     *
     * @param BoundingBoxL
     * @param BoundingBoxR
     * @param BoundingBoxT
     * @param BoundingBoxB
     * @param Completeness
     * @param Quality
     * @return @see {@link GbfinimgJavaWrapperDefinesReturnCodes}
     */
    public native int GetUpperPalmFingerPhalangesBoundingBox(
            GBJavaWrapperUtilIntForJavaToCExchange BoundingBoxL,
            GBJavaWrapperUtilIntForJavaToCExchange BoundingBoxR,
            GBJavaWrapperUtilIntForJavaToCExchange BoundingBoxT,
            GBJavaWrapperUtilIntForJavaToCExchange BoundingBoxB,
            GBJavaWrapperUtilIntForJavaToCExchange Completeness,
            GBJavaWrapperUtilIntForJavaToCExchange Quality
    );

    /**
     *
     * @param InputImage : input
     * @param InputSizeX : input
     * @param InputSizeY : input
     * @param InputImageType : input @see {@link GbfinimgJavaWrapperDefinesInputImageType}
     * @param ImageProcessingOptions : input @see {@link GbfinimgJavaWrapperDefinesProcessOptions}
     * @param SegmentImageSizeX : input
     * @param SegmentImagesizeY : input
     * @param ExternalInfoList : input @see {@link GbfinimgJavaWrapperDefinesSegmentImageExternalInfo}
     * @param ExternalInfoListSize : input
     * @param SegmentImageList : output @see {@link GbfinimgJavaWrapperDefineSegmentImageDescriptor}
     *                         NOTE1: for each segment the "SegmentImage" array should be at least
     *                         SegmentImageSizeX * SegmentImagesizeY;
     *                         NOTE2: the SegmentImageList size should be adequate to the
     *                         InputImageType; anyway, to be sure, declare it a 4-elements array
     * @param DetectedSegmentImageNumber : output
     * @param DetectionFailureDiagnostic : output
     * @return @see {@link GbfinimgJavaWrapperDefinesReturnCodes}
     */
    public native int ProcessImage(
            byte [] InputImage,
            int InputSizeX, int InputSizeY,
            int InputImageType, int ImageProcessingOptions,
            int SegmentImageSizeX, int SegmentImagesizeY,
            GbfinimgJavaWrapperDefinesSegmentImageExternalInfo [] ExternalInfoList,
            int ExternalInfoListSize,
            GbfinimgJavaWrapperDefineSegmentImageDescriptor [] SegmentImageList,
            GBJavaWrapperUtilIntForJavaToCExchange DetectedSegmentImageNumber,
            GBJavaWrapperUtilIntForJavaToCExchange DetectionFailureDiagnostic
    );

    public native int SetOpenedJavaFD(int[] oJFD, int numOfOJFD);
}
