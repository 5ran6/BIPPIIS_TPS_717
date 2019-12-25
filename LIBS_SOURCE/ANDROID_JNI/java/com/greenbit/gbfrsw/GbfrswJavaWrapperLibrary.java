package com.greenbit.gbfrsw;

import com.greenbit.utils.GBJavaWrapperUtilDoubleForJavaToCExchange;
import com.greenbit.utils.GBJavaWrapperUtilIntForJavaToCExchange;

public class GbfrswJavaWrapperLibrary {
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("GBFRSW_JNI");
    }

    public native String GetLastErrorString();

    public native int Load();

    public native int Unload();

    /**
     *
     * @param VersionField1
     * @param VersionField2
     * @param VersionField3
     * @param VersionField4
     * @param MaxImageSizeX
     * @param MaxImageSizeY
     * @param MinImageSizeX
     * @param MinImageSizeY
     * @param RequestedOperations @see {@link GbfrswJavaWrapperDefinesRequestedOperations}
     * @param MemoryBufferSize
     * @return
     */
    public native int GetVersionInfo(
            // INPUT
            int RequestedOperations,
            // OUTPUT
            GBJavaWrapperUtilIntForJavaToCExchange VersionField1,
            GBJavaWrapperUtilIntForJavaToCExchange VersionField2,
            GBJavaWrapperUtilIntForJavaToCExchange VersionField3,
            GBJavaWrapperUtilIntForJavaToCExchange VersionField4,
            GBJavaWrapperUtilIntForJavaToCExchange MaxImageSizeX,
            GBJavaWrapperUtilIntForJavaToCExchange MaxImageSizeY,
            GBJavaWrapperUtilIntForJavaToCExchange MinImageSizeX,
            GBJavaWrapperUtilIntForJavaToCExchange MinImageSizeY,
            GBJavaWrapperUtilIntForJavaToCExchange MemoryBufferSize
    );

    /**
     *
     * @param ImageSizeX
     * @param ImageSizeY
     * @param SampleCodeSize
     * @param PackedSampleCodeSize
     * @param TemplateCodeSize
     * @param CompactTemplateCodeSize
     * @return
     */
    public native int GetCodeMaxSize(
            // INPUT
            int   ImageSizeX,
            int   ImageSizeY,
            // OUTPUT
            GBJavaWrapperUtilIntForJavaToCExchange SampleCodeSize,
            GBJavaWrapperUtilIntForJavaToCExchange PackedSampleCodeSize,
            GBJavaWrapperUtilIntForJavaToCExchange TemplateCodeSize,
            GBJavaWrapperUtilIntForJavaToCExchange CompactTemplateCodeSize
    );

    /**
     *
     * @param ImageSizeX
     * @param ImageSizeY
     * @param ImageBuffer
     * @param ImageFlags @see {@link GbfrswJavaWrapperDefinesImageFlags}
     * @param Options @see {@link GbfrswJavaWrapperDefinesCodingOptions}
     * @param SampleCode
     * @return
     */
    public native int Coding(
            // INPUT
            int    ImageSizeX,
            int    ImageSizeY,
            byte [] ImageBuffer,
            int   ImageFlags,
            int   Options,
            // OUTPUT
            byte [] SampleCode
    );

    /**
     *
     * @param ImageSizeX use the same as @see Coding
     * @param ImageSizeY use the same as @see Coding
     * @param SampleCode
     * @param TemplateCode
     * @return
     */
    public native int Enroll(
            // INPUT
            int    ImageSizeX,
            int    ImageSizeY,
            // OUTPUT
            byte [] SampleCode,
            byte [] TemplateCode
    );

    /**
     *
     * @param SampleCode
     * @param TemplateCode
     * @param SpeedVsPrecisionTradeoff @see {@link GbfrswJavaWrapperDefinesSpeedPrecisionTradeoff}
     * @param UnmatchedDataFactor @see {@link GbfrswJavaWrapperDefinesUnmatchedDataFactorInfluenceRecommended}
     * @param Options @see {@link GbfrswJavaWrapperDefinesMatchOptions}
     * @param MaxRotationAngleDegree: the less the angle, the faster operation; @see {@link GbfrswJavaWrapperDefinesRanges}
     * @param MatchingScore @see {@link GbfrswJavaWrapperDefinesRanges}
     * @return
     */
    public native int Match(
            // INPUT
            byte [] SampleCode,
            byte [] TemplateCode,
            int SpeedVsPrecisionTradeoff,
            int UnmatchedDataFactor,
            int Options,
            int	MaxRotationAngleDegree,
            // OUTPUT
            GBJavaWrapperUtilDoubleForJavaToCExchange MatchingScore
    );

    /**
     *
     * @param Original
     * @param Converted VERY IMPORTANT NOTE:
     *                  check that its size can contain the data from the FMR; Greenbit suggests to
     *                  call
     *                  @see GetCodeMaxSize with the max image sizes as provided by
     *                  @see GetVersionInfo
     *                  This because no check is performed by the underlying C++ wrapper and, if
     *                  the size is not adequate, this could lead to a crash of the application.
     * @return
     */
    public native int Convert(
            // INPUT
            byte [] Original,
            // OUTPUT
            byte [] Converted
    );

    /**
     *
     * @param SampleCode1 can be null, but at least one of the other SampleCodeX MUST be NOT null
     * @param SampleCode2 can be null, but at least one of the other SampleCodeX MUST be NOT null
     * @param SampleCode3 can be null, but at least one of the other SampleCodeX MUST be NOT null
     * @param CorrectedSampleCode VERY IMPORTANT NOTE:
     *                            check that its size can contain the data from the FMR; Greenbit
     *                            suggests to call
     *                            @see GetCodeMaxSize with the max image sizes as provided by
     *                            @see GetVersionInfo
     *                            This because no check is performed by the underlying C++ wrapper
     *                            and, if the size is not adequate, this could lead to a crash of
     *                            the application.
     * @return
     */
    public native int Correct(
            // INPUT
            byte [] SampleCode1,
            byte [] SampleCode2,
            byte [] SampleCode3,
            // OUTPUT
            byte [] CorrectedSampleCode
    );

    /**
     *
     * @param InputCode
     * @param Size
     * @param SizeAfterConversion
     * @param Type @see {@link GbfrswJavaWrapperDefinesCodingTypes}
     * @param Version
     * @param ImageFlags @see {@link GbfrswJavaWrapperDefinesImageFlags}
     * @param Quality @see {@link GbfrswJavaWrapperDefinesRanges}
     * @param InformationCapacity @see {@link GbfrswJavaWrapperDefinesRanges}
     * @param ImageSx
     * @param ImageSy
     * @return
     */
    public native int GetCodeHeader(
            // INPUT
            byte [] InputCode,
            // OUTPUT
            GBJavaWrapperUtilIntForJavaToCExchange Size,
            GBJavaWrapperUtilIntForJavaToCExchange SizeAfterConversion,
            GBJavaWrapperUtilIntForJavaToCExchange Type,
            GBJavaWrapperUtilIntForJavaToCExchange Version,
            GBJavaWrapperUtilIntForJavaToCExchange ImageFlags,
            GBJavaWrapperUtilIntForJavaToCExchange Quality,
            GBJavaWrapperUtilDoubleForJavaToCExchange InformationCapacity,
            GBJavaWrapperUtilIntForJavaToCExchange ImageSx,
            GBJavaWrapperUtilIntForJavaToCExchange ImageSy
    );

    /**
     *
     * @param ImageSizeX
     * @param ImageSizeY
     * @param FormatType @see {@link GbfrswJavaWrapperDefinesISOFMRFormat}
     * @param GBProprietaryData @see {@link GbfrswJavaWrapperDefinesISOFMRGBProprietaryData}
     * @param MaxFRMLength
     * @return
     */
    public native int ISOGetMaxFMRLength(
            // INPUT
            int   ImageSizeX,
            int   ImageSizeY,
            int FormatType,
            int GBProprietaryData,
            // OUTPUT
            GBJavaWrapperUtilIntForJavaToCExchange MaxFRMLength
    );

    /**
     *
     * @param GBTemplate
     * @param ImageSizeX the same as provided at @see ISOGetMaxFMRLength
     * @param ImageSizeY the same as provided at @see ISOGetMaxFMRLength
     * @param FingerPosition @see {@link GbfrswJavaWrapperDefinesISOFMRFingerPositions}
     * @param FormatType the same as provided at  ISOGetMaxFMRLength.
     *                   VERY IMPORTANT NOTE:
     *                   it's important to store somewhere the value of format type, since the
     *                   <ISO/IEC 19794-2> record does not contain such information itself
     * @param GBProprietaryData the same as provided at ISOGetMaxFMRLength
     * @param FMRBuffer provide that its size is not less than the "MaxFMRLength" parameter of
     *                  the @see ISOGetMaxFMRLength
     * @param FMRBufferLen
     * @return
     */
    public native int ISOGBTemplateToFMR(
            // INPUT
            byte [] GBTemplate,
            int   ImageSizeX,
            int   ImageSizeY,
            int FingerPosition,
            int FormatType,
            int GBProprietaryData,
            // OUTPUT
            byte [] FMRBuffer,
            GBJavaWrapperUtilIntForJavaToCExchange FMRBufferLen
    );

    /**
     *
     * @param FMRBuffer
     * @param FormatType @see {@link GbfrswJavaWrapperDefinesISOFMRFormat} VERY IMPORTANT NOTE:
     *                   it's important that the value of format type has been stored somewhere when
     *                   the GB Template was converted to FMR format (or be known a priori),
     *                   since the <ISO/IEC 19794-2> record does not contain such information itself.
     * @param GBTemplate VERY IMPORTANT NOTE:
     *                   check that its size can contain the data from the FMR; Greenbit suggests to
     *                   call
     *                   @see GetCodeMaxSize with the max image sizes as provided by
     *                   @see GetVersionInfo
     *                   This because no check is performed by the underlying C++ wrapper and, if
     *                   the size is not adequate, this could lead to a crash of the application.
     * @param FingerPosition
     * @return
     */
    public native int ISOFMRToGBTemplate(
            // INPUT
            byte [] FMRBuffer,
            int FormatType,
            // OUTPUT
            byte [] GBTemplate,
            GBJavaWrapperUtilIntForJavaToCExchange FingerPosition
    );

    /**
     *
     * @param ImageSizeX
     * @param ImageSizeY
     * @param FormatType @see {@link GbfrswJavaWrapperDefinesISOFMCFormat}
     * @param MaxMinutiaeNumber maximum number of minutiae to be stored inside the FMC;
     *                          Application can use this parameter to limit the size of the
     *                          produced FMC; if set to 0, all minutiae will be stored.
     * @param MaxFMCLength
     * @return
     */
    public native int ISOGetMaxFMCLength(
            // INPUT
            int ImageSizeX, int ImageSizeY,
            int FormatType, int MaxMinutiaeNumber,
            // OUTPUT
            GBJavaWrapperUtilIntForJavaToCExchange MaxFMCLength
    );

    /**
     *
     * @param GBTemplate
     * @param ImageSizeX provide the same value as @see ISOGetMaxFMCLength
     * @param ImageSizeY provide the same value as @see ISOGetMaxFMCLength
     * @param FormatType provide the same value as @see ISOGetMaxFMCLength
     * @param MaxMinutiaeNumber provide the same value as @see ISOGetMaxFMCLength
     * @param FMCBuffer provide that its size is not less than the "MaxFMCLength" parameter of
     *                  the @see ISOGetMaxFMCLength
     * @param FMCBufferLen
     * @return
     */
    public native int ISOGBTemplateToFMC(
            // INPUT
            byte [] GBTemplate,
            int ImageSizeX, int ImageSizeY,
            int FormatType, int MaxMinutiaeNumber,
            // OUTPUT
            byte [] FMCBuffer,
            GBJavaWrapperUtilIntForJavaToCExchange FMCBufferLen
    );

    public native int SetOpenedJavaFD(int[] oJFD, int[] b_num, int[] devAdd, int numOfOJFD);
}
