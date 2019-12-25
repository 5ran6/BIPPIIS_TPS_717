/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package GBFRSW_JAVA_Functions;

import GBFRSW_JAVA_Defines.GBFRSW_TradeOff;
import GBFRSW_JAVA_Defines.GBFRSW_CodeHeader;
import GBFRSW_JAVA_Defines.GBFRSW_FileLicenseCheck;
import GBFRSW_JAVA_Defines.GBFRSW_LicenseUsbScannerCheck;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Pointer;
import com.sun.jna.NativeLong;
import com.sun.jna.ptr.IntByReference;
import com.sun.jna.ptr.ByteByReference;
import com.sun.jna.ptr.PointerByReference;
import com.sun.jna.ptr.DoubleByReference;
import com.sun.jna.ptr.FloatByReference;
import com.sun.jna.ptr.ShortByReference;
import com.sun.jna.WString;
import java.nio.ByteBuffer;

/**
 *
 * @author andrea
 */
public class GBFRSW_DllWrapper {

    public interface GBFRSW_Library extends Library {

        GBFRSW_Library INSTANCE
                = (GBFRSW_Library) Native.loadLibrary("GBFRSW", GBFRSW_Library.class);

        /**
         *
         * @param VersionField1
         * @param VersionField2
         * @param VersionField3
         * @param VersionField4
         * @param MaxImageSizeX Native is an int32 see point (1) of ReadME.txt
         * file
         * @param MaxImageSizeY Native is an int32 see point (1) of ReadME.txt
         * file
         * @param MinImageSizeX Native is an int32 see point (1) of ReadME.txt
         * file
         * @param MinImageSizeY Native is an int32 see point (1) of ReadME.txt
         * file
         * @param RequestedOperations Native is an int32 see point (1) of
         * ReadME.txt file
         * @see GBFRSW_JAVA_Defines.GBFRSW_MemoryRequestCodes
         * @param MemoryBufferSize Native is an int32 see point (1) of
         * ReadME.txt file
         * @return
         * @see GBFRSW_JAVA_DEFINES.GBFRSW_ReturnCodes
         */
        int GBFRSW_GetVersionInfo(ByteByReference VersionField1,
                ByteByReference VersionField2,
                ByteByReference VersionField3,
                ByteByReference VersionField4,
                IntByReference MaxImageSizeX,
                IntByReference MaxImageSizeY,
                IntByReference MinImageSizeX,
                IntByReference MinImageSizeY,
                int RequestedOperations,
                IntByReference MemoryBufferSize);

        /**
         *
         * @param ImageSizeX Native is an int32 see point (1) of ReadME.txt file
         * @param ImageSizeY Native is an int32 see point (1) of ReadME.txt file
         * @param SampleCodeSize Native is an int32 see point (1) of ReadME.txt
         * file
         * @param PackedSampleCodeSize Native is an int32 see point (1) of
         * ReadME.txt file
         * @param TemplateCodeSize Native is an int32 see point (1) of
         * ReadME.txt file
         * @param CompactTemplateCodeSize Native is an int32 see point (1) of
         * ReadME.txt file
         * @return
         * @see GBFRSW_JAVA_DEFINES.GBFRSW_ReturnCodes
         */
        int GBFRSW_CodeMaxSize(int ImageSizeX,
                int ImageSizeY,
                IntByReference SampleCodeSize,
                IntByReference PackedSampleCodeSize,
                IntByReference TemplateCodeSize,
                IntByReference CompactTemplateCodeSize);

        /**
         *
         * @param ErrorCode
         * @param Detail
         */
        void GBFRSW_GetLastError(IntByReference ErrorCode,
                IntByReference Detail);

        /**
         *
         * @param MemoryBuffer
         * @param MemoryBufferSize Native is an int32 see point (1) of
         * ReadME.txt file
         * @return
         * @see GBFRSW_JAVA_DEFINES.GBFRSW_ReturnCodes
         */
        public int GBFRSW_Init(Pointer MemoryBuffer,
                int MemoryBufferSize);

        /**
         *
         * @param ImageSizeX Native is an int32 see point (1) of ReadME.txt file
         * @param ImageSizeY Native is an int32 see point (1) of ReadME.txt file
         * @param ImageBuffer
         * @param ImageFlags
         * @see GBFRSW_JAVA_Defines.GBFRSW_ImageFlags
         * @param Options
         * @see GBFRSW_JAVA_Defines.GBFRSW_CodingOptions
         * @param SampleCode
         * @return
         * @see GBFRSW_JAVA_DEFINES.GBFRSW_ReturnCodes
         */
        int GBFRSW_Coding(int ImageSizeX,
                int ImageSizeY,
                byte[] ImageBuffer,
                byte ImageFlags,
                byte Options,
                byte[] SampleCode);

        /**
         *
         * @param SampleCode
         * @param TemplateCode
         * @return
         * @see GBFRSW_JAVA_DEFINES.GBFRSW_ReturnCodes
         */
        int GBFRSW_Enroll(byte[] SampleCode,
                byte[] TemplateCode);

        /**
         *
         * @param SampleCode
         * @param TemplateCode
         * @param SpeedVsPrecisionTradeoff
         * @see GBFRSW_TradeOff
         * @param UnmatchedDataFactor
         * @see GBFRSW_JAVA_Defines.GBFRSW_UnmatchedDataFactorRecommendations
         * @param Options
         * @see GBFRSW_JAVA_Defines.GBFRSW_MatchOptions
         * @param MaxRotationAngleDegree
         * @param MatchingScore
         * @see GBFRSW_JAVA_Defines.GBFRSW_MatchingScoreLimits
         * @return
         * @see GBFRSW_JAVA_DEFINES.GBFRSW_ReturnCodes
         */
        int GBFRSW_Match(
                byte[] SampleCode,
                byte[] TemplateCode,
                byte SpeedVsPrecisionTradeoff,
                byte UnmatchedDataFactor,
                byte Options,
                int MaxRotationAngleDegree,
                FloatByReference MatchingScore);

