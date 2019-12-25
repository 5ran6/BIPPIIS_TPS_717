using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace GBFRSW_NET
{
    namespace GBFRSW_NET_Definitions
    {
        public class GBFRSW_NET_FunctionReturnCodes
        {
            public const Int32 Success = 1;
            public const Int32 Error = 0;
            public const Int32 WrapperException = 255;
            public static String WrapperExceptionString = "";
        }

        public class GBFRSW_NET_ErrorCodes
        {
            public const Int32 Unknown = 0;
            public const Int32 NotEnoughMemory = 1;
            public const Int32 ImageAbsent = 2;
            public const Int32 SizeNotSupported = 3;
            public const Int32 CodeTypeNotSupported = 4;
            public const Int32 CodeVersionNotSupported = 5;
            public const Int32 PointerNotAligned = 6;
            public const Int32 CodeCorrupted = 7; // wrong CRC checksum 
            public const Int32 LowQualityFingerprint = 8;  // fail due to extremely low-quality fingerprint data
            public const Int32 LicenseNotGranted = 9; // call GBFRSW_GetLicenseInfo() for more information
            public const Int32 InvalidParameter = 10;
            public const Int32 UnknownFormat = 11;
            public const Int32 CoordCompactFormat = 12;
        }

        public class GBFRSW_NET_ErrorDetailCodes
        {
            public const Int32 Unknown = 0;
            public const Int32 Image = 1;
            public const Int32 SampleCode = 2;
            public const Int32 SampleCode2 = 3;
            public const Int32 SampleCode3 = 4;
            public const Int32 TemplateCode = 5;
            public const Int32 SampleCodePacked = 6;
            public const Int32 TemplateCodeCompact = 7;
        }

        public class GBFRSW_NET_MemoryRequestMasks
        {
            public const UInt32 Coding = 0x00000001;
            public const UInt32 Enroll = 0x00000002;
            public const UInt32 Match = 0x00000004;
            public const UInt32 Convert = 0x00000008;
            public const UInt32 Correct = 0x00000010;
            public const UInt32 All = 0x80000000;
        }

        public class GBFRSW_NET_ImageFlagsMasks
        {
            public const Byte LivescanImage = 0x01;
            public const Byte CardscanImage = 0x02;
            public const Byte FlatImage = 0x04;
            public const Byte RolledImage = 0x08;
            public const Byte LatentImage = 0x10;
            public const Byte FragmentImage = 0x20;
        }

        public class GBFRSW_NET_CodingOptionsMasks
        {
            public const Byte FingerprintPatternCheck = 0x01;
        }

        public class GBFRSW_NET_SpeedVSPrecisionTradeOff
        {
            public const Byte Robust = 2;
            public const Byte Normal = 3;
            public const Byte Fast = 4;
        }

        public class GBFRSW_NET_MatchOptionsMasks
        {
            public const Byte FragmentImage = 0x01;
        }

        public class GBFRSW_NET_MatchingScoreLimits
        {
            public const float LowerLimit = (float)(-10000);
            public const float UpperLimit = (float)(3.4e+36);
        }

        public class GBFRSW_NET_HeaderType
        {
            public const Byte Sample = 1;
            public const Byte SamplePacked = 2;
            public const Byte Template = 3;
            public const Byte TemplateCompact = 4;
        }

        public class GBFRSW_NET_HeaderQuality
        {
            public const Byte MaxQuality = 100;
            public const Byte MinQuality = 0;
        }

        public class GBFRSW_NET_CodeHeader
        {
            public UInt32 CRC;
            public UInt32 Size;
            public UInt32 SizeAfterConversion;
            public Byte Type;
            public Byte Version;
            public Byte ImageFlags;
            public Byte Quality;
            public float InformationCapacity;
            public UInt16 ImageSizeX;
            public UInt16 ImageSizeY;
            public Byte[] Reserve;
            public UInt32 SizeConfirm;

            public GBFRSW_NET_CodeHeader()
            {
                this.CRC = 0;
                this.Size = 0;
                this.SizeAfterConversion = 0;
                this.Type = 0;
                this.Version = 0;
                this.ImageFlags = 0;
                this.Quality = 0;
                this.InformationCapacity = (float)0.0;
                this.ImageSizeX = 0;
                this.ImageSizeY = 0;
                this.Reserve = new Byte[4];
                this.SizeConfirm = 0;
            }
        }

        public class GBFRSW_NET_UsbDriverErrorCodeValues
        {
            public const UInt32 NoError = 0;
            public const UInt32 UsbDriver = 1;
            public const UInt32 UsbDeviceNotFound = 2;
        }

        public class GBFRSW_NET_LicenseUSBScannerCheck
        {
            public GBFRSW_NET_LicenseUSBScannerCheck()
            {
                this.Result = 0;
                this.UsbDriverErrorCodeDetail = 0;
            }

            public Int32 Result;
            public UInt32 UsbDriverErrorCodeDetail;
        }

        public class GBFRSW_NET_LicenseFileCheck
        {
            public GBFRSW_NET_LicenseFileCheck()
            {
                this.ErrorCode = 0;
                this.Phase = 0;
                this.ErrorDetailString = "";
            }

            public Int32 ErrorCode;
            public Int32 Phase;
            public String ErrorDetailString;
        }

        public class GBFRSW_NET_ISO_FMR_FormatType
        {
            public const Byte Iso = 1;
            public const Byte Incits = 2;
            public const Byte Minex = 3;
        }

        public class GBFRSW_NET_ISO_GBProprietaryData
        {
            public const Byte None = 0;
            public const Byte Fast = 1;
            public const Byte Compressed = 2;
            // ver 1.3.5.0
            public const Byte ExcludeExtendedDataBlock = 3;
            // end ver 1.3.5.0
        }

        public class GBFRSW_NET_ISO_FingerPosition
        {
            public const Byte Unknown = 0;
            public const Byte RightThumb = 1;
            public const Byte RightIndex = 2;
            public const Byte RightMiddle = 3;
            public const Byte RightRing = 4;
            public const Byte RightLittle = 5;
            public const Byte LeftThumb = 6;
            public const Byte LeftIndex = 7;
            public const Byte LeftMiddle = 8;
            public const Byte LeftRing = 9;
            public const Byte LeftLittle = 10;
        }

        public class GBFRSW_NET_ISO_MinutiaeFormat
        {
            public const UInt16 Normal = 0x0004;
            public const UInt16 Compact = 0x0006;
        }

        public class GBFRSW_NET_ISO_BS_FingerPosition
        {
            public const Byte None = 0x00;
            public const Byte RightThumb = 0x05;
            public const Byte RightIndex = 0x09;
            public const Byte RightMiddle = 0x0D;
            public const Byte RightRing = 0x11;
            public const Byte RightLittle = 0x15;
            public const Byte LeftThumb = 0x06;
            public const Byte LeftIndex = 0x0A;
            public const Byte LeftMiddle = 0x0E;
            public const Byte LeftRing = 0x12;
            public const Byte LeftLittle = 0x16;
        }

        public class GBFRSW_NET_ISO_FMC_FormatType
        {
            public const UInt16 NormalFormat = 0x0004;
            public const UInt16 CompactFormat = 0x0006;
            public const UInt32 AllMinutiaeMustBeConsidered = 0;
        }
    }

    namespace GBFRSW_NET_Globals
    {
        public class MemoryGlobals
        {
            public static IntPtr MemBuffer = IntPtr.Zero;
        }
    }

    namespace GBFRSW_NET_Functions
    {

        internal class GBFRSW_NET_Dll_Wrapper
        {
            [DllImport("GBFRSW.dll", EntryPoint = "GBFRSW_GetVersionInfo")]
            public static extern int GBFRSW_GetVersionInfo(
                out Byte VersionField1,
                out Byte VersionField2,
                out Byte VersionField3,
                out Byte VersionField4,
                out UInt32 MaxImageSizeX,
                out UInt32 MaxImageSizeY,
                out UInt32 MinImageSizeX,
                out UInt32 MinImageSizeY,
                UInt32 RequestedOperations,
                out UInt32 MemoryBufferSize
                );

            [DllImport("GBFRSW.dll", EntryPoint = "GBFRSW_CodeMaxSize")]
            public static extern int GBFRSW_CodeMaxSize(
                UInt32 ImageSizeX,
                UInt32 ImageSizeY,
                out UInt32 SampleCodeSize,
                out UInt32 PackedSampleCodeSize,
                out UInt32 TemplateCodeSize,
                out UInt32 CompactTemplateCodeSize
                );

            [DllImport("GBFRSW.dll", EntryPoint = "GBFRSW_GetLastError")]
            public static extern void GBFRSW_GetLastError(
                out Int32 ErrorCode,
                out Int32 Detail
                );

            [DllImport("GBFRSW.dll", EntryPoint = "GBFRSW_Init")]
            public static extern int GBFRSW_Init(
                IntPtr MemoryBuffer,
                UInt32 MemoryBufferSize
                );

            [DllImport("GBFRSW.dll", EntryPoint = "GBFRSW_Coding")]
            public static extern int GBFRSW_Coding(
                UInt32 ImageSizeX,
                UInt32 ImageSizeY,
                IntPtr ImageBuffer,
                Byte ImageFlags,
                Byte Options,
                IntPtr SampleCode
                );

            [DllImport("GBFRSW.dll", EntryPoint = "GBFRSW_Enroll")]
            public static extern int GBFRSW_Enroll(
                IntPtr SampleCode,
                IntPtr TemplateCode
                );

            [DllImport("GBFRSW.dll", EntryPoint = "GBFRSW_Match")]
            public static extern int GBFRSW_Match(
                IntPtr SampleCode,
                IntPtr TemplateCode,
                Byte SpeedVsPrecisionTradeoff,
                Byte UnmatchedDataFactor,
                Byte Options,
                int MaxRotationAngleDegree,
                out float MatchingScore
                );

            [DllImport("GBFRSW.dll", EntryPoint = "GBFRSW_Convert")]
            public static extern int GBFRSW_Convert(
                IntPtr OriginalCodePtr,
                IntPtr ConvertedCodePtr
                );

            [DllImport("GBFRSW.dll", EntryPoint = "GBFRSW_Correct")]
            public static extern int GBFRSW_Correct(
                IntPtr SampleCode1,
                IntPtr SampleCode2,
                IntPtr SampleCode3,
                IntPtr CorrectedSampleCode
                );

            [StructLayout(LayoutKind.Sequential)]
            public struct GBFRSW_CODE_HEADER
            {
                public UInt32 CRC;
                public UInt32 Size;
                public UInt32 SizeAfterConversion;
                public Byte Type;
                public Byte Version;
                public Byte ImageFlags;
                public Byte Quality;
                public float InformationCapacity;
                public UInt16 ImageSizeX;
                public UInt16 ImageSizeY;
                [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
                public Byte[] Reserve;
                public UInt32 SizeConfirm;
            }

            [DllImport("GBFRSW.dll", EntryPoint = "GBFRSW_GetCodeHeader")]
            public static extern void GBFRSW_GetCodeHeader(
                ref GBFRSW_CODE_HEADER OutputHeader,
                IntPtr InputCode
                );

            [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
            public struct GBFRSW_LICENSE_USB_SCANNER_CHECK
            {
                public Int32 Result;
                public UInt32 UsbDriverErrorCodeDetail;
            }

            [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
            public struct GBFRSW_LICENSE_FILE_CHECK
            {
                public Int32 ErrorCode;
                public Int32 Phase;
                [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 1000)]
                public String ErrorDetailString;
            }

            [DllImport("GBFRSW.dll", EntryPoint = "GBFRSW_GetLicenseInfo")]
            public static extern void GBFRSW_GetLicenseInfo(
                ref GBFRSW_LICENSE_USB_SCANNER_CHECK ScannerBasedLicense,
                ref GBFRSW_LICENSE_FILE_CHECK FileBasedLicense
                );

            [DllImport("GBFRSW.dll", EntryPoint = "GBFRSW_ISO_GetMaxFMRLength")]
            public static extern int GBFRSW_ISO_GetMaxFMRLength(
                UInt32 ImageSizeX, UInt32 ImageSizeY,
                Byte FormatType, Byte GBProprietaryData,
                out UInt16 MaxFRMLength
                ); 

            [DllImport("GBFRSW.dll", EntryPoint = "GBFRSW_ISO_GBTemplateToFMR")]
            public static extern int GBFRSW_ISO_GBTemplateToFMR(
                IntPtr GBTemplate,
                IntPtr FMR,
                out UInt16 FMRLen,
                Byte FingerPos,
                Byte FormatType,
                Byte GBProprietaryData
                ); 

            [DllImport("GBFRSW.dll", EntryPoint = "GBFRSW_ISO_FMRToGBTemplate")]
            public static extern int GBFRSW_ISO_FMRToGBTemplate(
                IntPtr FMR,
                IntPtr GBTemplate,
                out Byte FingerPos,
                Byte FormatType
                );

            [DllImport("GBFRSW.dll", EntryPoint = "GBFRSW_ISO_GetMaxFMCLength")]
            public static extern int GBFRSW_ISO_GetMaxFMCLength(
                UInt16 FormatType,
                UInt32 ImageSizeX, UInt32 ImageSizeY,
                UInt32 MaxMinutiaeNum,
                out UInt16 MaxFMCLength
                );

            [DllImport("GBFRSW.dll", EntryPoint = "GBFRSW_ISO_GBTemplateToFMC")]
            public static extern int GBFRSW_ISO_GBTemplateToFMC(
                IntPtr GBTemplate,
                IntPtr FMC,
                out UInt16 FMCLen,
                UInt16 FormatType,
                UInt32 MaxMinutiaeNum
                );
        }

        public class GBFRSW_NET_ServiceFunctions
        {
            /**************************************
             * GBFRSW_NET_GetVersionInfo
            **************************************/
            public static Int32 GBFRSW_NET_GetVersionInfo(
                out Byte VersionField1,
                out Byte VersionField2,
                out Byte VersionField3,
                out Byte VersionField4,
                out UInt32 MaxImageSizeX,
                out UInt32 MaxImageSizeY,
                out UInt32 MinImageSizeX,
                out UInt32 MinImageSizeY,
                UInt32 RequestedOperations,
                out UInt32 MemoryBufferSize
               )
            {
                try
                {
                    Int32 RetVal = GBFRSW_NET_Dll_Wrapper.GBFRSW_GetVersionInfo(
                        out VersionField1, out VersionField2, out VersionField3, out VersionField4,
                        out MaxImageSizeX, out MaxImageSizeY, out MinImageSizeX, out MinImageSizeY,
                        RequestedOperations,
                        out MemoryBufferSize);

                    return RetVal;
                }
                catch (Exception ex)
                {
                    VersionField1 = 0; VersionField2 = 0; VersionField3 = 0; VersionField4 = 0;
                    MaxImageSizeX = 0; MaxImageSizeY = 0; MinImageSizeX = 0; MinImageSizeY = 0;
                    MemoryBufferSize = 0;

                    GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperExceptionString =
                        "GBFRSW_NET_GetVersionInfo: " + ex.Message;
                    return GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperException;
                }
            }

            /**************************************
             * GBFRSW_NET_CodeMaxSize
            **************************************/
            public static Int32 GBFRSW_NET_CodeMaxSize(
                UInt32 ImageSizeX,
                UInt32 ImageSizeY,
                out UInt32 SampleCodeSize,
                out UInt32 PackedSampleCodeSize,
                out UInt32 TemplateCodeSize,
                out UInt32 CompactTemplateCodeSize
               )
            {
                try
                {
                    Int32 RetVal = GBFRSW_NET_Dll_Wrapper.GBFRSW_CodeMaxSize(
                        ImageSizeX, ImageSizeY,
                        out SampleCodeSize, out PackedSampleCodeSize, 
                        out TemplateCodeSize, out CompactTemplateCodeSize
                        );

                    return RetVal;
                }
                catch (Exception ex)
                {
                    SampleCodeSize = 0; PackedSampleCodeSize = 0; 
                    TemplateCodeSize = 0; CompactTemplateCodeSize = 0;

                    GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperExceptionString =
                        "GBFRSW_NET_CodeMaxSize: " + ex.Message;
                    return GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperException;
                }
            }

            /**************************************
             * GBFRSW_NET_GetLastError
            **************************************/
            public static Int32 GBFRSW_NET_GetLastError(
                out Int32 ErrorCode,
                out Int32 Detail
               )
            {
                try
                {
                    GBFRSW_NET_Dll_Wrapper.GBFRSW_GetLastError(
                        out ErrorCode, out Detail
                        );

                    return GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.Success;
                }
                catch (Exception ex)
                {
                    ErrorCode = 0; Detail = 0;

                    GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperExceptionString =
                        "GBFRSW_NET_GetLastError: " + ex.Message;
                    return GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperException;
                }
            }            

            /**************************************
             * GBFRSW_NET_Init
            **************************************/
            public static Int32 GBFRSW_NET_Init(
                UInt32 MemoryBufferSize
               )
            {
                try
                {
                    if (GBFRSW_NET_Globals.MemoryGlobals.MemBuffer != IntPtr.Zero)
                    {
                        Marshal.FreeHGlobal(GBFRSW_NET_Globals.MemoryGlobals.MemBuffer);
                    }

                    GBFRSW_NET_Globals.MemoryGlobals.MemBuffer = Marshal.AllocHGlobal((int)MemoryBufferSize);

                    return GBFRSW_NET_Dll_Wrapper.GBFRSW_Init(GBFRSW_NET_Globals.MemoryGlobals.MemBuffer, MemoryBufferSize);
                }
                catch (Exception ex)
                {
                    GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperExceptionString =
                        "GBFRSW_NET_Init: " + ex.Message;
                    return GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperException;
                }
            }
        }

        public class GBFRSW_NET_CoreFunctions
        {
            /**************************************
             * GBFRSW_NET_Coding
            **************************************/
            public static Int32 GBFRSW_NET_Coding(
                UInt32  ImageSizeX,
                UInt32  ImageSizeY,
                IntPtr  ImageBuffer,
                Byte    ImageFlags,
                Byte    Options,
                out Byte [] SampleCode
               )
            {
                try
                {
                    UInt32 SampleCodeSize, PackCodeSize, TempCodeSize, CompTempCodeSize;

                    Int32 RetVal = GBFRSW_NET_Dll_Wrapper.GBFRSW_CodeMaxSize(
                        ImageSizeX, ImageSizeY, out SampleCodeSize, out PackCodeSize, out TempCodeSize, out CompTempCodeSize);
                    Console.WriteLine("GBFRSW_NET_Coding: ImageSizeX = " + ImageSizeX);
                    Console.WriteLine("GBFRSW_NET_Coding: ImageSizeY = " + ImageSizeY);

                    Console.WriteLine("GBFRSW_NET_Coding: SampleCodeSize = " + SampleCodeSize);
                    Console.WriteLine("GBFRSW_NET_Coding: PackCodeSize = " + PackCodeSize);
                    Console.WriteLine("GBFRSW_NET_Coding: TempCodeSize = " + TempCodeSize);
                    Console.WriteLine("GBFRSW_NET_Coding: CompTempCodeSize = " + CompTempCodeSize);

                    if (RetVal != GBFRSW_NET.GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.Success)
                    {
                        GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperExceptionString =
                            "GBFRSW_NET_Coding: error in calling GBFRSW_CodeMaxSize() function";
                        SampleCode = null;
                        return GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperException;
                    }
                    IntPtr SampleCodeBuffer = Marshal.AllocHGlobal((int)SampleCodeSize);
                    if (SampleCodeBuffer == IntPtr.Zero)
                    {
                        GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperExceptionString =
                           "GBFRSW_NET_Coding: error in alloc memory for sample buffer";
                        SampleCode = null;
                        return GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperException;
                    }

                    Console.WriteLine("GBFRSW_NET_Coding: before coding");

                    RetVal = GBFRSW_NET_Dll_Wrapper.GBFRSW_Coding(ImageSizeX, ImageSizeY,
                        ImageBuffer, ImageFlags, Options,
                        SampleCodeBuffer);
                    Console.WriteLine("GBFRSW_NET_Coding: after coding");

                    SampleCode = new Byte[SampleCodeSize];
                    Marshal.Copy(SampleCodeBuffer,SampleCode,0,SampleCode.Length);
                    Marshal.FreeHGlobal(SampleCodeBuffer);                

                    return RetVal;
                }
                catch (Exception ex)
                {
                    GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperExceptionString =
                        "GBFRSW_NET_Coding: " + ex.Message;
                    SampleCode = null;
                    return GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperException;
                }
            }

            /**************************************
             * GBFRSW_NET_Enroll
            **************************************/
            public static Int32 GBFRSW_NET_Enroll(
                Byte[] SampleCode,
                out Byte[] TemplateCode
               )
            {
                try
                {
                    UInt32 SampleCodeSize, PackCodeSize, TempCodeSize, CompTempCodeSize;
                    UInt32 ImageSizeX, ImageSizeY, MinSXui, MinSYui, MemBuffSizeui;
                    Byte V1b, V2b, V3b, V4b;

                    Int32 RetVal = GBFRSW_NET_Dll_Wrapper.GBFRSW_GetVersionInfo(
                        out V1b, out V2b, out V3b, out V4b,
                        out ImageSizeX, out ImageSizeY,
                        out MinSXui, out MinSYui,
                        GBFRSW_NET_Definitions.GBFRSW_NET_MemoryRequestMasks.All,
                        out MemBuffSizeui
                    );

                    if (RetVal != GBFRSW_NET.GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.Success)
                    {
                        GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperExceptionString =
                            "GBFRSW_NET_Enroll: error in calling GBFRSW_GetVersionInfo() function";
                        TemplateCode = null;
                        return GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperException;
                    }

                    RetVal = GBFRSW_NET_Dll_Wrapper.GBFRSW_CodeMaxSize(
                        ImageSizeX, ImageSizeY, out SampleCodeSize, out PackCodeSize, out TempCodeSize, out CompTempCodeSize);

                    if (RetVal != GBFRSW_NET.GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.Success)
                    {
                        GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperExceptionString =
                            "GBFRSW_NET_Enroll: error in calling GBFRSW_CodeMaxSize() function";
                        TemplateCode = null;
                        return GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperException;
                    }

                    IntPtr SampleCodeBuffer = Marshal.AllocHGlobal(SampleCode.Length);
                    Marshal.Copy(SampleCode, 0, SampleCodeBuffer, SampleCode.Length);

                    TemplateCode = new Byte[TempCodeSize];
                    for (int i = 0; i < TempCodeSize; i++)
                    {
                        TemplateCode.SetValue((Byte)0, i);
                    }
                    IntPtr TemplateCodeBuffer = Marshal.AllocHGlobal((int)TempCodeSize);   

                    Marshal.Copy(TemplateCode, 0, TemplateCodeBuffer, (int)TempCodeSize);                 

                    RetVal = GBFRSW_NET_Dll_Wrapper.GBFRSW_Enroll(
                        SampleCodeBuffer,TemplateCodeBuffer);

                    Marshal.FreeHGlobal(SampleCodeBuffer);

                    Marshal.Copy(TemplateCodeBuffer, TemplateCode, 0, TemplateCode.Length);
                    Marshal.FreeHGlobal(TemplateCodeBuffer);

                    return RetVal;
                }
                catch (Exception ex)
                {
                    GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperExceptionString =
                        "GBFRSW_NET_Enroll: " + ex.Message;
                    TemplateCode = null;
                    return GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperException;
                }
            }

            /**************************************
             * GBFRSW_NET_Match
            **************************************/
            public static Int32 GBFRSW_NET_Match(
                Byte[] SampleCode,
                Byte[] TemplateCode,
                Byte SpeedVsPrecisionTradeoff,
                Byte UnmatchedDataFactor,
                Byte Options,
                int MaxRotationAngleDegree,
                out float MatchingScore
               )
            {
                int PuntoDiArrivo = 0;
                try
                {
                    Int32 RetVal;
                    //UInt32 SampleCodeSize, PackCodeSize, TempCodeSize, CompTempCodeSize;
                    //UInt32 ImageSizeX, ImageSizeY, MinSXui, MinSYui, MemBuffSizeui;
                    //Byte V1b, V2b, V3b, V4b;

                    //RetVal = GBFRSW_NET_Dll_Wrapper.GBFRSW_GetVersionInfo(
                    //    out V1b, out V2b, out V3b, out V4b,
                    //    out ImageSizeX, out ImageSizeY,
                    //    out MinSXui, out MinSYui,
                    //    GBFRSW_NET_Definitions.GBFRSW_NET_MemoryRequestMasks.All,
                    //    out MemBuffSizeui
                    //);

                    //if (RetVal != GBFRSW_NET.GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.Success)
                    //{
                    //    GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperExceptionString =
                    //        "GBFRSW_NET_Match: error in calling GBFRSW_GetVersionInfo() function";
                    //    TemplateCode = null;
                    //    MatchingScore = (float)0.0;
                    //    return GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperException;
                    //}

                    //RetVal = GBFRSW_NET_Dll_Wrapper.GBFRSW_CodeMaxSize(
                    //    ImageSizeX, ImageSizeY, out SampleCodeSize, out PackCodeSize, out TempCodeSize, out CompTempCodeSize);

                    //if (RetVal != GBFRSW_NET.GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.Success)
                    //{
                    //    GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperExceptionString =
                    //        "GBFRSW_NET_Match: error in calling GBFRSW_CodeMaxSize() function";
                    //    TemplateCode = null;
                    //    MatchingScore = (float)0.0;
                    //    return GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperException;
                    //}

                    //IntPtr SampleCodeBuffer = Marshal.AllocHGlobal((int)SampleCodeSize);
                    IntPtr SampleCodeBuffer = Marshal.AllocHGlobal((int)SampleCode.Length);
                    PuntoDiArrivo = 1;
                    Marshal.Copy(SampleCode, 0, SampleCodeBuffer, SampleCode.Length);
                    PuntoDiArrivo = 2;
                    //IntPtr TemplateCodeBuffer = Marshal.AllocHGlobal((int)TempCodeSize);
                    IntPtr TemplateCodeBuffer = Marshal.AllocHGlobal((int)TemplateCode.Length);
                    PuntoDiArrivo = 3;
                    Marshal.Copy(TemplateCode, 0, TemplateCodeBuffer, TemplateCode.Length);
                    PuntoDiArrivo = 4;
                    RetVal = GBFRSW_NET_Dll_Wrapper.GBFRSW_Match(SampleCodeBuffer, TemplateCodeBuffer,
                        SpeedVsPrecisionTradeoff, UnmatchedDataFactor, Options, MaxRotationAngleDegree,
                        out MatchingScore);
                    PuntoDiArrivo = 5;
                    Marshal.FreeHGlobal(SampleCodeBuffer);
                    PuntoDiArrivo = 6;
                    Marshal.FreeHGlobal(TemplateCodeBuffer);
                    PuntoDiArrivo = 7;

                    return RetVal;
                }
                catch (Exception ex)
                {
                    MatchingScore = (float)0.0;
                    GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperExceptionString =
                        "GBFRSW_NET_Match, punto di arrivo: " + PuntoDiArrivo + ", " + ex.Message;
                    return GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperException;
                }
            }

            /**************************************
             * GBFRSW_NET_Convert
            **************************************/
            public static Int32 GBFRSW_NET_Convert(
                Byte[] OriginalCode,
                out Byte[] ConvertedCode
               )
            {
                try
                {

                    IntPtr OriginalCodeBuffer = Marshal.AllocHGlobal(OriginalCode.Length);
                    Marshal.Copy(OriginalCode, 0, OriginalCodeBuffer, OriginalCode.Length);

                    UInt32 SampleCodeSize, PackCodeSize, TempCodeSize, CompTempCodeSize;

                    GBFRSW_NET_Dll_Wrapper.GBFRSW_CODE_HEADER HeaderPtr = new
                        GBFRSW_NET_Dll_Wrapper.GBFRSW_CODE_HEADER();

                    GBFRSW_NET_Dll_Wrapper.GBFRSW_GetCodeHeader(
                        ref HeaderPtr, OriginalCodeBuffer);

                    Int32 RetVal = GBFRSW_NET_Dll_Wrapper.GBFRSW_CodeMaxSize(
                        HeaderPtr.ImageSizeX, HeaderPtr.ImageSizeY,
                        out SampleCodeSize, out PackCodeSize, out TempCodeSize, out CompTempCodeSize);
                    //Console.WriteLine("GBFRSW_NET_Convert: HeaderPtr.ImageSizeX = " + HeaderPtr.ImageSizeX);
                    //Console.WriteLine("GBFRSW_NET_Convert: HeaderPtr.ImageSizeY = " + HeaderPtr.ImageSizeY);

                    //Console.WriteLine("GBFRSW_NET_Convert: HeaderPtr.Size = " + HeaderPtr.Size+
                    //    ", GBFRSW_NET_Convert: HeaderPtr.SizeAfterConversion = " + HeaderPtr.SizeAfterConversion);

                    //Console.WriteLine("GBFRSW_NET_Convert: SampleCodeSize = " + SampleCodeSize+ ", PackCodeSize = " + PackCodeSize);
                    //Console.WriteLine("GBFRSW_NET_Convert: TempCodeSize = " + TempCodeSize+ ", CompTempCodeSize = " + CompTempCodeSize);

                    if (RetVal != GBFRSW_NET.GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.Success)
                    {
                        Marshal.FreeHGlobal(OriginalCodeBuffer);
                        GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperExceptionString =
                            "GBFRSW_NET_Convert: error in calling GBFRSW_CodeMaxSize() function";
                        ConvertedCode = null;
                        return GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperException;
                    }

                    UInt32 ConvertedCodeSize;
                    switch (HeaderPtr.Type)
                    {
                        case GBFRSW_NET_Definitions.GBFRSW_NET_HeaderType.Sample:
                            ConvertedCodeSize = PackCodeSize;
                            break;
                        case GBFRSW_NET_Definitions.GBFRSW_NET_HeaderType.SamplePacked:
                            ConvertedCodeSize = SampleCodeSize;
                            break;
                        case GBFRSW_NET_Definitions.GBFRSW_NET_HeaderType.Template:
                            ConvertedCodeSize = CompTempCodeSize;
                            break;
                        case GBFRSW_NET_Definitions.GBFRSW_NET_HeaderType.TemplateCompact:
                            ConvertedCodeSize = TempCodeSize;
                            break;
                        default :
                            ConvertedCodeSize = 0;
                            break;
                    }

                    //Console.WriteLine("GBFRSW_NET_Convert: ConvertedCodeSize = " + ConvertedCodeSize);

                    if (ConvertedCodeSize == 0)
                    {
                        Marshal.FreeHGlobal(OriginalCodeBuffer);
                        GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperExceptionString =
                            "GBFRSW_NET_Convert: Original code not recognized";
                        ConvertedCode = null;
                        return GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperException;
                    }

                    IntPtr ConvertedCodeBuffer = Marshal.AllocHGlobal((int)ConvertedCodeSize + 200);

                    RetVal = GBFRSW_NET_Dll_Wrapper.GBFRSW_Convert(
                        OriginalCodeBuffer, ConvertedCodeBuffer);

                    Marshal.FreeHGlobal(OriginalCodeBuffer);

                    GBFRSW_NET_Dll_Wrapper.GBFRSW_GetCodeHeader(
                        ref HeaderPtr, ConvertedCodeBuffer);

                    //Console.WriteLine("GBFRSW_NET_Convert: ConvertedCodeBuffer.Size = " + HeaderPtr.Size+ 
                    //    ", GBFRSW_NET_Convert: ConvertedCodeBuffer.SizeAfterConversion = " + HeaderPtr.SizeAfterConversion);

                    ConvertedCode = new Byte[ConvertedCodeSize];

                    Marshal.Copy(ConvertedCodeBuffer, ConvertedCode, 0, (int)ConvertedCodeSize);
                    //Console.WriteLine("GBFRSW_NET_Convert: before free");
                    Marshal.FreeHGlobal(ConvertedCodeBuffer);
                    //Console.WriteLine("GBFRSW_NET_Convert: after free");

                    return RetVal;
                }
                catch (Exception ex)
                {
                    GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperExceptionString =
                        "GBFRSW_NET_Convert: " + ex.Message;
                    ConvertedCode = null;
                    return GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperException;
                }
            }

            /**************************************
             * GBFRSW_NET_Correct
            **************************************/
            public static Int32 GBFRSW_NET_Correct(
                Byte[] SampleCode1,
                Byte[] SampleCode2,
                Byte[] SampleCode3,
                out Byte[] CorrectedSampleCode
               )
            {
                try
                {
                    UInt32 SampleCodeSize, PackCodeSize, TempCodeSize, CompTempCodeSize;
                    UInt32 ImageSizeX, ImageSizeY, MinSXui, MinSYui, MemBuffSizeui;
                    Byte V1b, V2b, V3b, V4b;

                    Int32 RetVal = GBFRSW_NET_Dll_Wrapper.GBFRSW_GetVersionInfo(
                        out V1b, out V2b, out V3b, out V4b,
                        out ImageSizeX, out ImageSizeY,
                        out MinSXui, out MinSYui,
                        GBFRSW_NET_Definitions.GBFRSW_NET_MemoryRequestMasks.All,
                        out MemBuffSizeui
                    );

                    if (RetVal != GBFRSW_NET.GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.Success)
                    {
                        GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperExceptionString =
                            "GBFRSW_NET_Correct: error in calling GBFRSW_GetVersionInfo() function";
                        CorrectedSampleCode = null;
                        return GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperException;
                    }

                    RetVal = GBFRSW_NET_Dll_Wrapper.GBFRSW_CodeMaxSize(
                        ImageSizeX, ImageSizeY, out SampleCodeSize, out PackCodeSize, out TempCodeSize, out CompTempCodeSize);

                    if (RetVal != GBFRSW_NET.GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.Success)
                    {
                        GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperExceptionString =
                            "GBFRSW_NET_Correct: error in calling GBFRSW_CodeMaxSize() function";
                        CorrectedSampleCode = null;
                        return GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperException;
                    }

                    IntPtr SampleCode1Buffer = Marshal.AllocHGlobal(SampleCode1.Length);
                    IntPtr SampleCode2Buffer = Marshal.AllocHGlobal(SampleCode2.Length);
                    IntPtr SampleCode3Buffer = Marshal.AllocHGlobal(SampleCode3.Length);
                    Marshal.Copy(SampleCode1, 0, SampleCode1Buffer, SampleCode1.Length);
                    Marshal.Copy(SampleCode2, 0, SampleCode2Buffer, SampleCode2.Length);
                    Marshal.Copy(SampleCode3, 0, SampleCode3Buffer, SampleCode3.Length);

                    IntPtr CorrectedSampleCodeCodeBuffer = Marshal.AllocHGlobal((int)SampleCodeSize);

                    RetVal = GBFRSW_NET_Dll_Wrapper.GBFRSW_Correct(
                        SampleCode1Buffer, SampleCode2Buffer, SampleCode3Buffer,
                        CorrectedSampleCodeCodeBuffer);

                    Marshal.FreeHGlobal(SampleCode1Buffer);
                    Marshal.FreeHGlobal(SampleCode2Buffer);
                    Marshal.FreeHGlobal(SampleCode3Buffer);

                    CorrectedSampleCode = new Byte[SampleCodeSize];

                    Marshal.Copy(CorrectedSampleCodeCodeBuffer, CorrectedSampleCode, 0, CorrectedSampleCode.Length);
                    Marshal.FreeHGlobal(CorrectedSampleCodeCodeBuffer);

                    return RetVal;
                }
                catch (Exception ex)
                {
                    GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperExceptionString =
                        "GBFRSW_NET_Correct: " + ex.Message;
                    CorrectedSampleCode = null;
                    return GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperException;
                }
            }

            /**************************************
             * GBFRSW_NET_GetCodeHeader
            **************************************/
            public static Int32 GBFRSW_NET_GetCodeHeader(
                out GBFRSW_NET_Definitions.GBFRSW_NET_CodeHeader Header,
                Byte[] Code
               )
            {
                try
                {
                    IntPtr CodeBuffer = Marshal.AllocHGlobal(Code.Length);
                    Marshal.Copy(Code, 0, CodeBuffer, Code.Length);

                    GBFRSW_NET_Dll_Wrapper.GBFRSW_CODE_HEADER HeaderPtr = new
                        GBFRSW_NET_Dll_Wrapper.GBFRSW_CODE_HEADER();

                    GBFRSW_NET_Dll_Wrapper.GBFRSW_GetCodeHeader(
                        ref HeaderPtr, CodeBuffer);

                    Marshal.FreeHGlobal(CodeBuffer);

                    Header = new GBFRSW_NET.GBFRSW_NET_Definitions.GBFRSW_NET_CodeHeader();

                    Header.CRC = HeaderPtr.CRC;
                    Header.Size = HeaderPtr.Size;
                    Header.SizeAfterConversion = HeaderPtr.SizeAfterConversion;
                    Header.Type = HeaderPtr.Type;
                    Header.Version = HeaderPtr.Version;
                    Header.ImageFlags = HeaderPtr.ImageFlags;
                    Header.Quality = HeaderPtr.Quality;
                    Header.InformationCapacity = HeaderPtr.InformationCapacity;
                    Header.ImageSizeX = HeaderPtr.ImageSizeX;
                    Header.ImageSizeY = HeaderPtr.ImageSizeY;
                    Header.SizeConfirm = HeaderPtr.SizeConfirm;
                    Header.Reserve = HeaderPtr.Reserve;

                    return GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.Success;
                }
                catch (Exception ex)
                {
                    GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperExceptionString =
                        "GBFRSW_NET_GetCodeHeader: " + ex.Message;
                    Header = null;
                    return GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperException;
                }
            }
            
        }

        public class GBFRSW_NET_LicenseCheckFunctions
        {
            /**************************************
             * GBFRSW_NET_GetLicenseInfo
            **************************************/
            public static Int32 GBFRSW_NET_GetLicenseInfo(
                out GBFRSW_NET_Definitions.GBFRSW_NET_LicenseUSBScannerCheck ScannerBasedLicense,
                out GBFRSW_NET_Definitions.GBFRSW_NET_LicenseFileCheck FileBasedLicense
               )
            {
                try
                {
                    GBFRSW_NET_Dll_Wrapper.GBFRSW_LICENSE_USB_SCANNER_CHECK WrapScanBasedLic = 
                        new GBFRSW_NET_Dll_Wrapper.GBFRSW_LICENSE_USB_SCANNER_CHECK();
                    GBFRSW_NET_Dll_Wrapper.GBFRSW_LICENSE_FILE_CHECK WrapFileBasedLic =
                        new GBFRSW_NET_Dll_Wrapper.GBFRSW_LICENSE_FILE_CHECK();

                    GBFRSW_NET_Dll_Wrapper.GBFRSW_GetLicenseInfo(ref WrapScanBasedLic, ref WrapFileBasedLic);

                    ScannerBasedLicense = new GBFRSW_NET.GBFRSW_NET_Definitions.GBFRSW_NET_LicenseUSBScannerCheck();
                    FileBasedLicense = new GBFRSW_NET.GBFRSW_NET_Definitions.GBFRSW_NET_LicenseFileCheck();

                    ScannerBasedLicense.Result = WrapScanBasedLic.Result;
                    ScannerBasedLicense.UsbDriverErrorCodeDetail = WrapScanBasedLic.UsbDriverErrorCodeDetail;

                    FileBasedLicense.ErrorCode = WrapFileBasedLic.ErrorCode;
                    FileBasedLicense.Phase = WrapFileBasedLic.Phase;
                    FileBasedLicense.ErrorDetailString = "" + WrapFileBasedLic.ErrorDetailString;

                    return GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.Success;
                }
                catch (Exception ex)
                {
                    GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperExceptionString =
                        "GBFRSW_NET_GetLicenseInfo: " + ex.Message;
                    FileBasedLicense = null;
                    ScannerBasedLicense = null;
                    return GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperException;
                }
            }
        }

        public class GBFRSW_NET_FormatConvertFunctions
        {
            /**************************************
             * GBFRSW_NET_ISO_GetMaxFMRLength
            **************************************/
            public static Int32 GBFRSW_NET_ISO_GetMaxFMRLength(
                UInt32 ImageSizeX,
                UInt32 ImageSizeY,
                Byte FormatType,
                Byte GBProprietaryData,
                out UInt16 MaxFMRLen
               )
            {
                try
                {
                    Int32 RetVal = GBFRSW_NET_Dll_Wrapper.GBFRSW_ISO_GetMaxFMRLength(
                        ImageSizeX, ImageSizeY,
                        FormatType,
                        GBProprietaryData,
                        out MaxFMRLen);

                    return RetVal;
                }
                catch (Exception ex)
                {
                    MaxFMRLen = 0;
                    GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperExceptionString =
                        "GBFRSW_NET_ISO_GetMaxFMRLength: " + ex.Message;
                    return GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperException;
                }
            }

            /**************************************
             * GBFRSW_NET_ISO_GBTemplateToFMR
            **************************************/
            public static Int32 GBFRSW_NET_ISO_GBTemplateToFMR(
                Byte[] GBTemplate,
                out Byte[] FMR,
                out UInt16 FMRLen,
                Byte FingerPos,
                Byte FormatType,
                Byte GBProprietaryData
               )
            {
                try
                {
                    UInt16 MaxFMRLen;

                    IntPtr OriginalCodeBuffer = Marshal.AllocHGlobal(GBTemplate.Length);
                    Marshal.Copy(GBTemplate, 0, OriginalCodeBuffer, GBTemplate.Length);

                    GBFRSW_NET_Dll_Wrapper.GBFRSW_CODE_HEADER HeaderPtr = new
                        GBFRSW_NET_Dll_Wrapper.GBFRSW_CODE_HEADER();

                    GBFRSW_NET_Dll_Wrapper.GBFRSW_GetCodeHeader(
                        ref HeaderPtr, OriginalCodeBuffer);

                    Int32 RetVal = GBFRSW_NET_Dll_Wrapper.GBFRSW_ISO_GetMaxFMRLength(
                        HeaderPtr.ImageSizeX, HeaderPtr.ImageSizeY,
                        FormatType,
                        GBProprietaryData,
                        out MaxFMRLen);
                    Marshal.FreeHGlobal(OriginalCodeBuffer);

                    IntPtr GBTemplateBuffer = Marshal.AllocHGlobal(GBTemplate.Length);
                    //Console.WriteLine("GBFRSW_NET_ISO_GBTemplateToFMR: 8, GBTemplateBuffer = " + GBTemplateBuffer.ToInt64());
                    //Console.WriteLine("GBFRSW_NET_ISO_GBTemplateToFMR: 8, GBTemplate.Length = " + GBTemplate.Length);
                    Marshal.Copy(GBTemplate, 0, GBTemplateBuffer, GBTemplate.Length);

                    IntPtr FMRBuffer = Marshal.AllocHGlobal(MaxFMRLen);
                    FMRLen = 0;
                    RetVal = GBFRSW_NET_Dll_Wrapper.GBFRSW_ISO_GBTemplateToFMR(
                        GBTemplateBuffer, FMRBuffer,
                        out FMRLen,
                        FingerPos,
                        FormatType,
                        GBProprietaryData);
                    //Console.WriteLine("GBFRSW_NET_ISO_GBTemplateToFMR: 10b, GBTemplateBuffer = " + GBTemplateBuffer.ToInt64());

                    Marshal.FreeHGlobal(GBTemplateBuffer);
                    
                    FMR = new Byte[FMRLen];
                    //Console.WriteLine("GBFRSW_NET_ISO_GBTemplateToFMR: 11");
                    Marshal.Copy(FMRBuffer, FMR, 0, FMRLen);
                    //Console.WriteLine("GBFRSW_NET_ISO_GBTemplateToFMR: 12");
                    Marshal.FreeHGlobal(FMRBuffer);
                    //Console.WriteLine("GBFRSW_NET_ISO_GBTemplateToFMR: 13");
                    return RetVal;
                }
                catch (Exception ex)
                {
                    FMRLen = 0;
                    GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperExceptionString =
                        "GBFRSW_NET_ISO_GBTemplateToFMR: " + ex.Message;
                    FMR = null;
                    return GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperException;
                }
            }

            /**************************************
             * GBFRSW_NET_ISO_FMRToGBTemplate
            **************************************/
            public static Int32 GBFRSW_NET_ISO_FMRToGBTemplate(
                Byte[] FMR,
                out Byte[] GBTemplate,
                out Byte FingerPos,
                Byte FormatType
               )
            {
                try
                {
                    UInt32 SampleCodeSize, PackCodeSize, TempCodeSize, CompTempCodeSize;
                    UInt32 ImageSizeX, ImageSizeY, MinSXui, MinSYui, MemBuffSizeui;
                    Byte V1b, V2b, V3b, V4b;

                    Int32 RetVal = GBFRSW_NET_Dll_Wrapper.GBFRSW_GetVersionInfo(
                        out V1b, out V2b, out V3b, out V4b,
                        out ImageSizeX, out ImageSizeY,
                        out MinSXui, out MinSYui,
                        GBFRSW_NET_Definitions.GBFRSW_NET_MemoryRequestMasks.All,
                        out MemBuffSizeui
                    );

                    if (RetVal != GBFRSW_NET.GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.Success)
                    {
                        GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperExceptionString =
                            "GBFRSW_NET_Correct: error in calling GBFRSW_GetVersionInfo() function";
                        GBTemplate = null;
                        FingerPos = 0;
                        return GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperException;
                    }

                    RetVal = GBFRSW_NET_Dll_Wrapper.GBFRSW_CodeMaxSize(
                        ImageSizeX, ImageSizeY, out SampleCodeSize, out PackCodeSize, out TempCodeSize, out CompTempCodeSize);

                    if (RetVal != GBFRSW_NET.GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.Success)
                    {
                        GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperExceptionString =
                            "GBFRSW_NET_Correct: error in calling GBFRSW_CodeMaxSize() function";
                        GBTemplate = null;
                        FingerPos = 0;
                        return GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperException;
                    }

                    IntPtr FMRBuffer = Marshal.AllocHGlobal(FMR.Length);
                    Marshal.Copy(FMR, 0, FMRBuffer, FMR.Length);

                    GBTemplate = new Byte[TempCodeSize];
                    for (int i = 0; i < TempCodeSize; i++)
                        GBTemplate.SetValue((Byte)0, i);
                    IntPtr GBTemplateBuffer = Marshal.AllocHGlobal((int)TempCodeSize);

                    Marshal.Copy(GBTemplate, 0, GBTemplateBuffer, (int)TempCodeSize);

                    RetVal = GBFRSW_NET_Dll_Wrapper.GBFRSW_ISO_FMRToGBTemplate(
                        FMRBuffer, GBTemplateBuffer,
                        out FingerPos,
                        FormatType);
                    Marshal.FreeHGlobal(FMRBuffer);

                    Marshal.Copy(GBTemplateBuffer, GBTemplate, 0, GBTemplate.Length);
                    Marshal.FreeHGlobal(GBTemplateBuffer);

                    return RetVal;
                }
                catch (Exception ex)
                {
                    FingerPos = 0;
                    GBTemplate = null;
                    GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperExceptionString =
                        "GBFRSW_NET_ISO_FMRToGBTemplate: " + ex.Message;
                    return GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperException;
                }
            }
            /**************************************
             * GBFRSW_NET_ISO_GetMaxFMCLength
            **************************************/
            public static Int32 GBFRSW_NET_ISO_GetMaxFMCLength(
                UInt16 FormatType,
                UInt32 ImageSizeX,
                UInt32 ImageSizeY,
                UInt32 MaxMinutiaeNum,
                out UInt16 MaxFMCLen
               )
            {
                try
                {
                    Int32 RetVal = GBFRSW_NET_Dll_Wrapper.GBFRSW_ISO_GetMaxFMCLength(
                        FormatType,
                        ImageSizeX, ImageSizeY,
                        MaxMinutiaeNum,
                        out MaxFMCLen);

                    return RetVal;
                }
                catch (Exception ex)
                {
                    MaxFMCLen = 0;
                    GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperExceptionString =
                        "GBFRSW_NET_ISO_GetMaxFMCLength: " + ex.Message;
                    return GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperException;
                }
            }

            /**************************************
             * GBFRSW_NET_ISO_GBTemplateToFMC
            **************************************/
            public static Int32 GBFRSW_NET_ISO_GBTemplateToFMC(
                Byte[] GBTemplate,
                out Byte[] FMC,
                out UInt16 FMCLen,
                UInt16 FormatType,
                UInt32 MaxMinutiaeNum
               )
            {
                try
                {
                    UInt16 MaxFMCLen;

                    IntPtr OriginalCodeBuffer = Marshal.AllocHGlobal(GBTemplate.Length);
                    Marshal.Copy(GBTemplate, 0, OriginalCodeBuffer, GBTemplate.Length);

                    GBFRSW_NET_Dll_Wrapper.GBFRSW_CODE_HEADER HeaderPtr = new
                        GBFRSW_NET_Dll_Wrapper.GBFRSW_CODE_HEADER();

                    GBFRSW_NET_Dll_Wrapper.GBFRSW_GetCodeHeader(
                        ref HeaderPtr, OriginalCodeBuffer);

                    Int32 RetVal = GBFRSW_NET_Dll_Wrapper.GBFRSW_ISO_GetMaxFMCLength(
                        FormatType,
                        HeaderPtr.ImageSizeX, HeaderPtr.ImageSizeY,
                        MaxMinutiaeNum,
                        out MaxFMCLen);

                    Marshal.FreeHGlobal(OriginalCodeBuffer);

                    if (RetVal != GBFRSW_NET.GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.Success)
                    {
                        GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperExceptionString =
                            "GBFRSW_NET_ISO_GBTemplateToFMC: error in calling GBFRSW_ISO_GetMaxFMCLength() function";
                        FMC = null;
                        FMCLen = 0;
                        return GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperException;
                    }

                    IntPtr GBTemplateBuffer = Marshal.AllocHGlobal(GBTemplate.Length);
                    Marshal.Copy(GBTemplate, 0, GBTemplateBuffer, GBTemplate.Length);

                    IntPtr FMCBuffer = Marshal.AllocHGlobal(MaxFMCLen);

                    RetVal = GBFRSW_NET_Dll_Wrapper.GBFRSW_ISO_GBTemplateToFMC(
                        GBTemplateBuffer, FMCBuffer,
                        out FMCLen,
                        FormatType, MaxMinutiaeNum);
                    Marshal.FreeHGlobal(GBTemplateBuffer);

                    FMC = new Byte[FMCLen];

                    Marshal.Copy(FMCBuffer, FMC, 0, FMCLen);
                    Marshal.FreeHGlobal(FMCBuffer);

                    return RetVal;
                }
                catch (Exception ex)
                {
                    FMCLen = 0;
                    GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperExceptionString =
                        "GBFRSW_NET_ISO_GBTemplateToFMR: " + ex.Message;
                    FMC = null;
                    return GBFRSW_NET_Definitions.GBFRSW_NET_FunctionReturnCodes.WrapperException;
                }
            }
        }
    }
}
