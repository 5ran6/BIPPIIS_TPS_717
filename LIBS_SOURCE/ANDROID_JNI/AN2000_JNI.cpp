//-------------------------------------------------------------//
//-------------------------------------------------------------//
// INCLUDE
//-------------------------------------------------------------//
//-------------------------------------------------------------//
#include <jni.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "GB_SystemConfiguration.h"
#include "an2k_dll.h"
#include "GB_JNI_Utils.h"
#include <android/log.h>

//-------------------------------------------------------------//
//-------------------------------------------------------------//
// DEFINE
//-------------------------------------------------------------//
//-------------------------------------------------------------//
#define FreeLibrary(ptr) dlclose(ptr)
#define LoadLibrary(path) dlopen(path,RTLD_LAZY)
#define GetProcAddress(ptr, fname) dlsym(ptr,fname)
#define a_printf(...) __android_log_print(ANDROID_LOG_DEBUG, "AN2000_LOG", __VA_ARGS__);

//-------------------------------------------------------------//
//-------------------------------------------------------------//
// GLOBALS
//-------------------------------------------------------------//
//-------------------------------------------------------------//
HMODULE An2000DllPtr = NULL;
char An2000_JNI_LastErrorString[1000];

int (__stdcall *Ptr_AN2000_Create_ANSI_NIST)(
		ANSI_NIST **oansi_nist,
		int Version,
		char *TypeOfTransaction,
		int Priority,
		char *DestinationAgency,
		char *OriginatingAgency,
		char *TransactionControlID,
		char *TransactionControlReference,
		float NominalScanningResolution,
		float NominalTransmitResolution,
		char *DomainNameID
) = NULL;

int (__stdcall *Ptr_AN2000_GetEFTSType1_Info)(
		RECORD *record,
		int *Version,                // version
		char *TransactionType,        // pass a char[12]
		int *Priority,                // priority
		char **pDestinationAgency,        // destination agency: no limits on length of this field, so
		// a developer needs next field:
		int *DestAgencyLen,            // DestinationAgency string length
		char **pOriginatingAgency,        // originating agency: no limits on length of this field, so
		// a developer needs next field:
		int *OriginatingAgencyLen,    // OriginatingAgency string length
		char **pTransContrNum,            // transaction control number: no limits on length of this field, so
		// a developer needs next field:
		int *TransContrNumLen,        // TransContrNum string length
		char **pTransContrRef,            // transaction control reference: no limits on length of this field, so
		// a developer needs next field:
		int *TransContrRefLen,        // TransContrRef string length
		float *NominalScanRes,            // Nominal Scanning Resolution
		float *NominalTXRes,            // Nominal Transmitting Resolution
		char **pDomain,                // Domain: no limits on length of this field, so
		// a developer needs next field:
		int *DomainLen                // Domain string length
) = NULL;

int (__stdcall *Ptr_AN2000_UpdateETFStype_1)(
		RECORD *orecord,
		float NominalScanningResolution,
		float NominalTransmitResolution
) = NULL;

int (__stdcall *Ptr_AN2000_image2ETFStype_14)(
		RECORD **orecord,
		unsigned char *FingerprintImage,
		const int FPImageSize,
		const int FPImageWidth,
		const int FPImageHeight,
		const int FPImagePixelDepth,
		const double ScanResolutionPPMM,
		char *CompressionType,
		const int ImpressionType,
		char *ImageSource,
		char *Comment,
		char *FingerPosition,
		FING_AMP_LIST *AmputationList,
		FOUR_FING_BOUND *boundaries,
		FOUR_FING_QUALITY *qualities,
		ALT_QUAL_LIST *segqual,
		ALT_QUAL_LIST *altqual
) = NULL;

int (__stdcall *Ptr_AN2000_image2ETFStype_4)(
		RECORD **orecord,
		unsigned char *FingerprintImage,
		const int FPImageSize,
		const int ImpressionType,
		const int FingerPosition,
		const int ScanResolutionID,
		const int FPImageWidth,
		const int FPImageHeight,
		const char *CompressionType
) = NULL;

int (__stdcall *Ptr_AN2000_UpdateETFStype_4)(
		RECORD *orecord,
		unsigned char *FingerprintImage,
		const int FPImageSize,
		const int ScanResolutionID,
		const int FPImageWidth,
		const int FPImageHeight
) = NULL;

int (__stdcall *Ptr_AN2000_Get_EFTStype4_Info)(
		RECORD *record,                    // input record
		unsigned char **ImagePtr,                // image frame: allocated by the library
		int *ImageSize,                // allocated memory for image frame (can be different from w x h)
		int *ImageImpression,        // image impression type (live-scan, roll, etc, see definitions)
		int finger_position[6],        // int indicating the finger position. See definitions
		int *ImageResolution,        // the scan resolution of the fingerprint image. See definitions
		int *w,
		int *h,                        // image dimensions
		char *ImageCompression        // image compression: see definitions: pass a char[10]
) = NULL;

int (__stdcall *Ptr_AN2000_UpdateETFStype_14)(
		RECORD *orecord,
		unsigned char *FingerprintImage,
		const int FPImageSize,
		const int FPImageWidth,
		const int FPImageHeight,
		const double ScanResolutionPPMM,
		FING_AMP_LIST *amp_list,
		FOUR_FING_BOUND *boundaries,
		FOUR_FING_QUALITY *qualities,
		ALT_QUAL_LIST *segqual,
		ALT_QUAL_LIST *altqual
) = NULL;

int (__stdcall *Ptr_AN2000_Get_EFTStype14_Info)(
		RECORD *record,                // input record
		unsigned char **ImagePtr,            // image frame: allocated by the library
		int *ImageSize,            // allocated memory for image frame (can be different from w x h)
		int *w,
		int *h,                    // image dimensions
		int *PixelDepth,            // bits per pixel
		double *Resolution_ppmm,    // resolution in pixel per mm
		char *ImageCompression,    // image compression: see definitions: pass a char[10]
		int *ImageImpression,    // image impression type (live-scan, roll, etc, see definitions)
		char *source_str,            // string identifying the originating source of the image, pass a char[44]
		char *comment,            // comment, pass a char[136]
		char finger_position_list[6][3],    // list of strings indicating the finger position.
		// See definitions, pass a list of char[3]
		// composed of 6 items
		FING_AMP_LIST **pamp_list,            // list of missing fingers and amputation codes: allocated
		// by the library
		FOUR_FING_BOUND **pboundaries,        // boundaries of each of four (or two thumbs) fingers in a slap
		FOUR_FING_QUALITY **pqualities,            // list of NFIQ quality of each of four or two thumbs or single  fingers in a slap
		ALT_QUAL_LIST **psegqual,            // list of segmentation quality for fingers
		ALT_QUAL_LIST **paltqual            // list of quality of each of four or two thumbs or single fingers in
		// a slap calculated with an algorithm different from NFIQ
) = NULL;

int (__stdcall *Ptr_AN2000_UpdateType1415ScaleUnits)(RECORD *recordToUpdate, int newScale) = NULL;

int (__stdcall *Ptr_AN2000_image2ETFStype_15)(
		RECORD **orecord,
		unsigned char *PPImage,
		const int PPImageSize,
		const int PPImageWidth,
		const int PPImageHeight,
		const int BitsPerPixel,
		const double PPImageResolutionPPMM,
		char *CompressionType,
		const int ImpressionType,
		char *ImageSource,
		char *Comment,
		char *PalmPosition,
		PALM_QUAL_LIST *PalmQualitiesList
) = NULL;

int (__stdcall *Ptr_AN2000_UpdateETFStype_15)(
		RECORD *orecord,
		unsigned char *PPImage,
		const int PPImageSize,
		const int PPImageWidth,
		const int PPImageHeight,
		const double PPImageResolutionPPMM,
		PALM_QUAL_LIST *PalmQualitiesList
) = NULL;

int (__stdcall *Ptr_AN2000_Get_EFTStype15_Info)(
		RECORD *record,                // input record
		unsigned char **ImagePtr,            // image frame: allocated by the library
		int *ImageSize,            // allocated memory for image frame (can be different from w x h)
		int *w,
		int *h,                    // image dimensions
		int *BitsPerPixel,        // bits per pixel
		double *Resolution_ppmm,    // resolution in pixel per mm
		char *ImageCompression,    // image compression: see definitions: pass a char[10]
		int *ImageImpression,    // image impression type (live-scan, roll, etc, see definitions)
		char *source_str,            // string identifying the originating source of the image, pass a char[44]
		char *comment,            // comment, pass a char[136]
		char *PalmPosition,        // string indicating the palm position. See definitions
		PALM_QUAL_LIST **PalmQualitiesList    // list of quality values calculated by using several algorhythms
) = NULL;

int (__stdcall *Ptr_AN2000_insert_ANSI_NIST_record_ETFS)(
		const int RecordIndex,
		RECORD *RecordToBeInserted,
		ANSI_NIST *ansi_nist
) = NULL;

int (__stdcall *Ptr_AN2000_delete_ANSI_NIST_record)(
		const int RecordIndex,
		ANSI_NIST *ansi_nist
) = NULL;

int (__stdcall *Ptr_AN2000_read_ANSI_NIST_buffer)(
		unsigned char *buffer,
		int bufLen,
		ANSI_NIST **oansi_nist
) = NULL;

int (__stdcall *Ptr_AN2000_write_ANSI_NIST_2_buffer)(
		unsigned char **pOutBuffer,
		int *bufLen,
		const ANSI_NIST *ansi_nist
) = NULL;

void (__stdcall *Ptr_AN2000_free_ANSI_NIST)(ANSI_NIST *ansi_nist) = NULL;

void (__stdcall *Ptr_AN2000_free_ANSI_NIST_record)(RECORD *record) = NULL;

void (__stdcall *Ptr_AN2000_free_ANSI_NIST_buffer)(void *buffer) = NULL;

void (__stdcall *Ptr_AN2000_free_ANSI_NIST_amp_list)(FING_AMP_LIST *amp_list) = NULL;

void (__stdcall *Ptr_AN2000_free_FOUR_FING_BOUND)(FOUR_FING_BOUND *boundaries) = NULL;

void (__stdcall *Ptr_AN2000_free_FOUR_FING_QUALITY)(FOUR_FING_QUALITY *qualities) = NULL;

void (__stdcall *Ptr_AN2000_free_ANSI_NIST_alt_qual_list)(ALT_QUAL_LIST *alt_qual_list) = NULL;

void (__stdcall *Ptr_AN2000_free_ANSI_NIST_palm_qual_list)(PALM_QUAL_LIST *palm_qual_list) = NULL;

///////////////////////////////////////////////////////////////
// AN2011
///////////////////////////////////////////////////////////////
int (__stdcall *Ptr_GB_AN2011_CreateAnsiNistStruct )(
		ANSI_NIST **pAnsiNist,
		GB_AN_ENGINE_String Tot,
		GB_AN2011_DateStruct *Dat,
		int Pry,
		GB_AN_ENGINE_String DestAgency,
		GB_AN_ENGINE_String OrigAgency,
		GB_AN_ENGINE_String TransContrNum,
		GB_AN_ENGINE_String TransContrRef,
		float NativeScanRes,
		float NominalTxRes,
		GB_AN_ENGINE_String DomainName,
		GB_AN_ENGINE_String DomainVersion,
		GB_AN2011_GmtStruct *Gmt,
		int CharEncIndex,
		GB_AN_ENGINE_String CharEncName,
		GB_AN_ENGINE_String CharEncVersion,
		GB_AN2011_ApplicationProfileStruct *Aps,
		int ApsNum,
		GB_AN_ENGINE_String DestAgName,
		GB_AN_ENGINE_String OrigAgName
) = NULL;

int (__stdcall *Ptr_GB_AN2011_GetType1RecordInfo)(
		RECORD *record,
		int *pVersion,
		GB_AN_ENGINE_String *pTypeOfTransaction,
		GB_AN2011_DateStruct *Dat,
		int *pPriority,
		GB_AN_ENGINE_String *pDestAgency,
		GB_AN_ENGINE_String *pOrigAgency,
		GB_AN_ENGINE_String *pTransContrNum,
		GB_AN_ENGINE_String *pTransContrRef,
		float *pNativeScanRes,
		float *pNominalTxRes,
		GB_AN_ENGINE_String *pDomainName,
		GB_AN_ENGINE_String *pDomainVersion,
		GB_AN2011_GmtStruct *Gmt,
		int *pCharEncIndex,
		GB_AN_ENGINE_String *pCharEncName,
		GB_AN_ENGINE_String *pCharEncVersion,
		GB_AN2011_ApplicationProfileStruct **pAps,
		int *pApsNum,
		GB_AN_ENGINE_String *pDestAgName,
		GB_AN_ENGINE_String *pOrigAgName
) = NULL;

int (__stdcall *Ptr_GB_AN2011_CreateType4Record)(
		RECORD **pRecord,
		int idc,
		int impressionType,
		unsigned char fgp,
		int imageScanResolution,
		int horizontalNumberOfPixels,
		int verticalNumberOfPixels,
		int compressionAlgorithm,
		unsigned char *imageBuffer,
		int imageBufferLen
) = NULL;

int (__stdcall *Ptr_GB_AN2011_GetType4RecordInfo)(
		RECORD *recordToRead,
		int *pIdc,
		int *pImpressionType,
		unsigned char *fgp,
		int *pImageScanResolution,
		int *pHorizontalNumberOfPixels,
		int *pVerticalNumberOfPixels,
		int *pCompressionAlgorithm,
		unsigned char **pImageBuffer,
		int *pImageBufferLen
) = NULL;

int (__stdcall *Ptr_GB_AN2011_CreateType14Record)(
		RECORD **pRecord,
		int idc,
		int impressionType,
		GB_AN_ENGINE_String sourceAgency,
		GB_AN2011_DateStruct *fingerprintCaptureDate,
		int horizontalNumberOfPixels,
		int verticalNumberOfPixels,
		int scaleUnits,
		int transmittedHorizontalPixelScale,
		int transmittedVerticalPixelScale,
		int compressionAlgorithm,
		int bitsPerPixel,
		int fingerprintPosition,
		GB_AN2011_PrintPositionDescriptorsStruct *ppd,
		GB_AN2011_PrintPositionCoordinatesStruct *ppc, int ppcNum,
		int scannedHorizontalPixelScale,
		int scannedVerticalPixelScale,
		GB_AN2011_AmputatedBandagedCodeStruct *amp, int ampNum,
		GB_AN_ENGINE_String comment,
		GB_AN2011_FingerSegmentPositionStruct *seg, int segNum,
		GB_AN2011_NistQualityMetricStruct *nqm, int nqmNum,
		GB_AN2011_FingerOrSegmentQualityMetricStruct *sqm, int sqmNum,
		GB_AN2011_FingerOrSegmentQualityMetricStruct *fqm, int fqmNum,
		GB_AN2011_AlternateSegmentPositionStruct *aseg, int asegNum,
		int simultaneousCaptureFingerprint,
		BOOL stitchedImageFlag,
		GB_AN_ENGINE_String deviceMonitoringMode,
		int fingerprintAcquisitionProfile,
		unsigned char *imageBuffer, int imageBufferLen
) = NULL;

int (__stdcall *Ptr_GB_AN2011_GetType14RecordInfo)(
		RECORD *recordToRead,
		int *pIdc,
		int *pImpressionType,
		GB_AN_ENGINE_String *pSourceAgency,
		GB_AN2011_DateStruct *fingerprintCaptureDate,
		int *pHorizontalNumberOfPixels,
		int *pVerticalNumberOfPixels,
		int *pScaleUnits,
		int *pTransmittedHorizontalPixelScale,
		int *pTransmittedVerticalPixelScale,
		int *pCompressionAlgorithm,
		int *pBitsPerPixel,
		int *pFingerprintPosition,
		GB_AN2011_PrintPositionDescriptorsStruct *ppd,
		GB_AN2011_PrintPositionCoordinatesStruct **pPpc, int *pPpcNum,
		int *pScannedHorizontalPixelScale,
		int *pScannedVerticalPixelScale,
		GB_AN2011_AmputatedBandagedCodeStruct **pAmp, int *pAmpNum,
		GB_AN_ENGINE_String *pComment,
		GB_AN2011_FingerSegmentPositionStruct **pSeg, int *pSegNum,
		GB_AN2011_NistQualityMetricStruct **pNqm, int *pNqmNum,
		GB_AN2011_FingerOrSegmentQualityMetricStruct **pSqm, int *pSqmNum,
		GB_AN2011_FingerOrSegmentQualityMetricStruct **pFqm, int *pFqmNum,
		GB_AN2011_AlternateSegmentPositionStruct **pAseg, int *pAsegNum,
		int *pSimultaneousCaptureFingerprint,
		BOOL *pStitchedImageFlag,
		GB_AN_ENGINE_String *pDeviceMonitoringMode,
		int *pFingerprintAcquisitionProfile,
		unsigned char **pImageBuffer, int *pImageBufferLen
) = NULL;

int (__stdcall *Ptr_GB_AN2011_CreateType15Record)(
		RECORD **pRecord,
		int idc,
		int impressionType,
		GB_AN_ENGINE_String sourceAgency,
		GB_AN2011_DateStruct *palmprintCaptureDate,
		int horizontalNumberOfPixels,
		int verticalNumberOfPixels,
		int scaleUnits,
		int transmittedHorizontalPixelScale,
		int transmittedVerticalPixelScale,
		int compressionAlgorithm,
		int bitsPerPixel,
		int fingerprintPosition,
		int scannedHorizontalPixelScale,
		int scannedVerticalPixelScale,
		GB_AN2011_AmputatedBandagedCodeStruct *amp, int ampNum,
		GB_AN_ENGINE_String comment,
		GB_AN2011_FingerOrSegmentQualityMetricStruct *pqm, int pqmNum,
		GB_AN_ENGINE_String deviceMonitoringMode,
		unsigned char *imageBuffer, int imageBufferLen
) = NULL;

int (__stdcall *Ptr_GB_AN2011_GetType15RecordInfo)(
		RECORD *recordToRead,
		int *pIdc,
		int *pImpressionType,
		GB_AN_ENGINE_String *pSourceAgency,
		GB_AN2011_DateStruct *fingerprintCaptureDate,
		int *pHorizontalNumberOfPixels,
		int *pVerticalNumberOfPixels,
		int *pScaleUnits,
		int *pTransmittedHorizontalPixelScale,
		int *pTransmittedVerticalPixelScale,
		int *pCompressionAlgorithm,
		int *pBitsPerPixel,
		int *pFingerprintPosition,
		int *pScannedHorizontalPixelScale,
		int *pScannedVerticalPixelScale,
		GB_AN2011_AmputatedBandagedCodeStruct **pAmp, int *pAmpNum,
		GB_AN_ENGINE_String *pComment,
		GB_AN2011_FingerOrSegmentQualityMetricStruct **pPqm, int *pPqmNum,
		GB_AN_ENGINE_String *pDeviceMonitoringMode,
		unsigned char **pImageBuffer, int *pImageBufferLen
) = NULL;

void (__stdcall *Ptr_GB_AN2011_AvoidLeadingZeroes)(BOOL avoid) = NULL;

void (__stdcall *Ptr_GB_AN2011_FreeAsegArray)(
		GB_AN2011_AlternateSegmentPositionStruct *asegArray, int asegNum
) = NULL;

///////////////////////////////////////////////////////////////
// AN Engine
///////////////////////////////////////////////////////////////
int (__stdcall *Ptr_GB_AN_ENGINE_CreateSubFieldFromValuesForTaggedRecords)(
		SUBFIELD **outSubField,
		GB_AN_ENGINE_StringArray valuesToInsertArray,
		int numOfValuesToInsert
) = NULL;

int (__stdcall *Ptr_GB_AN_ENGINE_CreateEmptyFieldForTaggedRecords)(
		FIELD **outField,
		int recordType,
		int fieldId
) = NULL;

int (__stdcall *Ptr_GB_AN_ENGINE_InsertSubfieldIntoFieldForTaggedRecords)(
		FIELD *fieldToUpdate,
		SUBFIELD *subfieldToInsert
) = NULL;

int (__stdcall *Ptr_GB_AN_ENGINE_CreateFieldFromValuesForTaggedRecords)(
		FIELD **outField,
		int recordType,
		int fieldId,
		GB_AN_ENGINE_StringArray valuesToInsertArray,
		int numOfValuesToInsert
) = NULL;

int (__stdcall *Ptr_GB_AN_ENGINE_CreateImageFieldForTaggedRecord)(
		FIELD **outField,
		int recordType,
		unsigned char *imageBuffer,
		int imageBufferLen
) = NULL;

int (__stdcall *Ptr_GB_AN_ENGINE_CreateEmptyTaggedRecord)(
		RECORD **outRecord,
		int recordType,
		int Idc
) = NULL;

int (__stdcall *Ptr_GB_AN_ENGINE_InsertFieldIntoTaggedRecord)(
		RECORD *recordToUpdate,
		FIELD *fieldToInsert
) = NULL;

int (__stdcall *Ptr_GB_AN_ENGINE_CreateBinaryRecord)(
		RECORD **pRecord,
		int recordType,
		int idc,
		int impressionType,
		unsigned char *fgp,
		int imageScanResolution,
		int horizontalNumberOfPixels,
		int verticalNumberOfPixels,
		int compressionAlgorithm,
		unsigned char *imageBuffer,
		int imageBufferLen
) = NULL;

int (__stdcall *Ptr_GB_AN_ENGINE_CreateEmptyAnsiNistStruct)(
		ANSI_NIST **pAnsinistToBeCreated
) = NULL;

int (__stdcall *Ptr_GB_AN_ENGINE_InsertRecordInAnsiNist)(
		RECORD *RecordToBeInserted,
		ANSI_NIST *ansi_nist,
		const int RecordIndex,
		BOOL AutomaticIDC
) = NULL;

int (__stdcall *Ptr_GB_AN_ENGINE_SearchFieldInTaggedRecord)(
		RECORD *recordToLookup,
		int FieldId,
		FIELD **FoundField
) = NULL;

int (__stdcall *Ptr_GB_AN_ENGINE_GetValuesFromFieldForTaggedRecords)(
		FIELD *inField,
		GB_AN_ENGINE_StringArray *pValuesToGet,
		int *pNumOfValuesToGet
) = NULL;

int (__stdcall *Ptr_GB_AN_ENGINE_GetValuesFromSubfieldForTaggedRecords)(
		SUBFIELD *inSubField,
		GB_AN_ENGINE_StringArray *pValuesToGet,
		int *pNumOfValuesToGet
) = NULL;

int (__stdcall *Ptr_GB_AN_ENGINE_GetImageFieldFromTaggedRecord)(
		RECORD *recordToRead, unsigned char **pImageBuffer, int *pImgLen
) = NULL;

int (__stdcall *Ptr_GB_AN_ENGINE_GetBinaryRecordInfo)(
		RECORD *recordToRead,
		int *pRecordType,
		int *pIdc,
		int *pImpressionType,
		unsigned char *fgp,
		int *pImageScanResolution,
		int *pHorizontalNumberOfPixels,
		int *pVerticalNumberOfPixels,
		int *pCompressionAlgorithm,
		unsigned char **pImageBuffer,
		int *pImageBufferLen
) = NULL;

int (__stdcall *Ptr_GB_AN_ENGINE_DeleteFieldFromTaggedRecord)(
		RECORD *recToBeUpdated,
		int fieldId
) = NULL;

int (__stdcall *Ptr_GB_AN_ENGINE_DeleteFieldFromTaggedRecordInAnsiNist)(
		ANSI_NIST *anStructToBeUpdated,
		int recordIndex,
		int fieldId
) = NULL;

void (__stdcall *Ptr_GB_AN_ENGINE_Free)(
		ANSI_NIST *anStructToBeFreed,
		RECORD *recordToBeFreed,
		FIELD *fieldToBeFreed,
		SUBFIELD *subfieldToBeFreed,
		ITEM *itemToBeFreed
) = NULL;

void (__stdcall *Ptr_GB_AN_ENGINE_FreeGenericMem)(
		void *MemToFree
) = NULL;

int (__stdcall *Ptr_GB_AN_ENGINE_FreeStringArray)(
		GB_AN_ENGINE_StringArray ValuesToFree,
		int numOfValuesToFree
) = NULL;