        /**
         *
         * @param OriginalCodePtr
         * @param ConvertedCodePtr
         * @return
         * @see GBFRSW_JAVA_DEFINES.GBFRSW_ReturnCodes
         */
        int GBFRSW_Convert(byte[] OriginalCodePtr,
                byte[] ConvertedCodePtr);

        /**
         * NOTE: GBFRSW_Correct() will be available in the next versions of
         * GBFRSW.
         *
         * @param SampleCode1
         * @param SampleCode2
         * @param SampleCode3
         * @param CorrectedSampleCode
         * @return
         * @see GBFRSW_JAVA_DEFINES.GBFRSW_ReturnCodes
         */
        int GBFRSW_Correct(byte[] SampleCode1,
                byte[] SampleCode2,
                byte[] SampleCode3,
                byte[] CorrectedSampleCode);

        /**
         *
         * @param OutputHeader
         * @param InputCode
         * @return
         * @see GBFRSW_JAVA_DEFINES.GBFRSW_ReturnCodes
         */
        int GBFRSW_GetCodeHeader(
                GBFRSW_CodeHeader OutputHeader,
                byte[] InputCode);

        /**
         *
         * @param ScannerBasedLicense
         * @param FileBasedLicense
         */
        void GBFRSW_GetLicenseInfo(
                GBFRSW_LicenseUsbScannerCheck ScannerBasedLicense,
                GBFRSW_FileLicenseCheck FileBasedLicense);

        /**
         *
         * @param ImageSizeX Native is an int32 see point (1) of ReadME.txt file
         * @param ImageSizeY Native is an int32 see point (1) of ReadME.txt file
         * @param FormatType
         * @see GBFRSW_JAVA_Defines.GBFRSW_FMR_Formats
         * @param GBProprietaryData
         * @see GBFRSW_JAVA_Defines.GBFRSW_GBPropDataStoringOptions
         * @param MaxFRMLength Native is an unsigned short * see point (1) of
         * ReadME.txt file
         * @return
         * @see GBFRSW_JAVA_DEFINES.GBFRSW_ReturnCodes
         */
        int GBFRSW_ISO_GetMaxFMRLength(
                int ImageSizeX,
                int ImageSizeY,
                byte FormatType,
                byte GBProprietaryData,
                ShortByReference MaxFRMLength);

        /**
         *
         * @param GBTemplate
         * @param FMR
         * @param FMRLen
         * @param FingerPos
         * @see GBFRSW_JAVA_Defines.GBFRSW_FMR_FingerPositions
         * @param FormatType
         * @see GBFRSW_JAVA_Defines.GBFRSW_FMR_Formats
         * @param GBProprietaryData
         * @see GBFRSW_JAVA_Defines.GBFRSW_GBPropDataStoringOptions
         * @return
         * @see GBFRSW_JAVA_DEFINES.GBFRSW_ReturnCodes
         */
        int GBFRSW_ISO_GBTemplateToFMR(byte[] GBTemplate,
                byte[] FMR,
                ShortByReference FMRLen,
                byte FingerPos,
                byte FormatType,
                byte GBProprietaryData);

        /**
         *
         * @param FMR
         * @param GBTemplate
         * @param FingerPos
         * @see GBFRSW_JAVA_Defines.GBFRSW_FMR_FingerPositions
         * @param FormatType
         * @see GBFRSW_JAVA_Defines.GBFRSW_FMR_Formats
         * @param GBProprietaryData
         * @return
         * @see GBFRSW_JAVA_DEFINES.GBFRSW_ReturnCodes
         */
        int GBFRSW_ISO_FMRToGBTemplate(byte[] FMR,
                byte[] GBTemplate,
                ByteByReference FingerPos,
                byte FormatType);

        /**
         *
         * @param FormatType
         * @see GBFRSW_JAVA_Defines.GBFRSW_FMC_FormatTypes
         * @param ImageSizeX Native is an unsigned int see point (1) of
         * ReadME.txt file
         * @param ImageSizeY Native is an unsigned int see point (1) of
         * ReadME.txt file
         * @param MaxMinutiaeNum Native is an unsigned int see point (1) of
         * ReadME.txt file
         * @param MaxFMCLength Native is an unsigned short * see point (1) of
         * ReadME.txt file
         * @return
         * @see GBFRSW_JAVA_DEFINES.GBFRSW_ReturnCodes
         */
        int GBFRSW_ISO_GetMaxFMCLength(
                short FormatType,
                int ImageSizeX,
                int ImageSizeY,
                int MaxMinutiaeNum,
                ShortByReference MaxFMCLength);

        /**
         *
         * @param GBTemplate
         * @param FMC
         * @param FMCLen Native is an unsigned short * see point (1) of
         * ReadME.txt file
         * @param FormatType
         * @see GBFRSW_JAVA_Defines.GBFRSW_FMC_FormatTypes
         * @param MaxMinutiaeNum Native is an unsigned int see point (1) of
         * ReadME.txt file
         * @return
         * @see GBFRSW_JAVA_DEFINES.GBFRSW_ReturnCodes
         */
        int GBFRSW_ISO_GBTemplateToFMC(
                byte[] GBTemplate,
                byte[] FMC,
                ShortByReference FMCLen,
                short FormatType,
                int MaxMinutiaeNum);
    }
}