//-------------------------------------------------------------//
//-------------------------------------------------------------//
// FUNCTIONS
//-------------------------------------------------------------//
//-------------------------------------------------------------//
extern "C"
{
JNIEXPORT jstring JNICALL
Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_GetLastErrorString(
		JNIEnv *env,
		jobject /* this */)
{
	FILE *fp = fopen("MYAN2K.txt", "w");
	if (fp != NULL)
	{
		fprintf(fp, "%s", An2000_JNI_LastErrorString);
		fclose(fp);
	}
	return env->NewStringUTF(An2000_JNI_LastErrorString);
}

void GbAn2000Util_FormatErrorString(int RetVal, char *ErrorStr)
{
	switch (RetVal)
	{
		case AN2K_DLL_NO_ERROR:
		{
			sprintf(ErrorStr, "NO ERROR");
			return;
		}
		case AN2K_DLL_BAD_PARAMETER:
		{
			sprintf(ErrorStr, "BAD PARAMETER");
			return;
		}
		case AN2K_DLL_MEMORY:
		{
			sprintf(ErrorStr, "MEMORY ALLOCATION");
			return;
		}
		case AN2K_DLL_FIELD_NOT_FOUND:
		{
			sprintf(ErrorStr, "FIELD NOT FOUND");
			return;
		}
		case AN2K_DLL_GENERIC:
		{
			sprintf(ErrorStr, "GENERIC ERROR");
			return;
		}
		case AN2K_DLL_RECORD_NOT_FOUND:
		{
			sprintf(ErrorStr, "RECORD NOT FOUND");
			return;
		}
		case AN2K_DLL_SUBFIELD_NOT_FOUND:
		{
			sprintf(ErrorStr, "SUBFIELD NOT FOUND");
			return;
		}
		case AN2K_DLL_ITEM_NOT_FOUND:
		{
			sprintf(ErrorStr, "ITEM NOT FOUND");
			return;
		}
		default:
		{
			sprintf(ErrorStr, "UNKNOWN");
			return;
		}
	}
}

JNIEXPORT jint JNICALL Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_Unload(
		JNIEnv *env,
		jobject /* this */)
{
	int ValToRet = AN2K_DLL_NO_ERROR;

	if (An2000DllPtr != NULL)
	{
		FreeLibrary(An2000DllPtr);
	}
	else
	{
		sprintf(An2000_JNI_LastErrorString,
		        "Unload: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
	}

	Ptr_AN2000_Create_ANSI_NIST = NULL;
	Ptr_AN2000_GetEFTSType1_Info = NULL;
	Ptr_AN2000_UpdateETFStype_1 = NULL;
	Ptr_AN2000_image2ETFStype_14 = NULL;
	Ptr_AN2000_UpdateETFStype_14 = NULL;
	Ptr_AN2000_Get_EFTStype14_Info = NULL;
	Ptr_AN2000_UpdateType1415ScaleUnits = NULL;
	Ptr_AN2000_image2ETFStype_4 = NULL;
	Ptr_AN2000_UpdateETFStype_4 = NULL;
	Ptr_AN2000_Get_EFTStype4_Info = NULL;
	Ptr_AN2000_image2ETFStype_15 = NULL;
	Ptr_AN2000_UpdateETFStype_15 = NULL;
	Ptr_AN2000_Get_EFTStype15_Info = NULL;
	Ptr_AN2000_insert_ANSI_NIST_record_ETFS = NULL;
	Ptr_AN2000_delete_ANSI_NIST_record = NULL;
	Ptr_AN2000_read_ANSI_NIST_buffer = NULL;
	Ptr_AN2000_free_ANSI_NIST = NULL;
	Ptr_AN2000_free_ANSI_NIST_record = NULL;
	Ptr_AN2000_free_ANSI_NIST_buffer = NULL;
	Ptr_AN2000_free_ANSI_NIST_amp_list = NULL;
	Ptr_AN2000_free_FOUR_FING_BOUND = NULL;
	Ptr_AN2000_free_FOUR_FING_QUALITY = NULL;
	Ptr_AN2000_free_ANSI_NIST_alt_qual_list = NULL;
	Ptr_AN2000_free_ANSI_NIST_palm_qual_list = NULL;
	Ptr_AN2000_write_ANSI_NIST_2_buffer = NULL;

	Ptr_GB_AN2011_CreateAnsiNistStruct = NULL;
	Ptr_GB_AN2011_GetType1RecordInfo = NULL;
	Ptr_GB_AN2011_CreateType4Record = NULL;
	Ptr_GB_AN2011_AvoidLeadingZeroes = NULL;
	Ptr_GB_AN2011_GetType4RecordInfo = NULL;
	Ptr_GB_AN2011_CreateType14Record = NULL;
	Ptr_GB_AN2011_GetType14RecordInfo = NULL;
	Ptr_GB_AN2011_FreeAsegArray = NULL;
	Ptr_GB_AN2011_CreateType15Record = NULL;
	Ptr_GB_AN2011_GetType15RecordInfo = NULL;

	Ptr_GB_AN_ENGINE_CreateSubFieldFromValuesForTaggedRecords = NULL;
	Ptr_GB_AN_ENGINE_CreateEmptyFieldForTaggedRecords = NULL;
	Ptr_GB_AN_ENGINE_InsertSubfieldIntoFieldForTaggedRecords = NULL;
	Ptr_GB_AN_ENGINE_CreateFieldFromValuesForTaggedRecords = NULL;
	Ptr_GB_AN_ENGINE_CreateImageFieldForTaggedRecord = NULL;
	Ptr_GB_AN_ENGINE_CreateEmptyTaggedRecord = NULL;
	Ptr_GB_AN_ENGINE_InsertFieldIntoTaggedRecord = NULL;
	Ptr_GB_AN_ENGINE_CreateBinaryRecord = NULL;
	Ptr_GB_AN_ENGINE_CreateEmptyAnsiNistStruct = NULL;
	Ptr_GB_AN_ENGINE_InsertRecordInAnsiNist = NULL;
	Ptr_GB_AN_ENGINE_SearchFieldInTaggedRecord = NULL;
	Ptr_GB_AN_ENGINE_GetValuesFromFieldForTaggedRecords = NULL;
	Ptr_GB_AN_ENGINE_GetValuesFromSubfieldForTaggedRecords = NULL;
	Ptr_GB_AN_ENGINE_GetImageFieldFromTaggedRecord = NULL;
	Ptr_GB_AN_ENGINE_GetBinaryRecordInfo = NULL;
	Ptr_GB_AN_ENGINE_DeleteFieldFromTaggedRecord = NULL;
	Ptr_GB_AN_ENGINE_DeleteFieldFromTaggedRecordInAnsiNist = NULL;
	Ptr_GB_AN_ENGINE_Free = NULL;
	Ptr_GB_AN_ENGINE_FreeGenericMem = NULL;
	Ptr_GB_AN_ENGINE_FreeStringArray = NULL;

	An2000DllPtr = NULL;

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_Load(
		JNIEnv *env,
		jobject /* this */)
{
	int ValToRet = AN2K_DLL_NO_ERROR;

	Dl_info pathInfo;
	dladdr((void *) Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_Load, &pathInfo);
	a_printf("Load: pathName = %s", pathInfo.dli_fname);

	//////////////////////////////
	// free library
	//////////////////////////////
	if (An2000DllPtr != NULL)
		Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_Unload(env, nullptr);

	//////////////////////////////
	// load library
	//////////////////////////////
	An2000DllPtr = LoadLibrary("libAN2K_LIB.so");
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load AN2K_LIB: %s", dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: AN2K_LIB loaded\n");

	//////////////////////////////
	// GB_AN_ENGINE_DeleteFieldFromTaggedRecordInAnsiNist
	//////////////////////////////
	Ptr_GB_AN_ENGINE_DeleteFieldFromTaggedRecordInAnsiNist = (int (*)(
			ANSI_NIST *anStructToBeUpdated,
			int recordIndex,
			int fieldId
	)) GetProcAddress(An2000DllPtr, "GB_AN_ENGINE_DeleteFieldFromTaggedRecordInAnsiNist");
	if (Ptr_GB_AN_ENGINE_DeleteFieldFromTaggedRecordInAnsiNist == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_GB_AN_ENGINE_DeleteFieldFromTaggedRecordInAnsiNist: %s",
		        dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GB_AN_ENGINE_DeleteFieldFromTaggedRecordInAnsiNist loaded\n");

	//////////////////////////////
	// GB_AN_ENGINE_DeleteFieldFromTaggedRecord
	//////////////////////////////
	Ptr_GB_AN_ENGINE_DeleteFieldFromTaggedRecord = (int (*)(
			RECORD *recToBeUpdated,
			int fieldId
	)) GetProcAddress(An2000DllPtr, "GB_AN_ENGINE_DeleteFieldFromTaggedRecord");
	if (Ptr_GB_AN_ENGINE_DeleteFieldFromTaggedRecord == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_GB_AN_ENGINE_DeleteFieldFromTaggedRecord: %s",
		        dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GB_AN_ENGINE_DeleteFieldFromTaggedRecord loaded\n");

	//////////////////////////////
	// GB_AN_ENGINE_GetBinaryRecordInfo
	//////////////////////////////
	Ptr_GB_AN_ENGINE_GetBinaryRecordInfo = (int (*)(
			RECORD *recordToRead,
			int *pRecordType,
			int *pIdc,
			int *pImpressionType,
			unsigned char *fgp,
			int *pImageScanResolution,
			int *pHorizontalNumberOfPixels,
			int *pVerticalNumberOfPixels,
			int *pCompressionAlgorithm,
			unsigned char **pImageBuffer,
			int *pImageBufferLen
	)) GetProcAddress(An2000DllPtr, "GB_AN_ENGINE_GetBinaryRecordInfo");
	if (Ptr_GB_AN_ENGINE_GetBinaryRecordInfo == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_GB_AN_ENGINE_GetBinaryRecordInfo: %s",
		        dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GB_AN_ENGINE_GetBinaryRecordInfo loaded\n");

	//////////////////////////////
	// GB_AN_ENGINE_GetImageFieldFromTaggedRecord
	//////////////////////////////
	Ptr_GB_AN_ENGINE_GetImageFieldFromTaggedRecord = (int (*)(
			RECORD *recordToRead, unsigned char **pImageBuffer, int *pImgLen
	)) GetProcAddress(An2000DllPtr, "GB_AN_ENGINE_GetImageFieldFromTaggedRecord");
	if (Ptr_GB_AN_ENGINE_GetImageFieldFromTaggedRecord == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_GB_AN_ENGINE_GetImageFieldFromTaggedRecord: %s",
		        dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GB_AN_ENGINE_GetImageFieldFromTaggedRecord loaded\n");

	//////////////////////////////
	// GB_AN_ENGINE_GetValuesFromFieldForTaggedRecords
	//////////////////////////////
	Ptr_GB_AN_ENGINE_GetValuesFromFieldForTaggedRecords = (int (*)(
			FIELD *inField,
			GB_AN_ENGINE_StringArray *pValuesToGet,
			int *pNumOfValuesToGet
	)) GetProcAddress(An2000DllPtr, "GB_AN_ENGINE_GetValuesFromFieldForTaggedRecords");
	if (Ptr_GB_AN_ENGINE_GetValuesFromFieldForTaggedRecords == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_GB_AN_ENGINE_GetValuesFromFieldForTaggedRecords: %s",
		        dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GB_AN_ENGINE_GetValuesFromFieldForTaggedRecords loaded\n");

	//////////////////////////////
	// GB_AN_ENGINE_FreeStringArray
	//////////////////////////////
	Ptr_GB_AN_ENGINE_FreeStringArray = (int (*)(
			GB_AN_ENGINE_StringArray ValuesToFree,
			int numOfValuesToFree
	)) GetProcAddress(An2000DllPtr, "GB_AN_ENGINE_FreeStringArray");
	if (Ptr_GB_AN_ENGINE_FreeStringArray == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_GB_AN_ENGINE_FreeStringArray: %s",
		        dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GB_AN_ENGINE_FreeStringArray loaded\n");

	//////////////////////////////
	// GB_AN_ENGINE_GetValuesFromFieldForTaggedRecords
	//////////////////////////////
	Ptr_GB_AN_ENGINE_GetValuesFromFieldForTaggedRecords = (int (*)(
			FIELD *inField,
			GB_AN_ENGINE_StringArray *pValuesToGet,
			int *pNumOfValuesToGet
	)) GetProcAddress(An2000DllPtr, "GB_AN_ENGINE_GetValuesFromFieldForTaggedRecords");
	if (Ptr_GB_AN_ENGINE_GetValuesFromFieldForTaggedRecords == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_GB_AN_ENGINE_GetValuesFromFieldForTaggedRecords: %s",
		        dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GB_AN_ENGINE_GetValuesFromFieldForTaggedRecords loaded\n");

	//////////////////////////////
	// GB_AN_ENGINE_SearchFieldInTaggedRecord
	//////////////////////////////
	Ptr_GB_AN_ENGINE_SearchFieldInTaggedRecord = (int (*)(
			RECORD *recordToLookup,
			int FieldId,
			FIELD **FoundField
	)) GetProcAddress(An2000DllPtr, "GB_AN_ENGINE_SearchFieldInTaggedRecord");
	if (Ptr_GB_AN_ENGINE_SearchFieldInTaggedRecord == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_GB_AN_ENGINE_SearchFieldInTaggedRecord: %s",
		        dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GB_AN_ENGINE_SearchFieldInTaggedRecord loaded\n");

	//////////////////////////////
	// GB_AN_ENGINE_InsertRecordInAnsiNist
	//////////////////////////////
	Ptr_GB_AN_ENGINE_InsertRecordInAnsiNist = (int (*)(
			RECORD *RecordToBeInserted,
			ANSI_NIST *ansi_nist,
			const int RecordIndex,
			BOOL AutomaticIDC
	)) GetProcAddress(An2000DllPtr, "GB_AN_ENGINE_InsertRecordInAnsiNist");
	if (Ptr_GB_AN_ENGINE_InsertRecordInAnsiNist == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_GB_AN_ENGINE_InsertRecordInAnsiNist: %s",
		        dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GB_AN_ENGINE_InsertRecordInAnsiNist loaded\n");

	//////////////////////////////
	// GB_AN_ENGINE_Free
	//////////////////////////////
	Ptr_GB_AN_ENGINE_Free = (void (*)(
			ANSI_NIST *anStructToBeFreed,
			RECORD *recordToBeFreed,
			FIELD *fieldToBeFreed,
			SUBFIELD *subfieldToBeFreed,
			ITEM *itemToBeFreed
	)) GetProcAddress(An2000DllPtr, "GB_AN_ENGINE_Free");
	if (Ptr_GB_AN_ENGINE_Free == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_GB_AN_ENGINE_Free: %s",
		        dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GB_AN_ENGINE_Free loaded\n");

	//////////////////////////////
	// GB_AN_ENGINE_CreateEmptyAnsiNistStruct
	//////////////////////////////
	Ptr_GB_AN_ENGINE_CreateEmptyAnsiNistStruct = (int (*)(
			ANSI_NIST **pAnsinistToBeCreated
	)) GetProcAddress(An2000DllPtr, "GB_AN_ENGINE_CreateEmptyAnsiNistStruct");
	if (Ptr_GB_AN_ENGINE_CreateEmptyAnsiNistStruct == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_GB_AN_ENGINE_CreateEmptyAnsiNistStruct: %s",
		        dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GB_AN_ENGINE_CreateEmptyAnsiNistStruct loaded\n");

	//////////////////////////////
	// GB_AN_ENGINE_CreateBinaryRecord
	//////////////////////////////
	Ptr_GB_AN_ENGINE_CreateBinaryRecord = (int (*)(
			RECORD **pRecord,
			int recordType,
			int idc,
			int impressionType,
			unsigned char *fgp,
			int imageScanResolution,
			int horizontalNumberOfPixels,
			int verticalNumberOfPixels,
			int compressionAlgorithm,
			unsigned char *imageBuffer,
			int imageBufferLen
	)) GetProcAddress(An2000DllPtr, "GB_AN_ENGINE_CreateBinaryRecord");
	if (Ptr_GB_AN_ENGINE_CreateBinaryRecord == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_GB_AN_ENGINE_CreateBinaryRecord: %s",
		        dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GB_AN_ENGINE_CreateBinaryRecord loaded\n");

	//////////////////////////////
	// GB_AN_ENGINE_InsertFieldIntoTaggedRecord
	//////////////////////////////
	Ptr_GB_AN_ENGINE_InsertFieldIntoTaggedRecord = (int (*)(
			RECORD *recordToUpdate,
			FIELD *fieldToInsert
	)) GetProcAddress(An2000DllPtr, "GB_AN_ENGINE_InsertFieldIntoTaggedRecord");
	if (Ptr_GB_AN_ENGINE_InsertFieldIntoTaggedRecord == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_GB_AN_ENGINE_InsertFieldIntoTaggedRecord: %s",
		        dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GB_AN_ENGINE_InsertFieldIntoTaggedRecord loaded\n");

	//////////////////////////////
	// GB_AN_ENGINE_CreateEmptyTaggedRecord
	//////////////////////////////
	Ptr_GB_AN_ENGINE_CreateEmptyTaggedRecord = (int (*)(
			RECORD **outRecord,
			int recordType,
			int Idc
	)) GetProcAddress(An2000DllPtr, "GB_AN_ENGINE_CreateEmptyTaggedRecord");
	if (Ptr_GB_AN_ENGINE_CreateEmptyTaggedRecord == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_GB_AN_ENGINE_CreateEmptyTaggedRecord: %s",
		        dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GB_AN_ENGINE_CreateEmptyTaggedRecord loaded\n");

	//////////////////////////////
	// GB_AN_ENGINE_CreateImageFieldForTaggedRecord
	//////////////////////////////
	Ptr_GB_AN_ENGINE_CreateImageFieldForTaggedRecord = (int (*)(
			FIELD **outField,
			int recordType,
			unsigned char *imageBuffer,
			int imageBufferLen
	)) GetProcAddress(An2000DllPtr, "GB_AN_ENGINE_CreateImageFieldForTaggedRecord");
	if (Ptr_GB_AN_ENGINE_CreateImageFieldForTaggedRecord == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_GB_AN_ENGINE_CreateImageFieldForTaggedRecord: %s",
		        dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GB_AN_ENGINE_CreateImageFieldForTaggedRecord loaded\n");

	//////////////////////////////
	// GB_AN_ENGINE_CreateFieldFromValuesForTaggedRecords
	//////////////////////////////
	Ptr_GB_AN_ENGINE_CreateFieldFromValuesForTaggedRecords = (int (*)(
			FIELD **outField,
			int recordType,
			int fieldId,
			GB_AN_ENGINE_StringArray valuesToInsertArray,
			int numOfValuesToInsert
	)) GetProcAddress(An2000DllPtr, "GB_AN_ENGINE_CreateFieldFromValuesForTaggedRecords");
	if (Ptr_GB_AN_ENGINE_CreateFieldFromValuesForTaggedRecords == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_GB_AN_ENGINE_CreateFieldFromValuesForTaggedRecords: %s",
		        dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GB_AN_ENGINE_CreateFieldFromValuesForTaggedRecords loaded\n");

	//////////////////////////////
	// GB_AN_ENGINE_InsertSubfieldIntoFieldForTaggedRecords
	//////////////////////////////
	Ptr_GB_AN_ENGINE_InsertSubfieldIntoFieldForTaggedRecords = (int (*)(
			FIELD *fieldToUpdate,
			SUBFIELD *subfieldToInsert
	)) GetProcAddress(An2000DllPtr, "GB_AN_ENGINE_InsertSubfieldIntoFieldForTaggedRecords");
	if (Ptr_GB_AN_ENGINE_InsertSubfieldIntoFieldForTaggedRecords == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_GB_AN_ENGINE_InsertSubfieldIntoFieldForTaggedRecords: %s",
		        dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GB_AN_ENGINE_InsertSubfieldIntoFieldForTaggedRecords loaded\n");

	//////////////////////////////
	// GB_AN_ENGINE_CreateEmptyFieldForTaggedRecords
	//////////////////////////////
	Ptr_GB_AN_ENGINE_CreateEmptyFieldForTaggedRecords = (int (*)(
			FIELD **outField,
			int recordType,
			int fieldId
	)) GetProcAddress(An2000DllPtr, "GB_AN_ENGINE_CreateEmptyFieldForTaggedRecords");
	if (Ptr_GB_AN_ENGINE_CreateEmptyFieldForTaggedRecords == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_GB_AN_ENGINE_CreateEmptyFieldForTaggedRecords: %s",
		        dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GB_AN_ENGINE_CreateEmptyFieldForTaggedRecords loaded\n");

	//////////////////////////////
	// GB_AN_ENGINE_CreateSubFieldFromValuesForTaggedRecords
	//////////////////////////////
	Ptr_GB_AN_ENGINE_CreateSubFieldFromValuesForTaggedRecords = (int (*)(
			SUBFIELD **outSubField,
			GB_AN_ENGINE_StringArray valuesToInsertArray,
			int numOfValuesToInsert
	)) GetProcAddress(An2000DllPtr, "GB_AN_ENGINE_CreateSubFieldFromValuesForTaggedRecords");
	if (Ptr_GB_AN_ENGINE_CreateSubFieldFromValuesForTaggedRecords == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_GB_AN_ENGINE_CreateSubFieldFromValuesForTaggedRecords: %s",
		        dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GB_AN_ENGINE_CreateSubFieldFromValuesForTaggedRecords loaded\n");

	//////////////////////////////
	// GB_AN2011_GetType15RecordInfo
	//////////////////////////////
	Ptr_GB_AN2011_GetType15RecordInfo = (int (*)(
			RECORD *recordToRead,
			int *pIdc,
			int *pImpressionType,
			GB_AN_ENGINE_String *pSourceAgency,
			GB_AN2011_DateStruct *fingerprintCaptureDate,
			int *pHorizontalNumberOfPixels,
			int *pVerticalNumberOfPixels,
			int *pScaleUnits,
			int *pTransmittedHorizontalPixelScale,
			int *pTransmittedVerticalPixelScale,
			int *pCompressionAlgorithm,
			int *pBitsPerPixel,
			int *pFingerprintPosition,
			int *pScannedHorizontalPixelScale,
			int *pScannedVerticalPixelScale,
			GB_AN2011_AmputatedBandagedCodeStruct **pAmp, int *pAmpNum,
			GB_AN_ENGINE_String *pComment,
			GB_AN2011_FingerOrSegmentQualityMetricStruct **pPqm, int *pPqmNum,
			GB_AN_ENGINE_String *pDeviceMonitoringMode,
			unsigned char **pImageBuffer, int *pImageBufferLen
	)) GetProcAddress(An2000DllPtr, "GB_AN2011_GetType15RecordInfo");
	if (Ptr_GB_AN2011_GetType15RecordInfo == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_GB_AN2011_GetType15RecordInfo: %s", dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GB_AN2011_GetType15RecordInfo loaded\n");

	//////////////////////////////
	// GB_AN2011_CreateType15Record
	//////////////////////////////
	Ptr_GB_AN2011_CreateType15Record = (int (*)(
			RECORD **pRecord,
			int idc,
			int impressionType,
			GB_AN_ENGINE_String sourceAgency,
			GB_AN2011_DateStruct *palmprintCaptureDate,
			int horizontalNumberOfPixels,
			int verticalNumberOfPixels,
			int scaleUnits,
			int transmittedHorizontalPixelScale,
			int transmittedVerticalPixelScale,
			int compressionAlgorithm,
			int bitsPerPixel,
			int fingerprintPosition,
			int scannedHorizontalPixelScale,
			int scannedVerticalPixelScale,
			GB_AN2011_AmputatedBandagedCodeStruct *amp, int ampNum,
			GB_AN_ENGINE_String comment,
			GB_AN2011_FingerOrSegmentQualityMetricStruct *pqm, int pqmNum,
			GB_AN_ENGINE_String deviceMonitoringMode,
			unsigned char *imageBuffer, int imageBufferLen
	)) GetProcAddress(An2000DllPtr, "GB_AN2011_CreateType15Record");
	if (Ptr_GB_AN2011_CreateType15Record == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_GB_AN2011_CreateType15Record: %s", dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GB_AN2011_CreateType15Record loaded\n");

	//////////////////////////////
	// GB_AN2011_FreeAsegArray
	//////////////////////////////
	Ptr_GB_AN2011_FreeAsegArray = (void (*)(
			GB_AN2011_AlternateSegmentPositionStruct *asegArray, int asegNum
	)) GetProcAddress(An2000DllPtr, "GB_AN2011_FreeAsegArray");
	if (Ptr_GB_AN2011_FreeAsegArray == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_GB_AN2011_FreeAsegArray: %s", dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GB_AN2011_FreeAsegArray loaded\n");

	//////////////////////////////
	// GB_AN2011_GetType14RecordInfo
	//////////////////////////////
	Ptr_GB_AN2011_GetType14RecordInfo = (int (*)(
			RECORD *recordToRead,
			int *pIdc,
			int *pImpressionType,
			GB_AN_ENGINE_String *pSourceAgency,
			GB_AN2011_DateStruct *fingerprintCaptureDate,
			int *pHorizontalNumberOfPixels,
			int *pVerticalNumberOfPixels,
			int *pScaleUnits,
			int *pTransmittedHorizontalPixelScale,
			int *pTransmittedVerticalPixelScale,
			int *pCompressionAlgorithm,
			int *pBitsPerPixel,
			int *pFingerprintPosition,
			GB_AN2011_PrintPositionDescriptorsStruct *ppd,
			GB_AN2011_PrintPositionCoordinatesStruct **pPpc, int *pPpcNum,
			int *pScannedHorizontalPixelScale,
			int *pScannedVerticalPixelScale,
			GB_AN2011_AmputatedBandagedCodeStruct **pAmp, int *pAmpNum,
			GB_AN_ENGINE_String *pComment,
			GB_AN2011_FingerSegmentPositionStruct **pSeg, int *pSegNum,
			GB_AN2011_NistQualityMetricStruct **pNqm, int *pNqmNum,
			GB_AN2011_FingerOrSegmentQualityMetricStruct **pSqm, int *pSqmNum,
			GB_AN2011_FingerOrSegmentQualityMetricStruct **pFqm, int *pFqmNum,
			GB_AN2011_AlternateSegmentPositionStruct **pAseg, int *pAsegNum,
			int *pSimultaneousCaptureFingerprint,
			BOOL *pStitchedImageFlag,
			GB_AN_ENGINE_String *pDeviceMonitoringMode,
			int *pFingerprintAcquisitionProfile,
			unsigned char **pImageBuffer, int *pImageBufferLen
	)) GetProcAddress(An2000DllPtr, "GB_AN2011_GetType14RecordInfo");
	if (Ptr_GB_AN2011_GetType14RecordInfo == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_GB_AN2011_GetType14RecordInfo: %s", dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GB_AN2011_GetType14RecordInfo loaded\n");

	//////////////////////////////
	// GB_AN2011_CreateType14Record
	//////////////////////////////
	Ptr_GB_AN2011_CreateType14Record = (int (*)(
			RECORD **pRecord,
			int idc,
			int impressionType,
			GB_AN_ENGINE_String sourceAgency,
			GB_AN2011_DateStruct *fingerprintCaptureDate,
			int horizontalNumberOfPixels,
			int verticalNumberOfPixels,
			int scaleUnits,
			int transmittedHorizontalPixelScale,
			int transmittedVerticalPixelScale,
			int compressionAlgorithm,
			int bitsPerPixel,
			int fingerprintPosition,
			GB_AN2011_PrintPositionDescriptorsStruct *ppd,
			GB_AN2011_PrintPositionCoordinatesStruct *ppc, int ppcNum,
			int scannedHorizontalPixelScale,
			int scannedVerticalPixelScale,
			GB_AN2011_AmputatedBandagedCodeStruct *amp, int ampNum,
			GB_AN_ENGINE_String comment,
			GB_AN2011_FingerSegmentPositionStruct *seg, int segNum,
			GB_AN2011_NistQualityMetricStruct *nqm, int nqmNum,
			GB_AN2011_FingerOrSegmentQualityMetricStruct *sqm, int sqmNum,
			GB_AN2011_FingerOrSegmentQualityMetricStruct *fqm, int fqmNum,
			GB_AN2011_AlternateSegmentPositionStruct *aseg, int asegNum,
			int simultaneousCaptureFingerprint,
			BOOL stitchedImageFlag,
			GB_AN_ENGINE_String deviceMonitoringMode,
			int fingerprintAcquisitionProfile,
			unsigned char *imageBuffer, int imageBufferLen
	)) GetProcAddress(An2000DllPtr, "GB_AN2011_CreateType14Record");
	if (Ptr_GB_AN2011_CreateType14Record == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_GB_AN2011_CreateType14Record: %s", dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GB_AN2011_CreateType14Record loaded\n");

	//////////////////////////////
	// GB_AN2011_GetType4RecordInfo
	//////////////////////////////
	Ptr_GB_AN2011_GetType4RecordInfo = (int (*)(
			RECORD *recordToRead,
			int *pIdc,
			int *pImpressionType,
			unsigned char *fgp,
			int *pImageScanResolution,
			int *pHorizontalNumberOfPixels,
			int *pVerticalNumberOfPixels,
			int *pCompressionAlgorithm,
			unsigned char **pImageBuffer,
			int *pImageBufferLen
	)) GetProcAddress(An2000DllPtr, "GB_AN2011_GetType4RecordInfo");
	if (Ptr_GB_AN2011_GetType4RecordInfo == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_GB_AN2011_GetType4RecordInfo: %s", dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GB_AN2011_GetType4RecordInfo loaded\n");

	//////////////////////////////
	// GB_AN2011_CreateType4Record
	//////////////////////////////
	Ptr_GB_AN2011_CreateType4Record = (int (*)(
			RECORD **pRecord,
			int idc,
			int impressionType,
			unsigned char fgp,
			int imageScanResolution,
			int horizontalNumberOfPixels,
			int verticalNumberOfPixels,
			int compressionAlgorithm,
			unsigned char *imageBuffer,
			int imageBufferLen
	)) GetProcAddress(An2000DllPtr, "GB_AN2011_CreateType4Record");
	if (Ptr_GB_AN2011_CreateType4Record == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_GB_AN2011_CreateType4Record: %s", dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GB_AN2011_CreateType4Record loaded\n");

	//////////////////////////////
	// GB_AN_ENGINE_FreeGenericMem
	//////////////////////////////
	Ptr_GB_AN_ENGINE_FreeGenericMem = (void (*)(
			void *
	)) GetProcAddress(An2000DllPtr, "GB_AN_ENGINE_FreeGenericMem");
	if (Ptr_GB_AN_ENGINE_FreeGenericMem == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_GB_AN_ENGINE_FreeGenericMem: %s", dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GB_AN_ENGINE_FreeGenericMem loaded\n");

	//////////////////////////////
	// GB_AN2011_AvoidLeadingZeroes
	//////////////////////////////
	Ptr_GB_AN2011_AvoidLeadingZeroes = (void (*)(
			BOOL
	)) GetProcAddress(An2000DllPtr, "GB_AN2011_AvoidLeadingZeroes");
	if (Ptr_GB_AN2011_AvoidLeadingZeroes == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_GB_AN2011_AvoidLeadingZeroes: %s", dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GB_AN2011_AvoidLeadingZeroes loaded\n");

	Ptr_GB_AN2011_AvoidLeadingZeroes(TRUE);

	//////////////////////////////
	// GB_AN2011_GetType1RecordInfo
	//////////////////////////////
	Ptr_GB_AN2011_GetType1RecordInfo = (int (*)(
			RECORD *record,
			int *pVersion,
			GB_AN_ENGINE_String *pTypeOfTransaction,
			GB_AN2011_DateStruct *Dat,
			int *pPriority,
			GB_AN_ENGINE_String *pDestAgency,
			GB_AN_ENGINE_String *pOrigAgency,
			GB_AN_ENGINE_String *pTransContrNum,
			GB_AN_ENGINE_String *pTransContrRef,
			float *pNativeScanRes,
			float *pNominalTxRes,
			GB_AN_ENGINE_String *pDomainName,
			GB_AN_ENGINE_String *pDomainVersion,
			GB_AN2011_GmtStruct *Gmt,
			int *pCharEncIndex,
			GB_AN_ENGINE_String *pCharEncName,
			GB_AN_ENGINE_String *pCharEncVersion,
			GB_AN2011_ApplicationProfileStruct **pAps,
			int *pApsNum,
			GB_AN_ENGINE_String *pDestAgName,
			GB_AN_ENGINE_String *pOrigAgName
	)) GetProcAddress(An2000DllPtr, "GB_AN2011_GetType1RecordInfo");
	if (Ptr_GB_AN2011_GetType1RecordInfo == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_GB_AN2011_GetType1RecordInfo: %s", dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GB_AN2011_GetType1RecordInfo loaded\n");

	//////////////////////////////
	// delete_ANSI_NIST_record
	//////////////////////////////
	Ptr_AN2000_delete_ANSI_NIST_record = (int (*)(
			const int RecordIndex,
			ANSI_NIST *ansi_nist
	)) GetProcAddress(An2000DllPtr, "delete_ANSI_NIST_record");
	if (Ptr_AN2000_delete_ANSI_NIST_record == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_AN2000_delete_ANSI_NIST_record: %s", dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_AN2000_delete_ANSI_NIST_record loaded\n");

	//////////////////////////////
	// GB_AN2011_CreateAnsiNistStruct
	//////////////////////////////
	Ptr_GB_AN2011_CreateAnsiNistStruct = (int (*)(
			ANSI_NIST **pAnsiNist,
			GB_AN_ENGINE_String Tot,
			GB_AN2011_DateStruct *Dat,
			int Pry,
			GB_AN_ENGINE_String DestAgency,
			GB_AN_ENGINE_String OrigAgency,
			GB_AN_ENGINE_String TransContrNum,
			GB_AN_ENGINE_String TransContrRef,
			float NativeScanRes,
			float NominalTxRes,
			GB_AN_ENGINE_String DomainName,
			GB_AN_ENGINE_String DomainVersion,
			GB_AN2011_GmtStruct *Gmt,
			int CharEncIndex,
			GB_AN_ENGINE_String CharEncName,
			GB_AN_ENGINE_String CharEncVersion,
			GB_AN2011_ApplicationProfileStruct *Aps,
			int ApsNum,
			GB_AN_ENGINE_String DestAgName,
			GB_AN_ENGINE_String OrigAgName
	)) GetProcAddress(An2000DllPtr, "GB_AN2011_CreateAnsiNistStruct");
	if (Ptr_GB_AN2011_CreateAnsiNistStruct == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_GB_AN2011_CreateAnsiNistStruct: %s", dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GB_AN2011_CreateAnsiNistStruct loaded\n");

	//////////////////////////////
	// free_ANSI_NIST_record
	//////////////////////////////
	Ptr_AN2000_free_ANSI_NIST_record = (void (*)(
			RECORD *
	)) GetProcAddress(An2000DllPtr, "free_ANSI_NIST_record");
	if (Ptr_AN2000_free_ANSI_NIST_record == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_AN2000_free_ANSI_NIST_record: %s", dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_AN2000_free_ANSI_NIST_record loaded\n");

	//////////////////////////////
	// read_ANSI_NIST_buffer
	//////////////////////////////
	Ptr_AN2000_read_ANSI_NIST_buffer = (int (*)(
			unsigned char *buffer,
			int bufLen,
			ANSI_NIST **oansi_nist
	)) GetProcAddress(An2000DllPtr, "read_ANSI_NIST_buffer");
	if (Ptr_AN2000_read_ANSI_NIST_buffer == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_AN2000_read_ANSI_NIST_buffer: %s", dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_AN2000_read_ANSI_NIST_buffer loaded\n");

	//////////////////////////////
	// insert_ANSI_NIST_record_ETFS
	//////////////////////////////
	Ptr_AN2000_insert_ANSI_NIST_record_ETFS = (int (*)(
			const int RecordIndex,
			RECORD *RecordToBeInserted,
			ANSI_NIST *ansi_nist
	)) GetProcAddress(An2000DllPtr, "insert_ANSI_NIST_record_ETFS");
	if (Ptr_AN2000_insert_ANSI_NIST_record_ETFS == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_AN2000_insert_ANSI_NIST_record_ETFS: %s", dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_AN2000_insert_ANSI_NIST_record_ETFS loaded\n");

	//////////////////////////////
	// free_ANSI_NIST_palm_qual_list
	//////////////////////////////
	Ptr_AN2000_free_ANSI_NIST_palm_qual_list = (void (*)(
			PALM_QUAL_LIST *
	)) GetProcAddress(An2000DllPtr, "free_ANSI_NIST_palm_qual_list");
	if (Ptr_AN2000_free_ANSI_NIST_palm_qual_list == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_AN2000_free_ANSI_NIST_palm_qual_list: %s", dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_AN2000_free_ANSI_NIST_palm_qual_list loaded\n");

	//////////////////////////////
	// Get_EFTStype15_Info
	//////////////////////////////
	Ptr_AN2000_Get_EFTStype15_Info = (int (*)(
			RECORD *record,                // input record
			unsigned char **ImagePtr,            // image frame: allocated by the library
			int *ImageSize,            // allocated memory for image frame (can be different from w x h)
			int *w,
			int *h,                    // image dimensions
			int *BitsPerPixel,        // bits per pixel
			double *Resolution_ppmm,    // resolution in pixel per mm
			char *ImageCompression,    // image compression: see definitions: pass a char[10]
			int *ImageImpression,    // image impression type (live-scan, roll, etc, see definitions)
			char *source_str,            // string identifying the originating source of the image, pass a char[44]
			char *comment,            // comment, pass a char[136]
			char *PalmPosition,        // string indicating the palm position. See definitions
			PALM_QUAL_LIST **PalmQualitiesList
	)) GetProcAddress(An2000DllPtr, "Get_EFTStype15_Info");
	if (Ptr_AN2000_Get_EFTStype15_Info == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_AN2000_Get_EFTStype15_Info: %s", dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_AN2000_Get_EFTStype15_Info loaded\n");

	//////////////////////////////
	// UpdateETFStype_15
	//////////////////////////////
	Ptr_AN2000_UpdateETFStype_15 = (int (*)(
			RECORD *orecord,
			unsigned char *PPImage,
			const int PPImageSize,
			const int PPImageWidth,
			const int PPImageHeight,
			const double PPImageResolutionPPMM,
			PALM_QUAL_LIST *PalmQualitiesList
	)) GetProcAddress(An2000DllPtr, "UpdateETFStype_15");
	if (Ptr_AN2000_UpdateETFStype_15 == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_AN2000_UpdateETFStype_15: %s", dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_AN2000_UpdateETFStype_15 loaded\n");

	//////////////////////////////
	// image2ETFStype_15
	//////////////////////////////
	Ptr_AN2000_image2ETFStype_15 = (int (*)(
			RECORD **orecord,
			unsigned char *PPImage,
			const int PPImageSize,
			const int PPImageWidth,
			const int PPImageHeight,
			const int BitsPerPixel,
			const double PPImageResolutionPPMM,
			char *CompressionType,
			const int ImpressionType,
			char *ImageSource,
			char *Comment,
			char *PalmPosition,
			PALM_QUAL_LIST *PalmQualitiesList
	)) GetProcAddress(An2000DllPtr, "image2ETFStype_15");
	if (Ptr_AN2000_image2ETFStype_15 == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_AN2000_image2ETFStype_15: %s", dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_AN2000_image2ETFStype_15 loaded\n");

	//////////////////////////////
	// Get_EFTStype4_Info
	//////////////////////////////
	Ptr_AN2000_Get_EFTStype4_Info = (int (*)(
			RECORD *record,                    // input record
			unsigned char **ImagePtr,                // image frame: allocated by the library
			int *ImageSize,                // allocated memory for image frame (can be different from w x h)
			int *ImageImpression,        // image impression type (live-scan, roll, etc, see definitions)
			int finger_position[6],        // int indicating the finger position. See definitions
			int *ImageResolution,        // the scan resolution of the fingerprint image. See definitions
			int *w,
			int *h,                        // image dimensions
			char *ImageCompression        // image compression: see definitions: pass a char[10]
	)) GetProcAddress(An2000DllPtr, "Get_EFTStype4_Info");
	if (Ptr_AN2000_Get_EFTStype4_Info == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_AN2000_Get_EFTStype4_Info: %s", dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_AN2000_Get_EFTStype4_Info loaded\n");

	//////////////////////////////
	// UpdateETFStype_4
	//////////////////////////////
	Ptr_AN2000_UpdateETFStype_4 = (int (*)(
			RECORD *orecord,
			unsigned char *FingerprintImage,
			const int FPImageSize,
			const int ScanResolutionID,
			const int FPImageWidth,
			const int FPImageHeight
	)) GetProcAddress(An2000DllPtr, "UpdateETFStype_4");
	if (Ptr_AN2000_UpdateETFStype_4 == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_AN2000_UpdateETFStype_4: %s", dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_AN2000_UpdateETFStype_4 loaded\n");

	//////////////////////////////
	// image2ETFStype_4
	//////////////////////////////
	Ptr_AN2000_image2ETFStype_4 = (int (*)(
			RECORD **orecord,
			unsigned char *FingerprintImage,
			const int FPImageSize,
			const int ImpressionType,
			const int FingerPosition,
			const int ScanResolutionID,
			const int FPImageWidth,
			const int FPImageHeight,
			const char *CompressionType
	)) GetProcAddress(An2000DllPtr, "image2ETFStype_4");
	if (Ptr_AN2000_image2ETFStype_4 == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_AN2000_image2ETFStype_4: %s", dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_AN2000_image2ETFStype_4 loaded\n");

	//////////////////////////////
	// UpdateType1415ScaleUnits
	//////////////////////////////
	Ptr_AN2000_UpdateType1415ScaleUnits = (int (*)(
			RECORD *recordToUpdate, int newScale
	)) GetProcAddress(An2000DllPtr, "UpdateType1415ScaleUnits");
	if (Ptr_AN2000_UpdateType1415ScaleUnits == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_AN2000_UpdateType1415ScaleUnits: %s", dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_AN2000_UpdateType1415ScaleUnits loaded\n");

	//////////////////////////////
	// Get_EFTStype14_Info
	//////////////////////////////
	Ptr_AN2000_Get_EFTStype14_Info = (int (*)(
			RECORD *record,                // input record
			unsigned char **ImagePtr,            // image frame: allocated by the library
			int *ImageSize,            // allocated memory for image frame (can be different from w x h)
			int *w,
			int *h,                    // image dimensions
			int *PixelDepth,            // bits per pixel
			double *Resolution_ppmm,    // resolution in pixel per mm
			char *ImageCompression,    // image compression: see definitions: pass a char[10]
			int *ImageImpression,    // image impression type (live-scan, roll, etc, see definitions)
			char *source_str,            // string identifying the originating source of the image,
			// pass a char[44]
			char *comment,            // comment, pass a char[136]
			char finger_position_list[6][3],    // list of strings indicating the finger position.
			// See definitions, pass a list of char[3]
			// composed of 6 items
			FING_AMP_LIST **pamp_list,            // list of missing fingers and amputation codes: allocated
			// by the library
			FOUR_FING_BOUND **pboundaries,        // boundaries of each of four (or two thumbs) fingers in a slap
			FOUR_FING_QUALITY **pqualities,            // list of NFIQ quality of each of four or two thumbs or single
			// fingers in a slap
			ALT_QUAL_LIST **psegqual,            // list of segmentation quality for fingers
			ALT_QUAL_LIST **paltqual
	)) GetProcAddress(An2000DllPtr, "Get_EFTStype14_Info");
	if (Ptr_AN2000_Get_EFTStype14_Info == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_AN2000_Get_EFTStype14_Info: %s", dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_AN2000_Get_EFTStype14_Info loaded\n");

	//////////////////////////////
	// UpdateETFStype_14
	//////////////////////////////
	Ptr_AN2000_UpdateETFStype_14 = (int (*)(
			RECORD *orecord,
			unsigned char *FingerprintImage,
			const int FPImageSize,
			const int FPImageWidth,
			const int FPImageHeight,
			const double ScanResolutionPPMM,
			FING_AMP_LIST *amp_list,
			FOUR_FING_BOUND *boundaries,
			FOUR_FING_QUALITY *qualities,
			ALT_QUAL_LIST *segqual,
			ALT_QUAL_LIST *altqual
	)) GetProcAddress(An2000DllPtr, "UpdateETFStype_14");
	if (Ptr_AN2000_UpdateETFStype_14 == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_AN2000_UpdateETFStype_14: %s", dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_AN2000_UpdateETFStype_14 loaded\n");

	//////////////////////////////
	// Create_ANSI_NIST
	//////////////////////////////
	Ptr_AN2000_Create_ANSI_NIST = (int (*)(
			ANSI_NIST **oansi_nist,
			int Version,
			char *TypeOfTransaction,
			int Priority,
			char *DestinationAgency,
			char *OriginatingAgency,
			char *TransactionControlID,
			char *TransactionControlReference,
			float NominalScanningResolution,
			float NominalTransmitResolution,
			char *DomainNameID
	)) GetProcAddress(An2000DllPtr, "Create_ANSI_NIST");
	if (Ptr_AN2000_Create_ANSI_NIST == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_AN2000_Create_ANSI_NIST: %s", dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_AN2000_Create_ANSI_NIST loaded\n");

	//////////////////////////////
	// GetEFTSType1_Info
	//////////////////////////////
	Ptr_AN2000_GetEFTSType1_Info = (int (*)(
			RECORD *record,
			int *Version,                // version
			char *TransactionType,        // pass a char[12]
			int *Priority,                // priority
			char **pDestinationAgency,        // destination agency: no limits on length of this field, so
			// a developer needs next field:
			int *DestAgencyLen,            // DestinationAgency string length
			char **pOriginatingAgency,        // originating agency: no limits on length of this field, so
			// a developer needs next field:
			int *OriginatingAgencyLen,    // OriginatingAgency string length
			char **pTransContrNum,            // transaction control number: no limits on length of this field, so
			// a developer needs next field:
			int *TransContrNumLen,        // TransContrNum string length
			char **pTransContrRef,            // transaction control reference: no limits on length of this field, so
			// a developer needs next field:
			int *TransContrRefLen,        // TransContrRef string length
			float *NominalScanRes,            // Nominal Scanning Resolution
			float *NominalTXRes,            // Nominal Transmitting Resolution
			char **pDomain,                // Domain: no limits on length of this field, so
			// a developer needs next field:
			int *DomainLen
	)) GetProcAddress(An2000DllPtr, "GetEFTSType1_Info");
	if (Ptr_AN2000_GetEFTSType1_Info == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_AN2000_GetEFTSType1_Info: %s", dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_AN2000_GetEFTSType1_Info loaded\n");

	//////////////////////////////
	// UpdateETFStype_1
	//////////////////////////////
	Ptr_AN2000_UpdateETFStype_1 = (int (*)(
			RECORD *,
			float,
			float
	)) GetProcAddress(An2000DllPtr, "UpdateETFStype_1");
	if (Ptr_AN2000_UpdateETFStype_1 == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_AN2000_UpdateETFStype_1: %s", dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_AN2000_UpdateETFStype_1 loaded\n");

	//////////////////////////////
	// image2ETFStype_14
	//////////////////////////////
	Ptr_AN2000_image2ETFStype_14 = (int (*)(
			RECORD **orecord,
			unsigned char *FingerprintImage,
			const int FPImageSize,
			const int FPImageWidth,
			const int FPImageHeight,
			const int FPImagePixelDepth,
			const double ScanResolutionPPMM,
			char *CompressionType,
			const int ImpressionType,
			char *ImageSource,
			char *Comment,
			char *FingerPosition,
			FING_AMP_LIST *AmputationList,
			FOUR_FING_BOUND *boundaries,
			FOUR_FING_QUALITY *qualities,
			ALT_QUAL_LIST *segqual,
			ALT_QUAL_LIST *altqual
	)) GetProcAddress(An2000DllPtr, "image2ETFStype_14");
	if (Ptr_AN2000_image2ETFStype_14 == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_AN2000_image2ETFStype_14: %s", dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_AN2000_image2ETFStype_14 loaded\n");

	//////////////////////////////
	// free_ANSI_NIST
	//////////////////////////////
	Ptr_AN2000_free_ANSI_NIST = (void (*)(
			ANSI_NIST *
	)) GetProcAddress(An2000DllPtr, "free_ANSI_NIST");
	if (Ptr_AN2000_free_ANSI_NIST == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_AN2000_free_ANSI_NIST: %s", dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_AN2000_free_ANSI_NIST loaded\n");

	//////////////////////////////
	// free_ANSI_NIST_amp_list
	//////////////////////////////
	Ptr_AN2000_free_ANSI_NIST_amp_list = (void (*)(
			FING_AMP_LIST *
	)) GetProcAddress(An2000DllPtr, "free_ANSI_NIST_amp_list");
	if (Ptr_AN2000_free_ANSI_NIST_amp_list == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_AN2000_free_ANSI_NIST_amp_list: %s", dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_AN2000_free_ANSI_NIST_amp_list loaded\n");

	//////////////////////////////
	// free_FOUR_FING_BOUND
	//////////////////////////////
	Ptr_AN2000_free_FOUR_FING_BOUND = (void (*)(
			FOUR_FING_BOUND *
	)) GetProcAddress(An2000DllPtr, "free_FOUR_FING_BOUND");
	if (Ptr_AN2000_free_FOUR_FING_BOUND == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_AN2000_free_FOUR_FING_BOUND: %s", dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_AN2000_free_FOUR_FING_BOUND loaded\n");

	//////////////////////////////
	// free_FOUR_FING_QUALITY
	//////////////////////////////
	Ptr_AN2000_free_FOUR_FING_QUALITY = (void (*)(
			FOUR_FING_QUALITY *
	)) GetProcAddress(An2000DllPtr, "free_FOUR_FING_QUALITY");
	if (Ptr_AN2000_free_FOUR_FING_QUALITY == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_AN2000_free_FOUR_FING_QUALITY: %s", dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_AN2000_free_FOUR_FING_QUALITY loaded\n");

	//////////////////////////////
	// free_ANSI_NIST_alt_qual_list
	//////////////////////////////
	Ptr_AN2000_free_ANSI_NIST_alt_qual_list = (void (*)(
			ALT_QUAL_LIST *
	)) GetProcAddress(An2000DllPtr, "free_ANSI_NIST_alt_qual_list");
	if (Ptr_AN2000_free_ANSI_NIST_alt_qual_list == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_AN2000_free_ANSI_NIST_alt_qual_list: %s", dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_AN2000_free_ANSI_NIST_alt_qual_list loaded\n");

	//////////////////////////////
	// free_ANSI_NIST_buffer
	//////////////////////////////
	Ptr_AN2000_free_ANSI_NIST_buffer = (void (*)(
			void *
	)) GetProcAddress(An2000DllPtr, "free_ANSI_NIST_buffer");
	if (Ptr_AN2000_free_ANSI_NIST_buffer == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_AN2000_free_ANSI_NIST_buffer: %s", dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_AN2000_free_ANSI_NIST_buffer loaded\n");

	//////////////////////////////
	// write_ANSI_NIST_2_buffer
	//////////////////////////////
	Ptr_AN2000_write_ANSI_NIST_2_buffer = (int (*)(
			unsigned char **pOutBuffer,
			int *bufLen,
			const ANSI_NIST *ansi_nist
	)) GetProcAddress(An2000DllPtr, "write_ANSI_NIST_2_buffer");
	if (Ptr_AN2000_write_ANSI_NIST_2_buffer == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "Load: Unable to load Ptr_AN2000_write_ANSI_NIST_2_buffer: %s", dlerror());
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_AN2000_write_ANSI_NIST_2_buffer loaded\n");

	return AN2K_DLL_NO_ERROR;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_AnEngineInsertRecordIntoAnsiNistStruct(
		JNIEnv *env,
		jobject, /* this */
		jobject jAnsiNistToUpdate,
		jobject jRecordToInsert,
		jint jRecordIndex,
		jint jAutomaticIdc // boolean as int
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineInsertRecordIntoAnsiNistStruct: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GB_AN_ENGINE_InsertRecordInAnsiNist == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineInsertRecordIntoAnsiNistStruct: Ptr_GB_AN_ENGINE_InsertRecordInAnsiNist is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jAnsiNistToUpdate == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineInsertRecordIntoAnsiNistStruct: jAnsiNistToUpdate is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jRecordToInsert == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineInsertRecordIntoAnsiNistStruct: jRecordToInsert is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	RECORD *RecordToInsert;
	GBANUTIL_GetCRecordFromJRecord(jRecordToInsert, &RecordToInsert, env);

	ANSI_NIST *AnsiNistToUpdate;
	GBANUTIL_GetCAnsiNistFromJAnsiNist(jAnsiNistToUpdate, &AnsiNistToUpdate, env);

	//////////////////////////////////
	// call function
	//////////////////////////////////
	SUBFIELD *outSubfield = NULL;
	int RetVal = Ptr_GB_AN_ENGINE_InsertRecordInAnsiNist(
			RecordToInsert,
			AnsiNistToUpdate,
			(int) jRecordIndex,
			((jAutomaticIdc != 0) ? 1 : 0)
	);
	if (RetVal != AN2K_DLL_NO_ERROR)
	{
		ValToRet = RetVal;
		char ErrorString[100];
		GbAn2000Util_FormatErrorString(RetVal, ErrorString);
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineInsertRecordIntoAnsiNistStruct: Ptr_GB_AN_ENGINE_InsertRecordInAnsiNist returned %s (%d)",
		        ErrorString, RetVal);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
	}

	//////////////////////////////////
	// free memory
	//////////////////////////////////

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_AnEngineDeleteFieldFromTaggedRecordInAnsiNist(
		JNIEnv *env,
		jobject, /* this */
		jobject jAnsiNistToUpdate,
		jint jRecordIndex,
		jint jFieldId
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineDeleteFieldFromTaggedRecordInAnsiNist: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GB_AN_ENGINE_DeleteFieldFromTaggedRecordInAnsiNist == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineDeleteFieldFromTaggedRecordInAnsiNist: Ptr_GB_AN_ENGINE_DeleteFieldFromTaggedRecordInAnsiNist is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jAnsiNistToUpdate == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineDeleteFieldFromTaggedRecordInAnsiNist: jRecordToUpdate is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////

	ANSI_NIST *AnsiNistToUpdate;
	GBANUTIL_GetCAnsiNistFromJAnsiNist(jAnsiNistToUpdate, &AnsiNistToUpdate, env);

	//////////////////////////////////
	// call function
	//////////////////////////////////
	int RetVal = Ptr_GB_AN_ENGINE_DeleteFieldFromTaggedRecordInAnsiNist(
			AnsiNistToUpdate,
			(int) jRecordIndex,
			(int) jFieldId
	);
	if (RetVal != AN2K_DLL_NO_ERROR)
	{
		ValToRet = RetVal;
		char ErrorString[100];
		GbAn2000Util_FormatErrorString(RetVal, ErrorString);
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineDeleteFieldFromTaggedRecordInAnsiNist: Ptr_GB_AN_ENGINE_DeleteFieldFromTaggedRecordInAnsiNist returned %s (%d)",
		        ErrorString, RetVal);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
	}

	//////////////////////////////////
	// free memory
	//////////////////////////////////

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_AnEngineDeleteFieldFromTaggedRecord(
		JNIEnv *env,
		jobject, /* this */
		jobject jRecordToUpdate,
		jint jFieldId
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "DeleteFieldFromTaggedRecord: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GB_AN_ENGINE_DeleteFieldFromTaggedRecord == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "DeleteFieldFromTaggedRecord: Ptr_GB_AN_ENGINE_DeleteFieldFromTaggedRecord is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jRecordToUpdate == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "DeleteFieldFromTaggedRecord: jRecordToUpdate is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////

	RECORD *RecordToUpdate;
	GBANUTIL_GetCRecordFromJRecord(jRecordToUpdate, &RecordToUpdate, env);

	//////////////////////////////////
	// call function
	//////////////////////////////////
	int RetVal = Ptr_GB_AN_ENGINE_DeleteFieldFromTaggedRecord(
			RecordToUpdate,
			(int) jFieldId
	);
	if (RetVal != AN2K_DLL_NO_ERROR)
	{
		ValToRet = RetVal;
		char ErrorString[100];
		GbAn2000Util_FormatErrorString(RetVal, ErrorString);
		sprintf(An2000_JNI_LastErrorString,
		        "DeleteFieldFromTaggedRecord: Ptr_GB_AN_ENGINE_DeleteFieldFromTaggedRecord returned %s (%d)",
		        ErrorString, RetVal);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
	}

	//////////////////////////////////
	// free memory
	//////////////////////////////////

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_AnEngineInsertFieldIntoTaggedRecord(
		JNIEnv *env,
		jobject, /* this */
		jobject jRecordToUpdate,
		jobject jFieldToInsert
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineInsertFieldIntoTaggedRecord: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GB_AN_ENGINE_InsertFieldIntoTaggedRecord == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineInsertFieldIntoTaggedRecord: Ptr_GB_AN_ENGINE_InsertFieldIntoTaggedRecord is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jRecordToUpdate == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineInsertFieldIntoTaggedRecord: jRecordToUpdate is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFieldToInsert == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineInsertFieldIntoTaggedRecord: jFieldToInsert is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	FIELD *FieldToInsert;
	GBANUTIL_GetCFieldFromJField(jFieldToInsert, &FieldToInsert, env);

	RECORD *RecordToUpdate;
	GBANUTIL_GetCRecordFromJRecord(jRecordToUpdate, &RecordToUpdate, env);

	//////////////////////////////////
	// call function
	//////////////////////////////////
	SUBFIELD *outSubfield = NULL;
	int RetVal = Ptr_GB_AN_ENGINE_InsertFieldIntoTaggedRecord(
			RecordToUpdate,
			FieldToInsert
	);
	if (RetVal != AN2K_DLL_NO_ERROR)
	{
		ValToRet = RetVal;
		char ErrorString[100];
		GbAn2000Util_FormatErrorString(RetVal, ErrorString);
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineInsertFieldIntoTaggedRecord: Ptr_GB_AN_ENGINE_InsertFieldIntoTaggedRecord returned %s (%d)",
		        ErrorString, RetVal);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
	}

	//////////////////////////////////
	// free memory
	//////////////////////////////////

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_AnEngineInsertSubfieldIntoFieldForTaggedRecords(
		JNIEnv *env,
		jobject, /* this */
		jobject jFieldToUpdate,
		jobject jSubfieldToInsert
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineInsertSubfieldIntoFieldForTaggedRecords: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GB_AN_ENGINE_InsertSubfieldIntoFieldForTaggedRecords == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineInsertSubfieldIntoFieldForTaggedRecords: Ptr_GB_AN_ENGINE_InsertSubfieldIntoFieldForTaggedRecords is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jFieldToUpdate == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineInsertSubfieldIntoFieldForTaggedRecords: jFieldToUpdate is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jSubfieldToInsert == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineInsertSubfieldIntoFieldForTaggedRecords: jSubfieldToInsert is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	FIELD *FieldToUpdate;
	GBANUTIL_GetCFieldFromJField(jFieldToUpdate, &FieldToUpdate, env);

	SUBFIELD *SubfieldToInsert;
	GBANUTIL_GetCSubfieldFromJSubfield(jSubfieldToInsert, &SubfieldToInsert, env);

	//////////////////////////////////
	// call function
	//////////////////////////////////
	SUBFIELD *outSubfield = NULL;
	int RetVal = Ptr_GB_AN_ENGINE_InsertSubfieldIntoFieldForTaggedRecords(
			FieldToUpdate,
			SubfieldToInsert
	);
	if (RetVal != AN2K_DLL_NO_ERROR)
	{
		ValToRet = RetVal;
		char ErrorString[100];
		GbAn2000Util_FormatErrorString(RetVal, ErrorString);
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineInsertSubfieldIntoFieldForTaggedRecords: Ptr_GB_AN_ENGINE_InsertSubfieldIntoFieldForTaggedRecords returned %s (%d)",
		        ErrorString, RetVal);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
	}

	//////////////////////////////////
	// free memory
	//////////////////////////////////

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_AnEngineCreateFieldFromValuesForTaggedRecords(
		JNIEnv *env,
		jobject, /* this */
		jobject jField,
		jint jRecordType,
		jint jFieldId,
		jobjectArray jValuesArray // String []
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineCreateFieldFromValuesForTaggedRecords: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GB_AN_ENGINE_CreateFieldFromValuesForTaggedRecords == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineCreateFieldFromValuesForTaggedRecords: Ptr_GB_AN_ENGINE_CreateFieldFromValuesForTaggedRecords is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jField == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineCreateFieldFromValuesForTaggedRecords: jSubfield is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jValuesArray == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineCreateFieldFromValuesForTaggedRecords: jValuesArray is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetArrayLength(jValuesArray) <= 0)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineCreateFieldFromValuesForTaggedRecords: jValuesArray size is <= 0");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	GB_AN_ENGINE_StringArray valuesToInsertArray;
	valuesToInsertArray = (GB_AN_ENGINE_StringArray) malloc(
			sizeof(GB_AN_ENGINE_String) * env->GetArrayLength(jValuesArray));
	for (int i = 0; i < env->GetArrayLength(jValuesArray); i++)
	{
		GB_AN_ENGINE_String value;
		jstring item = (jstring) env->GetObjectArrayElement(jValuesArray, i);
		GBANUTIL_GetStringFromJString(item, &value, env);
		valuesToInsertArray[i] = value;
	}
	//////////////////////////////////
	// call function
	//////////////////////////////////
	FIELD *outField = NULL;
	int RetVal = Ptr_GB_AN_ENGINE_CreateFieldFromValuesForTaggedRecords(
			&outField,
			(int) jRecordType, (int) jFieldId,
			valuesToInsertArray,
			env->GetArrayLength(jValuesArray)
	);
	if (RetVal != AN2K_DLL_NO_ERROR)
	{
		ValToRet = RetVal;
		char ErrorString[100];
		GbAn2000Util_FormatErrorString(RetVal, ErrorString);
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineCreateFieldFromValuesForTaggedRecords: Ptr_GB_AN_ENGINE_CreateFieldFromValuesForTaggedRecords returned %s (%d)",
		        ErrorString, RetVal);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		GBANUTIL_GetJFieldFromCField(NULL, jField, env);
	}
	else
	{
		GBANUTIL_GetJFieldFromCField(outField, jField, env);
	}

	//////////////////////////////////
	// free memory
	//////////////////////////////////
	for (int i = 0; i < env->GetArrayLength(jValuesArray); i++)
	{
		free(valuesToInsertArray[i]);
	}
	free(valuesToInsertArray);

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_AnEngineCreateImageFieldForTaggedRecord(
		JNIEnv *env,
		jobject, /* this */
		jobject jField,
		jint jRecordType,
		jbyteArray jImageBuffer
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineCreateImageFieldForTaggedRecord: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GB_AN_ENGINE_CreateImageFieldForTaggedRecord == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineCreateImageFieldForTaggedRecord: Ptr_GB_AN_ENGINE_CreateImageFieldForTaggedRecord is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jField == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineCreateImageFieldForTaggedRecord: jSubfield is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jImageBuffer == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineCreateImageFieldForTaggedRecord: jImageBuffer is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetArrayLength(jImageBuffer) <= 0)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineCreateImageFieldForTaggedRecord: jImageBuffer size is <= 0");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	unsigned char *ImageBuffer = new unsigned char[env->GetArrayLength(jImageBuffer)];
	env->GetByteArrayRegion(jImageBuffer, 0,
	                        env->GetArrayLength(jImageBuffer),
	                        reinterpret_cast<jbyte *>(ImageBuffer));
	//////////////////////////////////
	// call function
	//////////////////////////////////
	FIELD *outField = NULL;
	int RetVal = Ptr_GB_AN_ENGINE_CreateImageFieldForTaggedRecord(
			&outField,
			(int) jRecordType,
			ImageBuffer,
			env->GetArrayLength(jImageBuffer)
	);
	if (RetVal != AN2K_DLL_NO_ERROR)
	{
		ValToRet = RetVal;
		char ErrorString[100];
		GbAn2000Util_FormatErrorString(RetVal, ErrorString);
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineCreateImageFieldForTaggedRecord: Ptr_GB_AN_ENGINE_CreateImageFieldForTaggedRecord returned %s (%d)",
		        ErrorString, RetVal);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		GBANUTIL_GetJFieldFromCField(NULL, jField, env);
	}
	else
	{
		GBANUTIL_GetJFieldFromCField(outField, jField, env);
	}

	//////////////////////////////////
	// free memory
	//////////////////////////////////
	delete[] ImageBuffer;

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_AnEngineCreateSubFieldFromValuesForTaggedRecords(
		JNIEnv *env,
		jobject, /* this */
		jobject jSubfield,
		jobjectArray jValuesArray // String []
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineCreateSubFieldFromValuesForTaggedRecords: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GB_AN_ENGINE_CreateSubFieldFromValuesForTaggedRecords == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineCreateSubFieldFromValuesForTaggedRecords: Ptr_GB_AN_ENGINE_CreateSubFieldFromValuesForTaggedRecords is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jSubfield == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineCreateSubFieldFromValuesForTaggedRecords: jSubfield is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jValuesArray == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineCreateSubFieldFromValuesForTaggedRecords: jValuesArray is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetArrayLength(jValuesArray) <= 0)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineCreateSubFieldFromValuesForTaggedRecords: jValuesArray size is <= 0");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	GB_AN_ENGINE_StringArray valuesToInsertArray;
	valuesToInsertArray = (GB_AN_ENGINE_StringArray) malloc(
			sizeof(GB_AN_ENGINE_String) * env->GetArrayLength(jValuesArray));
	for (int i = 0; i < env->GetArrayLength(jValuesArray); i++)
	{
		GB_AN_ENGINE_String value;
		jstring item = (jstring) env->GetObjectArrayElement(jValuesArray, i);
		GBANUTIL_GetStringFromJString(item, &value, env);
		valuesToInsertArray[i] = value;
	}
	//////////////////////////////////
	// call function
	//////////////////////////////////
	SUBFIELD *outSubfield = NULL;
	int RetVal = Ptr_GB_AN_ENGINE_CreateSubFieldFromValuesForTaggedRecords(
			&outSubfield,
			valuesToInsertArray,
			env->GetArrayLength(jValuesArray)
	);
	if (RetVal != AN2K_DLL_NO_ERROR)
	{
		ValToRet = RetVal;
		char ErrorString[100];
		GbAn2000Util_FormatErrorString(RetVal, ErrorString);
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineCreateSubFieldFromValuesForTaggedRecords: Ptr_GB_AN_ENGINE_CreateSubFieldFromValuesForTaggedRecords returned %s (%d)",
		        ErrorString, RetVal);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		GBANUTIL_GetJSubfieldFromCSubfield(NULL, jSubfield, env);
	}
	else
	{
		GBANUTIL_GetJSubfieldFromCSubfield(outSubfield, jSubfield, env);
	}

	//////////////////////////////////
	// free memory
	//////////////////////////////////
	for (int i = 0; i < env->GetArrayLength(jValuesArray); i++)
	{
		free(valuesToInsertArray[i]);
	}
	free(valuesToInsertArray);

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_AnEngineCreateEmptyTaggedRecord(
		JNIEnv *env,
		jobject, /* this */
		jobject jRecord,
		jint jRecordType,
		jint jIdc
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineCreateEmptyTaggedRecord: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GB_AN_ENGINE_CreateEmptyTaggedRecord == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineCreateEmptyTaggedRecord: Ptr_GB_AN_ENGINE_CreateEmptyTaggedRecord is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jRecord == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineCreateEmptyTaggedRecord: jSubfield is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////

	//////////////////////////////////
	// call function
	//////////////////////////////////
	RECORD *outRecord = NULL;
	int RetVal = Ptr_GB_AN_ENGINE_CreateEmptyTaggedRecord(
			&outRecord,
			jRecordType,
			jIdc
	);
	if (RetVal != AN2K_DLL_NO_ERROR)
	{
		ValToRet = RetVal;
		char ErrorString[100];
		GbAn2000Util_FormatErrorString(RetVal, ErrorString);
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineCreateEmptyTaggedRecord: Ptr_GB_AN_ENGINE_CreateEmptyTaggedRecord returned %s (%d)",
		        ErrorString, RetVal);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		GBANUTIL_GetJRecordFromCRecord(NULL, jRecord, env);
	}
	else
	{
		GBANUTIL_GetJRecordFromCRecord(outRecord, jRecord, env);
	}

	//////////////////////////////////
	// free memory
	//////////////////////////////////

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_AnEngineSearchFieldInTaggedRecord(
		JNIEnv *env,
		jobject, /* this */
		jobject jRecord,
		jint jFieldId,
		jobject jFoundField
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;
	int RetVal;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineSearchFieldInTaggedRecord: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GB_AN_ENGINE_SearchFieldInTaggedRecord == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineSearchFieldInTaggedRecord: Ptr_GB_AN_ENGINE_SearchFieldInTaggedRecord is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jRecord == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineSearchFieldInTaggedRecord: jRecord is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFoundField == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineSearchFieldInTaggedRecord: jFoundField is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check java class
	//////////////////////////////////

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	// record
	RECORD *record;
	GBANUTIL_GetCRecordFromJRecord(jRecord, &record, env);
	// found field
	FIELD *FoundField;

	//////////////////////////////////
	// call function
	//////////////////////////////////
	RetVal = Ptr_GB_AN_ENGINE_SearchFieldInTaggedRecord(
			record,
			(int) jFieldId,
			&FoundField
	);
	if (RetVal != AN2K_DLL_NO_ERROR)
	{
		ValToRet = RetVal;
		char ErrorString[100];
		GbAn2000Util_FormatErrorString(RetVal, ErrorString);
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineSearchFieldInTaggedRecord: Ptr_GB_AN_ENGINE_SearchFieldInTaggedRecord returned %s (%d)",
		        ErrorString,
		        RetVal);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		GBANUTIL_GetJFieldFromCField(NULL, jFoundField, env);
	}
	else
	{
		GBANUTIL_GetJFieldFromCField(FoundField, jFoundField, env);
	}

	//////////////////////////////////
	// free memory
	//////////////////////////////////

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_AnEngineCreateBinaryRecord(
		JNIEnv *env,
		jobject, /* this */
		jobject jRecord,
		jint jRecordType,
		jint jIdc,
		jint jImpressionType,
		jbyteArray jFingerPositionArray,
		jint jImageScanResolution,
		jint jHorizontalNumberOfPixels,
		jint jVerticalNumberOfPixels,
		jint jCompressionAlgorithm,
		jbyteArray jFingerprintImage
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;
	int RetVal;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineCreateBinaryRecord: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GB_AN_ENGINE_CreateBinaryRecord == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineCreateBinaryRecord: Ptr_GB_AN_ENGINE_CreateBinaryRecord is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jRecord == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineCreateBinaryRecord: jRecord is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFingerprintImage == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineCreateBinaryRecord: jFingerprintImage is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jHorizontalNumberOfPixels <= 0)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineCreateBinaryRecord: jHorizontalNumberOfPixels <= 0");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jVerticalNumberOfPixels <= 0)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineCreateBinaryRecord: jVerticalNumberOfPixels <= 0");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if ((env)->GetArrayLength(jFingerprintImage) <
	    (jHorizontalNumberOfPixels * jVerticalNumberOfPixels))
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineCreateBinaryRecord: jFingerprintImage too small");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFingerPositionArray == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineCreateBinaryRecord: jFingerPositionArray is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if ((env)->GetArrayLength(jFingerPositionArray) < 6)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineCreateBinaryRecord: jFingerPositionArray too small");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check java class
	//////////////////////////////////

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	// record
	RECORD *record;
	// image
	unsigned char *imageBuffer = new unsigned char[jHorizontalNumberOfPixels *
	                                               jVerticalNumberOfPixels];
	env->GetByteArrayRegion(jFingerprintImage, 0,
	                        jHorizontalNumberOfPixels * jVerticalNumberOfPixels,
	                        reinterpret_cast<jbyte *>(imageBuffer));
	// fgp
	unsigned char *fgp = new unsigned char[(env)->GetArrayLength(jFingerPositionArray)];
	env->GetByteArrayRegion(jFingerPositionArray, 0, (env)->GetArrayLength(jFingerPositionArray),
	                        reinterpret_cast<jbyte *>(fgp));

	//////////////////////////////////
	// call function
	//////////////////////////////////
	RetVal = Ptr_GB_AN_ENGINE_CreateBinaryRecord(
			&record,
			(int) jRecordType,
			(int) jIdc,
			(int) jImpressionType,
			fgp,
			(int) jImageScanResolution,
			(int) jHorizontalNumberOfPixels,
			(int) jVerticalNumberOfPixels,
			(int) jCompressionAlgorithm,
			imageBuffer, (int) (jHorizontalNumberOfPixels * jVerticalNumberOfPixels)
	);
	if (RetVal != AN2K_DLL_NO_ERROR)
	{
		ValToRet = RetVal;
		char ErrorString[100];
		GbAn2000Util_FormatErrorString(RetVal, ErrorString);
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineCreateBinaryRecord: Ptr_GB_AN_ENGINE_CreateBinaryRecord returned %s (%d)",
		        ErrorString,
		        RetVal);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		GBANUTIL_GetJRecordFromCRecord(NULL, jRecord, env);
	}
	else
	{
		GBANUTIL_GetJRecordFromCRecord(record, jRecord, env);
	}

	//////////////////////////////////
	// free memory
	//////////////////////////////////
	free(imageBuffer);
	free(fgp);

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_AnEngineCreateEmptyAnsiNist(
		JNIEnv *env,
		jobject, /* this */
		jobject jAnsiNist
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineCreateEmptyAnsiNist: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GB_AN_ENGINE_CreateEmptyAnsiNistStruct == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineCreateEmptyAnsiNist: Ptr_GB_AN_ENGINE_CreateEmptyAnsiNistStruct is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (Ptr_GB_AN_ENGINE_Free == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineCreateEmptyAnsiNist: Ptr_GB_AN_ENGINE_Free is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jAnsiNist == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineCreateEmptyAnsiNist: jAnsiNist is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check java class
	//////////////////////////////////

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////

	//////////////////////////////////
	// call function
	//////////////////////////////////
	ANSI_NIST *ansiNist = NULL;
	int RetVal = Ptr_GB_AN_ENGINE_CreateEmptyAnsiNistStruct(
			&ansiNist
	);
	if (RetVal != AN2K_DLL_NO_ERROR)
	{
		ValToRet = RetVal;
		char ErrorString[100];
		GbAn2000Util_FormatErrorString(RetVal, ErrorString);
		sprintf(An2000_JNI_LastErrorString,
		        "CreateAnsiNist: Ptr_GB_AN_ENGINE_CreateEmptyAnsiNistStruct returned %s (%d)",
		        ErrorString,
		        RetVal);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		GBANUTIL_GetJAnsiNistFromCAnsiNist(NULL, jAnsiNist, env);
	}
	else
	{
		GBANUTIL_GetJAnsiNistFromCAnsiNist(ansiNist, jAnsiNist, env);
	}

	//////////////////////////////////
	// free memory
	//////////////////////////////////
	if (ansiNist != NULL) Ptr_GB_AN_ENGINE_Free(ansiNist, NULL, NULL, NULL, NULL);

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_AnEngineGetValuesFromSubfieldForTaggedRecords(
		JNIEnv *env,
		jobject, /* this */
		jobject jSubfield,
		jobject jValues // GBJavaWrapperUtilStringArrayForJavaToCExchange
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetValuesFromSubfieldForTaggedRecords: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GB_AN_ENGINE_GetValuesFromSubfieldForTaggedRecords == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetValuesFromSubfieldForTaggedRecords: Ptr_GB_AN_ENGINE_GetValuesFromSubfieldForTaggedRecords is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jSubfield == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetValuesFromSubfieldForTaggedRecords: jSubfield is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jValues == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetValuesFromSubfieldForTaggedRecords: jValues is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	SUBFIELD *inField;
	GBANUTIL_GetCSubfieldFromJSubfield(jSubfield, &inField, env);

	//////////////////////////////////
	// call function
	//////////////////////////////////
	GB_AN_ENGINE_StringArray Values;
	int ValuesNum;

	int RetVal = Ptr_GB_AN_ENGINE_GetValuesFromSubfieldForTaggedRecords(
			inField,
			&Values,
			&ValuesNum
	);
	if (RetVal != AN2K_DLL_NO_ERROR)
	{
		ValToRet = RetVal;
		char ErrorString[100];
		GbAn2000Util_FormatErrorString(RetVal, ErrorString);
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetValuesFromSubfieldForTaggedRecords: Ptr_GB_AN_ENGINE_GetValuesFromSubfieldForTaggedRecords returned %s (%d)",
		        ErrorString, RetVal);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		GBANUTIL_GetJStringArrayExchangeFromCStringArray(NULL, 0, jValues, env);
	}
	else
	{
		GBANUTIL_GetJStringArrayExchangeFromCStringArray(Values, ValuesNum, jValues, env);
	}

	//////////////////////////////////
	// free memory
	//////////////////////////////////
	if (Values != NULL) Ptr_GB_AN_ENGINE_FreeStringArray(Values, ValuesNum);

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_AnEngineGetSubfieldsListFromField(
		JNIEnv *env,
		jobject, /* this */
		jobject jField,
		jobjectArray jListOfSubields // int
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;

	//////////////////////////////////
	// check functions
	//////////////////////////////////

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jField == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetSubfieldsListFromField: jField is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jListOfSubields == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetSubfieldsListFromField: jListOfSubields is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check java class
	//////////////////////////////////

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	FIELD *field;
	GBANUTIL_GetCFieldFromJField(jField, &field, env);
	if (field == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetSubfieldsListFromField: field struct contains a NULL pointer");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetArrayLength(jListOfSubields) < field->num_subfields)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetSubfieldsListFromField: size of jListOfSubields (%d) is too small, should be >= %d",
		        env->GetArrayLength(jListOfSubields),
		        field->num_subfields);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}


	//////////////////////////////////
	// call function
	//////////////////////////////////

	//////////////////////////////////
	// copy parameters
	//////////////////////////////////
	for (int i = 0; i < field->num_subfields; i++)
	{
		jobject item = env->GetObjectArrayElement(jListOfSubields, i);
		SUBFIELD *subfield = field->subfields[i];
		GBANUTIL_GetJSubfieldFromCSubfield(subfield, item, env);
	}

	//////////////////////////////////
	// free memory
	//////////////////////////////////

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_AnEngineGetFieldsListFromRecord(
		JNIEnv *env,
		jobject, /* this */
		jobject jRecord,
		jobjectArray jListOfFields // int
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;

	//////////////////////////////////
	// check functions
	//////////////////////////////////

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jRecord == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetFieldsListFromRecord: jRecord is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jListOfFields == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetFieldsListFromRecord: jListOfFields is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check java class
	//////////////////////////////////

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	RECORD *Record;
	GBANUTIL_GetCRecordFromJRecord(jRecord, &Record, env);
	if (Record == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetFieldsListFromRecord: Record struct contains a NULL pointer");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetArrayLength(jListOfFields) < Record->num_fields)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetFieldsListFromRecord: size of jListOfFields (%d) is too small, should be >= %d",
		        env->GetArrayLength(jListOfFields),
		        Record->num_fields);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}


	//////////////////////////////////
	// call function
	//////////////////////////////////

	//////////////////////////////////
	// copy parameters
	//////////////////////////////////
	for (int i = 0; i < Record->num_fields; i++)
	{
		jobject item = env->GetObjectArrayElement(jListOfFields, i);
		FIELD *field = Record->fields[i];
		GBANUTIL_GetJFieldFromCField(field, item, env);
	}

	//////////////////////////////////
	// free memory
	//////////////////////////////////

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_AnEngineGetRecordsListFromAnsiNist(
		JNIEnv *env,
		jobject, /* this */
		jobject jAnsiNist,
		jobjectArray jListOfRecords // int
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;

	//////////////////////////////////
	// check functions
	//////////////////////////////////

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jAnsiNist == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetRecordsListFromAnsiNist: jAnsiNist is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jListOfRecords == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetRecordsListFromAnsiNist: jVersion is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check java class
	//////////////////////////////////

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	ANSI_NIST *Ansinist;
	GBANUTIL_GetCAnsiNistFromJAnsiNist(jAnsiNist, &Ansinist, env);
	if (Ansinist == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetRecordsListFromAnsiNist: Ansinist struct contains a NULL pointer");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetArrayLength(jListOfRecords) < Ansinist->num_records)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetRecordsListFromAnsiNist: size of jListOfRecords (%d) is too small, should be >= %d",
		        env->GetArrayLength(jListOfRecords),
		        Ansinist->num_records);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}


	//////////////////////////////////
	// call function
	//////////////////////////////////

	//////////////////////////////////
	// copy parameters
	//////////////////////////////////
	for (int i = 0; i < Ansinist->num_records; i++)
	{
		jobject item = env->GetObjectArrayElement(jListOfRecords, i);
		RECORD *record = Ansinist->records[i];
		GBANUTIL_GetJRecordFromCRecord(record, item, env);
	}

	//////////////////////////////////
	// free memory
	//////////////////////////////////

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_AnEngineGetAnsiNistVersionAndNumOfRecords(
		JNIEnv *env,
		jobject, /* this */
		jobject jAnsiNist,
		jobject jVersion, // int
		jobject jNumRecords // int
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;

	//////////////////////////////////
	// check functions
	//////////////////////////////////

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jAnsiNist == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetAnsiNistVersion: jAnsiNist is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jVersion == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetAnsiNistVersion: jVersion is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jNumRecords == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetAnsiNistVersion: jNumRecords is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check java class
	//////////////////////////////////

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	ANSI_NIST *Ansinist;
	GBANUTIL_GetCAnsiNistFromJAnsiNist(jAnsiNist, &Ansinist, env);
	if (Ansinist == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetAnsiNistVersion: Ansinist struct contains a NULL pointer");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}


	//////////////////////////////////
	// call function
	//////////////////////////////////

	//////////////////////////////////
	// copy parameters
	//////////////////////////////////
	GBANUTIL_GetJIntExchangeFromCInt((int) Ansinist->version, jVersion, env);
	GBANUTIL_GetJIntExchangeFromCInt((int) Ansinist->num_records, jNumRecords, env);

	//////////////////////////////////
	// free memory
	//////////////////////////////////

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_AnEngineGetRecordTypeAndNumFields(
		JNIEnv *env,
		jobject, /* this */
		jobject jRecord,
		jobject jType, // int
		jobject jNumFields // int
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;

	//////////////////////////////////
	// check functions
	//////////////////////////////////

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jRecord == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetRecordTypeAndNumFields: jRecord is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jType == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetRecordTypeAndNumFields: jType is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jNumFields == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetRecordTypeAndNumFields: jNumFields is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check java class
	//////////////////////////////////

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	RECORD *Record;
	GBANUTIL_GetCRecordFromJRecord(jRecord, &Record, env);
	if (Record == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetRecordTypeAndNumFields: Record struct contains a NULL pointer");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}


	//////////////////////////////////
	// call function
	//////////////////////////////////

	//////////////////////////////////
	// copy parameters
	//////////////////////////////////
	GBANUTIL_GetJIntExchangeFromCInt((int) Record->type, jType, env);
	GBANUTIL_GetJIntExchangeFromCInt((int) Record->num_fields, jNumFields, env);

	//////////////////////////////////
	// free memory
	//////////////////////////////////

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_AnEngineGetFieldGeneralInfo(
		JNIEnv *env,
		jobject, /* this */
		jobject jField,
		jobject jContainingRecordType, // int
		jobject jFieldId, // int
		jobject jNumSubfields // int
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;

	//////////////////////////////////
	// check functions
	//////////////////////////////////

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jField == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetFieldGeneralInfo: jField is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jContainingRecordType == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetFieldGeneralInfo: jContainingRecordType is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFieldId == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetFieldGeneralInfo: jFieldId is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jNumSubfields == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetFieldGeneralInfo: jNumSubfields is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check java class
	//////////////////////////////////

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	FIELD *field;
	GBANUTIL_GetCFieldFromJField(jField, &field, env);
	if (field == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetFieldGeneralInfo: field struct contains a NULL pointer");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}


	//////////////////////////////////
	// call function
	//////////////////////////////////

	//////////////////////////////////
	// copy parameters
	//////////////////////////////////
	GBANUTIL_GetJIntExchangeFromCInt((int) field->field_int, jFieldId, env);
	GBANUTIL_GetJIntExchangeFromCInt((int) field->num_subfields, jNumSubfields, env);
	GBANUTIL_GetJIntExchangeFromCInt((int) field->record_type, jContainingRecordType, env);

	//////////////////////////////////
	// free memory
	//////////////////////////////////

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_AnEngineGetBinaryRecordInfo(
		JNIEnv *env,
		jobject, /* this */
		jobject jRecord,
		jobject jRecordType, // int
		jobject jIdc, // int
		jobject jImpressionType, // int
		jobject jFgp, // GBJavaWrapperUtilByteArrayForJavaToCExchange
		jobject jImageScanResolution, // int
		jobject jHorizontalNumberOfPixels, // int
		jobject jVerticalNumberOfPixels, // int
		jobject jCompressionAlgorithm, // int
		jobject jImageBuffer // GBJavaWrapperUtilByteArrayForJavaToCExchange
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetBinaryRecordInfo: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GB_AN_ENGINE_GetBinaryRecordInfo == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetBinaryRecordInfo: Ptr_GB_AN_ENGINE_GetBinaryRecordInfo is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jRecord == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetBinaryRecordInfo: jRecord is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jRecordType == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetBinaryRecordInfo: jRecordType is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jIdc == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetBinaryRecordInfo: jIdc is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jImpressionType == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetBinaryRecordInfo: jImpressionType is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFgp == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetBinaryRecordInfo: jFgp is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jImageScanResolution == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetBinaryRecordInfo: jImageScanResolution is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jHorizontalNumberOfPixels == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetBinaryRecordInfo: jHorizontalNumberOfPixels is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jVerticalNumberOfPixels == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetBinaryRecordInfo: jVerticalNumberOfPixels is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jCompressionAlgorithm == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetBinaryRecordInfo: jCompressionAlgorithm is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jImageBuffer == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetBinaryRecordInfo: jImageBuffer is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	RECORD *record;
	GBANUTIL_GetCRecordFromJRecord(jRecord, &record, env);

	//////////////////////////////////
	// call function
	//////////////////////////////////
	unsigned char *ImageBuffer;
	int ImgLen;
	int RecordType, Idc, ImpressionType;
	unsigned char fgp[6];
	int ImageScanResolution, HorizontalNumberOfPixels, VerticalNumberOfPixels, CompressionAlgorithm;

	int RetVal = Ptr_GB_AN_ENGINE_GetBinaryRecordInfo(
			record,
			&RecordType, &Idc, &ImpressionType,
			fgp, &ImageScanResolution, &HorizontalNumberOfPixels, &VerticalNumberOfPixels,
			&CompressionAlgorithm,
			&ImageBuffer,
			&ImgLen
	);
	if (RetVal != AN2K_DLL_NO_ERROR)
	{
		ValToRet = RetVal;
		char ErrorString[100];
		GbAn2000Util_FormatErrorString(RetVal, ErrorString);
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetBinaryRecordInfo: Ptr_GB_AN_ENGINE_GetBinaryRecordInfo returned %s (%d)",
		        ErrorString, RetVal);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
	}
	else
	{
		GBANUTIL_GetJIntExchangeFromCInt(RecordType, jRecordType, env);
		GBANUTIL_GetJIntExchangeFromCInt(Idc, jIdc, env);
		GBANUTIL_GetJIntExchangeFromCInt(ImpressionType, jImpressionType, env);
		GBANUTIL_GetJIntExchangeFromCInt(ImageScanResolution, jImageScanResolution, env);
		GBANUTIL_GetJIntExchangeFromCInt(HorizontalNumberOfPixels, jHorizontalNumberOfPixels, env);
		GBANUTIL_GetJIntExchangeFromCInt(VerticalNumberOfPixels, jVerticalNumberOfPixels, env);
		GBANUTIL_GetJIntExchangeFromCInt(CompressionAlgorithm, jCompressionAlgorithm, env);
		GBANUTIL_GetJBufferFromCBuffer(fgp, 6, jFgp, env);
		GBANUTIL_GetJBufferFromCBuffer(ImageBuffer, ImgLen, jImageBuffer, env);
	}

	//////////////////////////////////
	// free memory
	//////////////////////////////////
	if (ImageBuffer != NULL) Ptr_GB_AN_ENGINE_FreeGenericMem(ImageBuffer);

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_AnEngineGetImageFieldFromTaggedRecord(
		JNIEnv *env,
		jobject, /* this */
		jobject jRecord,
		jobject jImageBuffer // GBJavaWrapperUtilByteArrayForJavaToCExchange
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetImageFieldFromTaggedRecord: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GB_AN_ENGINE_GetImageFieldFromTaggedRecord == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetImageFieldFromTaggedRecord: Ptr_GB_AN_ENGINE_GetImageFieldFromTaggedRecord is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jRecord == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetImageFieldFromTaggedRecord: jRecord is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jImageBuffer == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetImageFieldFromTaggedRecord: jImageBuffer is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	RECORD *record;
	GBANUTIL_GetCRecordFromJRecord(jRecord, &record, env);

	//////////////////////////////////
	// call function
	//////////////////////////////////
	unsigned char *ImageBuffer;
	int ImgLen;

	int RetVal = Ptr_GB_AN_ENGINE_GetImageFieldFromTaggedRecord(
			record,
			&ImageBuffer,
			&ImgLen
	);
	if (RetVal != AN2K_DLL_NO_ERROR)
	{
		ValToRet = RetVal;
		char ErrorString[100];
		GbAn2000Util_FormatErrorString(RetVal, ErrorString);
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetImageFieldFromTaggedRecord: Ptr_GB_AN_ENGINE_GetImageFieldFromTaggedRecord returned %s (%d)",
		        ErrorString, RetVal);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		GBANUTIL_GetJBufferFromCBuffer(NULL, 0, jImageBuffer, env);
	}
	else
	{
		GBANUTIL_GetJBufferFromCBuffer(ImageBuffer, ImgLen, jImageBuffer, env);
	}

	//////////////////////////////////
	// free memory
	//////////////////////////////////
	if (ImageBuffer != NULL) Ptr_GB_AN_ENGINE_FreeGenericMem(ImageBuffer);

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_AnEngineGetValuesFromFieldForTaggedRecords(
		JNIEnv *env,
		jobject, /* this */
		jobject jField,
		jobject jValues // GBJavaWrapperUtilStringArrayForJavaToCExchange
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetValuesFromFieldForTaggedRecords: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GB_AN_ENGINE_GetValuesFromFieldForTaggedRecords == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetValuesFromFieldForTaggedRecords: Ptr_GB_AN_ENGINE_GetValuesFromFieldForTaggedRecords is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jField == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetValuesFromFieldForTaggedRecords: jSubfield is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jValues == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineGetValuesFromFieldForTaggedRecords: jValues is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	FIELD *inField;
	GBANUTIL_GetCFieldFromJField(jField, &inField, env);

	//////////////////////////////////
	// call function
	//////////////////////////////////
	GB_AN_ENGINE_StringArray Values;
	int ValuesNum;

	int RetVal = Ptr_GB_AN_ENGINE_GetValuesFromFieldForTaggedRecords(
			inField,
			&Values,
			&ValuesNum
	);
	if (RetVal != AN2K_DLL_NO_ERROR)
	{
		ValToRet = RetVal;
		char ErrorString[100];
		GbAn2000Util_FormatErrorString(RetVal, ErrorString);
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineCreateEmptyFieldForTaggedRecords: Ptr_GB_AN_ENGINE_GetValuesFromFieldForTaggedRecords returned %s (%d)",
		        ErrorString, RetVal);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		GBANUTIL_GetJStringArrayExchangeFromCStringArray(NULL, 0, jValues, env);
	}
	else
	{
		GBANUTIL_GetJStringArrayExchangeFromCStringArray(Values, ValuesNum, jValues, env);
	}

	//////////////////////////////////
	// free memory
	//////////////////////////////////
	if (Values != NULL) Ptr_GB_AN_ENGINE_FreeStringArray(Values, ValuesNum);

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_AnEngineCreateEmptyFieldForTaggedRecords(
		JNIEnv *env,
		jobject, /* this */
		jobject jField,
		jint jRecordType,
		jint jFieldId
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineCreateEmptyFieldForTaggedRecords: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GB_AN_ENGINE_CreateEmptyFieldForTaggedRecords == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineCreateEmptyFieldForTaggedRecords: Ptr_GB_AN_ENGINE_CreateEmptyFieldForTaggedRecords is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jField == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineCreateEmptyFieldForTaggedRecords: jSubfield is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////

	//////////////////////////////////
	// call function
	//////////////////////////////////
	FIELD *outField = NULL;
	int RetVal = Ptr_GB_AN_ENGINE_CreateEmptyFieldForTaggedRecords(
			&outField,
			jRecordType,
			jFieldId
	);
	if (RetVal != AN2K_DLL_NO_ERROR)
	{
		ValToRet = RetVal;
		char ErrorString[100];
		GbAn2000Util_FormatErrorString(RetVal, ErrorString);
		sprintf(An2000_JNI_LastErrorString,
		        "AnEngineCreateEmptyFieldForTaggedRecords: Ptr_GB_AN_ENGINE_CreateEmptyFieldForTaggedRecords returned %s (%d)",
		        ErrorString, RetVal);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		GBANUTIL_GetJFieldFromCField(NULL, jField, env);
	}
	else
	{
		GBANUTIL_GetJFieldFromCField(outField, jField, env);
	}

	//////////////////////////////////
	// free memory
	//////////////////////////////////

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_CreateAnsiNist2011(
		JNIEnv *env,
		jobject, /* this */
		jobject jAnsiNist,
		jstring jTransactionType, // MANDATORY
		jobject jDate,
		jint jPriority,
		jstring jDestinationAgency, // MANDATORY
		jstring jOriginatingAgency, // MANDATORY
		jstring jTransactionControlID, // MANDATORY
		jstring jTransactionControlReference, // OPTIONAL, NULL to avoid
		jdouble jNativeScanResolution, // MANDATORY, 0 if no type-4 is present, else [1-99]
		jdouble jNominalTransmitResolution, // OPTIONAL, -1 to avoid, 0 if no type-4 is present, else [1-99]
		jstring jDomainNameID, // OPTIONAL, NULL to avoid
		jstring jDomainVersion, // OPTIONAL, NULL to avoid
		jobject jGmt, // OPTIONAL, NULL to avoid
		jint jCharEncIndex, // OPTIONAL, -1 to avoid, Values: 0, 2, 3, 4, [128-999]
		jstring jCharEncName, // OPTIONAL, NULL to avoid, but MUST be present if jCharEncIndex is valid
		jstring jCharEncVersion, // OPTIONAL, NULL to avoid
		jobjectArray jAps, // OPTIONAL, NULL to avoid
		jstring jDestAgName, // OPTIONAL, NULL to avoid
		jstring jOrigAgName // OPTIONAL, NULL to avoid
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateAnsiNist2011: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GB_AN2011_CreateAnsiNistStruct == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateAnsiNist2011: Ptr_GB_AN2011_CreateAnsiNistStruct is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jAnsiNist == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateAnsiNist2011: jAnsiNist is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jTransactionType == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateAnsiNist2011: jTransactionType is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetStringUTFLength(jTransactionType) > 16)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateAnsiNist2011: jTransactionType is too long (%d characters), max 16", env->GetStringUTFLength(jTransactionType));
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jDate == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateAnsiNist2011: jDate is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jDestinationAgency == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateAnsiNist2011: jDestinationAgency is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jOriginatingAgency == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateAnsiNist2011: jOriginatingAgency is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jTransactionControlID == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateAnsiNist2011: jTransactionControlID is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jNativeScanResolution < 0)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateAnsiNist2011: jNativeScanResolution < 0");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check java class
	//////////////////////////////////

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	// TypeOfTransaction
	char *TypeOfTransaction = NULL;
	GBANUTIL_GetStringFromJString(jTransactionType, &TypeOfTransaction, env);
	// Transaction Date
	GB_AN2011_DateStruct TransactionDate;
	GBAN2011UTIL_GetCDateFromJDate(env, jDate, &TransactionDate, An2000_JNI_LastErrorString);
	// DestinationAgency
	char *DestinationAgency = NULL;
	GBANUTIL_GetStringFromJString(jDestinationAgency, &DestinationAgency, env);
	// OriginatingAgency
	char *OriginatingAgency = NULL;
	GBANUTIL_GetStringFromJString(jOriginatingAgency, &OriginatingAgency, env);
	// TransactionControlID
	char *TransactionControlID = NULL;
	GBANUTIL_GetStringFromJString(jTransactionControlID, &TransactionControlID, env);
	// TransactionControlReference
	char *TransactionControlReference = NULL;
	GBANUTIL_GetStringFromJString(jTransactionControlReference, &TransactionControlReference, env);
	// DomainNameID
	char *DomainNameID = NULL;
	GBANUTIL_GetStringFromJString(jDomainNameID, &DomainNameID, env);
	// DomainNameID
	char *DomainVersion = NULL;
	GBANUTIL_GetStringFromJString(jDomainVersion, &DomainVersion, env);
	// GMT
	GB_AN2011_GmtStruct Gmt, *pGmt;
	if (jGmt != NULL)
	{
		GBAN2011UTIL_GetCGmtFromJGmt(env, jGmt, &Gmt, An2000_JNI_LastErrorString);
		pGmt = &Gmt;
	}
	else pGmt = NULL;
	// CharEncName
	char *CharEncName = NULL;
	GBANUTIL_GetStringFromJString(jCharEncName, &CharEncName, env);
	// CharEncVersion
	char *CharEncVersion = NULL;
	GBANUTIL_GetStringFromJString(jCharEncVersion, &CharEncVersion, env);
	// APS
	GB_AN2011_ApplicationProfileStruct *pAps;
	if (jAps != NULL)
	{
		GBAN2011UTIL_GetCAppProfileArrayFromJAppProfileArray(env, jAps, &pAps,
		                                                     An2000_JNI_LastErrorString);
	}
	else pAps = NULL;
	// DestAgName
	char *DestAgName = NULL;
	GBANUTIL_GetStringFromJString(jDestAgName, &DestAgName, env);
	// DestAgName
	char *OrigAgName = NULL;
	GBANUTIL_GetStringFromJString(jOrigAgName, &OrigAgName, env);

	//////////////////////////////////
	// call function
	//////////////////////////////////
	ANSI_NIST *ansiNist = NULL;
	int RetVal = Ptr_GB_AN2011_CreateAnsiNistStruct(
			&ansiNist,
			TypeOfTransaction,
			&TransactionDate,
			(int) jPriority,
			DestinationAgency,
			OriginatingAgency,
			TransactionControlID,
			TransactionControlReference,
			(float) jNativeScanResolution,
			(float) jNominalTransmitResolution,
			DomainNameID,
			DomainVersion,
			pGmt,
			(int) jCharEncIndex,
			CharEncName,
			CharEncVersion,
			pAps,
			(int) (env->GetArrayLength(jAps)),
			DestAgName,
			OrigAgName
	);
	if (RetVal != AN2K_DLL_NO_ERROR)
	{
		ValToRet = RetVal;
		char ErrorString[100];
		GbAn2000Util_FormatErrorString(RetVal, ErrorString);
		sprintf(An2000_JNI_LastErrorString,
		        "CreateAnsiNist2011: Ptr_GB_AN2011_CreateAnsiNistStruct returned %s (%d)",
		        ErrorString, RetVal);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		GBANUTIL_GetJAnsiNistFromCAnsiNist(NULL, jAnsiNist, env);
	}
	else
	{
		GBANUTIL_GetJAnsiNistFromCAnsiNist(ansiNist, jAnsiNist, env);
	}

	//////////////////////////////////
	// free memory
	//////////////////////////////////
	if (TypeOfTransaction != NULL) free(TypeOfTransaction);
	if (DestinationAgency != NULL) free(DestinationAgency);
	if (OriginatingAgency != NULL) free(OriginatingAgency);
	if (TransactionControlID != NULL) free(TransactionControlID);
	if (TransactionControlReference != NULL) free(TransactionControlReference);
	if (DomainNameID != NULL) free(DomainNameID);
	if (DomainVersion != NULL) free(DomainVersion);
	if (CharEncName != NULL) free(CharEncName);
	if (CharEncVersion != NULL) free(CharEncVersion);
	if (DestAgName != NULL) free(DestAgName);
	if (OrigAgName != NULL) free(OrigAgName);
	if (pAps != NULL) GBAN2011UTIL_FreeAppProfileArray(pAps);

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_FreeRecordAllocatedMemory(
		JNIEnv *env,
		jobject, /* this */
		//-------------------------
		// INPUT/OUTPUT
		//-------------------------
		jobject jRecord
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "FreeRecordAllocatedMemory: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_AN2000_free_ANSI_NIST_record == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "FreeRecordAllocatedMemory: Ptr_AN2000_free_ANSI_NIST_record is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jRecord == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "FreeRecordAllocatedMemory: jRecord is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check java class
	//////////////////////////////////

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	RECORD *Record;
	GBANUTIL_GetCRecordFromJRecord(jRecord, &Record, env);
	if (Record == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "FreeRecordAllocatedMemory: Record struct contains a NULL pointer");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// call function
	//////////////////////////////////
	Ptr_AN2000_free_ANSI_NIST_record(Record);
	Record = NULL;
	GBANUTIL_GetJRecordFromCRecord(Record, jRecord, env);

	//////////////////////////////////
	// free memory
	//////////////////////////////////

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_FreeAnsiNistAllocatedMemory(
		JNIEnv *env,
		jobject, /* this */
		//-------------------------
		// INPUT/OUTPUT
		//-------------------------
		jobject jAnsiNist
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "FreeAnsiNistAllocatedMemory: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_AN2000_free_ANSI_NIST == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "FreeAnsiNistAllocatedMemory: Ptr_AN2000_free_ANSI_NIST is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jAnsiNist == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "FreeAnsiNistAllocatedMemory: jAnsiNist is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check java class
	//////////////////////////////////

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	ANSI_NIST *AnsiNist;
	GBANUTIL_GetCAnsiNistFromJAnsiNist(jAnsiNist, &AnsiNist, env);
	if (AnsiNist == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "FreeAnsiNistAllocatedMemory: AnsiNist struct contains a NULL pointer");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// call function
	//////////////////////////////////
	Ptr_AN2000_free_ANSI_NIST(AnsiNist);
	AnsiNist = NULL;
	GBANUTIL_GetJAnsiNistFromCAnsiNist(AnsiNist, jAnsiNist, env);

	//////////////////////////////////
	// free memory
	//////////////////////////////////

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_ReadAnsiNistFromBuffer(
		JNIEnv *env,
		jobject, /* this */
		//-------------------------
		// INPUT
		//-------------------------
		jbyteArray jInputBuffer,
		//-------------------------
		// OUTPUT
		//-------------------------
		jobject jAnsiNist
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "ReadAnsiNistFromBuffer: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_AN2000_read_ANSI_NIST_buffer == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "ReadAnsiNistFromBuffer: Ptr_AN2000_read_ANSI_NIST_buffer is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jAnsiNist == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "ReadAnsiNistFromBuffer: jAnsiNist is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jInputBuffer == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "ReadAnsiNistFromBuffer: jInputBuffer is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check java class
	//////////////////////////////////

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	unsigned char *InputBuffer = new unsigned char[env->GetArrayLength(jInputBuffer)];
	env->GetByteArrayRegion(jInputBuffer, 0, env->GetArrayLength(jInputBuffer),
	                        reinterpret_cast<jbyte *>(InputBuffer));

	//////////////////////////////////
	// call function
	//////////////////////////////////
	ANSI_NIST *ansiNist = NULL;
	int RetVal = Ptr_AN2000_read_ANSI_NIST_buffer(
			InputBuffer,
			(int) (env->GetArrayLength(jInputBuffer)),
			&ansiNist
	);
	ValToRet = RetVal;
	if (RetVal != AN2K_DLL_NO_ERROR)
	{
		char ErrorString[100];
		GbAn2000Util_FormatErrorString(RetVal, ErrorString);
		sprintf(An2000_JNI_LastErrorString,
		        "ReadAnsiNistFromBuffer: Ptr_AN2000_read_ANSI_NIST_buffer returned %s (%d)",
		        ErrorString, RetVal);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		GBANUTIL_GetJAnsiNistFromCAnsiNist(NULL, jAnsiNist, env);
	}
	else
	{
		GBANUTIL_GetJAnsiNistFromCAnsiNist(ansiNist, jAnsiNist, env);
	}

	//////////////////////////////////
	// free memory
	//////////////////////////////////
	delete[] InputBuffer;

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_GetType15Info2011(
		JNIEnv *env,
		jobject, /* this */
		jobject jRecord, // Record
		jobject jIdc, // int
		jobject jImpressionType, // int
		jobject jSourceAgency, // string
		jobject jfingerprintCaptureDate, // GB_AN2011_DateStruct
		jobject jHorizontalNumberOfPixels, // int
		jobject jVerticalNumberOfPixels, // int
		jobject jScaleUnits, // int
		jobject jTransmittedHorizontalPixelScale, // int
		jobject jTransmittedVerticalPixelScale, // int
		jobject jCompressionAlgorithm, // int
		jobject jBitsPerPixel, // int
		jobject jFingerprintPosition, // int
		jobject jScannedHorizontalPixelScale, // int
		jobject jScannedVerticalPixelScale, // int
		jobjectArray jAmp, // GB_AN2011_AmputatedBandagedCodeStruct[5]
		jobject jAmpNum, // int
		jobject jComment, // string
		jobjectArray jSqm, // GB_AN2011_FingerOrSegmentQualityMetricStruct[5]
		jobject jSqmNum, // int
		jobject jDeviceMonitoringMode, // string
		jobject jImageBuffer // byte []
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;
	int RetVal;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info2011: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GB_AN2011_GetType15RecordInfo == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info2011: Ptr_GB_AN2011_GetType15RecordInfo is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jRecord == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info2011: jRecord is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jIdc == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info2011: jIdc is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jImpressionType == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info2011: jImpressionType is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jSourceAgency == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info2011: jSourceAgency is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jfingerprintCaptureDate == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info2011: jfingerprintCaptureDate is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jHorizontalNumberOfPixels == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info2011: jHorizontalNumberOfPixels is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jVerticalNumberOfPixels == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info2011: jVerticalNumberOfPixels is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jScaleUnits == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info2011: jScaleUnits is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jTransmittedHorizontalPixelScale == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info2011: jTransmittedHorizontalPixelScale is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jTransmittedVerticalPixelScale == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info2011: jTransmittedVerticalPixelScale is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jCompressionAlgorithm == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info2011: jCompressionAlgorithm is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jBitsPerPixel == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info2011: jBitsPerPixel is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFingerprintPosition == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info2011: jFingerprintPosition is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jScannedHorizontalPixelScale == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info2011: jScannedHorizontalPixelScale is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jScannedVerticalPixelScale == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info2011: jScannedVerticalPixelScale is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jAmp == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info2011: jAmp is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetArrayLength(jAmp) < 5)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info2011: jAmp length must be at least 5");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jAmpNum == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info2011: jAmpNum is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jComment == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info2011: jComment is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	// sqm
	if (jSqm == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info2011: jSqm is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetArrayLength(jSqm) < 5)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info2011: jSqm length must be at least 5");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jSqmNum == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info2011: jSqmNum is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jDeviceMonitoringMode == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info2011: jDeviceMonitoringMode is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jImageBuffer == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info2011: jImageBuffer is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check java class
	//////////////////////////////////

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	// record
	RECORD *record;
	GBANUTIL_GetCRecordFromJRecord(jRecord, &record, env);
	if (record == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info2011: Record stored into jRecord is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// call function
	//////////////////////////////////
	int Idc, ImpressionType;
	GB_AN_ENGINE_String SourceAgency;
	GB_AN2011_DateStruct FingerprintCaptureDate;
	int HorizontalNumberOfPixels, VerticalNumberOfPixels, ScaleUnits,
			TransmittedHorizontalPixelScale, TransmittedVerticalPixelScale,
			CompressionAlgorithm, BitsPerPixel, FingerprintPosition;
	int ScannedHorizontalPixelScale, ScannedVerticalPixelScale;
	GB_AN2011_AmputatedBandagedCodeStruct *Amp;
	int AmpNum;
	GB_AN_ENGINE_String Comment;
	GB_AN2011_FingerOrSegmentQualityMetricStruct *Sqm;
	int SqmNum;
	GB_AN_ENGINE_String DeviceMonitoringMode;
	unsigned char *ImageBuffer;
	int ImageBufferLen;
	RetVal = Ptr_GB_AN2011_GetType15RecordInfo(
			record,
			&Idc,
			&ImpressionType,
			&SourceAgency,
			&FingerprintCaptureDate,
			&HorizontalNumberOfPixels, &VerticalNumberOfPixels,
			&ScaleUnits,
			&TransmittedHorizontalPixelScale, &TransmittedVerticalPixelScale,
			&CompressionAlgorithm, &BitsPerPixel, &FingerprintPosition,
			&ScannedHorizontalPixelScale, &ScannedVerticalPixelScale,
			&Amp, &AmpNum,
			&Comment,
			&Sqm, &SqmNum,
			&DeviceMonitoringMode,
			&ImageBuffer,
			&ImageBufferLen
	);
	if (RetVal != AN2K_DLL_NO_ERROR)
	{
		ValToRet = RetVal;
		char ErrorString[100];
		GbAn2000Util_FormatErrorString(RetVal, ErrorString);
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info2011: Ptr_GB_AN2011_GetType15RecordInfo returned %s (%d)",
		        ErrorString,
		        RetVal);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// set output values
	//////////////////////////////////
	GBANUTIL_GetJIntExchangeFromCInt(Idc, jIdc, env);
	GBANUTIL_GetJIntExchangeFromCInt(ImpressionType, jImpressionType, env);
	GBANUTIL_GetJIntExchangeFromCInt(HorizontalNumberOfPixels, jHorizontalNumberOfPixels, env);
	GBANUTIL_GetJIntExchangeFromCInt(VerticalNumberOfPixels, jVerticalNumberOfPixels, env);
	GBANUTIL_GetJIntExchangeFromCInt(ScaleUnits, jScaleUnits, env);
	GBANUTIL_GetJIntExchangeFromCInt(TransmittedHorizontalPixelScale,
	                                 jTransmittedHorizontalPixelScale, env);
	GBANUTIL_GetJIntExchangeFromCInt(TransmittedVerticalPixelScale, jTransmittedVerticalPixelScale,
	                                 env);
	GBANUTIL_GetJIntExchangeFromCInt(CompressionAlgorithm, jCompressionAlgorithm, env);
	GBANUTIL_GetJIntExchangeFromCInt(BitsPerPixel, jBitsPerPixel, env);
	GBANUTIL_GetJIntExchangeFromCInt(FingerprintPosition, jFingerprintPosition, env);
	GBANUTIL_GetJIntExchangeFromCInt(ScannedHorizontalPixelScale, jScannedHorizontalPixelScale,
	                                 env);
	GBANUTIL_GetJIntExchangeFromCInt(ScannedVerticalPixelScale, jScannedVerticalPixelScale, env);

	GBANUTIL_GetJStringExchangeFromCString(SourceAgency, jSourceAgency, env);
	GBANUTIL_GetJStringExchangeFromCString(Comment, jComment, env);
	GBANUTIL_GetJStringExchangeFromCString(DeviceMonitoringMode, jDeviceMonitoringMode, env);

	GBANUTIL_GetJBufferFromCBuffer(ImageBuffer, ImageBufferLen, jImageBuffer, env);

	GBAN2011UTIL_GetJDateFromCDate(env, jfingerprintCaptureDate, &FingerprintCaptureDate,
	                               An2000_JNI_LastErrorString);

	/******************
	 * AMP
	 *****************/
	// Init output array
	GB_AN2011_AmputatedBandagedCodeStruct dummyAmp;
	dummyAmp.AmpBandCode[0] = 0;
	dummyAmp.FrictionRidgePosition = -1;
	for (int i = 0; i < env->GetArrayLength(jAmp); i++)
	{
		jobject item = env->GetObjectArrayElement(jAmp, i);
		GBAN2011UTIL_GetJAmputatedBandagedCodeFromCAmputatedBandagedCode(env, item, &dummyAmp,
		                                                                 An2000_JNI_LastErrorString);
	}
	// set the correct structs
	if (AmpNum > env->GetArrayLength(jAmp)) AmpNum = env->GetArrayLength(jAmp);
	for (int i = 0; i < AmpNum; i++)
	{
		jobject item = env->GetObjectArrayElement(jAmp, i);
		GBAN2011UTIL_GetJAmputatedBandagedCodeFromCAmputatedBandagedCode(env, item, &(Amp[i]),
		                                                                 An2000_JNI_LastErrorString);
	}
	GBANUTIL_GetJIntExchangeFromCInt(AmpNum, jAmpNum, env);

	/******************
	 * SQM
	 *****************/
	// Init output array
	GB_AN2011_FingerOrSegmentQualityMetricStruct dummySqm;
	dummySqm.AlgorithmProductId = -1;
	dummySqm.AlgorithmVendorId = -1;
	dummySqm.QualityValue = -1;
	dummySqm.FrictionRidgePosition = -1;
	for (int i = 0; i < env->GetArrayLength(jSqm); i++)
	{
		jobject item = env->GetObjectArrayElement(jSqm, i);
		GBAN2011UTIL_GetJFingOrSegQualityMetricFromCFingOrSegQualityMetric(env, item, &dummySqm,
		                                                                   An2000_JNI_LastErrorString);
	}
	// set the correct structs
	if (SqmNum > env->GetArrayLength(jSqm)) SqmNum = env->GetArrayLength(jSqm);
	for (int i = 0; i < SqmNum; i++)
	{
		jobject item = env->GetObjectArrayElement(jSqm, i);
		GBAN2011UTIL_GetJFingOrSegQualityMetricFromCFingOrSegQualityMetric(env, item, &(Sqm[i]),
		                                                                   An2000_JNI_LastErrorString);
	}
	GBANUTIL_GetJIntExchangeFromCInt(SqmNum, jSqmNum, env);

	//////////////////////////////////
	// free memory
	//////////////////////////////////
	if (SourceAgency != NULL) Ptr_GB_AN_ENGINE_FreeGenericMem(SourceAgency);
	if (Amp != NULL) Ptr_GB_AN_ENGINE_FreeGenericMem(Amp);
	if (Comment != NULL) Ptr_GB_AN_ENGINE_FreeGenericMem(Comment);
	if (Sqm != NULL) Ptr_GB_AN_ENGINE_FreeGenericMem(Sqm);
	if (DeviceMonitoringMode != NULL) Ptr_GB_AN_ENGINE_FreeGenericMem(DeviceMonitoringMode);
	if (ImageBuffer != NULL) Ptr_GB_AN_ENGINE_FreeGenericMem(ImageBuffer);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_GetType14Info2011(
		JNIEnv *env,
		jobject, /* this */
		jobject jRecord, // Record
		jobject jIdc, // int
		jobject jImpressionType, // int
		jobject jSourceAgency, // string
		jobject jfingerprintCaptureDate, // GB_AN2011_DateStruct
		jobject jHorizontalNumberOfPixels, // int
		jobject jVerticalNumberOfPixels, // int
		jobject jScaleUnits, // int
		jobject jTransmittedHorizontalPixelScale, // int
		jobject jTransmittedVerticalPixelScale, // int
		jobject jCompressionAlgorithm, // int
		jobject jBitsPerPixel, // int
		jobject jFingerprintPosition, // int
		jobject jPpd, // GB_AN2011_PrintPositionDescriptorsStruct
		jobjectArray jPpc, // GB_AN2011_PrintPositionCoordinatesStruct[12]
		jobject jPpcNum, // int
		jobject jScannedHorizontalPixelScale, // int
		jobject jScannedVerticalPixelScale, // int
		jobjectArray jAmp, // GB_AN2011_AmputatedBandagedCodeStruct[5]
		jobject jAmpNum, // int
		jobject jComment, // string
		jobjectArray jSeg, // GB_AN2011_FingerSegmentPositionStruct[5]
		jobject jSegNum, // int
		jobjectArray jNqm, // GB_AN2011_NistQualityMetricStruct[5]
		jobject jNqmNum, // int
		jobjectArray jSqm, // GB_AN2011_FingerOrSegmentQualityMetricStruct[5]
		jobject jSqmNum, // int
		jobjectArray jFqm, // GB_AN2011_FingerOrSegmentQualityMetricStruct[5]
		jobject jFqmNum, // int
		jobjectArray jAseg, // GB_AN2011_AlternateSegmentPositionStruct[5]
		jobject jAsegNum, // int
		jobject jSimultaneousCaptureFingerprint, // int
		jobject jStitchedImageFlag, // boolean as int
		jobject jDeviceMonitoringMode, // string
		jobject jFingerprintAcquisitionProfile, // int
		jobject jImageBuffer // byte []
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;
	int RetVal;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GB_AN2011_GetType14RecordInfo == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: Ptr_GB_AN2011_GetType14RecordInfo is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jRecord == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jRecord is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jIdc == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jIdc is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jImpressionType == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jImpressionType is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jSourceAgency == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jSourceAgency is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jfingerprintCaptureDate == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jfingerprintCaptureDate is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jHorizontalNumberOfPixels == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jHorizontalNumberOfPixels is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jVerticalNumberOfPixels == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jVerticalNumberOfPixels is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jScaleUnits == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jScaleUnits is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jTransmittedHorizontalPixelScale == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jTransmittedHorizontalPixelScale is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jTransmittedVerticalPixelScale == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jTransmittedVerticalPixelScale is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jCompressionAlgorithm == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jCompressionAlgorithm is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jBitsPerPixel == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jBitsPerPixel is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFingerprintPosition == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jFingerprintPosition is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jPpd == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jPpd is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jPpc == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jPpc is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetArrayLength(jPpc) < 12)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jPpc length must be at least 12");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jPpcNum == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jPpcNum is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jScannedHorizontalPixelScale == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jScannedHorizontalPixelScale is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jScannedVerticalPixelScale == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jScannedVerticalPixelScale is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jAmp == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jAmp is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetArrayLength(jAmp) < 5)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jAmp length must be at least 5");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jAmpNum == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jAmpNum is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jComment == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jComment is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jSeg == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jSeg is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetArrayLength(jSeg) < 5)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jSeg length must be at least 5");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jSegNum == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jSegNum is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	// nqm
	if (jNqm == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jNqm is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetArrayLength(jNqm) < 5)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jNqm length must be at least 5");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jNqmNum == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jNqmNum is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	// sqm
	if (jSqm == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jSqm is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetArrayLength(jSqm) < 5)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jSqm length must be at least 5");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jSqmNum == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jSqmNum is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	// jFqm
	if (jFqm == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jFqm is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetArrayLength(jFqm) < 5)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jFqm length must be at least 5");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFqmNum == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jFqmNum is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	// Aseg
	if (jAseg == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jAseg is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetArrayLength(jAseg) < 5)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jAseg length must be at least 5");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jAsegNum == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jAsegNum is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jSimultaneousCaptureFingerprint == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jSimultaneousCaptureFingerprint is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jStitchedImageFlag == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jStitchedImageFlag is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jDeviceMonitoringMode == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jDeviceMonitoringMode is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFingerprintAcquisitionProfile == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jFingerprintAcquisitionProfile is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jImageBuffer == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: jImageBuffer is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check java class
	//////////////////////////////////

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	// record
	RECORD *record;
	GBANUTIL_GetCRecordFromJRecord(jRecord, &record, env);
	if (record == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: Record stored into jRecord is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// call function
	//////////////////////////////////
	int Idc, ImpressionType;
	GB_AN_ENGINE_String SourceAgency;
	GB_AN2011_DateStruct FingerprintCaptureDate;
	int HorizontalNumberOfPixels, VerticalNumberOfPixels, ScaleUnits,
			TransmittedHorizontalPixelScale, TransmittedVerticalPixelScale,
			CompressionAlgorithm, BitsPerPixel, FingerprintPosition;
	GB_AN2011_PrintPositionDescriptorsStruct Ppd;
	GB_AN2011_PrintPositionCoordinatesStruct *Ppc;
	int PpcNum;
	int ScannedHorizontalPixelScale, ScannedVerticalPixelScale;
	GB_AN2011_AmputatedBandagedCodeStruct *Amp;
	int AmpNum;
	GB_AN_ENGINE_String Comment;
	GB_AN2011_FingerSegmentPositionStruct *Seg;
	int SegNum;
	GB_AN2011_NistQualityMetricStruct *Nqm;
	int NqmNum;
	GB_AN2011_FingerOrSegmentQualityMetricStruct *Sqm;
	int SqmNum;
	GB_AN2011_FingerOrSegmentQualityMetricStruct *Fqm;
	int FqmNum;
	GB_AN2011_AlternateSegmentPositionStruct *Aseg;
	int AsegNum;
	int SimultaneousCaptureFingerprint;
	BOOL StitchedImageFlag;
	GB_AN_ENGINE_String DeviceMonitoringMode;
	int FingerprintAcquisitionProfile;
	unsigned char *ImageBuffer;
	int ImageBufferLen;
	RetVal = Ptr_GB_AN2011_GetType14RecordInfo(
			record,
			&Idc,
			&ImpressionType,
			&SourceAgency,
			&FingerprintCaptureDate,
			&HorizontalNumberOfPixels, &VerticalNumberOfPixels,
			&ScaleUnits,
			&TransmittedHorizontalPixelScale, &TransmittedVerticalPixelScale,
			&CompressionAlgorithm, &BitsPerPixel, &FingerprintPosition,
			&Ppd,
			&Ppc, &PpcNum,
			&ScannedHorizontalPixelScale, &ScannedVerticalPixelScale,
			&Amp, &AmpNum,
			&Comment,
			&Seg, &SegNum,
			&Nqm, &NqmNum,
			&Sqm, &SqmNum,
			&Fqm, &FqmNum,
			&Aseg, &AsegNum,
			&SimultaneousCaptureFingerprint,
			&StitchedImageFlag,
			&DeviceMonitoringMode,
			&FingerprintAcquisitionProfile,
			&ImageBuffer,
			&ImageBufferLen
	);
	if (RetVal != AN2K_DLL_NO_ERROR)
	{
		ValToRet = RetVal;
		char ErrorString[100];
		GbAn2000Util_FormatErrorString(RetVal, ErrorString);
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info2011: Ptr_GB_AN2011_GetType14RecordInfo returned %s (%d)",
		        ErrorString,
		        RetVal);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// set output values
	//////////////////////////////////
	GBANUTIL_GetJIntExchangeFromCInt(Idc, jIdc, env);
	GBANUTIL_GetJIntExchangeFromCInt(ImpressionType, jImpressionType, env);
	GBANUTIL_GetJIntExchangeFromCInt(HorizontalNumberOfPixels, jHorizontalNumberOfPixels, env);
	GBANUTIL_GetJIntExchangeFromCInt(VerticalNumberOfPixels, jVerticalNumberOfPixels, env);
	GBANUTIL_GetJIntExchangeFromCInt(ScaleUnits, jScaleUnits, env);
	GBANUTIL_GetJIntExchangeFromCInt(TransmittedHorizontalPixelScale,
	                                 jTransmittedHorizontalPixelScale, env);
	GBANUTIL_GetJIntExchangeFromCInt(TransmittedVerticalPixelScale, jTransmittedVerticalPixelScale,
	                                 env);
	GBANUTIL_GetJIntExchangeFromCInt(CompressionAlgorithm, jCompressionAlgorithm, env);
	GBANUTIL_GetJIntExchangeFromCInt(BitsPerPixel, jBitsPerPixel, env);
	GBANUTIL_GetJIntExchangeFromCInt(FingerprintPosition, jFingerprintPosition, env);
	GBANUTIL_GetJIntExchangeFromCInt(ScannedHorizontalPixelScale, jScannedHorizontalPixelScale,
	                                 env);
	GBANUTIL_GetJIntExchangeFromCInt(ScannedVerticalPixelScale, jScannedVerticalPixelScale, env);
	GBANUTIL_GetJIntExchangeFromCInt(SimultaneousCaptureFingerprint,
	                                 jSimultaneousCaptureFingerprint, env);
	GBANUTIL_GetJIntExchangeFromCInt(FingerprintAcquisitionProfile, jFingerprintAcquisitionProfile,
	                                 env);
	if (StitchedImageFlag) GBANUTIL_GetJIntExchangeFromCInt(1, jStitchedImageFlag, env);
	else GBANUTIL_GetJIntExchangeFromCInt(0, jStitchedImageFlag, env);

	GBANUTIL_GetJStringExchangeFromCString(SourceAgency, jSourceAgency, env);
	GBANUTIL_GetJStringExchangeFromCString(Comment, jComment, env);
	GBANUTIL_GetJStringExchangeFromCString(DeviceMonitoringMode, jDeviceMonitoringMode, env);

	GBANUTIL_GetJBufferFromCBuffer(ImageBuffer, ImageBufferLen, jImageBuffer, env);

	GBAN2011UTIL_GetJDateFromCDate(env, jfingerprintCaptureDate, &FingerprintCaptureDate,
	                               An2000_JNI_LastErrorString);

	GBAN2011UTIL_GetJPrintPositionDescFromCPrintPositionDesc(env, jPpd, &Ppd,
	                                                         An2000_JNI_LastErrorString);

	/******************
	 * PPC
	 *****************/
	// Init output array
	GB_AN2011_PrintPositionCoordinatesStruct dummyPPC;
	dummyPPC.BottomVerticalCoordinate = -1;
	dummyPPC.FullFingerView[0] = 0;
	dummyPPC.LeftHorizontalCoordinate = -1;
	dummyPPC.LocationOfASegment[0] = 0;
	dummyPPC.RightHorizontalCoordinate = -1;
	dummyPPC.TopVerticalCoordinate = -1;
	for (int i = 0; i < env->GetArrayLength(jPpc); i++)
	{
		jobject item = env->GetObjectArrayElement(jPpc, i);
		GBAN2011UTIL_GetJPrintPositionCoordFromCPrintPositionCoord(env, item, &dummyPPC,
		                                                           An2000_JNI_LastErrorString);
	}
	// set the correct structs
	if (PpcNum > env->GetArrayLength(jPpc)) PpcNum = env->GetArrayLength(jPpc);
	for (int i = 0; i < PpcNum; i++)
	{
		jobject item = env->GetObjectArrayElement(jPpc, i);
		GBAN2011UTIL_GetJPrintPositionCoordFromCPrintPositionCoord(env, item, &(Ppc[i]),
		                                                           An2000_JNI_LastErrorString);
	}
	GBANUTIL_GetJIntExchangeFromCInt(PpcNum, jPpcNum, env);

	/******************
	 * AMP
	 *****************/
	// Init output array
	GB_AN2011_AmputatedBandagedCodeStruct dummyAmp;
	dummyAmp.AmpBandCode[0] = 0;
	dummyAmp.FrictionRidgePosition = -1;
	for (int i = 0; i < env->GetArrayLength(jAmp); i++)
	{
		jobject item = env->GetObjectArrayElement(jAmp, i);
		GBAN2011UTIL_GetJAmputatedBandagedCodeFromCAmputatedBandagedCode(env, item, &dummyAmp,
		                                                                 An2000_JNI_LastErrorString);
	}
	// set the correct structs
	if (AmpNum > env->GetArrayLength(jAmp)) AmpNum = env->GetArrayLength(jAmp);
	for (int i = 0; i < AmpNum; i++)
	{
		jobject item = env->GetObjectArrayElement(jAmp, i);
		GBAN2011UTIL_GetJAmputatedBandagedCodeFromCAmputatedBandagedCode(env, item, &(Amp[i]),
		                                                                 An2000_JNI_LastErrorString);
	}
	GBANUTIL_GetJIntExchangeFromCInt(AmpNum, jAmpNum, env);

	/******************
	 * SEG
	 *****************/
	// Init output array
	GB_AN2011_FingerSegmentPositionStruct dummySeg;
	dummySeg.TopVerticalCoordinate = -1;
	dummySeg.RightHorizontalCoordinate = -1;
	dummySeg.LeftHorizontalCoordinate = -1;
	dummySeg.BottomVerticalCoordinate = -1;
	dummySeg.FrictionRidgePosition = -1;
	for (int i = 0; i < env->GetArrayLength(jSeg); i++)
	{
		jobject item = env->GetObjectArrayElement(jSeg, i);
		GBAN2011UTIL_GetJFingerSegmentPositionFromCFingerSegmentPosition(env, item, &dummySeg,
		                                                                 An2000_JNI_LastErrorString);
	}
	// set the correct structs
	if (SegNum > env->GetArrayLength(jSeg)) SegNum = env->GetArrayLength(jSeg);
	for (int i = 0; i < SegNum; i++)
	{
		jobject item = env->GetObjectArrayElement(jSeg, i);
		GBAN2011UTIL_GetJFingerSegmentPositionFromCFingerSegmentPosition(env, item, &(Seg[i]),
		                                                                 An2000_JNI_LastErrorString);
	}
	GBANUTIL_GetJIntExchangeFromCInt(SegNum, jSegNum, env);

	/******************
	 * NQM
	 *****************/
	// Init output array
	GB_AN2011_NistQualityMetricStruct dummyNqm;
	dummyNqm.Quality = -1;
	dummyNqm.FrictionRidgePosition = -1;
	for (int i = 0; i < env->GetArrayLength(jNqm); i++)
	{
		jobject item = env->GetObjectArrayElement(jNqm, i);
		GBAN2011UTIL_GetJNistQualityMetricFromCNistQualityMetric(env, item, &dummyNqm,
		                                                         An2000_JNI_LastErrorString);
	}
	// set the correct structs
	if (NqmNum > env->GetArrayLength(jNqm)) NqmNum = env->GetArrayLength(jNqm);
	for (int i = 0; i < NqmNum; i++)
	{
		jobject item = env->GetObjectArrayElement(jNqm, i);
		GBAN2011UTIL_GetJNistQualityMetricFromCNistQualityMetric(env, item, &(Nqm[i]),
		                                                         An2000_JNI_LastErrorString);
	}
	GBANUTIL_GetJIntExchangeFromCInt(NqmNum, jNqmNum, env);

	/******************
	 * SQM
	 *****************/
	// Init output array
	GB_AN2011_FingerOrSegmentQualityMetricStruct dummySqm;
	dummySqm.AlgorithmProductId = -1;
	dummySqm.AlgorithmVendorId = -1;
	dummySqm.QualityValue = -1;
	dummySqm.FrictionRidgePosition = -1;
	for (int i = 0; i < env->GetArrayLength(jSqm); i++)
	{
		jobject item = env->GetObjectArrayElement(jSqm, i);
		GBAN2011UTIL_GetJFingOrSegQualityMetricFromCFingOrSegQualityMetric(env, item, &dummySqm,
		                                                                   An2000_JNI_LastErrorString);
	}
	// set the correct structs
	if (SqmNum > env->GetArrayLength(jSqm)) SqmNum = env->GetArrayLength(jSqm);
	for (int i = 0; i < SqmNum; i++)
	{
		jobject item = env->GetObjectArrayElement(jSqm, i);
		GBAN2011UTIL_GetJFingOrSegQualityMetricFromCFingOrSegQualityMetric(env, item, &(Sqm[i]),
		                                                                   An2000_JNI_LastErrorString);
	}
	GBANUTIL_GetJIntExchangeFromCInt(SqmNum, jSqmNum, env);

	/******************
	 * FQM
	 *****************/
	// Init output array
	for (int i = 0; i < env->GetArrayLength(jFqm); i++)
	{
		jobject item = env->GetObjectArrayElement(jFqm, i);
		GBAN2011UTIL_GetJFingOrSegQualityMetricFromCFingOrSegQualityMetric(env, item, &dummySqm,
		                                                                   An2000_JNI_LastErrorString);
	}
	// set the correct structs
	if (FqmNum > env->GetArrayLength(jFqm)) FqmNum = env->GetArrayLength(jFqm);
	for (int i = 0; i < FqmNum; i++)
	{
		jobject item = env->GetObjectArrayElement(jFqm, i);
		GBAN2011UTIL_GetJFingOrSegQualityMetricFromCFingOrSegQualityMetric(env, item, &(Fqm[i]),
		                                                                   An2000_JNI_LastErrorString);
	}
	GBANUTIL_GetJIntExchangeFromCInt(FqmNum, jFqmNum, env);

	/******************
	 * ASEG
	 *****************/
	// Init output array
	GB_AN2011_AlternateSegmentPositionStruct dummyAseg;
	dummyAseg.NumberOfPoints = 0;
	dummyAseg.Points = NULL;
	dummyAseg.FrictionRidgePosition = -1;
	for (int i = 0; i < env->GetArrayLength(jAseg); i++)
	{
		jobject item = env->GetObjectArrayElement(jAseg, i);
		GBAN2011UTIL_GetJAlternateQualityMetricFromCAlternateQualityMetric(env, item, &dummyAseg,
		                                                                   An2000_JNI_LastErrorString);
	}
	// set the correct structs
	if (AsegNum > env->GetArrayLength(jAseg)) AsegNum = env->GetArrayLength(jAseg);
	for (int i = 0; i < AsegNum; i++)
	{
		jobject item = env->GetObjectArrayElement(jAseg, i);
		GBAN2011UTIL_GetJAlternateQualityMetricFromCAlternateQualityMetric(env, item, &(Aseg[i]),
		                                                                   An2000_JNI_LastErrorString);
	}
	GBANUTIL_GetJIntExchangeFromCInt(AsegNum, jAsegNum, env);

	//////////////////////////////////
	// free memory
	//////////////////////////////////
	if (SourceAgency != NULL) Ptr_GB_AN_ENGINE_FreeGenericMem(SourceAgency);
	if (Ppc != NULL) Ptr_GB_AN_ENGINE_FreeGenericMem(Ppc);
	if (Amp != NULL) Ptr_GB_AN_ENGINE_FreeGenericMem(Amp);
	if (Comment != NULL) Ptr_GB_AN_ENGINE_FreeGenericMem(Comment);
	if (Seg != NULL) Ptr_GB_AN_ENGINE_FreeGenericMem(Seg);
	if (Nqm != NULL) Ptr_GB_AN_ENGINE_FreeGenericMem(Nqm);
	if (Sqm != NULL) Ptr_GB_AN_ENGINE_FreeGenericMem(Sqm);
	if (Fqm != NULL) Ptr_GB_AN_ENGINE_FreeGenericMem(Fqm);
	if (Aseg != NULL) Ptr_GB_AN2011_FreeAsegArray(Aseg, AsegNum);
	if (DeviceMonitoringMode != NULL) Ptr_GB_AN_ENGINE_FreeGenericMem(DeviceMonitoringMode);
	if (ImageBuffer != NULL) Ptr_GB_AN_ENGINE_FreeGenericMem(ImageBuffer);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_GetType14Info(
		JNIEnv *env,
		jobject, /* this */
		jobject jRecord, // Record
		jobject jFingerprintImage, // byte []
		jobject jFPImageWidth, // int
		jobject jFPImageHeight, // int
		jobject jPixelDepth, // int
		jobject jScanResolutionPPMM, // double
		jobject jCompressionType, // string
		jobject jImpressionType, // int
		jobject jImageSource, // string
		jobject jComment, // string
		jobjectArray jFingerPosition, // string [6],
		jobjectArray jAmputationList,
		jobject jAmputatedFingerNumber, // valid items number in jAmputationList
		jobjectArray jboundaries,
		jobject jSegmentedFingerNumber, // valid items number in jboundaries
		jobjectArray jqualities,
		jobject jNfiqEvaluatedFingerNumber, // valid items number in jqualities
		jobjectArray jsegqual,
		jobject jProprietaryQualityEvaluatedFingerNumber, // valid items number in jsegqual
		jobjectArray jaltqual,
		jobject jAlternateQualityEvaluatedFingerNumber // valid items number in jaltqual
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;
	int RetVal;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_AN2000_Get_EFTStype14_Info == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info: Ptr_AN2000_Get_EFTStype14_Info is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jRecord == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info: jRecord is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFingerprintImage == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info: jFingerprintImage is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFPImageWidth == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info: jFPImageWidth <= 0");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFPImageHeight == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info: jFPImageHeight <= 0");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jCompressionType == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info: jCompressionType is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jImageSource == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info: jImageSource is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jComment == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info: jComment is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFingerPosition == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info: jFingerPosition is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetArrayLength(jFingerPosition) < 6)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info: jFingerPosition length must be at least 6");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jAmputationList == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info: jAmputationList is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetArrayLength(jAmputationList) < 4)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info: jAmputationList length must be at least 4");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jAmputatedFingerNumber == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info: jAmputatedFingerNumber is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jboundaries == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info: jboundaries is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetArrayLength(jboundaries) < 4)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info: jboundaries length must be at least 4");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jSegmentedFingerNumber == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info: jSegmentedFingerNumber is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jqualities == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info: jqualities is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetArrayLength(jqualities) < 4)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info: jqualities length must be at least 4");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jNfiqEvaluatedFingerNumber == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info: jNfiqEvaluatedFingerNumber is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jsegqual == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info: jsegqual is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetArrayLength(jsegqual) < 4)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info: jsegqual length must be at least 4");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jProprietaryQualityEvaluatedFingerNumber == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info: jProprietaryQualityEvaluatedFingerNumber is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jaltqual == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info: jaltqual is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetArrayLength(jaltqual) < 4)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info: jaltqual length must be at least 4");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jAlternateQualityEvaluatedFingerNumber == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info: jAlternateQualityEvaluatedFingerNumber is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check java class
	//////////////////////////////////

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	// record
	RECORD *record;
	GBANUTIL_GetCRecordFromJRecord(jRecord, &record, env);
	if (record == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info: Record stored into jRecord is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// call function
	//////////////////////////////////
	unsigned char *ImagePtr;            // image frame: allocated by the library
	int ImageSize, w, h, PixelDepth;
	double Resolution_ppmm;    // resolution in pixel per mm
	char ImageCompression[10];    // image compression: see definitions: pass a char[10]
	int ImageImpression;    // image impression type (live-scan, roll, etc, see definitions)
	char source_str[44];
	char comment[136];
	char finger_position_list[6][3];
	FING_AMP_LIST *amp_list;
	FOUR_FING_BOUND *boundaries;
	FOUR_FING_QUALITY *qualities;
	ALT_QUAL_LIST *segqual, *altqual;
	RetVal = Ptr_AN2000_Get_EFTStype14_Info(
			record,
			&ImagePtr,
			&ImageSize, &w, &h,
			&PixelDepth,
			&Resolution_ppmm,
			ImageCompression,
			&ImageImpression,
			source_str,
			comment,
			finger_position_list,
			&amp_list,
			&boundaries,
			&qualities,
			&segqual,
			&altqual            // list of quality of each of four or two thumbs or single fingers in
			// a slap calculated with an algorithm different from NFIQ
	);
	if (RetVal != AN2K_DLL_NO_ERROR)
	{
		ValToRet = RetVal;
		char ErrorString[100];
		GbAn2000Util_FormatErrorString(RetVal, ErrorString);
		sprintf(An2000_JNI_LastErrorString,
		        "GetType14Info: Ptr_AN2000_Get_EFTStype14_Info returned %s (%d)", ErrorString,
		        RetVal);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// set output values
	//////////////////////////////////
	// image
	GBANUTIL_GetJBufferFromCBuffer(ImagePtr, ImageSize, jFingerprintImage, env);
	GBANUTIL_GetJIntExchangeFromCInt(w, jFPImageWidth, env);
	GBANUTIL_GetJIntExchangeFromCInt(h, jFPImageHeight, env);
	// pixel depth
	GBANUTIL_GetJIntExchangeFromCInt(PixelDepth, jPixelDepth, env);
	// Resolution
	GBANUTIL_GetJDoubleExchangeFromCDouble(Resolution_ppmm, jScanResolutionPPMM, env);
	// compression type
	GBANUTIL_GetJStringExchangeFromCString(ImageCompression, jCompressionType, env);
	// ImageImpression
	GBANUTIL_GetJIntExchangeFromCInt(ImageImpression, jImpressionType, env);
	// source_str
	GBANUTIL_GetJStringExchangeFromCString(source_str, jImageSource, env);
	// comment
	GBANUTIL_GetJStringExchangeFromCString(comment, jComment, env);
	// finger_position_list
	for (int i = 0; i < 6; i++)
	{
		jobject obj = (jobject) env->GetObjectArrayElement(jFingerPosition, i);
		finger_position_list[i][2] = 0;
		GBANUTIL_GetJStringExchangeFromCString(finger_position_list[i], obj, env);
	}
	// amp_list
	if (amp_list != NULL)
	{
		GBANUTIL_GetJAmpStructArrayFromCAmpList(env, jAmputationList, amp_list,
		                                        An2000_JNI_LastErrorString);
		GBANUTIL_GetJIntExchangeFromCInt(amp_list->num_fingers, jAmputatedFingerNumber, env);
	}
	else
	{
		GBANUTIL_GetJIntExchangeFromCInt(0, jAmputatedFingerNumber, env);
	}
	// boundaries
	if (boundaries != NULL)
	{
		GBANUTIL_GetJSegmBoundsStructArrayFromCSegmBoundsList(env, jboundaries, boundaries,
		                                                      An2000_JNI_LastErrorString);
		GBANUTIL_GetJIntExchangeFromCInt(boundaries->num_fingers, jSegmentedFingerNumber, env);
	}
	else
	{
		GBANUTIL_GetJIntExchangeFromCInt(0, jSegmentedFingerNumber, env);
	}
	// qualities
	if (qualities != NULL)
	{
		GBANUTIL_GetJFingerQualityStructArrayFromCFingerQualityList(env, jqualities, qualities,
		                                                            An2000_JNI_LastErrorString);
		GBANUTIL_GetJIntExchangeFromCInt(qualities->num_fingers, jNfiqEvaluatedFingerNumber, env);
	}
	else
	{
		GBANUTIL_GetJIntExchangeFromCInt(0, jNfiqEvaluatedFingerNumber, env);
	}
	// segqual
	if (segqual != NULL)
	{
		GBANUTIL_GetJAlternateFingerQualityStructArrayFromCAlternateFingerQualityList(env, jsegqual,
		                                                                              segqual,
		                                                                              An2000_JNI_LastErrorString);
		GBANUTIL_GetJIntExchangeFromCInt(segqual->num_fingers,
		                                 jProprietaryQualityEvaluatedFingerNumber, env);
	}
	else
	{
		GBANUTIL_GetJIntExchangeFromCInt(0, jProprietaryQualityEvaluatedFingerNumber, env);
	}
	// altqual
	if (altqual != NULL)
	{
		GBANUTIL_GetJAlternateFingerQualityStructArrayFromCAlternateFingerQualityList(env, jaltqual,
		                                                                              altqual,
		                                                                              An2000_JNI_LastErrorString);
		GBANUTIL_GetJIntExchangeFromCInt(altqual->num_fingers,
		                                 jAlternateQualityEvaluatedFingerNumber, env);
	}
	else
	{
		GBANUTIL_GetJIntExchangeFromCInt(0, jAlternateQualityEvaluatedFingerNumber, env);
	}

	//////////////////////////////////
	// free memory
	//////////////////////////////////
	Ptr_AN2000_free_ANSI_NIST_buffer(ImagePtr);
	if (amp_list != NULL) Ptr_AN2000_free_ANSI_NIST_amp_list(amp_list);
	if (boundaries != NULL) Ptr_AN2000_free_FOUR_FING_BOUND(boundaries);
	if (qualities != NULL) Ptr_AN2000_free_FOUR_FING_QUALITY(qualities);
	if (segqual != NULL) Ptr_AN2000_free_ANSI_NIST_alt_qual_list(segqual);
	if (altqual != NULL) Ptr_AN2000_free_ANSI_NIST_alt_qual_list(altqual);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_GetType15Info(
		JNIEnv *env,
		jobject, /* this */
		jobject jRecord, // Record
		jobject jPPImage, // byte []
		jobject jPPImageWidth, // int
		jobject jPPImageHeight, // int
		jobject jScanResolutionPPMM, // double
		jobject jCompressionType, // string
		jobject jImpressionType, // int
		jobject jImageSource, // string
		jobject jComment, // string
		jobject jPalmPosition, // string,
		jobjectArray jPalmQualitiesList,
		jobject jPalmAlgorithmsNumber // valid items number in jPalmQualitiesList
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;
	int RetVal;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_AN2000_Get_EFTStype15_Info == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info: Ptr_AN2000_Get_EFTStype15_Info is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jRecord == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info: jRecord is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jPPImage == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info: jPPImage is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jPPImageWidth == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info: jPPImageWidth <= 0");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jPPImageHeight == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info: jPPImageHeight <= 0");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jCompressionType == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info: jCompressionType is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jImageSource == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info: jImageSource is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jComment == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info: jComment is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jPalmPosition == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info: jPalmPosition is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jPalmQualitiesList == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info: jAmputationList is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetArrayLength(jPalmQualitiesList) < 10)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info: jPalmQualitiesList length must be at least 10");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jPalmAlgorithmsNumber == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info: jPalmAlgorithmsNumber is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check java class
	//////////////////////////////////

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	// record
	RECORD *record;
	GBANUTIL_GetCRecordFromJRecord(jRecord, &record, env);
	if (record == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info: Record stored into jRecord is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// call function
	//////////////////////////////////
	unsigned char *ImagePtr;            // image frame: allocated by the library
	int ImageSize, w, h;
	int BitsPerPixel;
	double Resolution_ppmm;    // resolution in pixel per mm
	char ImageCompression[10];    // image compression: see definitions: pass a char[10]
	int ImageImpression;    // image impression type (live-scan, roll, etc, see definitions)
	char source_str[44];
	char comment[136];
	char PalmPosition[3];
	PALM_QUAL_LIST *PalmQualitiesList;
	RetVal = Ptr_AN2000_Get_EFTStype15_Info(
			record,
			&ImagePtr,
			&ImageSize, &w, &h,
			&BitsPerPixel,
			&Resolution_ppmm,
			ImageCompression,
			&ImageImpression,
			source_str,
			comment,
			PalmPosition,
			&PalmQualitiesList
	);
	if (RetVal != AN2K_DLL_NO_ERROR)
	{
		ValToRet = RetVal;
		char ErrorString[100];
		GbAn2000Util_FormatErrorString(RetVal, ErrorString);
		sprintf(An2000_JNI_LastErrorString,
		        "GetType15Info: Ptr_AN2000_Get_EFTStype15_Info returned %s (%d)", ErrorString,
		        RetVal);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// set output values
	//////////////////////////////////
	// image
	GBANUTIL_GetJBufferFromCBuffer(ImagePtr, ImageSize, jPPImage, env);
	GBANUTIL_GetJIntExchangeFromCInt(w, jPPImageWidth, env);
	GBANUTIL_GetJIntExchangeFromCInt(h, jPPImageHeight, env);
	// Resolution
	GBANUTIL_GetJDoubleExchangeFromCDouble(Resolution_ppmm, jScanResolutionPPMM, env);
	// compression type
	GBANUTIL_GetJStringExchangeFromCString(ImageCompression, jCompressionType, env);
	// ImageImpression
	GBANUTIL_GetJIntExchangeFromCInt(ImageImpression, jImpressionType, env);
	// source_str
	GBANUTIL_GetJStringExchangeFromCString(source_str, jImageSource, env);
	// comment
	GBANUTIL_GetJStringExchangeFromCString(comment, jComment, env);
	// PalmPosition
	GBANUTIL_GetJStringExchangeFromCString(PalmPosition, jPalmPosition, env);
	// PalmQualitiesList
	if (PalmQualitiesList != NULL)
	{
		GBANUTIL_GetJPalmQualityStructArrayFromCPalmQualityList(env, jPalmQualitiesList,
		                                                        PalmQualitiesList,
		                                                        An2000_JNI_LastErrorString);
		GBANUTIL_GetJIntExchangeFromCInt(PalmQualitiesList->AlgorhythmsNumber,
		                                 jPalmAlgorithmsNumber, env);
	}
	else
	{
		GBANUTIL_GetJIntExchangeFromCInt(0, jPalmAlgorithmsNumber, env);
	}

	//////////////////////////////////
	// free memory
	//////////////////////////////////
	Ptr_AN2000_free_ANSI_NIST_buffer(ImagePtr);
	if (PalmQualitiesList != NULL) Ptr_AN2000_free_ANSI_NIST_palm_qual_list(PalmQualitiesList);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_GetType4Info2011(
		JNIEnv *env,
		jobject, /* this */
		jobject jRecord, // Record
		jobject jIdc, // int
		jobject jImpressionType, // int
		jobject jFingerPosition, // int
		jobject jScanResolutionId, // int
		jobject jFPImageWidth, // int
		jobject jFPImageHeight, // int
		jobject jCompressionType, // int
		jobject jFingerprintImage // byte []
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;
	int RetVal;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType4Info2011: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GB_AN2011_GetType4RecordInfo == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType4Info2011: Ptr_AN2000_Get_EFTStype4_Info is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jRecord == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType4Info2011: jRecord is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jIdc == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType4Info2011: jIdc is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFingerprintImage == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType4Info2011: jFingerprintImage is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFPImageWidth == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType4Info2011: jFPImageWidth <= 0");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFPImageHeight == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType4Info2011: jFPImageHeight <= 0");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jCompressionType == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType4Info2011: jCompressionType is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFingerPosition == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType4Info2011: jFingerPosition is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check java class
	//////////////////////////////////

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	// record
	RECORD *record;
	GBANUTIL_GetCRecordFromJRecord(jRecord, &record, env);
	if (record == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType4Info2011: Record stored into jRecord is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// call function
	//////////////////////////////////
	unsigned char *ImagePtr;            // image frame: allocated by the library
	int ImageSize, w, h, PixelDepth, Idc;
	int ScanResolution;    // resolution Id
	int ImageCompression;    // image compression:
	int ImageImpression;    // image impression type (live-scan, roll, etc, see definitions)
	unsigned char finger_position;
	RetVal = Ptr_GB_AN2011_GetType4RecordInfo(
			record,
			&Idc,
			&ImageImpression,
			&finger_position,
			&ScanResolution,
			&w, &h,
			&ImageCompression,
			&ImagePtr,
			&ImageSize
	);
	if (RetVal != AN2K_DLL_NO_ERROR)
	{
		ValToRet = RetVal;
		char ErrorString[100];
		GbAn2000Util_FormatErrorString(RetVal, ErrorString);
		sprintf(An2000_JNI_LastErrorString,
		        "GetType4Info2011: Ptr_AN2000_Get_EFTStype4_Info returned %s (%d)", ErrorString,
		        RetVal);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// set output values
	//////////////////////////////////
	GBANUTIL_GetJIntExchangeFromCInt(Idc, jIdc, env);
	GBANUTIL_GetJIntExchangeFromCInt(ImageImpression, jImpressionType, env);
	GBANUTIL_GetJIntExchangeFromCInt((int) finger_position, jFingerPosition, env);
	GBANUTIL_GetJIntExchangeFromCInt(ScanResolution, jScanResolutionId, env);
	GBANUTIL_GetJIntExchangeFromCInt(w, jFPImageWidth, env);
	GBANUTIL_GetJIntExchangeFromCInt(h, jFPImageHeight, env);
	GBANUTIL_GetJIntExchangeFromCInt(ImageCompression, jCompressionType, env);
	GBANUTIL_GetJBufferFromCBuffer(ImagePtr, ImageSize, jFingerprintImage, env);

	//////////////////////////////////
	// free memory
	//////////////////////////////////
	Ptr_GB_AN_ENGINE_FreeGenericMem(ImagePtr);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_GetType4Info(
		JNIEnv *env,
		jobject, /* this */
		jobject jRecord, // Record
		jobject jFingerprintImage, // byte []
		jobject jFPImageWidth, // int
		jobject jFPImageHeight, // int
		jobject jScanResolutionId, // int
		jobject jCompressionType, // string
		jobject jImpressionType, // int
		jobjectArray jFingerPosition // int [6]
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;
	int RetVal;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType4Info: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_AN2000_Get_EFTStype4_Info == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType4Info: Ptr_AN2000_Get_EFTStype4_Info is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jRecord == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType4Info: jRecord is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFingerprintImage == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType4Info: jFingerprintImage is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFPImageWidth == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType4Info: jFPImageWidth <= 0");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFPImageHeight == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType4Info: jFPImageHeight <= 0");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jCompressionType == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType4Info: jCompressionType is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFingerPosition == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType4Info: jFingerPosition is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetArrayLength(jFingerPosition) < 6)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType4Info: jFingerPosition length must be at least 6");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check java class
	//////////////////////////////////

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	// record
	RECORD *record;
	GBANUTIL_GetCRecordFromJRecord(jRecord, &record, env);
	if (record == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType4Info: Record stored into jRecord is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// call function
	//////////////////////////////////
	unsigned char *ImagePtr;            // image frame: allocated by the library
	int ImageSize, w, h, PixelDepth;
	int ScanResolution;    // resolution Id
	char ImageCompression[10];    // image compression: see definitions: pass a char[10]
	int ImageImpression;    // image impression type (live-scan, roll, etc, see definitions)
	int finger_position[6];
	RetVal = Ptr_AN2000_Get_EFTStype4_Info(
			record,
			&ImagePtr,
			&ImageSize,
			&ImageImpression,
			finger_position,
			&ScanResolution,
			&w, &h,
			ImageCompression
	);
	if (RetVal != AN2K_DLL_NO_ERROR)
	{
		ValToRet = RetVal;
		char ErrorString[100];
		GbAn2000Util_FormatErrorString(RetVal, ErrorString);
		sprintf(An2000_JNI_LastErrorString,
		        "GetType4Info: Ptr_AN2000_Get_EFTStype4_Info returned %s (%d)", ErrorString,
		        RetVal);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// set output values
	//////////////////////////////////
	// image
	GBANUTIL_GetJBufferFromCBuffer(ImagePtr, ImageSize, jFingerprintImage, env);
	GBANUTIL_GetJIntExchangeFromCInt(w, jFPImageWidth, env);
	GBANUTIL_GetJIntExchangeFromCInt(h, jFPImageHeight, env);
	// Resolution
	GBANUTIL_GetJIntExchangeFromCInt(ScanResolution, jScanResolutionId, env);
	// compression type
	GBANUTIL_GetJStringExchangeFromCString(ImageCompression, jCompressionType, env);
	// ImageImpression
	GBANUTIL_GetJIntExchangeFromCInt(ImageImpression, jImpressionType, env);
	// finger_position_list
	for (int i = 0; i < 6; i++)
	{
		jobject obj = (jobject) env->GetObjectArrayElement(jFingerPosition, i);
		GBANUTIL_GetJIntExchangeFromCInt(finger_position[i], obj, env);
	}

	//////////////////////////////////
	// free memory
	//////////////////////////////////
	Ptr_AN2000_free_ANSI_NIST_buffer(ImagePtr);

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_CreateType15Record2011(
		JNIEnv *env,
		jobject, /* this */
		jobject jRecord, // MANDATORY
		jint jIdc, // MANDATORY
		jint jImpressionType, // MANDATORY
		jstring jSourceAgency, // MANDATORY
		jobject jFingerprintCaptureDate, // GB_AN2011_DateStruct, // MANDATORY
		jint jHorizontalNumberOfPixels, // MANDATORY
		jint jVerticalNumberOfPixels, // MANDATORY
		jint jScaleUnits, // MANDATORY
		jint jTransmittedHorizontalPixelScale, // MANDATORY
		jint jTransmittedVerticalPixelScale, // MANDATORY
		jint jCompressionAlgorithm, // MANDATORY
		jint jBitsPerPixel, // MANDATORY
		jint jFingerprintPosition, // MANDATORY
		jint jScannedHorizontalPixelScale, // OPTIONAL
		jint jScannedVerticalPixelScale, // OPTIONAL
		jobjectArray jAmp, // GB_AN2011_AmputatedBandagedCodeStruct[] // OPTIONAL
		jstring jComment, // OPTIONAL
		jobjectArray jSqm, // GB_AN2011_FingerOrSegmentQualityMetricStruct[] // OPTIONAL
		jstring jDeviceMonitoringMode, // OPTIONAL
		jbyteArray jFingerprintImage // MANDATORY
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;
	int RetVal;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateType15Record2011: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GB_AN2011_CreateType15Record == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateType15Record2011: Ptr_GB_AN2011_CreateType15Record is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jRecord == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateType15Record2011: jRecord is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFingerprintImage == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateType15Record2011: jFingerprintImage is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jSourceAgency == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateType15Record2011: jSourceAgency is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFingerprintCaptureDate == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateType15Record2011: jFingerprintCaptureDate is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jHorizontalNumberOfPixels <= 0)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateType15Record2011: jHorizontalNumberOfPixels <= 0");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jVerticalNumberOfPixels <= 0)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateType15Record2011: jVerticalNumberOfPixels <= 0");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if ((env)->GetArrayLength(jFingerprintImage) <
	    (jHorizontalNumberOfPixels * jVerticalNumberOfPixels))
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateType15Record2011: jFingerprintImage too small");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check java class
	//////////////////////////////////

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	// record
	RECORD *record;

	// sourceAgency
	char *sourceAgency = NULL;
	if (jSourceAgency != NULL) GBANUTIL_GetStringFromJString(jSourceAgency, &sourceAgency, env);

	// comment
	char *comment = NULL;
	if (jComment != NULL) GBANUTIL_GetStringFromJString(jComment, &comment, env);

	// deviceMonitoringMode
	char *deviceMonitoringMode = NULL;
	if (jDeviceMonitoringMode != NULL)
		GBANUTIL_GetStringFromJString(jDeviceMonitoringMode, &deviceMonitoringMode, env);

	// FingerprintImage
	unsigned char *FingerprintImage = new unsigned char[jHorizontalNumberOfPixels *
	                                                    jVerticalNumberOfPixels];
	env->GetByteArrayRegion(jFingerprintImage, 0,
	                        jHorizontalNumberOfPixels * jVerticalNumberOfPixels,
	                        reinterpret_cast<jbyte *>(FingerprintImage));
	// fingerprintCaptureDate Date
	GB_AN2011_DateStruct fingerprintCaptureDate;
	GBAN2011UTIL_GetCDateFromJDate(env, jFingerprintCaptureDate, &fingerprintCaptureDate,
	                               An2000_JNI_LastErrorString);

	// amp
	GB_AN2011_AmputatedBandagedCodeStruct *amp = NULL;
	int ampLen = 0;
	if (jAmp != NULL)
	{
		if ((env)->GetArrayLength(jAmp) > 0)
		{
			ampLen = (env)->GetArrayLength(jAmp);
			amp = (GB_AN2011_AmputatedBandagedCodeStruct *) malloc((env)->GetArrayLength(jAmp) *
			                                                       sizeof(GB_AN2011_AmputatedBandagedCodeStruct));
			for (int i = 0; i < (env)->GetArrayLength(jAmp); i++)
			{
				jobject item = env->GetObjectArrayElement(jAmp, i);
				GBAN2011UTIL_GetCAmputatedBandagedCodeFromJAmputatedBandagedCode(env, item,
				                                                                 &(amp[i]),
				                                                                 An2000_JNI_LastErrorString);
			}
		}
	}

	// sqm
	GB_AN2011_FingerOrSegmentQualityMetricStruct *sqm = NULL;
	int sqmLen = 0;
	if (jSqm != NULL)
	{
		if ((env)->GetArrayLength(jSqm) > 0)
		{
			sqmLen = (env)->GetArrayLength(jSqm);
			sqm = (GB_AN2011_FingerOrSegmentQualityMetricStruct *) malloc(
					(env)->GetArrayLength(jSqm) *
					sizeof(GB_AN2011_FingerOrSegmentQualityMetricStruct));
			for (int i = 0; i < (env)->GetArrayLength(jSqm); i++)
			{
				jobject item = env->GetObjectArrayElement(jSqm, i);
				GBAN2011UTIL_GetCFingOrSegQualityMetricFromJFingOrSegQualityMetric(env, item,
				                                                                   &(sqm[i]),
				                                                                   An2000_JNI_LastErrorString);
			}
		}
	}

	//////////////////////////////////
	// call function
	//////////////////////////////////
	RetVal = Ptr_GB_AN2011_CreateType15Record(
			&record,
			(int) jIdc,
			(int) jImpressionType,
			sourceAgency,
			&fingerprintCaptureDate,
			(int) jHorizontalNumberOfPixels, (int) jVerticalNumberOfPixels,
			(int) jScaleUnits, (int) jTransmittedHorizontalPixelScale,
			(int) jTransmittedVerticalPixelScale,
			(int) jCompressionAlgorithm,
			(int) jBitsPerPixel,
			(int) jFingerprintPosition,
			(int) jScannedHorizontalPixelScale, (int) jScannedVerticalPixelScale,
			amp, ampLen,
			comment,
			sqm, sqmLen,
			deviceMonitoringMode,
			FingerprintImage, env->GetArrayLength(jFingerprintImage)
	);
	if (RetVal != AN2K_DLL_NO_ERROR)
	{
		ValToRet = RetVal;
		char ErrorString[100];
		GbAn2000Util_FormatErrorString(RetVal, ErrorString);
		sprintf(An2000_JNI_LastErrorString,
		        "CreateType15Record2011: Ptr_GB_AN2011_CreateType15Record returned %s (%d)",
		        ErrorString, RetVal);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		GBANUTIL_GetJRecordFromCRecord(NULL, jRecord, env);
	}
	else
	{
		GBANUTIL_GetJRecordFromCRecord(record, jRecord, env);
	}

	//////////////////////////////////
	// free memory
	//////////////////////////////////
	free(FingerprintImage);
	if (sourceAgency != NULL) free(sourceAgency);
	if (comment != NULL) free(comment);
	if (deviceMonitoringMode != NULL) free(deviceMonitoringMode);
	if (amp != NULL) free(amp);
	if (sqm != NULL) free(sqm);

	return ValToRet;
}


JNIEXPORT jint JNICALL
Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_CreateType14Record2011(
		JNIEnv *env,
		jobject, /* this */
		jobject jRecord, // MANDATORY
		jint jIdc, // MANDATORY
		jint jImpressionType, // MANDATORY
		jstring jSourceAgency, // MANDATORY
		jobject jFingerprintCaptureDate, // GB_AN2011_DateStruct, // MANDATORY
		jint jHorizontalNumberOfPixels, // MANDATORY
		jint jVerticalNumberOfPixels, // MANDATORY
		jint jScaleUnits, // MANDATORY
		jint jTransmittedHorizontalPixelScale, // MANDATORY
		jint jTransmittedVerticalPixelScale, // MANDATORY
		jint jCompressionAlgorithm, // MANDATORY
		jint jBitsPerPixel, // MANDATORY
		jint jFingerprintPosition, // MANDATORY
		jobject jPpd, // GB_AN2011_PrintPositionDescriptorsStruct // MANDATORY if FingerprintPosition == 19
		jobjectArray jPpc, // GB_AN2011_PrintPositionCoordinatesStruct[] // OPTIONAL
		jint jScannedHorizontalPixelScale, // OPTIONAL
		jint jScannedVerticalPixelScale, // OPTIONAL
		jobjectArray jAmp, // GB_AN2011_AmputatedBandagedCodeStruct[] // OPTIONAL
		jstring jComment, // OPTIONAL
		jobjectArray jSeg, // GB_AN2011_FingerSegmentPositionStruct[] // MANDATORY if FingerprintPosition in [13-15] or [40-50]
		jobjectArray jNqm, // GB_AN2011_NistQualityMetricStruct[] // OPTIONAL
		jobjectArray jSqm, // GB_AN2011_FingerOrSegmentQualityMetricStruct[] // OPTIONAL
		jobjectArray jFqm, // GB_AN2011_FingerOrSegmentQualityMetricStruct[] // OPTIONAL
		jobjectArray jAseg, // GB_AN2011_AlternateSegmentPositionStruct[] // OPTIONAL
		jint jSimultaneousCaptureFingerprint, // OPTIONAL
		jint jStitchedImageFlag, // boolean, // OPTIONAL
		jstring jDeviceMonitoringMode, // OPTIONAL
		jint jFingerprintAcquisitionProfile, // OPTIONAL
		jbyteArray jFingerprintImage // MANDATORY
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;
	int RetVal;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateType14Record2011: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GB_AN2011_CreateType14Record == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateType14Record2011: Ptr_GB_AN2011_CreateType14Record is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jRecord == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateType14Record2011: jRecord is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFingerprintImage == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateType14Record2011: jFingerprintImage is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jSourceAgency == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateType14Record2011: jSourceAgency is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFingerprintCaptureDate == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateType14Record2011: jFingerprintCaptureDate is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jHorizontalNumberOfPixels <= 0)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateType14Record2011: jHorizontalNumberOfPixels <= 0");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jVerticalNumberOfPixels <= 0)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateType14Record2011: jVerticalNumberOfPixels <= 0");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if ((env)->GetArrayLength(jFingerprintImage) <
	    (jHorizontalNumberOfPixels * jVerticalNumberOfPixels))
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateType14Record2011: jFingerprintImage too small");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFingerprintPosition == GB_TABLE_AN2011_8_EJI_OR_TIP)
	{
		if (jPpd == NULL)
		{
			ValToRet = AN2K_DLL_BAD_PARAMETER;
			sprintf(An2000_JNI_LastErrorString,
			        "CreateType14Record2011: jPpd is NULL");
			a_printf ("%s\n", An2000_JNI_LastErrorString);
			return ValToRet;
		}
	}
	if (
			((jFingerprintPosition >= GB_TABLE_AN2011_8_PLAIN_RIGHT_SLAP_4) &&
			 (jFingerprintPosition <= GB_TABLE_AN2011_8_PLAIN_LEFT_THUMBS_2)) ||
			((jFingerprintPosition >= GB_TABLE_AN2011_8_RIGHT_INDEX_MIDDLE) &&
			 (jFingerprintPosition <= GB_TABLE_AN2011_8_LEFT_MIDDLE_RING_LITTLE))
			)
	{
		if (jSeg == NULL)
		{
			ValToRet = AN2K_DLL_BAD_PARAMETER;
			sprintf(An2000_JNI_LastErrorString,
			        "CreateType14Record2011: jSeg is NULL");
			a_printf ("%s\n", An2000_JNI_LastErrorString);
			return ValToRet;
		}
	}

	//////////////////////////////////
	// check java class
	//////////////////////////////////

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	// record
	RECORD *record;

	// sourceAgency
	char *sourceAgency = NULL;
	if (jSourceAgency != NULL) GBANUTIL_GetStringFromJString(jSourceAgency, &sourceAgency, env);

	// sourceAgency
	char *comment = NULL;
	if (jComment != NULL) GBANUTIL_GetStringFromJString(jComment, &comment, env);

	// sourceAgency
	char *deviceMonitoringMode = NULL;
	if (jDeviceMonitoringMode != NULL)
		GBANUTIL_GetStringFromJString(jDeviceMonitoringMode, &deviceMonitoringMode, env);

	// FingerprintImage
	unsigned char *FingerprintImage = new unsigned char[jHorizontalNumberOfPixels *
	                                                    jVerticalNumberOfPixels];
	env->GetByteArrayRegion(jFingerprintImage, 0,
	                        jHorizontalNumberOfPixels * jVerticalNumberOfPixels,
	                        reinterpret_cast<jbyte *>(FingerprintImage));
	// fingerprintCaptureDate Date
	GB_AN2011_DateStruct fingerprintCaptureDate;
	GBAN2011UTIL_GetCDateFromJDate(env, jFingerprintCaptureDate, &fingerprintCaptureDate,
	                               An2000_JNI_LastErrorString);

	// stitchedImageFlag
	BOOL stitchedImageFlag = (jStitchedImageFlag != 0) ? TRUE : FALSE;

	// ppd
	GB_AN2011_PrintPositionDescriptorsStruct ppd;
	if (jPpd != NULL)
		GBAN2011UTIL_GetCPrintPositionDescFromJPrintPositionDesc(env, jPpd, &ppd,
		                                                         An2000_JNI_LastErrorString);

	// ppc
	GB_AN2011_PrintPositionCoordinatesStruct *ppc = NULL;
	int ppcLen = 0;
	if (jPpc != NULL)
	{
		if ((env)->GetArrayLength(jPpc) > 0)
		{
			ppcLen = (env)->GetArrayLength(jPpc);
			ppc = (GB_AN2011_PrintPositionCoordinatesStruct *) malloc(ppcLen *
			                                                          sizeof(GB_AN2011_PrintPositionCoordinatesStruct));
			for (int i = 0; i < ppcLen; i++)
			{
				jobject item = env->GetObjectArrayElement(jPpc, i);
				GBAN2011UTIL_GetCPrintPositionCoordFromJPrintPositionCoord(env, item, &(ppc[i]),
				                                                           An2000_JNI_LastErrorString);
			}
		}
	}

	// amp
	GB_AN2011_AmputatedBandagedCodeStruct *amp = NULL;
	int ampLen = 0;
	if (jAmp != NULL)
	{
		if ((env)->GetArrayLength(jAmp) > 0)
		{
			ampLen = (env)->GetArrayLength(jAmp);
			amp = (GB_AN2011_AmputatedBandagedCodeStruct *) malloc((env)->GetArrayLength(jAmp) *
			                                                       sizeof(GB_AN2011_AmputatedBandagedCodeStruct));
			for (int i = 0; i < (env)->GetArrayLength(jAmp); i++)
			{
				jobject item = env->GetObjectArrayElement(jAmp, i);
				GBAN2011UTIL_GetCAmputatedBandagedCodeFromJAmputatedBandagedCode(env, item,
				                                                                 &(amp[i]),
				                                                                 An2000_JNI_LastErrorString);
			}
		}
	}

	// seg
	GB_AN2011_FingerSegmentPositionStruct *seg = NULL;
	int segLen = 0;
	if (jSeg != NULL)
	{
		if ((env)->GetArrayLength(jSeg) > 0)
		{
			segLen = (env)->GetArrayLength(jSeg);
			seg = (GB_AN2011_FingerSegmentPositionStruct *) malloc((env)->GetArrayLength(jSeg) *
			                                                       sizeof(GB_AN2011_FingerSegmentPositionStruct));
			for (int i = 0; i < (env)->GetArrayLength(jSeg); i++)
			{
				jobject item = env->GetObjectArrayElement(jSeg, i);
				GBAN2011UTIL_GetCFingerSegmentPositionFromJFingerSegmentPosition(env, item,
				                                                                 &(seg[i]),
				                                                                 An2000_JNI_LastErrorString);
			}
		}
	}

	// nqm
	GB_AN2011_NistQualityMetricStruct *nqm = NULL;
	int nqmLen = 0;
	if (jNqm != NULL)
	{
		if ((env)->GetArrayLength(jNqm) > 0)
		{
			nqmLen = (env)->GetArrayLength(jNqm);
			nqm = (GB_AN2011_NistQualityMetricStruct *) malloc((env)->GetArrayLength(jNqm) *
			                                                   sizeof(GB_AN2011_NistQualityMetricStruct));
			for (int i = 0; i < (env)->GetArrayLength(jNqm); i++)
			{
				jobject item = env->GetObjectArrayElement(jNqm, i);
				GBAN2011UTIL_GetCNistQualityMetricFromJNistQualityMetric(env, item, &(nqm[i]),
				                                                         An2000_JNI_LastErrorString);
			}
		}
	}

	// sqm
	GB_AN2011_FingerOrSegmentQualityMetricStruct *sqm = NULL;
	int sqmLen = 0;
	if (jSqm != NULL)
	{
		if ((env)->GetArrayLength(jSqm) > 0)
		{
			sqmLen = (env)->GetArrayLength(jSqm);
			sqm = (GB_AN2011_FingerOrSegmentQualityMetricStruct *) malloc(
					(env)->GetArrayLength(jSqm) *
					sizeof(GB_AN2011_FingerOrSegmentQualityMetricStruct));
			for (int i = 0; i < (env)->GetArrayLength(jSqm); i++)
			{
				jobject item = env->GetObjectArrayElement(jSqm, i);
				GBAN2011UTIL_GetCFingOrSegQualityMetricFromJFingOrSegQualityMetric(env, item,
				                                                                   &(sqm[i]),
				                                                                   An2000_JNI_LastErrorString);
			}
		}
	}

	// sqm
	GB_AN2011_FingerOrSegmentQualityMetricStruct *fqm = NULL;
	int fqmLen = 0;
	if (jFqm != NULL)
	{
		if ((env)->GetArrayLength(jFqm) > 0)
		{
			fqmLen = (env)->GetArrayLength(jFqm);
			fqm = (GB_AN2011_FingerOrSegmentQualityMetricStruct *) malloc(
					(env)->GetArrayLength(jFqm) *
					sizeof(GB_AN2011_FingerOrSegmentQualityMetricStruct));
			for (int i = 0; i < (env)->GetArrayLength(jFqm); i++)
			{
				jobject item = env->GetObjectArrayElement(jFqm, i);
				GBAN2011UTIL_GetCFingOrSegQualityMetricFromJFingOrSegQualityMetric(env, item,
				                                                                   &(fqm[i]),
				                                                                   An2000_JNI_LastErrorString);
			}
		}
	}

	// aseg
	GB_AN2011_AlternateSegmentPositionStruct *aseg = NULL;
	int asegLen = 0;
	if (jAseg != NULL)
	{
		if ((env)->GetArrayLength(jAseg) > 0)
		{
			asegLen = (env)->GetArrayLength(jAseg);
			aseg = (GB_AN2011_AlternateSegmentPositionStruct *) malloc(
					(env)->GetArrayLength(jAseg) *
					sizeof(GB_AN2011_AlternateSegmentPositionStruct));
			for (int i = 0; i < (env)->GetArrayLength(jAseg); i++)
			{
				jobject item = env->GetObjectArrayElement(jAseg, i);
				GBAN2011UTIL_GetCAlternateQualityMetricFromJAlternateQualityMetric(env, item,
				                                                                   &(aseg[i]),
				                                                                   An2000_JNI_LastErrorString);
			}
		}
	}

	//////////////////////////////////
	// call function
	//////////////////////////////////
	RetVal = Ptr_GB_AN2011_CreateType14Record(
			&record,
			(int) jIdc,
			(int) jImpressionType,
			sourceAgency,
			&fingerprintCaptureDate,
			(int) jHorizontalNumberOfPixels, (int) jVerticalNumberOfPixels,
			(int) jScaleUnits, (int) jTransmittedHorizontalPixelScale,
			(int) jTransmittedVerticalPixelScale,
			(int) jCompressionAlgorithm,
			(int) jBitsPerPixel,
			(int) jFingerprintPosition,
			&ppd,
			ppc, ppcLen,
			(int) jScannedHorizontalPixelScale, (int) jScannedVerticalPixelScale,
			amp, ampLen,
			comment,
			seg, segLen,
			nqm, nqmLen,
			sqm, sqmLen,
			fqm, fqmLen,
			aseg, asegLen,
			(int) jSimultaneousCaptureFingerprint,
			stitchedImageFlag,
			deviceMonitoringMode,
			(int) jFingerprintAcquisitionProfile,
			FingerprintImage, env->GetArrayLength(jFingerprintImage)
	);
	if (RetVal != AN2K_DLL_NO_ERROR)
	{
		ValToRet = RetVal;
		char ErrorString[100];
		GbAn2000Util_FormatErrorString(RetVal, ErrorString);
		sprintf(An2000_JNI_LastErrorString,
		        "CreateType14Record2011: Ptr_GB_AN2011_CreateType14Record returned %s (%d)",
		        ErrorString, RetVal);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		GBANUTIL_GetJRecordFromCRecord(NULL, jRecord, env);
	}
	else
	{
		GBANUTIL_GetJRecordFromCRecord(record, jRecord, env);
	}

	//////////////////////////////////
	// free memory
	//////////////////////////////////
	free(FingerprintImage);
	if (sourceAgency != NULL) free(sourceAgency);
	if (comment != NULL) free(comment);
	if (deviceMonitoringMode != NULL) free(deviceMonitoringMode);
	if (ppc != NULL) free(ppc);
	if (amp != NULL) free(amp);
	if (seg != NULL) free(seg);
	if (nqm != NULL) free(nqm);
	if (sqm != NULL) free(sqm);
	if (fqm != NULL) free(fqm);
	if (aseg != NULL) free(aseg);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_ImageToType14Record(
		JNIEnv *env,
		jobject, /* this */
		jobject jRecord,
		jbyteArray jFingerprintImage,
		jint jFPImageWidth,
		jint jFPImageHeight,
		jdouble jScanResolutionPPMM,
		jstring jCompressionType,
		jint jImpressionType,
		jstring jImageSource,
		jstring jComment,
		jstring jFingerPosition,
		jobjectArray jAmputationList,
		jobjectArray jboundaries,
		jobjectArray jqualities,
		jobjectArray jsegqual,
		jobjectArray jaltqual
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;
	int RetVal;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "ImageToType14Record: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_AN2000_image2ETFStype_14 == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "ImageToType14Record: Ptr_AN2000_image2ETFStype_14 is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jRecord == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "ImageToType14Record: jRecord is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFingerprintImage == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "ImageToType14Record: jFingerprintImage is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFPImageWidth <= 0)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "ImageToType14Record: jFPImageWidth <= 0");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFPImageHeight <= 0)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "ImageToType14Record: jFPImageHeight <= 0");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if ((env)->GetArrayLength(jFingerprintImage) < (jFPImageWidth * jFPImageHeight))
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "ImageToType14Record: jFingerprintImage too small");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jCompressionType == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "ImageToType14Record: jCompressionType is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jImageSource == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "ImageToType14Record: jImageSource is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetStringUTFLength(jImageSource) >= MAX_SOURCE_STR_LEN)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "ImageToType14Record: jImageSource too long, can be max %d", MAX_SOURCE_STR_LEN);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jComment != NULL)
	{
		if (env->GetStringUTFLength(jComment) >= MAX_COMMENT_LEN)
		{
			ValToRet = AN2K_DLL_BAD_PARAMETER;
			sprintf(An2000_JNI_LastErrorString,
			        "ImageToType14Record: jComment too long, can be max %d", MAX_COMMENT_LEN);
			a_printf ("%s\n", An2000_JNI_LastErrorString);
			return ValToRet;
		}
	}
	if (jFingerPosition == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "ImageToType14Record: jFingerPosition is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check java class
	//////////////////////////////////

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	// record
	RECORD *record;

	// amputation list
	FING_AMP_LIST AmputationList, *pAmpList = NULL;
	if (jAmputationList != NULL)
	{
		RetVal = GBANUTIL_GetCAmpListFromJAmpStructArray(env, jAmputationList, &AmputationList,
		                                                 An2000_JNI_LastErrorString);
		if (RetVal == AN2K_DLL_NO_ERROR) pAmpList = &AmputationList;
		else
		{
			if (pAmpList != NULL) GBANUTIL_FreeAmpListFields(pAmpList);
			return RetVal;
		}
	}
	// boundaries
	FOUR_FING_BOUND boundaries, *pBoundaries = NULL;
	if (jboundaries != NULL)
	{
		RetVal = GBANUTIL_GetCSegmBoundsListFromJSegmBoundsStructArray(env, jboundaries,
		                                                               &boundaries,
		                                                               An2000_JNI_LastErrorString);
		if (RetVal == AN2K_DLL_NO_ERROR) pBoundaries = &boundaries;
		else
		{
			if (pAmpList != NULL) GBANUTIL_FreeAmpListFields(pAmpList);
			if (pBoundaries != NULL) GBANUTIL_FreeSegmBoundsFields(pBoundaries);
			return RetVal;
		}
	}
	// qualities
	FOUR_FING_QUALITY qualities, *pQualities = NULL;
	if (jqualities != NULL)
	{
		RetVal = GBANUTIL_GetCFingerQualityListFromJFingerQualityStructArray(env, jqualities,
		                                                                     &qualities,
		                                                                     An2000_JNI_LastErrorString);
		if (RetVal == AN2K_DLL_NO_ERROR) pQualities = &qualities;
		else
		{
			if (pAmpList != NULL) GBANUTIL_FreeAmpListFields(pAmpList);
			if (pBoundaries != NULL) GBANUTIL_FreeSegmBoundsFields(pBoundaries);
			if (pQualities != NULL) GBANUTIL_FreeFingerQualityFields(pQualities);
			return RetVal;
		}
	}
	// segment qualities
	ALT_QUAL_LIST segqual, *pSegqual = NULL;
	if (jsegqual != NULL)
	{
		RetVal = GBANUTIL_GetCAlternateFingerQualityListFromJFingerAlternateQualityStructArray(env,
		                                                                                       jsegqual,
		                                                                                       &segqual,
		                                                                                       An2000_JNI_LastErrorString);
		if (RetVal == AN2K_DLL_NO_ERROR) pSegqual = &segqual;
		else
		{
			if (pAmpList != NULL) GBANUTIL_FreeAmpListFields(pAmpList);
			if (pBoundaries != NULL) GBANUTIL_FreeSegmBoundsFields(pBoundaries);
			if (pQualities != NULL) GBANUTIL_FreeFingerQualityFields(pQualities);
			if (pSegqual != NULL) GBANUTIL_FreeAlternateFingerQualityFields(pSegqual);
			return RetVal;
		}
	}
	// alternate qualities
	ALT_QUAL_LIST altqual, *pAltqual = NULL;
	if (jaltqual != NULL)
	{
		RetVal = GBANUTIL_GetCAlternateFingerQualityListFromJFingerAlternateQualityStructArray(env,
		                                                                                       jaltqual,
		                                                                                       &altqual,
		                                                                                       An2000_JNI_LastErrorString);
		if (RetVal == AN2K_DLL_NO_ERROR) pAltqual = &altqual;
		else
		{
			if (pAmpList != NULL) GBANUTIL_FreeAmpListFields(pAmpList);
			if (pBoundaries != NULL) GBANUTIL_FreeSegmBoundsFields(pBoundaries);
			if (pQualities != NULL) GBANUTIL_FreeFingerQualityFields(pQualities);
			if (pSegqual != NULL) GBANUTIL_FreeAlternateFingerQualityFields(pSegqual);
			if (pAltqual != NULL) GBANUTIL_FreeAlternateFingerQualityFields(pAltqual);
			return RetVal;
		}
	}
	// image
	unsigned char *FingerprintImage = new unsigned char[jFPImageWidth * jFPImageHeight];
	env->GetByteArrayRegion(jFingerprintImage, 0, jFPImageWidth * jFPImageHeight,
	                        reinterpret_cast<jbyte *>(FingerprintImage));
	// compression type
	char *CompressionType;
	GBANUTIL_GetStringFromJString(jCompressionType, &CompressionType, env);
	// ImageSource
	char *ImageSource;
	GBANUTIL_GetStringFromJString(jImageSource, &ImageSource, env);
	// Comment
	char *Comment;
	if (jComment != NULL) GBANUTIL_GetStringFromJString(jComment, &Comment, env);
	else Comment = NULL;
	// FingerPosition
	char *FingerPosition;
	GBANUTIL_GetStringFromJString(jFingerPosition, &FingerPosition, env);

	//////////////////////////////////
	// call function
	//////////////////////////////////
	RetVal = Ptr_AN2000_image2ETFStype_14(
			&record,
			FingerprintImage,
			(int) (jFPImageWidth * jFPImageHeight),
			(int) jFPImageWidth, (int) jFPImageHeight,
			(int) 8, jScanResolutionPPMM, CompressionType,
			(int) jImpressionType,
			ImageSource,
			Comment,
			FingerPosition,
			pAmpList, pBoundaries, pQualities, pSegqual, pAltqual
	);
	if (RetVal != AN2K_DLL_NO_ERROR)
	{
		ValToRet = RetVal;
		char ErrorString[100];
		GbAn2000Util_FormatErrorString(RetVal, ErrorString);
		sprintf(An2000_JNI_LastErrorString,
		        "ImageToType14Record: Ptr_AN2000_image2ETFStype_14 returned %s (%d)", ErrorString,
		        RetVal);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		GBANUTIL_GetJRecordFromCRecord(NULL, jRecord, env);
	}
	else
	{
		GBANUTIL_GetJRecordFromCRecord(record, jRecord, env);
	}

	//////////////////////////////////
	// free memory
	//////////////////////////////////
	free(FingerprintImage);
	free(CompressionType);
	free(ImageSource);
	free(FingerPosition);
	if (pAmpList != NULL) GBANUTIL_FreeAmpListFields(pAmpList);
	if (pBoundaries != NULL) GBANUTIL_FreeSegmBoundsFields(pBoundaries);
	if (pQualities != NULL) GBANUTIL_FreeFingerQualityFields(pQualities);
	if (pSegqual != NULL) GBANUTIL_FreeAlternateFingerQualityFields(pSegqual);
	if (pAltqual != NULL) GBANUTIL_FreeAlternateFingerQualityFields(pAltqual);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_ImageToType15Record(
		JNIEnv *env,
		jobject, /* this */
		jobject jRecord,
		jbyteArray jPPImage,
		jint jPPImageWidth,
		jint jPPImageHeight,
		jdouble jScanResolutionPPMM,
		jstring jCompressionType,
		jint jImpressionType,
		jstring jImageSource,
		jstring jComment,
		jstring jPalmPosition,
		jobjectArray jPalmQualitiesList
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;
	int RetVal;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "ImageToType15Record: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_AN2000_image2ETFStype_15 == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "ImageToType15Record: Ptr_AN2000_image2ETFStype_15 is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jRecord == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "ImageToType15Record: jRecord is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jPPImage == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "ImageToType15Record: jPPImage is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jPPImageWidth <= 0)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "ImageToType15Record: jPPImageWidth <= 0");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jPPImageHeight <= 0)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "ImageToType15Record: jPPImageHeight <= 0");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if ((env)->GetArrayLength(jPPImage) < (jPPImageWidth * jPPImageHeight))
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "ImageToType15Record: jPPImage too small");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jCompressionType == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "ImageToType15Record: jCompressionType is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jImageSource == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "ImageToType15Record: jImageSource is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetStringUTFLength(jImageSource) >= MAX_SOURCE_STR_LEN)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "ImageToType15Record: jImageSource too long, can be max %d", MAX_SOURCE_STR_LEN);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jComment != NULL)
	{
		if (env->GetStringUTFLength(jComment) >= MAX_COMMENT_LEN)
		{
			ValToRet = AN2K_DLL_BAD_PARAMETER;
			sprintf(An2000_JNI_LastErrorString,
			        "ImageToType15Record: jComment too long, can be max %d", MAX_COMMENT_LEN);
			a_printf ("%s\n", An2000_JNI_LastErrorString);
			return ValToRet;
		}
	}
	if (jPalmPosition == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "ImageToType15Record: jPalmPosition is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check java class
	//////////////////////////////////

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	// record
	RECORD *record;

	// amputation list
	PALM_QUAL_LIST PalmQualitiesList, *pPalmQualitiesList = NULL;
	if (jPalmQualitiesList != NULL)
	{
		RetVal = GBANUTIL_GetCPalmQualityListFromJPalmQualityStructArray(env, jPalmQualitiesList,
		                                                                 &PalmQualitiesList,
		                                                                 An2000_JNI_LastErrorString);
		if (RetVal == AN2K_DLL_NO_ERROR) pPalmQualitiesList = &PalmQualitiesList;
		else
		{
			if (pPalmQualitiesList != NULL) GBANUTIL_FreePalmQualityFields(pPalmQualitiesList);
			return RetVal;
		}
	}
	// image
	unsigned char *PPImage = new unsigned char[jPPImageWidth * jPPImageHeight];
	env->GetByteArrayRegion(jPPImage, 0, jPPImageWidth * jPPImageHeight,
	                        reinterpret_cast<jbyte *>(PPImage));
	// compression type
	char *CompressionType;
	GBANUTIL_GetStringFromJString(jCompressionType, &CompressionType, env);
	// ImageSource
	char *ImageSource;
	GBANUTIL_GetStringFromJString(jImageSource, &ImageSource, env);
	// Comment
	char *Comment;
	if (jComment != NULL) GBANUTIL_GetStringFromJString(jComment, &Comment, env);
	else Comment = NULL;
	// PalmPosition
	char *PalmPosition;
	GBANUTIL_GetStringFromJString(jPalmPosition, &PalmPosition, env);

	//////////////////////////////////
	// call function
	//////////////////////////////////
	RetVal = Ptr_AN2000_image2ETFStype_15(
			&record,
			PPImage,
			(int) (jPPImageWidth * jPPImageHeight),
			(int) jPPImageWidth, (int) jPPImageHeight,
			(int) 8, jScanResolutionPPMM, CompressionType,
			(int) jImpressionType,
			ImageSource,
			Comment,
			PalmPosition,
			pPalmQualitiesList
	);
	if (RetVal != AN2K_DLL_NO_ERROR)
	{
		ValToRet = RetVal;
		char ErrorString[100];
		GbAn2000Util_FormatErrorString(RetVal, ErrorString);
		sprintf(An2000_JNI_LastErrorString,
		        "ImageToType15Record: Ptr_AN2000_image2ETFStype_15 returned %s (%d)", ErrorString,
		        RetVal);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		GBANUTIL_GetJRecordFromCRecord(NULL, jRecord, env);
	}
	else
	{
		GBANUTIL_GetJRecordFromCRecord(record, jRecord, env);
	}

	//////////////////////////////////
	// free memory
	//////////////////////////////////
	free(PPImage);
	free(CompressionType);
	free(ImageSource);
	free(PalmPosition);
	if (pPalmQualitiesList != NULL) GBANUTIL_FreePalmQualityFields(pPalmQualitiesList);

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_CreateType4Record2011(
		JNIEnv *env,
		jobject, /* this */
		jobject jRecord,
		jint jIdc,
		jint jImpressionType,
		jint jFingerPosition,
		jint jScanResolutionId,
		jint jFPImageWidth,
		jint jFPImageHeight,
		jint jCompressionType,
		jbyteArray jFingerprintImage
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;
	int RetVal;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateType4Record2011: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GB_AN2011_CreateType4Record == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateType4Record2011: Ptr_GB_AN2011_CreateType4Record is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jRecord == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateType4Record2011: jRecord is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFingerprintImage == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateType4Record2011: jFingerprintImage is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFPImageWidth <= 0)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateType4Record2011: jFPImageWidth <= 0");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFPImageHeight <= 0)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateType4Record2011: jFPImageHeight <= 0");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if ((env)->GetArrayLength(jFingerprintImage) < (jFPImageWidth * jFPImageHeight))
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateType4Record2011: jFingerprintImage too small");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check java class
	//////////////////////////////////

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	// record
	RECORD *record;
	// image
	unsigned char *FingerprintImage = new unsigned char[jFPImageWidth * jFPImageHeight];
	env->GetByteArrayRegion(jFingerprintImage, 0, jFPImageWidth * jFPImageHeight,
	                        reinterpret_cast<jbyte *>(FingerprintImage));

	//////////////////////////////////
	// call function
	//////////////////////////////////
	RetVal = Ptr_GB_AN2011_CreateType4Record(
			&record,
			(int) jIdc,
			(int) jImpressionType,
			(unsigned char) jFingerPosition,
			(int) jScanResolutionId,
			(int) jFPImageWidth, (int) jFPImageHeight,
			(int) jCompressionType,
			FingerprintImage,
			(int) (jFPImageWidth * jFPImageHeight)

	);
	if (RetVal != AN2K_DLL_NO_ERROR)
	{
		ValToRet = RetVal;
		char ErrorString[100];
		GbAn2000Util_FormatErrorString(RetVal, ErrorString);
		sprintf(An2000_JNI_LastErrorString,
		        "CreateType4Record2011: Ptr_GB_AN2011_CreateType4Record returned %s (%d)",
		        ErrorString, RetVal);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		GBANUTIL_GetJRecordFromCRecord(NULL, jRecord, env);
	}
	else
	{
		GBANUTIL_GetJRecordFromCRecord(record, jRecord, env);
	}

	//////////////////////////////////
	// free memory
	//////////////////////////////////
	free(FingerprintImage);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_ImageToType4Record(
		JNIEnv *env,
		jobject, /* this */
		jobject jRecord,
		jbyteArray jFingerprintImage,
		jint jFPImageWidth,
		jint jFPImageHeight,
		jint jScanResolutionId,
		jstring jCompressionType,
		jint jImpressionType,
		jint jFingerPosition
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;
	int RetVal;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "ImageToType4Record: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_AN2000_image2ETFStype_4 == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "ImageToType4Record: Ptr_AN2000_image2ETFStype_4 is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jRecord == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "ImageToType4Record: jRecord is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFingerprintImage == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "ImageToType4Record: jFingerprintImage is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFPImageWidth <= 0)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "ImageToType4Record: jFPImageWidth <= 0");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFPImageHeight <= 0)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "ImageToType4Record: jFPImageHeight <= 0");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if ((env)->GetArrayLength(jFingerprintImage) < (jFPImageWidth * jFPImageHeight))
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "ImageToType4Record: jFingerprintImage too small");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jCompressionType == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "ImageToType4Record: jCompressionType is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check java class
	//////////////////////////////////

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	// record
	RECORD *record;
	// image
	unsigned char *FingerprintImage = new unsigned char[jFPImageWidth * jFPImageHeight];
	env->GetByteArrayRegion(jFingerprintImage, 0, jFPImageWidth * jFPImageHeight,
	                        reinterpret_cast<jbyte *>(FingerprintImage));
	// compression type
	char *CompressionType;
	GBANUTIL_GetStringFromJString(jCompressionType, &CompressionType, env);

	//////////////////////////////////
	// call function
	//////////////////////////////////
	RetVal = Ptr_AN2000_image2ETFStype_4(
			&record,
			FingerprintImage,
			(int) (jFPImageWidth * jFPImageHeight),
			(int) jImpressionType,
			(int) jFingerPosition,
			(int) jScanResolutionId,
			(int) jFPImageWidth, (int) jFPImageHeight,
			CompressionType
	);
	if (RetVal != AN2K_DLL_NO_ERROR)
	{
		ValToRet = RetVal;
		char ErrorString[100];
		GbAn2000Util_FormatErrorString(RetVal, ErrorString);
		sprintf(An2000_JNI_LastErrorString,
		        "ImageToType4Record: Ptr_AN2000_image2ETFStype_4 returned %s (%d)", ErrorString,
		        RetVal);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		GBANUTIL_GetJRecordFromCRecord(NULL, jRecord, env);
	}
	else
	{
		GBANUTIL_GetJRecordFromCRecord(record, jRecord, env);
	}

	//////////////////////////////////
	// free memory
	//////////////////////////////////
	free(FingerprintImage);
	free(CompressionType);

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_UpdateType1415ScaleUnits(
		JNIEnv *env,
		jobject, /* this */
		jobject jRecord,
		jint jNewScale
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;
	int RetVal;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "UpdateType1415ScaleUnits: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_AN2000_UpdateType1415ScaleUnits == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "UpdateType1415ScaleUnits: Ptr_AN2000_UpdateType1415ScaleUnits is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jRecord == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "UpdateType1415ScaleUnits: jRecord is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check java class
	//////////////////////////////////

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	// record
	RECORD *record;
	GBANUTIL_GetCRecordFromJRecord(jRecord, &record, env);
	if (record == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "UpdateType1415ScaleUnits: Record stored into jRecord is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// call function
	//////////////////////////////////
	RetVal = Ptr_AN2000_UpdateType1415ScaleUnits(
			record,
			(int) jNewScale
	);
	if (RetVal != AN2K_DLL_NO_ERROR)
	{
		ValToRet = RetVal;
		char ErrorString[100];
		GbAn2000Util_FormatErrorString(RetVal, ErrorString);
		sprintf(An2000_JNI_LastErrorString,
		        "UpdateType1415ScaleUnits: Ptr_AN2000_UpdateType1415ScaleUnits returned %s (%d)",
		        ErrorString, RetVal);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
	}

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_UpdateType14Record(
		JNIEnv *env,
		jobject, /* this */
		jobject jRecord,
		jbyteArray jFingerprintImage,
		jint jFPImageWidth,
		jint jFPImageHeight,
		jdouble jScanResolutionPPMM,
		jobjectArray jAmputationList,
		jobjectArray jboundaries,
		jobjectArray jqualities,
		jobjectArray jsegqual,
		jobjectArray jaltqual
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;
	int RetVal;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "UpdateType14Record: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_AN2000_UpdateETFStype_14 == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "UpdateType14Record: Ptr_AN2000_UpdateETFStype_14 is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jRecord == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "UpdateType14Record: jRecord is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFingerprintImage == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "UpdateType14Record: jFingerprintImage is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFPImageWidth <= 0)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "UpdateType14Record: jFPImageWidth <= 0");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFPImageHeight <= 0)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "UpdateType14Record: jFPImageHeight <= 0");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if ((env)->GetArrayLength(jFingerprintImage) < (jFPImageWidth * jFPImageHeight))
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "UpdateType14Record: jFingerprintImage too small");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check java class
	//////////////////////////////////

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	// record
	RECORD *record;
	GBANUTIL_GetCRecordFromJRecord(jRecord, &record, env);
	if (record == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "UpdateType14Record: Record stored into jRecord is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	// amputation list
	FING_AMP_LIST AmputationList, *pAmpList = NULL;
	if (jAmputationList != NULL)
	{
		RetVal = GBANUTIL_GetCAmpListFromJAmpStructArray(env, jAmputationList, &AmputationList,
		                                                 An2000_JNI_LastErrorString);
		if (RetVal == AN2K_DLL_NO_ERROR) pAmpList = &AmputationList;
		else
		{
			if (pAmpList != NULL) GBANUTIL_FreeAmpListFields(pAmpList);
			return RetVal;
		}
	}
	// boundaries
	FOUR_FING_BOUND boundaries, *pBoundaries = NULL;
	if (jboundaries != NULL)
	{
		RetVal = GBANUTIL_GetCSegmBoundsListFromJSegmBoundsStructArray(env, jboundaries,
		                                                               &boundaries,
		                                                               An2000_JNI_LastErrorString);
		if (RetVal == AN2K_DLL_NO_ERROR) pBoundaries = &boundaries;
		else
		{
			if (pAmpList != NULL) GBANUTIL_FreeAmpListFields(pAmpList);
			if (pBoundaries != NULL) GBANUTIL_FreeSegmBoundsFields(pBoundaries);
			return RetVal;
		}
	}
	// qualities
	FOUR_FING_QUALITY qualities, *pQualities = NULL;
	if (jqualities != NULL)
	{
		RetVal = GBANUTIL_GetCFingerQualityListFromJFingerQualityStructArray(env, jqualities,
		                                                                     &qualities,
		                                                                     An2000_JNI_LastErrorString);
		if (RetVal == AN2K_DLL_NO_ERROR) pQualities = &qualities;
		else
		{
			if (pAmpList != NULL) GBANUTIL_FreeAmpListFields(pAmpList);
			if (pBoundaries != NULL) GBANUTIL_FreeSegmBoundsFields(pBoundaries);
			if (pQualities != NULL) GBANUTIL_FreeFingerQualityFields(pQualities);
			return RetVal;
		}
	}
	// segment qualities
	ALT_QUAL_LIST segqual, *pSegqual = NULL;
	if (jsegqual != NULL)
	{
		RetVal = GBANUTIL_GetCAlternateFingerQualityListFromJFingerAlternateQualityStructArray(env,
		                                                                                       jsegqual,
		                                                                                       &segqual,
		                                                                                       An2000_JNI_LastErrorString);
		if (RetVal == AN2K_DLL_NO_ERROR) pSegqual = &segqual;
		else
		{
			if (pAmpList != NULL) GBANUTIL_FreeAmpListFields(pAmpList);
			if (pBoundaries != NULL) GBANUTIL_FreeSegmBoundsFields(pBoundaries);
			if (pQualities != NULL) GBANUTIL_FreeFingerQualityFields(pQualities);
			if (pSegqual != NULL) GBANUTIL_FreeAlternateFingerQualityFields(pSegqual);
			return RetVal;
		}
	}
	// alternate qualities
	ALT_QUAL_LIST altqual, *pAltqual = NULL;
	if (jaltqual != NULL)
	{
		RetVal = GBANUTIL_GetCAlternateFingerQualityListFromJFingerAlternateQualityStructArray(env,
		                                                                                       jaltqual,
		                                                                                       &altqual,
		                                                                                       An2000_JNI_LastErrorString);
		if (RetVal == AN2K_DLL_NO_ERROR) pAltqual = &altqual;
		else
		{
			if (pAmpList != NULL) GBANUTIL_FreeAmpListFields(pAmpList);
			if (pBoundaries != NULL) GBANUTIL_FreeSegmBoundsFields(pBoundaries);
			if (pQualities != NULL) GBANUTIL_FreeFingerQualityFields(pQualities);
			if (pSegqual != NULL) GBANUTIL_FreeAlternateFingerQualityFields(pSegqual);
			if (pAltqual != NULL) GBANUTIL_FreeAlternateFingerQualityFields(pAltqual);
			return RetVal;
		}
	}
	// image
	unsigned char *FingerprintImage = new unsigned char[jFPImageWidth * jFPImageHeight];
	env->GetByteArrayRegion(jFingerprintImage, 0, jFPImageWidth * jFPImageHeight,
	                        reinterpret_cast<jbyte *>(FingerprintImage));

	//////////////////////////////////
	// call function
	//////////////////////////////////
	RetVal = Ptr_AN2000_UpdateETFStype_14(
			record,
			FingerprintImage,
			(int) (jFPImageWidth * jFPImageHeight),
			(int) jFPImageWidth, (int) jFPImageHeight,
			jScanResolutionPPMM,
			pAmpList, pBoundaries, pQualities, pSegqual, pAltqual
	);
	if (RetVal != AN2K_DLL_NO_ERROR)
	{
		ValToRet = RetVal;
		char ErrorString[100];
		GbAn2000Util_FormatErrorString(RetVal, ErrorString);
		sprintf(An2000_JNI_LastErrorString,
		        "UpdateType14Record: Ptr_AN2000_UpdateETFStype_14 returned %s (%d)", ErrorString,
		        RetVal);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
	}

	//////////////////////////////////
	// free memory
	//////////////////////////////////
	free(FingerprintImage);
	if (pAmpList != NULL) GBANUTIL_FreeAmpListFields(pAmpList);
	if (pBoundaries != NULL) GBANUTIL_FreeSegmBoundsFields(pBoundaries);
	if (pQualities != NULL) GBANUTIL_FreeFingerQualityFields(pQualities);
	if (pSegqual != NULL) GBANUTIL_FreeAlternateFingerQualityFields(pSegqual);
	if (pAltqual != NULL) GBANUTIL_FreeAlternateFingerQualityFields(pAltqual);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_UpdateType15Record(
		JNIEnv *env,
		jobject, /* this */
		jobject jRecord,
		jbyteArray jPPImage,
		jint jPPImageWidth,
		jint jPPImageHeight,
		jdouble jScanResolutionPPMM,
		jobjectArray jPalmQualitiesList
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;
	int RetVal;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "UpdateType15Record: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_AN2000_UpdateETFStype_15 == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "UpdateType15Record: Ptr_AN2000_UpdateETFStype_15 is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jRecord == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "UpdateType15Record: jRecord is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jPPImage == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "UpdateType15Record: jPPImage is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jPPImageWidth <= 0)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "UpdateType15Record: jPPImageWidth <= 0");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jPPImageHeight <= 0)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "UpdateType15Record: jPPImageHeight <= 0");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if ((env)->GetArrayLength(jPPImage) < (jPPImageWidth * jPPImageHeight))
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "UpdateType15Record: jPPImage too small");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check java class
	//////////////////////////////////

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	// record
	RECORD *record;
	GBANUTIL_GetCRecordFromJRecord(jRecord, &record, env);
	if (record == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "UpdateType15Record: Record stored into jRecord is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	// amputation list
	PALM_QUAL_LIST PalmQualitiesList, *pPalmQualitiesList = NULL;
	if (jPalmQualitiesList != NULL)
	{
		RetVal = GBANUTIL_GetCPalmQualityListFromJPalmQualityStructArray(env, jPalmQualitiesList,
		                                                                 &PalmQualitiesList,
		                                                                 An2000_JNI_LastErrorString);
		if (RetVal == AN2K_DLL_NO_ERROR) pPalmQualitiesList = &PalmQualitiesList;
		else
		{
			if (pPalmQualitiesList != NULL) GBANUTIL_FreePalmQualityFields(pPalmQualitiesList);
			return RetVal;
		}
	}
	// image
	unsigned char *PPImage = new unsigned char[jPPImageWidth * jPPImageHeight];
	env->GetByteArrayRegion(jPPImage, 0, jPPImageWidth * jPPImageHeight,
	                        reinterpret_cast<jbyte *>(PPImage));

	//////////////////////////////////
	// call function
	//////////////////////////////////
	RetVal = Ptr_AN2000_UpdateETFStype_15(
			record,
			PPImage,
			(int) (jPPImageWidth * jPPImageHeight),
			(int) jPPImageWidth, (int) jPPImageHeight,
			jScanResolutionPPMM,
			pPalmQualitiesList
	);
	if (RetVal != AN2K_DLL_NO_ERROR)
	{
		ValToRet = RetVal;
		char ErrorString[100];
		GbAn2000Util_FormatErrorString(RetVal, ErrorString);
		sprintf(An2000_JNI_LastErrorString,
		        "UpdateType15Record: Ptr_AN2000_UpdateETFStype_15 returned %s (%d)", ErrorString,
		        RetVal);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
	}

	//////////////////////////////////
	// free memory
	//////////////////////////////////
	free(PPImage);
	if (pPalmQualitiesList != NULL) GBANUTIL_FreePalmQualityFields(pPalmQualitiesList);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_UpdateType4Record(
		JNIEnv *env,
		jobject, /* this */
		jobject jRecord,
		jbyteArray jFingerprintImage,
		jint jFPImageWidth,
		jint jFPImageHeight,
		jdouble jScanResolutionId
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;
	int RetVal;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "UpdateType4Record: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_AN2000_UpdateETFStype_4 == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "UpdateType4Record: Ptr_AN2000_UpdateETFStype_4 is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jRecord == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "UpdateType4Record: jRecord is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFingerprintImage == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "UpdateType4Record: jFingerprintImage is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFPImageWidth <= 0)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "UpdateType4Record: jFPImageWidth <= 0");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFPImageHeight <= 0)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "UpdateType4Record: jFPImageHeight <= 0");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if ((env)->GetArrayLength(jFingerprintImage) < (jFPImageWidth * jFPImageHeight))
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "UpdateType4Record: jFingerprintImage too small");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check java class
	//////////////////////////////////

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	// record
	RECORD *record;
	GBANUTIL_GetCRecordFromJRecord(jRecord, &record, env);
	if (record == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "UpdateType4Record: Record stored into jRecord is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	// image
	unsigned char *FingerprintImage = new unsigned char[jFPImageWidth * jFPImageHeight];
	env->GetByteArrayRegion(jFingerprintImage, 0, jFPImageWidth * jFPImageHeight,
	                        reinterpret_cast<jbyte *>(FingerprintImage));

	//////////////////////////////////
	// call function
	//////////////////////////////////
	RetVal = Ptr_AN2000_UpdateETFStype_4(
			record,
			FingerprintImage,
			(int) (jFPImageWidth * jFPImageHeight),
			(int) jScanResolutionId,
			(int) jFPImageWidth, (int) jFPImageHeight
	);
	if (RetVal != AN2K_DLL_NO_ERROR)
	{
		ValToRet = RetVal;
		char ErrorString[100];
		GbAn2000Util_FormatErrorString(RetVal, ErrorString);
		sprintf(An2000_JNI_LastErrorString,
		        "UpdateType4Record: Ptr_AN2000_UpdateETFStype_4 returned %s (%d)", ErrorString,
		        RetVal);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
	}

	//////////////////////////////////
	// free memory
	//////////////////////////////////
	free(FingerprintImage);

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_UpdateType1RecordInfoInAnsiNist(
		JNIEnv *env,
		jobject, /* this */
		jobject jAnsiNist,
		jdouble jNominalScanningResolution,
		jdouble jNominalTransmitResolution
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "UpdateType1RecordInfoInAnsiNist: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_AN2000_UpdateETFStype_1 == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "UpdateType1RecordInfoInAnsiNist: Ptr_AN2000_UpdateETFStype_1 is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jAnsiNist == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "UpdateType1RecordInfoInAnsiNist: jAnsiNist is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check java class
	//////////////////////////////////

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	ANSI_NIST *AnsiNist;
	GBANUTIL_GetCAnsiNistFromJAnsiNist(jAnsiNist, &AnsiNist, env);

	if (AnsiNist == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist: AnsiNist struct contains a NULL pointer");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if ((AnsiNist->num_records <= 0) || (AnsiNist->records[0]->type != TYPE_1_ID))
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist: AnsiNist struct does not contain Type1 records");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// call function
	//////////////////////////////////
	RECORD *record = AnsiNist->records[0];
	int RetVal = Ptr_AN2000_UpdateETFStype_1(
			record,
			(float) jNominalScanningResolution,
			(float) jNominalTransmitResolution
	);
	if (RetVal != AN2K_DLL_NO_ERROR)
	{
		ValToRet = RetVal;
		char ErrorString[100];
		GbAn2000Util_FormatErrorString(RetVal, ErrorString);
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist: Ptr_AN2000_UpdateETFStype_1 returned %s (%d)",
		        ErrorString, RetVal);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
	}

	//////////////////////////////////
	// free memory
	//////////////////////////////////

	return ValToRet;
}


JNIEXPORT jint JNICALL
Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_WriteAnsiNistToBuffer(
		JNIEnv *env,
		jobject, /* this */
		//---------------------------
		// INPUT
		//---------------------------
		jobject jAnsiNist,
		//---------------------------
		// Output
		//---------------------------
		jobject jOutBuffer
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "WriteAnsiNistToBuffer: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_AN2000_write_ANSI_NIST_2_buffer == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "WriteAnsiNistToBuffer: Ptr_AN2000_write_ANSI_NIST_2_buffer is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jAnsiNist == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "WriteAnsiNistToBuffer: jAnsiNist is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jOutBuffer == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "WriteAnsiNistToBuffer: jOutBuffer is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check java class
	//////////////////////////////////

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	ANSI_NIST *AnsiNist;
	GBANUTIL_GetCAnsiNistFromJAnsiNist(jAnsiNist, &AnsiNist, env);

	if (AnsiNist == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "WriteAnsiNistToBuffer: AnsiNist struct contains a NULL pointer");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// call function
	//////////////////////////////////
	unsigned char *OutBuffer;
	int BuffLen;
	int RetVal = Ptr_AN2000_write_ANSI_NIST_2_buffer(
			&OutBuffer, &BuffLen,
			AnsiNist
	);
	if (RetVal != AN2K_DLL_NO_ERROR)
	{
		ValToRet = RetVal;
		char ErrorString[100];
		GbAn2000Util_FormatErrorString(RetVal, ErrorString);
		sprintf(An2000_JNI_LastErrorString,
		        "WriteAnsiNistToBuffer: Ptr_AN2000_write_ANSI_NIST_2_buffer returned %s (%d)",
		        ErrorString, RetVal);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// copy values
	//////////////////////////////////
	GBANUTIL_GetJBufferFromCBuffer(OutBuffer, BuffLen, jOutBuffer, env);

	//////////////////////////////////
	// free memory
	//////////////////////////////////
	Ptr_AN2000_free_ANSI_NIST_buffer(OutBuffer);

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_GetType1InfoFromAnsiNist2011(
		JNIEnv *env,
		jobject, /* this */
		jobject jAnsiNist,
		jobject jVersion, // int
		jobject jTypeOfTransaction, // string
		jobject jDat, // GB_AN2011_DateStruct
		jobject jPriority, // int
		jobject jDestAgency, // string
		jobject jOrigAgency, // string
		jobject jTransContrNum, // string
		jobject jTransContrRef, // string
		jobject jNativeScanRes, // float
		jobject jNominalTxRes, // float
		jobject jDomainName, // string
		jobject jDomainVersion, // string
		jobject jGmt, // GB_AN2011_GmtStruct
		jobject jCharEncIndex,
		jobject jCharEncName, // string
		jobject jCharEncVersion, // string
		jobjectArray jAps, // GB_AN2011_ApplicationProfileStruct, 99
		jobject jApsNum, // int
		jobject jDestAgName, // string
		jobject jOrigAgName // string
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist2011: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GB_AN2011_GetType1RecordInfo == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist2011: Ptr_GB_AN2011_GetType1RecordInfo is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GB_AN_ENGINE_FreeGenericMem == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist2011: Ptr_GB_AN_ENGINE_FreeGenericMem is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jAnsiNist == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist2011: jAnsiNist is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jVersion == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist2011: jVersion is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jTypeOfTransaction == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist2011: jTypeOfTransaction is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jPriority == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist2011: jPriority is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jDat == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist2011: jDat is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jDestAgency == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist2011: jDestAgency is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jOrigAgency == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist2011: jOrigAgency is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jTransContrNum == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist2011: jTransContrNum is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jTransContrRef == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist2011: jTransContrRef is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jNativeScanRes == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist2011: jNativeScanRes is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jNominalTxRes == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist2011: jNominalTxRes is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jDomainName == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist2011: jDomainName is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jDomainVersion == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist2011: jDomainVersion is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jGmt == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist2011: jGmt is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jCharEncIndex == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist2011: jCharEncIndex is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jCharEncName == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist2011: jCharEncName is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jCharEncVersion == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist2011: jCharEncVersion is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jAps == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist2011: jAps is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jApsNum == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist2011: jApsNum is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jOrigAgName == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist2011: jOrigAgName is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jDestAgName == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist2011: jDestAgName is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check java class
	//////////////////////////////////

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	ANSI_NIST *AnsiNist;
	GBANUTIL_GetCAnsiNistFromJAnsiNist(jAnsiNist, &AnsiNist, env);
	if (AnsiNist == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist2011: AnsiNist struct contains a NULL pointer");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if ((AnsiNist->num_records <= 0) || (AnsiNist->records[0]->type != TYPE_1_ID))
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist2011: AnsiNist struct does not contain Type1 records");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// call function
	//////////////////////////////////
	RECORD *record = AnsiNist->records[0];
	int Version;
	GB_AN_ENGINE_String TypeOfTransaction;
	GB_AN2011_DateStruct Dat;
	int Priority;
	GB_AN_ENGINE_String DestAgency;
	GB_AN_ENGINE_String OrigAgency;
	GB_AN_ENGINE_String TransContrNum;
	GB_AN_ENGINE_String TransContrRef;
	float NativeScanRes;
	float NominalTxRes;
	GB_AN_ENGINE_String DomainName;
	GB_AN_ENGINE_String DomainVersion;
	GB_AN2011_GmtStruct Gmt;
	int CharEncIndex;
	GB_AN_ENGINE_String CharEncName;
	GB_AN_ENGINE_String CharEncVersion;
	GB_AN2011_ApplicationProfileStruct *Aps;
	int ApsNum;
	GB_AN_ENGINE_String DestAgName;
	GB_AN_ENGINE_String OrigAgName;
	int RetVal = Ptr_GB_AN2011_GetType1RecordInfo(
			record,
			&Version,
			&TypeOfTransaction,
			&Dat,
			&Priority,
			&DestAgency,
			&OrigAgency,
			&TransContrNum,
			&TransContrRef,
			&NativeScanRes,
			&NominalTxRes,
			&DomainName,
			&DomainVersion,
			&Gmt,
			&CharEncIndex,
			&CharEncName,
			&CharEncVersion,
			&Aps,
			&ApsNum,
			&DestAgName,
			&OrigAgName
	);
	//////////////////////////////////
	// copy parameters
	//////////////////////////////////
	if (RetVal != AN2K_DLL_NO_ERROR)
	{
		ValToRet = RetVal;
		char ErrorString[100];
		GbAn2000Util_FormatErrorString(RetVal, ErrorString);
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist2011: Ptr_AN2000_GetEFTSType1_Info returned %s (%d)",
		        ErrorString, RetVal);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
	}
	else
	{
		GBANUTIL_GetJIntExchangeFromCInt(Version, jVersion, env);
		GBANUTIL_GetJIntExchangeFromCInt(Priority, jPriority, env);
		GBANUTIL_GetJIntExchangeFromCInt(ApsNum, jApsNum, env);
		GBANUTIL_GetJIntExchangeFromCInt(CharEncIndex, jCharEncIndex, env);
		GBANUTIL_GetJDoubleExchangeFromCDouble(NativeScanRes, jNativeScanRes, env);
		GBANUTIL_GetJDoubleExchangeFromCDouble(NominalTxRes, jNominalTxRes, env);

		if (TypeOfTransaction != NULL)
			GBANUTIL_GetJStringExchangeFromCString(TypeOfTransaction, jTypeOfTransaction, env);
		else GBANUTIL_GetJStringExchangeFromCString((char *)"", jTypeOfTransaction, env);
		if (DestAgency != NULL)
			GBANUTIL_GetJStringExchangeFromCString(DestAgency, jDestAgency, env);
		else GBANUTIL_GetJStringExchangeFromCString((char *)"", jDestAgency, env);
		if (OrigAgency != NULL)
			GBANUTIL_GetJStringExchangeFromCString(OrigAgency, jOrigAgency, env);
		else GBANUTIL_GetJStringExchangeFromCString((char *)"", jOrigAgency, env);
		if (TransContrNum != NULL)
			GBANUTIL_GetJStringExchangeFromCString(TransContrNum, jTransContrNum, env);
		else GBANUTIL_GetJStringExchangeFromCString((char *)"", jTransContrNum, env);
		if (TransContrRef != NULL)
			GBANUTIL_GetJStringExchangeFromCString(TransContrRef, jTransContrRef, env);
		else GBANUTIL_GetJStringExchangeFromCString((char *)"", jTransContrRef, env);
		if (DomainName != NULL)
			GBANUTIL_GetJStringExchangeFromCString(DomainName, jDomainName, env);
		else GBANUTIL_GetJStringExchangeFromCString((char *)"", jDomainName, env);
		if (DomainVersion != NULL)
			GBANUTIL_GetJStringExchangeFromCString(DomainVersion, jDomainVersion, env);
		else GBANUTIL_GetJStringExchangeFromCString((char *)"", jDomainVersion, env);
		if (CharEncName != NULL)
			GBANUTIL_GetJStringExchangeFromCString(CharEncName, jCharEncName, env);
		else GBANUTIL_GetJStringExchangeFromCString((char *)"", jCharEncName, env);
		if (CharEncVersion != NULL)
			GBANUTIL_GetJStringExchangeFromCString(CharEncVersion, jCharEncVersion, env);
		else GBANUTIL_GetJStringExchangeFromCString((char *)"", jCharEncVersion, env);
		if (DestAgName != NULL)
			GBANUTIL_GetJStringExchangeFromCString(DestAgName, jDestAgName, env);
		else GBANUTIL_GetJStringExchangeFromCString((char *)"", jDestAgName, env);
		if (OrigAgName != NULL)
			GBANUTIL_GetJStringExchangeFromCString(OrigAgName, jOrigAgName, env);
		else GBANUTIL_GetJStringExchangeFromCString((char *)"", jOrigAgName, env);

		GBAN2011UTIL_GetJGmtFromCGmt(env, jGmt, &Gmt, An2000_JNI_LastErrorString);
		GBAN2011UTIL_GetJDateFromCDate(env, jDat, &Dat, An2000_JNI_LastErrorString);
		GBAN2011UTIL_GetJAppProfileArrayFromCAppProfileArray(env, jAps, Aps, ApsNum,
		                                                     An2000_JNI_LastErrorString);
	}

	//////////////////////////////////
	// free memory
	//////////////////////////////////
	if (TypeOfTransaction != NULL) Ptr_GB_AN_ENGINE_FreeGenericMem(TypeOfTransaction);
	if (DestAgency != NULL) Ptr_GB_AN_ENGINE_FreeGenericMem(DestAgency);
	if (OrigAgency != NULL) Ptr_GB_AN_ENGINE_FreeGenericMem(OrigAgency);
	if (TransContrNum != NULL) Ptr_GB_AN_ENGINE_FreeGenericMem(TransContrNum);
	if (TransContrRef != NULL) Ptr_GB_AN_ENGINE_FreeGenericMem(TransContrRef);
	if (DomainName != NULL) Ptr_GB_AN_ENGINE_FreeGenericMem(DomainName);
	if (DomainVersion != NULL) Ptr_GB_AN_ENGINE_FreeGenericMem(DomainVersion);
	if (CharEncName != NULL) Ptr_GB_AN_ENGINE_FreeGenericMem(CharEncName);
	if (CharEncVersion != NULL) Ptr_GB_AN_ENGINE_FreeGenericMem(CharEncVersion);
	if (DestAgName != NULL) Ptr_GB_AN_ENGINE_FreeGenericMem(DestAgName);
	if (OrigAgName != NULL) Ptr_GB_AN_ENGINE_FreeGenericMem(OrigAgName);
	if (Aps != NULL & ApsNum > 0) Ptr_GB_AN_ENGINE_FreeGenericMem(Aps);

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_GetType1InfoFromAnsiNist(
		JNIEnv *env,
		jobject, /* this */
		jobject jAnsiNist,
		jobject jVersion,                // version
		jobject jTransactionType,        // transaction type
		jobject jPriority,                // priority
		jobject jDestinationAgency,        // destination agency
		jobject jOriginatingAgency,        // originating agency
		jobject jTransContrNum,            // transaction control number
		jobject jTransContrRef,            // transaction control reference
		jobject jNominalScanRes,            // Nominal Scanning Resolution
		jobject jNominalTXRes,            // Nominal Transmitting Resolution
		jobject jDomain                // Domain
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_AN2000_GetEFTSType1_Info == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist: Ptr_AN2000_GetEFTSType1_Info is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jAnsiNist == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist: jAnsiNist is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jVersion == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist: jVersion is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jTransactionType == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist: jTransactionType is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jPriority == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist: jPriority is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jDestinationAgency == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist: jDestinationAgency is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jOriginatingAgency == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist: jOriginatingAgency is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jTransContrNum == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist: jTransContrNum is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jTransContrRef == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist: jTransContrRef is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jNominalScanRes == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist: jNominalScanRes is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jNominalTXRes == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist: jNominalTXRes is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jDomain == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist: jDomain is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check java class
	//////////////////////////////////

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	ANSI_NIST *AnsiNist;
	GBANUTIL_GetCAnsiNistFromJAnsiNist(jAnsiNist, &AnsiNist, env);
	if (AnsiNist == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist: AnsiNist struct contains a NULL pointer");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if ((AnsiNist->num_records <= 0) || (AnsiNist->records[0]->type != TYPE_1_ID))
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist: AnsiNist struct does not contain Type1 records");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// call function
	//////////////////////////////////
	RECORD *record = AnsiNist->records[0];
	int Version, Priority, DestAgencyLen, OriginatingAgencyLen, TransContrNumLen,
			TransContrRefLen, DomainLen;
	float NominalScanRes, NominalTXRes;
	char TransactionType[12], *DestinationAgency, *OriginatingAgency, *TransContrNum,
			*TransContrRef, *Domain;
	int RetVal = Ptr_AN2000_GetEFTSType1_Info(
			record,
			&Version,                // version
			TransactionType,        // pass a char[12]
			&Priority,                // priority
			&DestinationAgency,        // destination agency: no limits on length of this field, so
			// a developer needs next field:
			&DestAgencyLen,
			&OriginatingAgency,
			&OriginatingAgencyLen,
			&TransContrNum,
			&TransContrNumLen,
			&TransContrRef,
			&TransContrRefLen,
			&NominalScanRes,
			&NominalTXRes,
			&Domain,
			&DomainLen                // Domain string length
	);

	//////////////////////////////////
	// copy parameters
	//////////////////////////////////
	if (RetVal != AN2K_DLL_NO_ERROR)
	{
		ValToRet = RetVal;
		char ErrorString[100];
		GbAn2000Util_FormatErrorString(RetVal, ErrorString);
		sprintf(An2000_JNI_LastErrorString,
		        "GetType1InfoFromAnsiNist: Ptr_AN2000_GetEFTSType1_Info returned %s (%d)",
		        ErrorString, RetVal);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
	}
	else
	{
		// integer parameters
		GBANUTIL_GetJIntExchangeFromCInt(Version, jVersion, env);
		GBANUTIL_GetJIntExchangeFromCInt(Priority, jPriority, env);
		// double parameters
		GBANUTIL_GetJDoubleExchangeFromCDouble((double) NominalScanRes, jNominalScanRes, env);
		GBANUTIL_GetJDoubleExchangeFromCDouble((double) NominalTXRes, jNominalTXRes, env);
		// strings
		GBANUTIL_GetJStringExchangeFromCString(TransactionType, jTransactionType, env);
		GBANUTIL_GetJStringExchangeFromCString(DestinationAgency, jDestinationAgency, env);
		GBANUTIL_GetJStringExchangeFromCString(OriginatingAgency, jOriginatingAgency, env);
		GBANUTIL_GetJStringExchangeFromCString(TransContrNum, jTransContrNum, env);
		if (TransContrRef != NULL)
			GBANUTIL_GetJStringExchangeFromCString(TransContrRef, jTransContrRef, env);
		else
			GBANUTIL_GetJStringExchangeFromCString((char *)"", jTransContrRef, env);
		GBANUTIL_GetJStringExchangeFromCString(Domain, jDomain, env);
	}

	//////////////////////////////////
	// free memory
	//////////////////////////////////
	if (DestinationAgency != NULL) Ptr_AN2000_free_ANSI_NIST_buffer(DestinationAgency);
	if (OriginatingAgency != NULL) Ptr_AN2000_free_ANSI_NIST_buffer(OriginatingAgency);
	if (TransContrNum != NULL) Ptr_AN2000_free_ANSI_NIST_buffer(TransContrNum);
	if (TransContrRef != NULL) Ptr_AN2000_free_ANSI_NIST_buffer(TransContrRef);
	if (Domain != NULL) Ptr_AN2000_free_ANSI_NIST_buffer(Domain);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_CreateAnsiNist(
		JNIEnv *env,
		jobject, /* this */
		jobject jAnsiNist,
		jint jVersion,
		jstring jTransactionType, // MANDATORY
		jint jPriority,
		jstring jDestinationAgency, // MANDATORY
		jstring jOriginatingAgency, // MANDATORY
		jstring jTransactionControlID, // MANDATORY
		jstring jTransactionControlReference, // OPTIONAL
		jdouble jNominalScanningResolution,
		jdouble jNominalTransmitResolution,
		jstring jDomainNameID // MANDATORY
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateAnsiNist: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_AN2000_Create_ANSI_NIST == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateAnsiNist: Ptr_AN2000_Create_ANSI_NIST is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (Ptr_AN2000_free_ANSI_NIST == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateAnsiNist: Ptr_AN2000_free_ANSI_NIST is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (Ptr_AN2000_free_ANSI_NIST_buffer == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateAnsiNist: Ptr_AN2000_free_ANSI_NIST_buffer is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jAnsiNist == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateAnsiNist: jAnsiNist is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jTransactionType == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateAnsiNist: jTransactionType is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jDestinationAgency == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateAnsiNist: jDestinationAgency is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jOriginatingAgency == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateAnsiNist: jOriginatingAgency is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jTransactionControlID == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateAnsiNist: jTransactionControlID is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jDomainNameID == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "CreateAnsiNist: jDomainNameID is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check java class
	//////////////////////////////////

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	// TypeOfTransaction
	char *TypeOfTransaction = NULL;
	GBANUTIL_GetStringFromJString(jTransactionType, &TypeOfTransaction, env);
	// DestinationAgency
	char *DestinationAgency = NULL;
	GBANUTIL_GetStringFromJString(jDestinationAgency, &DestinationAgency, env);
	// OriginatingAgency
	char *OriginatingAgency = NULL;
	GBANUTIL_GetStringFromJString(jOriginatingAgency, &OriginatingAgency, env);
	// TransactionControlID
	char *TransactionControlID = NULL;
	GBANUTIL_GetStringFromJString(jTransactionControlID, &TransactionControlID, env);
	// TransactionControlReference
	char *TransactionControlReference = NULL;
	GBANUTIL_GetStringFromJString(jTransactionControlReference, &TransactionControlReference, env);
	// DestinationAgency
	char *DomainNameID = NULL;
	GBANUTIL_GetStringFromJString(jDomainNameID, &DomainNameID, env);

	//////////////////////////////////
	// call function
	//////////////////////////////////
	ANSI_NIST *ansiNist = NULL;
	int RetVal = Ptr_AN2000_Create_ANSI_NIST(
			&ansiNist,
			(int) jVersion,
			TypeOfTransaction,
			(int) jPriority,
			DestinationAgency,
			OriginatingAgency,
			TransactionControlID,
			TransactionControlReference,
			(float) jNominalScanningResolution,
			(float) jNominalTransmitResolution,
			DomainNameID
	);
	if (RetVal != AN2K_DLL_NO_ERROR)
	{
		ValToRet = RetVal;
		char ErrorString[100];
		GbAn2000Util_FormatErrorString(RetVal, ErrorString);
		sprintf(An2000_JNI_LastErrorString,
		        "CreateAnsiNist: Ptr_AN2000_Create_ANSI_NIST returned %s (%d)", ErrorString,
		        RetVal);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		GBANUTIL_GetJAnsiNistFromCAnsiNist(NULL, jAnsiNist, env);
	}
	else
	{
		GBANUTIL_GetJAnsiNistFromCAnsiNist(ansiNist, jAnsiNist, env);
	}

	//////////////////////////////////
	// free memory
	//////////////////////////////////
	if (TypeOfTransaction != NULL) free(TypeOfTransaction);
	if (DestinationAgency != NULL) free(DestinationAgency);
	if (OriginatingAgency != NULL) free(OriginatingAgency);
	if (TransactionControlID != NULL) free(TransactionControlID);
	if (TransactionControlReference != NULL) free(TransactionControlReference);
	if (DomainNameID != NULL) free(DomainNameID);

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_InsertRecordIntoAnsiNistStruct(
		JNIEnv *env,
		jobject, /* this */
		jobject jAnsiNist,
		jobject jRecord,
		jint jInsertionIndex // set to a value < 0 to insert in the last position
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "InsertRecordIntoAnsiNistStruct: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_AN2000_insert_ANSI_NIST_record_ETFS == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "InsertRecordIntoAnsiNistStruct: Ptr_AN2000_insert_ANSI_NIST_record_ETFS is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jAnsiNist == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "InsertRecordIntoAnsiNistStruct: jAnsiNist is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if (jRecord == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "InsertRecordIntoAnsiNistStruct: jRecord is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check java class
	//////////////////////////////////

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	ANSI_NIST *AnsiNist;
	GBANUTIL_GetCAnsiNistFromJAnsiNist(jAnsiNist, &AnsiNist, env);
	if (AnsiNist == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "InsertRecordIntoAnsiNistStruct: AnsiNist struct contains a NULL pointer");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	if ((AnsiNist->num_records <= 0) || (AnsiNist->records[0]->type != TYPE_1_ID))
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "InsertRecordIntoAnsiNistStruct: AnsiNist struct does not contain Type1 records");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	// record
	RECORD *record;
	GBANUTIL_GetCRecordFromJRecord(jRecord, &record, env);
	if (record == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "InsertRecordIntoAnsiNistStruct: Record stored into jRecord is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}
	// insertionIndex
	if (jInsertionIndex < 0) jInsertionIndex = AnsiNist->num_records;
	if (AnsiNist->num_records > (int) jInsertionIndex)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "InsertRecordIntoAnsiNistStruct: InsertionIndex > number of records into the ANSINIST struct");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// call function
	//////////////////////////////////
	int RetVal = Ptr_AN2000_insert_ANSI_NIST_record_ETFS(
			(int) jInsertionIndex,
			record,
			AnsiNist
	);
	//////////////////////////////////
	// copy parameters
	//////////////////////////////////
	if (RetVal != AN2K_DLL_NO_ERROR)
	{
		ValToRet = RetVal;
		char ErrorString[100];
		GbAn2000Util_FormatErrorString(RetVal, ErrorString);
		sprintf(An2000_JNI_LastErrorString,
		        "InsertRecordIntoAnsiNistStruct: Ptr_AN2000_insert_ANSI_NIST_record_ETFS returned %s (%d)",
		        ErrorString, RetVal);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
	}

	//////////////////////////////////
	// free memory
	//////////////////////////////////

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_ansinistitl_Gban2000JavaWrapperLibrary_DeleteRecordFromAnsiNistStruct(
		JNIEnv *env,
		jobject, /* this */
		jobject jAnsiNist,
		jint jRecordToDeleteIndex
)
{
	int ValToRet = AN2K_DLL_NO_ERROR;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (An2000DllPtr == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "DeleteRecordFromAnsiNistStruct: An2000DllPtr is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_AN2000_delete_ANSI_NIST_record == NULL)
	{
		ValToRet = AN2K_DLL_GENERIC;
		sprintf(An2000_JNI_LastErrorString,
		        "DeleteRecordFromAnsiNistStruct: Ptr_AN2000_delete_ANSI_NIST_record is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jAnsiNist == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "DeleteRecordFromAnsiNistStruct: jAnsiNist is NULL");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check java class
	//////////////////////////////////

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	ANSI_NIST *AnsiNist;
	GBANUTIL_GetCAnsiNistFromJAnsiNist(jAnsiNist, &AnsiNist, env);
	if (AnsiNist == NULL)
	{
		ValToRet = AN2K_DLL_BAD_PARAMETER;
		sprintf(An2000_JNI_LastErrorString,
		        "DeleteRecordFromAnsiNistStruct: AnsiNist struct contains a NULL pointer");
		a_printf ("%s\n", An2000_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// call function
	//////////////////////////////////
	int RetVal = Ptr_AN2000_delete_ANSI_NIST_record(
			(int) jRecordToDeleteIndex,
			AnsiNist
	);
	//////////////////////////////////
	// copy parameters
	//////////////////////////////////
	if (RetVal != AN2K_DLL_NO_ERROR)
	{
		ValToRet = RetVal;
		char ErrorString[100];
		GbAn2000Util_FormatErrorString(RetVal, ErrorString);
		sprintf(An2000_JNI_LastErrorString,
		        "DeleteRecordFromAnsiNistStruct: Ptr_AN2000_delete_ANSI_NIST_record returned %s (%d)",
		        ErrorString, RetVal);
		a_printf ("%s\n", An2000_JNI_LastErrorString);
	}

	//////////////////////////////////
	// free memory
	//////////////////////////////////

	return ValToRet;
}

} // extern "C"

