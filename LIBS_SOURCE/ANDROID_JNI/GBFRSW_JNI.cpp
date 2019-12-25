//-------------------------------------------------------------//
//-------------------------------------------------------------//
// INCLUDE
//-------------------------------------------------------------//
//-------------------------------------------------------------//
#include <jni.h>
#include <string>
#include <dlfcn.h>
#include "GB_SystemConfiguration.h"
#include "gbfrsw.h"
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
#define a_printf(...) __android_log_print(ANDROID_LOG_DEBUG, "GBFRSW_LOG", __VA_ARGS__);

//-------------------------------------------------------------//
//-------------------------------------------------------------//
// GLOBALS
//-------------------------------------------------------------//
//-------------------------------------------------------------//
HMODULE GbfrswDllPtr = NULL;
char Gbfrsw_JNI_LastErrorString[1000];

unsigned char *GBFRSW_MemoryBuffer = NULL;

int (__stdcall *Ptr_GBFRSW_GetVersionInfo)(
	unsigned char *VersionField1,
	unsigned char *VersionField2,
	unsigned char *VersionField3,
	unsigned char *VersionField4,
	unsigned int *MaxImageSizeX,
	unsigned int *MaxImageSizeY,
	unsigned int *MinImageSizeX,
	unsigned int *MinImageSizeY,
	unsigned int RequestedOperations,
	unsigned int *MemoryBufferSize);

int (__stdcall *Ptr_GBFRSW_CodeMaxSize)(
	unsigned int ImageSizeX,
	unsigned int ImageSizeY,
	unsigned int *SampleCodeSize,
	unsigned int *PackedSampleCodeSize,
	unsigned int *TemplateCodeSize,
	unsigned int *CompactTemplateCodeSize);

void (__stdcall *Ptr_GBFRSW_GetLastError)(
	int *ErrorCode,
	int *Detail);

void (__stdcall *Ptr_GBFRSW_GetLicenseInfo)(
	GBFRSW_LICENSE_USB_SCANNER_CHECK *ScannerBasedLicense,
	GBFRSW_LICENSE_FILE_CHECK *FileBasedLicense);

int (__stdcall *Ptr_GBFRSW_Coding)(
	unsigned int ImageSizeX,
	unsigned int ImageSizeY,
	unsigned char *ImageBuffer,
	unsigned char ImageFlags,
	unsigned char Options,
	unsigned char *SampleCode);

int (__stdcall *Ptr_GBFRSW_Enroll)(
	unsigned char *SampleCode,
	unsigned char *TemplateCode);

int (__stdcall *Ptr_GBFRSW_Match)(
	unsigned char *SampleCode,
	unsigned char *TemplateCode,
	unsigned char SpeedVsPrecisionTradeoff,
	unsigned char UnmatchedDataFactor,
	unsigned char Options,
	int MaxRotationAngleDegree,
	float *MatchingScore);

int (__stdcall *Ptr_GBFRSW_Convert)(
	unsigned char *OriginalCodePtr,
	unsigned char *ConvertedCodePtr);

int (__stdcall *Ptr_GBFRSW_Correct)(
	unsigned char *SampleCode1,
	unsigned char *SampleCode2,
	unsigned char *SampleCode3,
	unsigned char *CorrectedSampleCode);

int (__stdcall *Ptr_GBFRSW_ISO_GetMaxFMRLength)(
	unsigned int ImageSizeX,
	unsigned int ImageSizeY,
	unsigned char FormatType,
	unsigned char GBProprietaryData,
	unsigned short *MaxFRMLength);

int (__stdcall *Ptr_GBFRSW_ISO_GBTemplateToFMR)(
	unsigned char *GBTemplate,
	unsigned char *FMR,
	unsigned short *FMRLen,
	unsigned char FingerPos,
	unsigned char FormatType,
	unsigned char GBProprietaryData);

int (__stdcall *Ptr_GBFRSW_ISO_FMRToGBTemplate)(
	unsigned char *FMR,
	unsigned char *GBTemplate,
	unsigned char *FingerPos,
	unsigned char FormatType);

int (__stdcall *Ptr_GBFRSW_ISO_GetMaxFMCLength)(
	unsigned short FormatType,
	unsigned int ImageSizeX,
	unsigned int ImageSizeY,
	unsigned int MaxMinutiaeNum,
	unsigned short *MaxFMCLength);

int (__stdcall *Ptr_GBFRSW_ISO_GBTemplateToFMC)(
	unsigned char *GBTemplate,
	unsigned char *FMC,
	unsigned short *FMCLen,
	unsigned short FormatType,
	unsigned int MaxMinutiaeNum);

int (__stdcall *Ptr_GBFRSW_SetJavaFD)(
	int * javafd, 
	int * bus_number, 
	int *dev_address, 
	int device_count);

//-------------------------------------------------------------//
//-------------------------------------------------------------//
// FUNCTIONS
//-------------------------------------------------------------//
//-------------------------------------------------------------//
extern "C"
{

//----------------------------------------------------------------------------------------------------------------
void SetGbfrswError(const char *FunctionName)
//----------------------------------------------------------------------------------------------------------------
{
	char DummyString[1000];
	char DummyString2[1000], DummyString3[1000], DummyString4[1000];
	int ErrorCode;
	int ErrorDetail;
	GBFRSW_LICENSE_USB_SCANNER_CHECK ScannerLicense;
	GBFRSW_LICENSE_FILE_CHECK NodeLockedLicense;

	Ptr_GBFRSW_GetLastError(&ErrorCode, &ErrorDetail);
	if (ErrorCode == GBFRSW_ERROR_LICENSE_NOT_GRANTED)
	{
		Ptr_GBFRSW_GetLicenseInfo(&ScannerLicense, &NodeLockedLicense);

		if (ScannerLicense.Result == GBSCAN_ERROR_USB_DRIVER)
			sprintf(DummyString2, "usb driver error %x", ScannerLicense.UsbDriverErrorCodeDetail);
		else if (ScannerLicense.Result == GBSCAN_ERROR_USB_DEVICE_NOT_FOUND)
			sprintf(DummyString2, "scanner not found");
		else
			sprintf(DummyString2, "unknow reason\n");
		sprintf(DummyString3, "1. GB USB scanner based license not found: %s", DummyString2);

		sprintf(DummyString4,
				"2. SW node-locked license check fail: ErrorCode %d, Phase %d, Description: %s",
				NodeLockedLicense.ErrorCode,
				NodeLockedLicense.Phase,
				NodeLockedLicense.ErrorDetailString);

		// error string
		sprintf(DummyString, "GBFRSW license check fail: %s; %s", DummyString3, DummyString4);
	}
	else
	{
		// Error code
		if (ErrorCode == GBFRSW_ERROR_NOT_ENOUGH_MEMORY)
			sprintf(DummyString2, "GBFRSW_ERROR_NOT_ENOUGH_MEMORY");
		else if (ErrorCode == GBFRSW_ERROR_IMAGE_ABSENT)
			sprintf(DummyString2, "GBFRSW_ERROR_IMAGE_ABSENT");
		else if (ErrorCode == GBFRSW_ERROR_SIZE_NOT_SUPPORTED)
			sprintf(DummyString2, "GBFRSW_ERROR_SIZE_NOT_SUPPORTED");
		else if (ErrorCode == GBFRSW_ERROR_GBFRSW_CODE_TYPE_NOT_SUPPORTED)
			sprintf(DummyString2, "GBFRSW_ERROR_GBFRSW_CODE_TYPE_NOT_SUPPORTED");
		else if (ErrorCode == GBFRSW_ERROR_CODE_VERSION_NOT_SUPPORTED)
			sprintf(DummyString2, "GBFRSW_ERROR_CODE_VERSION_NOT_SUPPORTED");
		else if (ErrorCode == GBFRSW_ERROR_POINTER_NOT_ALIGNED)
			sprintf(DummyString2, "GBFRSW_ERROR_POINTER_NOT_ALIGNED");
		else if (ErrorCode == GBFRSW_ERROR_CODE_CORRUPTED)
			sprintf(DummyString2, "GBFRSW_ERROR_CODE_CORRUPTED");
		else if (ErrorCode == GBFRSW_ERROR_LOW_QUALITY_FINGERPRINT)
			sprintf(DummyString2, "GBFRSW_ERROR_LOW_QUALITY_FINGERPRINT");
		else
			sprintf(DummyString2, "UNKNOWN");
		// detail
		if (ErrorDetail == GBFRSW_DETAIL_IMAGE)
			sprintf(DummyString3, "GBFRSW_DETAIL_IMAGE");
		else if (ErrorDetail == GBFRSW_DETAIL_SAMPLE_CODE)
			sprintf(DummyString3, "GBFRSW_DETAIL_SAMPLE_CODE");
		else if (ErrorDetail == GBFRSW_DETAIL_SAMPLE_CODE_2)
			sprintf(DummyString3, "GBFRSW_DETAIL_SAMPLE_CODE_2");
		else if (ErrorDetail == GBFRSW_DETAIL_SAMPLE_CODE_3)
			sprintf(DummyString3, "GBFRSW_DETAIL_SAMPLE_CODE_3");
		else if (ErrorDetail == GBFRSW_DETAIL_TEMPLATE_CODE)
			sprintf(DummyString3, "GBFRSW_DETAIL_TEMPLATE_CODE");
		else if (ErrorDetail == GBFRSW_DETAIL_SAMPLE_CODE_PACKED)
			sprintf(DummyString3, "GBFRSW_DETAIL_SAMPLE_CODE_PACKED");
		else if (ErrorDetail == GBFRSW_DETAIL_TEMPLATE_CODE_COMPACT)
			sprintf(DummyString3, "GBFRSW_DETAIL_TEMPLATE_CODE_COMPACT");
		else
			sprintf(DummyString3, "UNKNOWN");

		// error string
		sprintf(DummyString, "Error String: %s; Error Detail (error is associated with): %s",
				DummyString2, DummyString2);

		printf("\n\nError detail (error is associated with): ");
	}

	sprintf(Gbfrsw_JNI_LastErrorString, "An error happened during <%s> function execution: %s",
			FunctionName, DummyString);
}

JNIEXPORT jstring JNICALL Java_com_greenbit_gbfrsw_GbfrswJavaWrapperLibrary_GetLastErrorString(
	JNIEnv *env,
	jobject /* this */)
{
	return env->NewStringUTF(Gbfrsw_JNI_LastErrorString);
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbfrsw_GbfrswJavaWrapperLibrary_Unload(
	JNIEnv *env,
	jobject /* this */)
{
	int ValToRet = GBFRSW_SUCCESS;

	if (GbfrswDllPtr != NULL)
	{
		FreeLibrary(GbfrswDllPtr);
	}
	else
	{
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Unload: GbfrswDllPtr is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
	}

	Ptr_GBFRSW_GetVersionInfo = NULL;
	Ptr_GBFRSW_Coding = NULL;
	Ptr_GBFRSW_CodeMaxSize = NULL;
	Ptr_GBFRSW_GetLastError = NULL;
	Ptr_GBFRSW_Enroll = NULL;
	Ptr_GBFRSW_Match = NULL;
	Ptr_GBFRSW_Convert = NULL;
	Ptr_GBFRSW_Correct = NULL;
	Ptr_GBFRSW_GetLicenseInfo = NULL;
	Ptr_GBFRSW_ISO_GetMaxFMRLength = NULL;
	Ptr_GBFRSW_ISO_GBTemplateToFMR = NULL;
	Ptr_GBFRSW_ISO_FMRToGBTemplate = NULL;
	Ptr_GBFRSW_ISO_GetMaxFMCLength = NULL;
	Ptr_GBFRSW_ISO_GBTemplateToFMC = NULL;
	Ptr_GBFRSW_SetJavaFD = NULL;

	GbfrswDllPtr = NULL;

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbfrsw_GbfrswJavaWrapperLibrary_Load(
	JNIEnv *env,
	jobject /* this */)
{
	int ValToRet = GBFRSW_SUCCESS;

	Dl_info pathInfo;
	dladdr((void *) Java_com_greenbit_gbfrsw_GbfrswJavaWrapperLibrary_Load, &pathInfo);
	a_printf("Load: pathName = %s", pathInfo.dli_fname);

	//////////////////////////////
	// free library
	//////////////////////////////
	if (GbfrswDllPtr != NULL)
		Java_com_greenbit_gbfrsw_GbfrswJavaWrapperLibrary_Unload(env, nullptr);

	//////////////////////////////
	// load library
	//////////////////////////////
	GbfrswDllPtr = LoadLibrary("libGBFRSW.so");
	if (GbfrswDllPtr == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Load: Unable to load GBFRSW: %s", dlerror());
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: GBFRSW loaded\n");

	//////////////////////////////
	// GBFRSW_GetVersionInfo
	//////////////////////////////
	Ptr_GBFRSW_GetVersionInfo = (int (*)(
		unsigned char *VersionField1,
		unsigned char *VersionField2,
		unsigned char *VersionField3,
		unsigned char *VersionField4,
		unsigned int *MaxImageSizeX,
		unsigned int *MaxImageSizeY,
		unsigned int *MinImageSizeX,
		unsigned int *MinImageSizeY,
		unsigned int RequestedOperations,
		unsigned int *MemoryBufferSize
	)) GetProcAddress(GbfrswDllPtr, "GBFRSW_GetVersionInfo");
	if (Ptr_GBFRSW_GetVersionInfo == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Load: Unable to load Ptr_GBFRSW_GetVersionInfo: %s", dlerror());
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GBFRSW_GetVersionInfo loaded\n");

	//////////////////////////////
	// GBFRSW_CodeMaxSize
	//////////////////////////////
	Ptr_GBFRSW_CodeMaxSize = (int (*)(
		unsigned int ImageSizeX,
		unsigned int ImageSizeY,
		unsigned int *SampleCodeSize,
		unsigned int *PackedSampleCodeSize,
		unsigned int *TemplateCodeSize,
		unsigned int *CompactTemplateCodeSize
	)) GetProcAddress(GbfrswDllPtr, "GBFRSW_CodeMaxSize");
	if (Ptr_GBFRSW_CodeMaxSize == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Load: Unable to load Ptr_GBFRSW_CodeMaxSize: %s", dlerror());
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GBFRSW_CodeMaxSize loaded\n");

	//////////////////////////////
	// GBFRSW_GetLastError
	//////////////////////////////
	Ptr_GBFRSW_GetLastError = (void (*)(
		int *ErrorCode,
		int *Detail
	)) GetProcAddress(GbfrswDllPtr, "GBFRSW_GetLastError");
	if (Ptr_GBFRSW_GetLastError == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Load: Unable to load Ptr_GBFRSW_GetLastError: %s", dlerror());
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GBFRSW_GetLastError loaded\n");

	//////////////////////////////
	// GBFRSW_GetLicenseInfo
	//////////////////////////////
	Ptr_GBFRSW_GetLicenseInfo = (void (*)(
		GBFRSW_LICENSE_USB_SCANNER_CHECK *ScannerBasedLicense,
		GBFRSW_LICENSE_FILE_CHECK *FileBasedLicense
	)) GetProcAddress(GbfrswDllPtr, "GBFRSW_GetLicenseInfo");
	if (Ptr_GBFRSW_GetLicenseInfo == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Load: Unable to load Ptr_GBFRSW_GetLicenseInfo: %s", dlerror());
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GBFRSW_GetLicenseInfo loaded\n");

	//////////////////////////////
	// GBFRSW_Coding
	//////////////////////////////
	Ptr_GBFRSW_Coding = (int (*)(
		unsigned int ImageSizeX,
		unsigned int ImageSizeY,
		unsigned char *ImageBuffer,
		unsigned char ImageFlags,
		unsigned char Options,
		unsigned char *SampleCode
	)) GetProcAddress(GbfrswDllPtr, "GBFRSW_Coding");
	if (Ptr_GBFRSW_Coding == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Load: Unable to load Ptr_GBFRSW_Coding: %s", dlerror());
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GBFRSW_Coding loaded\n");

	//////////////////////////////
	// GBFRSW_Enroll
	//////////////////////////////
	Ptr_GBFRSW_Enroll = (int (*)(
		unsigned char *SampleCode,
		unsigned char *TemplateCode
	)) GetProcAddress(GbfrswDllPtr, "GBFRSW_Enroll");
	if (Ptr_GBFRSW_Enroll == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Load: Unable to load Ptr_GBFRSW_Enroll: %s", dlerror());
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GBFRSW_Enroll loaded\n");

	//////////////////////////////
	// GBFRSW_Match
	//////////////////////////////
	Ptr_GBFRSW_Match = (int (*)(
		unsigned char *SampleCode,
		unsigned char *TemplateCode,
		unsigned char SpeedVsPrecisionTradeoff,
		unsigned char UnmatchedDataFactor,
		unsigned char Options,
		int MaxRotationAngleDegree,
		float *MatchingScore
	)) GetProcAddress(GbfrswDllPtr, "GBFRSW_Match");
	if (Ptr_GBFRSW_Match == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Load: Unable to load Ptr_GBFRSW_Match: %s", dlerror());
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GBFRSW_Match loaded\n");

	//////////////////////////////
	// GBFRSW_Convert
	//////////////////////////////
	Ptr_GBFRSW_Convert = (int (*)(
		unsigned char *,
		unsigned char *
	)) GetProcAddress(GbfrswDllPtr, "GBFRSW_Convert");
	if (Ptr_GBFRSW_Convert == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Load: Unable to load Ptr_GBFRSW_Convert: %s", dlerror());
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GBFRSW_Convert loaded\n");

	//////////////////////////////
	// GBFRSW_Correct
	//////////////////////////////
	Ptr_GBFRSW_Correct = (int (*)(
		unsigned char *SampleCode1,
		unsigned char *SampleCode2,
		unsigned char *SampleCode3,
		unsigned char *CorrectedSampleCode
	)) GetProcAddress(GbfrswDllPtr, "GBFRSW_Correct");
	if (Ptr_GBFRSW_Correct == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Load: Unable to load Ptr_GBFRSW_Correct: %s", dlerror());
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GBFRSW_Correct loaded\n");

	//////////////////////////////
	// GBFRSW_ISO_GetMaxFMRLength
	//////////////////////////////
	Ptr_GBFRSW_ISO_GetMaxFMRLength = (int (*)(
		unsigned int ImageSizeX,
		unsigned int ImageSizeY,
		unsigned char FormatType,
		unsigned char GBProprietaryData,
		unsigned short *MaxFRMLength
	)) GetProcAddress(GbfrswDllPtr, "GBFRSW_ISO_GetMaxFMRLength");
	if (Ptr_GBFRSW_ISO_GetMaxFMRLength == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Load: Unable to load Ptr_GBFRSW_ISO_GetMaxFMRLength: %s", dlerror());
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GBFRSW_ISO_GetMaxFMRLength loaded\n");

	//////////////////////////////
	// GBFRSW_ISO_GBTemplateToFMR
	//////////////////////////////
	Ptr_GBFRSW_ISO_GBTemplateToFMR = (int (*)(
		unsigned char *GBTemplate,
		unsigned char *FMR,
		unsigned short *FMRLen,
		unsigned char FingerPos,
		unsigned char FormatType,
		unsigned char GBProprietaryData
	)) GetProcAddress(GbfrswDllPtr, "GBFRSW_ISO_GBTemplateToFMR");
	if (Ptr_GBFRSW_ISO_GBTemplateToFMR == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Load: Unable to load Ptr_GBFRSW_ISO_GBTemplateToFMR: %s", dlerror());
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GBFRSW_ISO_GBTemplateToFMR loaded\n");

	//////////////////////////////
	// GBFRSW_ISO_FMRToGBTemplate
	//////////////////////////////
	Ptr_GBFRSW_ISO_FMRToGBTemplate = (int (*)(
		unsigned char *FMR,
		unsigned char *GBTemplate,
		unsigned char *FingerPos,
		unsigned char FormatType
	)) GetProcAddress(GbfrswDllPtr, "GBFRSW_ISO_FMRToGBTemplate");
	if (Ptr_GBFRSW_ISO_FMRToGBTemplate == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Load: Unable to load Ptr_GBFRSW_ISO_FMRToGBTemplate: %s", dlerror());
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GBFRSW_ISO_FMRToGBTemplate loaded\n");

	//////////////////////////////
	// GBFRSW_ISO_GetMaxFMCLength
	//////////////////////////////
	Ptr_GBFRSW_ISO_GetMaxFMCLength = (int (*)(
		unsigned short FormatType,
		unsigned int ImageSizeX,
		unsigned int ImageSizeY,
		unsigned int MaxMinutiaeNum,
		unsigned short *MaxFMCLength
	)) GetProcAddress(GbfrswDllPtr, "GBFRSW_ISO_GetMaxFMCLength");
	if (Ptr_GBFRSW_ISO_GetMaxFMCLength == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Load: Unable to load Ptr_GBFRSW_ISO_GetMaxFMCLength: %s", dlerror());
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GBFRSW_ISO_GetMaxFMCLength loaded\n");

	//////////////////////////////
	// GBFRSW_ISO_GBTemplateToFMC
	//////////////////////////////
	Ptr_GBFRSW_ISO_GBTemplateToFMC = (int (*)(
		unsigned char *GBTemplate,
		unsigned char *FMC,
		unsigned short *FMCLen,
		unsigned short FormatType,
		unsigned int MaxMinutiaeNum
	)) GetProcAddress(GbfrswDllPtr, "GBFRSW_ISO_GBTemplateToFMC");
	if (Ptr_GBFRSW_ISO_GBTemplateToFMC == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Load: Unable to load Ptr_GBFRSW_ISO_GBTemplateToFMC: %s", dlerror());
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GBFRSW_ISO_GBTemplateToFMC loaded\n");

	//////////////////////////////
	// GBFRSW_SetJavaFD
	//////////////////////////////
	Ptr_GBFRSW_SetJavaFD = (int (*)(
		int * javafd,
		int * bus_number,
		int *dev_address,
		int device_count
	)) GetProcAddress(GbfrswDllPtr, "GBFRSW_SetJavaFD");
	if (Ptr_GBFRSW_SetJavaFD == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Load: Unable to load Ptr_GBFRSW_SetJavaFD: %s", dlerror());
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GBFRSW_SetJavaFD loaded\n");

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbfrsw_GbfrswJavaWrapperLibrary_SetOpenedJavaFD(
	JNIEnv *env,
	jobject thisObj,
	// INPUT
	jintArray openedJavaFd, jintArray bus_number, jintArray devAddress,
	jint numberOfOpenedJavaFD
)
{
	int ValToRet = GBFRSW_SUCCESS;

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (numberOfOpenedJavaFD < 0) {
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"SetOpenedJavaFD: numberOfOpenedJavaFD less than 0");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	} else if (numberOfOpenedJavaFD == 0) {
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"SetOpenedJavaFD: numberOfOpenedJavaFD = 0, no operations to perform");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	if (openedJavaFd == NULL) {
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"SetOpenedJavaFD: openedJavaFd is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	} else if ((env)->GetArrayLength(openedJavaFd) < numberOfOpenedJavaFD) {
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"SetOpenedJavaFD: openedJavaFd length too small");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	if (bus_number == NULL) {
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"SetOpenedJavaFD: bus_number is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	} else if ((env)->GetArrayLength(bus_number) < numberOfOpenedJavaFD) {
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"SetOpenedJavaFD: bus_number length too small");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	if (devAddress == NULL) {
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"SetOpenedJavaFD: devAddress is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	} else if ((env)->GetArrayLength(devAddress) < numberOfOpenedJavaFD) {
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"SetOpenedJavaFD: devAddress length too small");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	// openedJavaFD
	jint *jintOpenedJavaFD = (env)->GetIntArrayElements(openedJavaFd, NULL);
	if (jintOpenedJavaFD == NULL) {
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"SetOpenedJavaFD: cannot get elements from openedJavaFd");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	int *cppOpenedJavaFD = new int[numberOfOpenedJavaFD];
	if (cppOpenedJavaFD == NULL) {
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"SetOpenedJavaFD: cannot alloc space for cppOpenedJavaFD");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		(env)->ReleaseIntArrayElements(openedJavaFd, jintOpenedJavaFD, 0);
		return ValToRet;
	}
	for (int i = 0; i < numberOfOpenedJavaFD; i++) {
		cppOpenedJavaFD[i] = jintOpenedJavaFD[i];
	}
	(env)->ReleaseIntArrayElements(openedJavaFd, jintOpenedJavaFD, 0);
	// bus_number
	jint *jintBus_number = (env)->GetIntArrayElements(bus_number, NULL);
	if (jintBus_number == NULL) {
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"SetOpenedJavaFD: cannot get elements from bus_number");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	int *cppBus_number = new int[numberOfOpenedJavaFD];
	if (cppBus_number == NULL) {
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"SetOpenedJavaFD: cannot alloc space for cppBus_number");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		(env)->ReleaseIntArrayElements(bus_number, jintBus_number, 0);
		delete[] cppOpenedJavaFD;
		return ValToRet;
	}
	for (int i = 0; i < numberOfOpenedJavaFD; i++) {
		cppBus_number[i] = jintBus_number[i];
	}
	(env)->ReleaseIntArrayElements(bus_number, jintBus_number, 0);
	// bus_number
	jint *jintDevAddress = (env)->GetIntArrayElements(devAddress, NULL);
	if (jintDevAddress == NULL) {
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"SetOpenedJavaFD: cannot get elements from devAddress");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	int *cppDevAddress = new int[numberOfOpenedJavaFD];
	if (cppDevAddress == NULL) {
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"SetOpenedJavaFD: cannot alloc space for cppDevAddress");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		(env)->ReleaseIntArrayElements(devAddress, jintDevAddress, 0);
		delete[] cppOpenedJavaFD;
		delete[] cppBus_number;
		return ValToRet;
	}
	for (int i = 0; i < numberOfOpenedJavaFD; i++) {
		cppDevAddress[i] = jintDevAddress[i];
	}
	(env)->ReleaseIntArrayElements(devAddress, jintDevAddress, 0);

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (GbfrswDllPtr == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"SetOpenedJavaFD: GbfrswDllPtr is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBFRSW_SetJavaFD == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"SetOpenedJavaFD: Ptr_GBFRSW_SetJavaFD is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// call function
	//////////////////////////////////
	ValToRet = Ptr_GBFRSW_SetJavaFD(
		cppOpenedJavaFD, cppBus_number, cppDevAddress,
		numberOfOpenedJavaFD);
	//////////////////////////////
	// release cpp arrays
	//////////////////////////////
	delete[] cppOpenedJavaFD;
	delete[] cppBus_number;
	delete[] cppDevAddress;

	//////////////////////////////////
	// convert to Java
	//////////////////////////////////
	if (ValToRet == GBFRSW_SUCCESS)
	{
		a_printf ("SetOpenedJavaFD: resources released \n");
	}
	else
	{
		SetGbfrswError("Ptr_GBFRSW_SetJavaFD");
	}
	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbfrsw_GbfrswJavaWrapperLibrary_ISOGetMaxFMCLength(
	JNIEnv *env,
	jobject thisObj,
	// INPUT
	jint jImageSizeX, jint jImageSizeY,
	jint jFormatType, jint jMaxMinutiaeNum,
	// OUTPUT
	jobject jMaxFMCLength
)
{
	int ValToRet = GBFRSW_SUCCESS;

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jMaxFMCLength == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"ISOGetMaxFMCLength: jMaxFMCLength is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (GbfrswDllPtr == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"ISOGetMaxFMCLength: GbfrswDllPtr is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBFRSW_ISO_GetMaxFMCLength == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"ISOGetMaxFMCLength: Ptr_GBFRSW_ISO_GetMaxFMCLength is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	///////////////////////////////////
	// convert java to C
	///////////////////////////////////

	//////////////////////////////////
	// call function
	//////////////////////////////////
	unsigned short MaxFMCLength;

	ValToRet = Ptr_GBFRSW_ISO_GetMaxFMCLength(
		jFormatType,
		jImageSizeX, jImageSizeY,
		jMaxMinutiaeNum,
		&MaxFMCLength);

	//////////////////////////////////
	// convert to Java
	//////////////////////////////////
	if (ValToRet == GBFRSW_SUCCESS)
	{
		/****************
		 * width, length,
		 * bits per pixel,
		 * ppi, lossyflag
		 */
		GBANUTIL_GetJIntExchangeFromCInt((int) (MaxFMCLength), jMaxFMCLength,env);
	}
	else
	{
		SetGbfrswError("Ptr_GBFRSW_ISO_GetMaxFMRLength");
	}
	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_gbfrsw_GbfrswJavaWrapperLibrary_ISOGBTemplateToFMC(
	JNIEnv *env,
	jobject thisObj,
	// INPUT
	jbyteArray jGBTemplate,
	jint jImageSizeX, jint jImageSizeY,
	jint jFormatType, jint jMaxMinutiaeNum,
	// OUTPUT
	jbyteArray jFMCBuffer,
	jobject jFMCBufferLen
)
{
	int ValToRet = GBFRSW_SUCCESS;

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jFMCBufferLen == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"ISOGBTemplateToFMC(: jFMCBufferLen is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFMCBuffer == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"ISOGBTemplateToFMC(: jFMCBuffer is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	if (jGBTemplate == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"ISOGBTemplateToFMC(: jGBTemplate is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (GbfrswDllPtr == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"ISOGBTemplateToFMC: GbfrswDllPtr is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBFRSW_ISO_GetMaxFMCLength == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"ISOGBTemplateToFMC: Ptr_GBFRSW_ISO_GetMaxFMCLength is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBFRSW_ISO_GBTemplateToFMC == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"ISOGBTemplateToFMC: Ptr_GBFRSW_ISO_GBTemplateToFMC is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// call GetMaxFMCLen in order to
	// check that the FMcBuffer len
	// is adequate
	//////////////////////////////////
	unsigned short MaxFMCLen;
	ValToRet = Ptr_GBFRSW_ISO_GetMaxFMCLength(
		jFormatType,
		jImageSizeX, jImageSizeY, jMaxMinutiaeNum, &MaxFMCLen);
	if (ValToRet != GBFRSW_SUCCESS)
	{
		SetGbfrswError("ISOGBTemplateToFMC: call to Ptr_GBFRSW_ISO_GetMaxFMCLength");
		return ValToRet;
	}

	/************************************
	 * Get original code header in order
	 * to calculate minimum size
	 */
	unsigned int SampleCodeSize, PackedSampleCodeSize, TemplateCodeSize, CompactTemplateCodeSize;
	if (Ptr_GBFRSW_CodeMaxSize != NULL)
	{
		ValToRet = Ptr_GBFRSW_CodeMaxSize(
			jImageSizeX, jImageSizeY, &SampleCodeSize,
			&PackedSampleCodeSize, &TemplateCodeSize, &CompactTemplateCodeSize
		);
		if (ValToRet != GBFRSW_SUCCESS)
		{
			SetGbfrswError("ISOGBTemplateToFMC while calling GBFRSW_CodeMaxSize");
			return ValToRet;
		}
	}
	else
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"ISOGBTemplateToFMC: Ptr_GBFRSW_CodeMaxSize is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	if (env->GetArrayLength(jGBTemplate) < (sizeof(GBFRSW_CODE_HEADER)))
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"ISOGBTemplateToFMC: jGBTemplate length %d not enough",
				env->GetArrayLength(jGBTemplate));
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	GBFRSW_CODE_HEADER OriginalHeader;
	int minimumSizeForGBTemplate;
	env->GetByteArrayRegion(jGBTemplate, 0, sizeof(GBFRSW_CODE_HEADER),
							reinterpret_cast<jbyte *>(&OriginalHeader));
	if (OriginalHeader.Type == GBFRSW_CODE_TYPE_SAMPLE)
	{
		minimumSizeForGBTemplate = SampleCodeSize;
	}
	else if (OriginalHeader.Type == GBFRSW_CODE_TYPE_SAMPLE_PACKED)
	{
		minimumSizeForGBTemplate = PackedSampleCodeSize;
	}
	else if (OriginalHeader.Type == GBFRSW_CODE_TYPE_TEMPLATE)
	{
		minimumSizeForGBTemplate = TemplateCodeSize;
	}
	else if (OriginalHeader.Type == GBFRSW_CODE_TYPE_TEMPLATE_COMPACT)
	{
		minimumSizeForGBTemplate = CompactTemplateCodeSize;
	}
	else
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"ISOGBTemplateToFMC: type not recognized for jGBTemplate");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	/***********************************
	 * Original code size
	 */
	if (env->GetArrayLength(jGBTemplate) < (minimumSizeForGBTemplate))
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"ISOGBTemplateToFMC: jGBTemplate length %d not enough, should be at least %d",
				env->GetArrayLength(jGBTemplate), minimumSizeForGBTemplate);
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	if (env->GetArrayLength(jFMCBuffer) < (MaxFMCLen))
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"ISOGBTemplateToFMC: jFMCBuffer length %d not enough, should be at least %d",
				env->GetArrayLength(jFMCBuffer), MaxFMCLen);
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	///////////////////////////////////
	// convert java to C
	///////////////////////////////////
	unsigned char *GBTemplate = new unsigned char[minimumSizeForGBTemplate];
	env->GetByteArrayRegion(jGBTemplate, 0, minimumSizeForGBTemplate,
							reinterpret_cast<jbyte *>(GBTemplate));
	unsigned char *FMCBuffer = new unsigned char[MaxFMCLen];

	//////////////////////////////////
	// call function
	//////////////////////////////////
	unsigned short FMCBufferLen;

	ValToRet = Ptr_GBFRSW_ISO_GBTemplateToFMC(
		GBTemplate, FMCBuffer, &FMCBufferLen, jFormatType, jMaxMinutiaeNum
	);
	delete[] GBTemplate;

	//////////////////////////////////
	// convert to Java
	//////////////////////////////////
	if (ValToRet == GBFRSW_SUCCESS)
	{
		GBANUTIL_GetJIntExchangeFromCInt((int) (FMCBufferLen), jFMCBufferLen,env);
		env->SetByteArrayRegion(jFMCBuffer, 0, FMCBufferLen,
								reinterpret_cast<jbyte *>(FMCBuffer));
	}
	else
	{
		SetGbfrswError("Ptr_GBFRSW_ISO_GBTemplateToFMC");
	}
	delete[] FMCBuffer;
	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_gbfrsw_GbfrswJavaWrapperLibrary_ISOFMRToGBTemplate(
	JNIEnv *env,
	jobject thisObj,
	// INPUT
	jbyteArray jFMRBuffer,
	jint jFormatType,
	// OUTPUT
	jbyteArray jGBTemplate,
	jobject jFingerPosition
)
{
	int ValToRet = GBFRSW_SUCCESS;

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jFMRBuffer == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"ISOFMRToGBTemplate: jFMRBuffer is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	if (jGBTemplate == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"ISOFMRToGBTemplate: jGBTemplate is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (GbfrswDllPtr == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"ISOFMRToGBTemplate: GbfrswDllPtr is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBFRSW_ISO_FMRToGBTemplate == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"ISOFMRToGBTemplate: Ptr_GBFRSW_ISO_FMRToGBTemplate is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	///////////////////////////////////
	// convert java to C
	///////////////////////////////////
	unsigned char *GBTemplate = new unsigned char[env->GetArrayLength(jGBTemplate)];
	unsigned char *FMRBuffer = new unsigned char[env->GetArrayLength(jFMRBuffer)];
	env->GetByteArrayRegion(jFMRBuffer, 0, env->GetArrayLength(jFMRBuffer),
							reinterpret_cast<jbyte *>(FMRBuffer));

	//////////////////////////////////
	// call function
	//////////////////////////////////
	unsigned char FingerPos;

	ValToRet = Ptr_GBFRSW_ISO_FMRToGBTemplate(FMRBuffer, GBTemplate, &FingerPos, jFormatType);
	delete[] FMRBuffer;

	//////////////////////////////////
	// convert to Java
	//////////////////////////////////
	if (ValToRet == GBFRSW_SUCCESS)
	{
		GBANUTIL_GetJIntExchangeFromCInt((int) (FingerPos), jFingerPosition,env);
		env->SetByteArrayRegion(jGBTemplate, 0, env->GetArrayLength(jGBTemplate),
								reinterpret_cast<jbyte *>(GBTemplate));
	}
	else
	{
		SetGbfrswError("Ptr_GBFRSW_ISO_FMRToGBTemplate");
	}
	delete[] GBTemplate;
	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_gbfrsw_GbfrswJavaWrapperLibrary_ISOGBTemplateToFMR(
	JNIEnv *env,
	jobject thisObj,
	// INPUT
	jbyteArray jGBTemplate,
	jint jImageSizeX, jint jImageSizeY,
	jint jFingerPosition,
	jint jFormatType, jint jGBProprietaryData,
	// OUTPUT
	jbyteArray jFMRBuffer,
	jobject jFMRBufferLen
)
{
	int ValToRet = GBFRSW_SUCCESS;

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jFMRBufferLen == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"ISOGBTemplateToFMR: jFMRBufferLen is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFMRBuffer == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"ISOGBTemplateToFMR: jFMRBuffer is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	if (jGBTemplate == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"ISOGBTemplateToFMR: jGBTemplate is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (GbfrswDllPtr == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"ISOGBTemplateToFMR: GbfrswDllPtr is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBFRSW_ISO_GetMaxFMRLength == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"ISOGBTemplateToFMR: Ptr_GBFRSW_ISO_GetMaxFMRLength is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBFRSW_ISO_GBTemplateToFMR == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"ISOGBTemplateToFMR: Ptr_GBFRSW_ISO_GBTemplateToFMR is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// call GetMaxFMRLen in order to
	// check that the FMRBuffer len
	// is adequate
	//////////////////////////////////
	unsigned short MaxFMRLen;
	ValToRet = Ptr_GBFRSW_ISO_GetMaxFMRLength(
		jImageSizeX, jImageSizeY,
		jFormatType, jGBProprietaryData, &MaxFMRLen);
	if (ValToRet != GBFRSW_SUCCESS)
	{
		SetGbfrswError("ISOGBTemplateToFMR: call to GBFRSW_ISO_GetMaxFMRLength");
		return ValToRet;
	}

	/************************************
	 * Get original code header in order
	 * to calculate minimum size
	 */
	unsigned int SampleCodeSize, PackedSampleCodeSize, TemplateCodeSize, CompactTemplateCodeSize;
	if (Ptr_GBFRSW_CodeMaxSize != NULL)
	{
		ValToRet = Ptr_GBFRSW_CodeMaxSize(
			jImageSizeX, jImageSizeY, &SampleCodeSize,
			&PackedSampleCodeSize, &TemplateCodeSize, &CompactTemplateCodeSize
		);
		if (ValToRet != GBFRSW_SUCCESS)
		{
			SetGbfrswError("ISOGBTemplateToFMR while calling GBFRSW_CodeMaxSize");
			return ValToRet;
		}
	}
	else
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"ISOGBTemplateToFMR: Ptr_GBFRSW_CodeMaxSize is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	if (env->GetArrayLength(jGBTemplate) < (sizeof(GBFRSW_CODE_HEADER)))
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"ISOGBTemplateToFMR: jGBTemplate length %d not enough",
				env->GetArrayLength(jGBTemplate));
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	GBFRSW_CODE_HEADER OriginalHeader;
	int minimumSizeForGBTemplate;
	env->GetByteArrayRegion(jGBTemplate, 0, sizeof(GBFRSW_CODE_HEADER),
							reinterpret_cast<jbyte *>(&OriginalHeader));
	if (OriginalHeader.Type == GBFRSW_CODE_TYPE_SAMPLE)
	{
		minimumSizeForGBTemplate = SampleCodeSize;
	}
	else if (OriginalHeader.Type == GBFRSW_CODE_TYPE_SAMPLE_PACKED)
	{
		minimumSizeForGBTemplate = PackedSampleCodeSize;
	}
	else if (OriginalHeader.Type == GBFRSW_CODE_TYPE_TEMPLATE)
	{
		minimumSizeForGBTemplate = TemplateCodeSize;
	}
	else if (OriginalHeader.Type == GBFRSW_CODE_TYPE_TEMPLATE_COMPACT)
	{
		minimumSizeForGBTemplate = CompactTemplateCodeSize;
	}
	else
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"ISOGBTemplateToFMR: type not recognized for jGBTemplate");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	/***********************************
	 * Original code size
	 */
	if (env->GetArrayLength(jGBTemplate) < (minimumSizeForGBTemplate))
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"ISOGBTemplateToFMR: jGBTemplate length %d not enough, should be at least %d",
				env->GetArrayLength(jGBTemplate), minimumSizeForGBTemplate);
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	if (env->GetArrayLength(jFMRBuffer) < (MaxFMRLen))
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"ISOGBTemplateToFMR: jFMRBuffer length %d not enough, should be at least %d",
				env->GetArrayLength(jFMRBuffer), MaxFMRLen);
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	///////////////////////////////////
	// convert java to C
	///////////////////////////////////
	unsigned char *GBTemplate = new unsigned char[minimumSizeForGBTemplate];
	env->GetByteArrayRegion(jGBTemplate, 0, minimumSizeForGBTemplate,
							reinterpret_cast<jbyte *>(GBTemplate));
	unsigned char *FMRBuffer = new unsigned char[MaxFMRLen];

	//////////////////////////////////
	// call function
	//////////////////////////////////
	unsigned short FMRBufferLen;

	ValToRet = Ptr_GBFRSW_ISO_GBTemplateToFMR(
		GBTemplate, FMRBuffer, &FMRBufferLen, jFingerPosition, jFormatType, jGBProprietaryData
	);
	delete[] GBTemplate;

	//////////////////////////////////
	// convert to Java
	//////////////////////////////////
	if (ValToRet == GBFRSW_SUCCESS)
	{
		GBANUTIL_GetJIntExchangeFromCInt((int) (FMRBufferLen), jFMRBufferLen,env);
		env->SetByteArrayRegion(jFMRBuffer, 0, FMRBufferLen,
								reinterpret_cast<jbyte *>(FMRBuffer));
	}
	else
	{
		SetGbfrswError("Ptr_GBFRSW_ISO_GBTemplateToFMR");
	}
	delete[] FMRBuffer;
	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_gbfrsw_GbfrswJavaWrapperLibrary_ISOGetMaxFMRLength(
	JNIEnv *env,
	jobject thisObj,
	// INPUT
	jint jImageSizeX, jint jImageSizeY,
	jint jFormatType, jint jGBProprietaryData,
	// OUTPUT
	jobject jMaxFMRLength
)
{
	int ValToRet = GBFRSW_SUCCESS;

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jMaxFMRLength == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"ISOGetMaxFMRLength: jMaxFMRLength is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (GbfrswDllPtr == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"ISOGetMaxFMRLength: GbfrswDllPtr is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBFRSW_ISO_GetMaxFMRLength == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"ISOGetMaxFMRLength: Ptr_GBFRSW_ISO_GetMaxFMRLength is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	///////////////////////////////////
	// convert java to C
	///////////////////////////////////

	//////////////////////////////////
	// call function
	//////////////////////////////////
	unsigned short MaxFRMLength;

	ValToRet = Ptr_GBFRSW_ISO_GetMaxFMRLength(
		jImageSizeX, jImageSizeY,
		jFormatType, jGBProprietaryData,
		&MaxFRMLength);

	//////////////////////////////////
	// convert to Java
	//////////////////////////////////
	if (ValToRet == GBFRSW_SUCCESS)
	{
		/****************
		 * width, length,
		 * bits per pixel,
		 * ppi, lossyflag
		 */
		GBANUTIL_GetJIntExchangeFromCInt((int) (MaxFRMLength), jMaxFMRLength,env);
	}
	else
	{
		SetGbfrswError("GBFRSW_ISO_GetMaxFMRLength");
	}
	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbfrsw_GbfrswJavaWrapperLibrary_GetCodeHeader(
	JNIEnv *env,
	jobject thisObj,
	// INPUT
	jbyteArray jInputCode,
	// OUTPUT
	jobject jSize, jobject jSizeAfterConversion,
	jobject jType, jobject jVersion,
	jobject jImageFlags, jobject jQuality,
	jobject jInformationCapacity,
	jobject jImageSx, jobject jImageSy
)
{
	int ValToRet = GBFRSW_SUCCESS;

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jInputCode == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"GetCodeHeader: jInputCode is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	/************************************
	 * Get original code header in order
	 * to calculate minimum size
	 */
	if (env->GetArrayLength(jInputCode) < (sizeof(GBFRSW_CODE_HEADER)))
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"GetCodeHeader: jInputCode length %d not enough", env->GetArrayLength(jInputCode));
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	GBFRSW_CODE_HEADER OriginalHeader;
	int minimumSizeForOriginal;
	env->GetByteArrayRegion(jInputCode, 0, sizeof(GBFRSW_CODE_HEADER),
							reinterpret_cast<jbyte *>(&OriginalHeader));
	minimumSizeForOriginal = OriginalHeader.Size;
	if (OriginalHeader.Type == GBFRSW_CODE_TYPE_SAMPLE)
	{
	}
	else if (OriginalHeader.Type == GBFRSW_CODE_TYPE_SAMPLE_PACKED)
	{
	}
	else if (OriginalHeader.Type == GBFRSW_CODE_TYPE_TEMPLATE)
	{
	}
	else if (OriginalHeader.Type == GBFRSW_CODE_TYPE_TEMPLATE_COMPACT)
	{
	}
	else
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"GetCodeHeader: type not recognized for jOriginalCodePtr");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	/***********************************
	 * Original code size
	 */
	if (env->GetArrayLength(jInputCode) < (minimumSizeForOriginal))
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"GetCodeHeader: jOriginalCodePtr length %d not enough, should be at least %d",
				env->GetArrayLength(jInputCode), minimumSizeForOriginal);
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// set parameters
	//////////////////////////////////
	if (jSizeAfterConversion != NULL)
		GBANUTIL_GetJIntExchangeFromCInt((int) (OriginalHeader.SizeAfterConversion), jSizeAfterConversion,env);
	if (jSize != NULL)
		GBANUTIL_GetJIntExchangeFromCInt((int) (OriginalHeader.Size), jSize,env);
	if (jType != NULL)
		GBANUTIL_GetJIntExchangeFromCInt((int) (OriginalHeader.Type), jType,env);
	if (jVersion != NULL)
		GBANUTIL_GetJIntExchangeFromCInt((int) (OriginalHeader.Version), jVersion,env);
	if (jImageFlags != NULL)
		GBANUTIL_GetJIntExchangeFromCInt((int) (OriginalHeader.ImageFlags), jImageFlags,env);
	if (jQuality != NULL)
		GBANUTIL_GetJIntExchangeFromCInt((int) (OriginalHeader.Quality), jQuality,env);
	if (jImageSx != NULL)
		GBANUTIL_GetJIntExchangeFromCInt((int) (OriginalHeader.ImageSizeX), jImageSx,env);
	if (jImageSy != NULL)
		GBANUTIL_GetJIntExchangeFromCInt((int) (OriginalHeader.ImageSizeY), jImageSy,env);
	// float
	if (jInformationCapacity != NULL)
		GBANUTIL_GetJDoubleExchangeFromCDouble((double) (OriginalHeader.InformationCapacity), jInformationCapacity, env);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbfrsw_GbfrswJavaWrapperLibrary_GetVersionInfo(
	JNIEnv *env,
	jobject thisObj,
	// INPUT
	jint jRequestedOperations,
	// OUTPUT
	jobject jVer1, jobject jVer2, jobject jVer3, jobject jVer4,
	jobject jMaxImageSx, jobject jMaxImageSy,
	jobject jMinImageSx, jobject jMinImageSy,
	jobject jMemoryBufferSize)
{
	int ValToRet = GBFRSW_SUCCESS;

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jVer1 == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"GetVersionInfo: jVer1 is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	if (jVer2 == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"GetVersionInfo: jVer2 is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	if (jVer3 == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"GetVersionInfo: jVer3 is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	if (jVer4 == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"GetVersionInfo: jVer4 is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	if (jMaxImageSx == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"GetVersionInfo: jMaxImageSx is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	if (jMaxImageSy == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"GetVersionInfo: jMaxImageSy is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	if (jMinImageSx == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"GetVersionInfo: jMinImageSx is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	if (jMinImageSy == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"GetVersionInfo: jMinImageSy is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	if (jMemoryBufferSize == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"GetVersionInfo: jMemoryBufferSize is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (GbfrswDllPtr == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"GetVersionInfo: GbfrswDllPtr is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBFRSW_GetVersionInfo == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"GetVersionInfo: Ptr_GBFRSW_GetVersionInfo is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	///////////////////////////////////
	// convert java to C
	///////////////////////////////////

	//////////////////////////////////
	// call function
	//////////////////////////////////
	unsigned char VersionField1;
	unsigned char VersionField2;
	unsigned char VersionField3;
	unsigned char VersionField4;
	unsigned int MaxImageSizeX;
	unsigned int MaxImageSizeY;
	unsigned int MinImageSizeX;
	unsigned int MinImageSizeY;
	unsigned int MemoryBufferSize;

	ValToRet = Ptr_GBFRSW_GetVersionInfo(&VersionField1, &VersionField2, &VersionField3,
										 &VersionField4,
										 &MaxImageSizeX, &MaxImageSizeY, &MinImageSizeX,
										 &MinImageSizeY,
										 jRequestedOperations, &MemoryBufferSize);

	//////////////////////////////////
	// convert to Java
	//////////////////////////////////
	if (ValToRet == GBFRSW_SUCCESS)
	{
		/****************
		 * width, length,
		 * bits per pixel,
		 * ppi, lossyflag
		 */
		GBANUTIL_GetJIntExchangeFromCInt((int) (VersionField1), jVer1,env);
		GBANUTIL_GetJIntExchangeFromCInt((int) (VersionField2), jVer2,env);
		GBANUTIL_GetJIntExchangeFromCInt((int) (VersionField3), jVer3,env);
		GBANUTIL_GetJIntExchangeFromCInt((int) (VersionField4), jVer4,env);
		GBANUTIL_GetJIntExchangeFromCInt((int) (MaxImageSizeX), jMaxImageSx,env);
		GBANUTIL_GetJIntExchangeFromCInt((int) (MaxImageSizeY), jMaxImageSy,env);
		GBANUTIL_GetJIntExchangeFromCInt((int) (MinImageSizeX), jMinImageSx,env);
		GBANUTIL_GetJIntExchangeFromCInt((int) (MinImageSizeY), jMinImageSy,env);
		GBANUTIL_GetJIntExchangeFromCInt((int) (MemoryBufferSize), jMemoryBufferSize,env);

		/*********************
		 * init the GBFRSW
		 */
		int (__stdcall *Ptr_GBFRSW_Init)(
			unsigned char *MemoryBuffer,
			unsigned int MemoryBufferSize);

		Ptr_GBFRSW_Init = (int (*)(unsigned char *, unsigned int))
			GetProcAddress(GbfrswDllPtr, "GBFRSW_Init");
		if (Ptr_GBFRSW_Init == NULL)
		{
			ValToRet = GBFRSW_ERROR;
			sprintf(Gbfrsw_JNI_LastErrorString,
					"GetVersionInfo: Unable to load Ptr_GBFRSW_Init: %s", dlerror());
			a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
			return ValToRet;
		}

		if (GBFRSW_MemoryBuffer == NULL)
		{
			GBFRSW_MemoryBuffer = new unsigned char[MemoryBufferSize];
			if (GBFRSW_MemoryBuffer == NULL)
			{
				ValToRet = GBFRSW_ERROR;
				sprintf(Gbfrsw_JNI_LastErrorString,
						"GetVersionInfo: Not enough memory for memory buffer:");
				a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
				return ValToRet;
			}
		}

		ValToRet = Ptr_GBFRSW_Init(GBFRSW_MemoryBuffer, MemoryBufferSize);
		if (ValToRet != GBFRSW_SUCCESS)
		{
			SetGbfrswError("GBFRSW_GetVersionInfo");
		}
	}
	else
	{
		SetGbfrswError("GBFRSW_GetVersionInfo");
	}
	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbfrsw_GbfrswJavaWrapperLibrary_GetCodeMaxSize(
	JNIEnv *env,
	jobject thisObj,
	// INPUT
	jint jImageSizeX, jint jImageSizeY,
	// OUTPUT
	jobject jSampleCodeSize, jobject jPackedSampleCodeSize,
	jobject jTemplateCodeSize, jobject jCompactTemplateCodeSize
)
{
	int ValToRet = GBFRSW_SUCCESS;

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jSampleCodeSize == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"GetCodeMaxSize: jSampleCodeSize is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	if (jPackedSampleCodeSize == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"GetCodeMaxSize: jPackedSampleCodeSize is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	if (jTemplateCodeSize == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"GetCodeMaxSize: jTemplateCodeSize is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	if (jCompactTemplateCodeSize == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"GetCodeMaxSize: jCompactTemplateCodeSize is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (GbfrswDllPtr == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"GetCodeMaxSize: GbfrswDllPtr is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBFRSW_CodeMaxSize == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"GetCodeMaxSize: Ptr_GBFRSW_CodeMaxSize is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	///////////////////////////////////
	// convert java to C
	///////////////////////////////////

	//////////////////////////////////
	// call function
	//////////////////////////////////
	unsigned int SampleCodeSize;
	unsigned int PackedSampleCodeSize;
	unsigned int TemplateCodeSize;
	unsigned int CompactTemplateCodeSize;

	ValToRet = Ptr_GBFRSW_CodeMaxSize(jImageSizeX, jImageSizeY,
									  &SampleCodeSize, &PackedSampleCodeSize,
									  &TemplateCodeSize, &CompactTemplateCodeSize);

	//////////////////////////////////
	// convert to Java
	//////////////////////////////////
	if (ValToRet == GBFRSW_SUCCESS)
	{
		/****************
		 * width, length,
		 * bits per pixel,
		 * ppi, lossyflag
		 */
		GBANUTIL_GetJIntExchangeFromCInt((int) (SampleCodeSize), jSampleCodeSize,env);
		GBANUTIL_GetJIntExchangeFromCInt((int) (PackedSampleCodeSize), jPackedSampleCodeSize,env);
		GBANUTIL_GetJIntExchangeFromCInt((int) (TemplateCodeSize), jTemplateCodeSize,env);
		GBANUTIL_GetJIntExchangeFromCInt((int) (CompactTemplateCodeSize), jCompactTemplateCodeSize,env);
	}
	else
	{
		SetGbfrswError("GBFRSW_CodeMaxSize");
	}
	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbfrsw_GbfrswJavaWrapperLibrary_Coding(
	JNIEnv *env,
	jobject thisObj,
	// INPUT
	jint jImageSizeX, jint jImageSizeY,
	jbyteArray jImageBuffer,
	jint jImageFlags, jint jOptions,
	// OUTPUT
	jbyteArray jSampleCode
)
{
	int ValToRet = GBFRSW_SUCCESS;

	/***********************************
	 * CALL INFO AND SIZE FUNCTION IN
	 * ORDER TO PROCEED
	 */
	unsigned char v;
	unsigned int Msx, Msy, msx, msy;
	unsigned int mbf;
	if (Ptr_GBFRSW_GetVersionInfo != NULL)
	{
		ValToRet = Ptr_GBFRSW_GetVersionInfo(
			&v, &v, &v, &v,
			&Msx, &Msy, &msx, &msy,
			GBFRSW_MEMORY_REQUEST_CODING,
			&mbf
		);
		if (ValToRet != GBFRSW_SUCCESS)
		{
			SetGbfrswError("GBFRSW_Coding while calling GBFRSW_GetVersionInfo");
			return ValToRet;
		}
	}
	else
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Coding: Ptr_GBFRSW_GetVersionInfo is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	unsigned int SampleCodeSize;
	if (Ptr_GBFRSW_CodeMaxSize != NULL)
	{
		ValToRet = Ptr_GBFRSW_CodeMaxSize(
			jImageSizeX, jImageSizeY, &SampleCodeSize,
			NULL, NULL, NULL
		);
		if (ValToRet != GBFRSW_SUCCESS)
		{
			SetGbfrswError("GBFRSW_Coding while calling GBFRSW_CodeMaxSize");
			return ValToRet;
		}
	}
	else
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Coding: Ptr_GBFRSW_CodeMaxSize is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jImageSizeX < msx || jImageSizeX > Msx)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Coding: jImageSizeX outside range");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	if (jImageSizeY < msy || jImageSizeY > Msy)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Coding: jImageSizeY outside range");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	if (jImageBuffer == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Coding: jImageBuffer is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetArrayLength(jImageBuffer) < (jImageSizeX * jImageSizeY))
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Coding: jImageBuffer length %d not enough, should be at least %d",
				env->GetArrayLength(jImageBuffer), (jImageSizeX * jImageSizeY));
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	if (jSampleCode == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Coding: jSampleCode is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetArrayLength(jSampleCode) < (SampleCodeSize))
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Coding: jSampleCode length %d not enough, should be at least %d",
				env->GetArrayLength(jSampleCode), (SampleCodeSize));
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (GbfrswDllPtr == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Coding: GbfrswDllPtr is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBFRSW_Coding == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Coding: Ptr_GBFRSW_Coding is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	///////////////////////////////////
	// convert java to C
	///////////////////////////////////
	/************************
	 * source
	 */
	unsigned char *PtrToSource = new unsigned char[jImageSizeX * jImageSizeY];
	env->GetByteArrayRegion(jImageBuffer, 0, jImageSizeX * jImageSizeY,
							reinterpret_cast<jbyte *>(PtrToSource));
	/*************************
	 * destination
	 */
	unsigned char *PtrToDestination = new unsigned char[SampleCodeSize];

	//////////////////////////////////
	// call function
	//////////////////////////////////
	ValToRet = Ptr_GBFRSW_Coding(jImageSizeX, jImageSizeY, PtrToSource,
								 jImageFlags, jOptions,
								 PtrToDestination);
	delete[] PtrToSource;

	//////////////////////////////////
	// convert to Java
	//////////////////////////////////
	if (ValToRet == GBFRSW_SUCCESS)
	{
		/****************
		 * width, length,
		 * bits per pixel,
		 * ppi, lossyflag
		 */
		env->SetByteArrayRegion(
			jSampleCode, 0, SampleCodeSize,
			reinterpret_cast<jbyte *>(PtrToDestination));
	}
	else
	{
		SetGbfrswError("GBFRSW_Coding");
	}
	delete[] PtrToDestination;

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbfrsw_GbfrswJavaWrapperLibrary_Enroll(
	JNIEnv *env,
	jobject thisObj,
	// INPUT
	jint jImageSizeX, jint jImageSizeY,
	jbyteArray jSampleCode,
	// OUTPUT
	jbyteArray jTemplateCode
)
{
	int ValToRet = GBFRSW_SUCCESS;

	/***********************************
	 * CALL INFO AND SIZE FUNCTION IN
	 * ORDER TO PROCEED
	 */
	unsigned char v;
	unsigned int Msx, Msy, msx, msy;
	unsigned int mbf;
	if (Ptr_GBFRSW_GetVersionInfo != NULL)
	{
		ValToRet = Ptr_GBFRSW_GetVersionInfo(
			&v, &v, &v, &v,
			&Msx, &Msy, &msx, &msy,
			GBFRSW_MEMORY_REQUEST_CODING,
			&mbf
		);
		if (ValToRet != GBFRSW_SUCCESS)
		{
			SetGbfrswError("Enroll while calling GBFRSW_GetVersionInfo");
			return ValToRet;
		}
	}
	else
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Enroll: Ptr_GBFRSW_GetVersionInfo is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	unsigned int SampleCodeSize, TemplateCodeSize;
	if (Ptr_GBFRSW_CodeMaxSize != NULL)
	{
		ValToRet = Ptr_GBFRSW_CodeMaxSize(
			jImageSizeX, jImageSizeY, &SampleCodeSize,
			NULL, &TemplateCodeSize, NULL
		);
		if (ValToRet != GBFRSW_SUCCESS)
		{
			SetGbfrswError("Enroll while calling GBFRSW_CodeMaxSize");
			return ValToRet;
		}
	}
	else
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Enroll: Ptr_GBFRSW_CodeMaxSize is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jImageSizeX < msx || jImageSizeX > Msx)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Enroll: jImageSizeX outside range");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	if (jImageSizeY < msy || jImageSizeY > Msy)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Enroll: jImageSizeY outside range");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	if (jSampleCode == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Enroll: jSampleCode is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetArrayLength(jSampleCode) < (SampleCodeSize))
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Enroll: jSampleCode length %d not enough, should be at least %d",
				env->GetArrayLength(jSampleCode), (SampleCodeSize));
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	if (jTemplateCode == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Enroll: jTemplateCode is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetArrayLength(jTemplateCode) < (TemplateCodeSize))
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Enroll: jTemplateCode length %d not enough, should be at least %d",
				env->GetArrayLength(jTemplateCode), (TemplateCodeSize));
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (GbfrswDllPtr == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Enroll: GbfrswDllPtr is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBFRSW_Enroll == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Enroll: Ptr_GBFRSW_Enroll is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	///////////////////////////////////
	// convert java to C
	///////////////////////////////////
	/************************
	 * source
	 */
	unsigned char *PtrToSource = new unsigned char[SampleCodeSize];

	env->GetByteArrayRegion(jSampleCode, 0, SampleCodeSize,
							reinterpret_cast<jbyte *>(PtrToSource));
	/*************************
	 * destination
	 */
	unsigned char *PtrToDestination = new unsigned char[TemplateCodeSize];

	//////////////////////////////////
	// call function
	//////////////////////////////////
	ValToRet = Ptr_GBFRSW_Enroll(
		PtrToSource,
		PtrToDestination);
	delete[] PtrToSource;

	//////////////////////////////////
	// convert to Java
	//////////////////////////////////
	if (ValToRet == GBFRSW_SUCCESS)
	{
		/****************
		 * width, length,
		 * bits per pixel,
		 * ppi, lossyflag
		 */
		env->SetByteArrayRegion(
			jTemplateCode, 0, TemplateCodeSize,
			reinterpret_cast<jbyte *>(PtrToDestination));
	}
	else
	{
		SetGbfrswError("Ptr_GBFRSW_Enroll");
	}
	delete[] PtrToDestination;

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbfrsw_GbfrswJavaWrapperLibrary_Match(
	JNIEnv *env,
	jobject thisObj,
	// INPUT
	jbyteArray jSampleCode, jbyteArray jTemplateCode,
	jint jSpeedVsPrecisionTradeoff, jint jUnmatchedDataFactor,
	jint jOptions, jint jMaxRotationAngleDegree,
	// OUTPUT
	jobject jMatchingScore
)
{
	int ValToRet = GBFRSW_SUCCESS;

	//////////////////////////////
	// load java class and
	// methods
	//////////////////////////////

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jSampleCode == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Match: jSampleCode is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetArrayLength(jSampleCode) <= (0))
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Enroll: jSampleCode length 0");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	if (jTemplateCode == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Match: jTemplateCode is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetArrayLength(jTemplateCode) <= (0))
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Enroll: jTemplateCode length 0");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	if (jMatchingScore == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Match: jMatchingScore is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (GbfrswDllPtr == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Match: GbfrswDllPtr is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBFRSW_Match == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Match: Ptr_GBFRSW_Match is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	///////////////////////////////////
	// convert java to C
	///////////////////////////////////
	int SampleLen = (env->GetArrayLength(jSampleCode));
	int TemplateLen = (env->GetArrayLength(jTemplateCode));

	/************************
	 * source
	 */
	unsigned char *PtrToSampleCode = new unsigned char[SampleLen];
	env->GetByteArrayRegion(jSampleCode, 0, SampleLen, reinterpret_cast<jbyte *>(PtrToSampleCode));
	unsigned char *PtrToTemplateCode = new unsigned char[TemplateLen];
	env->GetByteArrayRegion(jTemplateCode, 0, TemplateLen,
							reinterpret_cast<jbyte *>(PtrToTemplateCode));

	//////////////////////////////////
	// call function
	//////////////////////////////////
	float MatchingScore;
	ValToRet = Ptr_GBFRSW_Match(
		PtrToSampleCode, PtrToTemplateCode,
		jSpeedVsPrecisionTradeoff, jUnmatchedDataFactor,
		jOptions, jMaxRotationAngleDegree,
		&MatchingScore
	);
	delete[] PtrToSampleCode;
	delete[] PtrToTemplateCode;

	//////////////////////////////////
	// convert to Java
	//////////////////////////////////
	if (ValToRet == GBFRSW_SUCCESS)
	{
		GBANUTIL_GetJDoubleExchangeFromCDouble((double) (MatchingScore), jMatchingScore,env);
	}
	else
	{
		SetGbfrswError("GBFRSW_Match");
	}

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbfrsw_GbfrswJavaWrapperLibrary_Convert(
	JNIEnv *env,
	jobject thisObj,
	// INPUT
	jbyteArray jOriginalCodePtr,
	// OUTPUT
	jbyteArray jConvertedCodePtr
)
{
	int ValToRet = GBFRSW_SUCCESS;

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	/************************************
	 * check Original and Converted are
	 * not NULL
	 */
	if (jOriginalCodePtr == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Convert: jOriginalCodePtr is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	if (jConvertedCodePtr == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Convert: jTemplateCode is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (GbfrswDllPtr == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Convert: GbfrswDllPtr is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBFRSW_Convert == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Convert: Ptr_GBFRSW_Convert is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	///////////////////////////////////
	// convert java to C
	///////////////////////////////////
	/************************
	 * source
	 */
	unsigned char *PtrToSource = new unsigned char[env->GetArrayLength(jOriginalCodePtr)];
	env->GetByteArrayRegion(jOriginalCodePtr, 0, env->GetArrayLength(jOriginalCodePtr),
							reinterpret_cast<jbyte *>(PtrToSource));
	/*************************
	 * destination
	 */
	unsigned char *PtrToDestination = new unsigned char[env->GetArrayLength(jConvertedCodePtr)];

	//////////////////////////////////
	// call function
	//////////////////////////////////
	ValToRet = Ptr_GBFRSW_Convert(
		PtrToSource,
		PtrToDestination);
	delete[] PtrToSource;

	//////////////////////////////////
	// convert to Java
	//////////////////////////////////
	if (ValToRet == GBFRSW_SUCCESS)
	{
		/****************
		 * width, length,
		 * bits per pixel,
		 * ppi, lossyflag
		 */
		env->SetByteArrayRegion(
			jConvertedCodePtr, 0, env->GetArrayLength(jConvertedCodePtr),
			reinterpret_cast<jbyte *>(PtrToDestination));
	}
	else
	{
		SetGbfrswError("Ptr_GBFRSW_Convert");
	}
	delete[] PtrToDestination;

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbfrsw_GbfrswJavaWrapperLibrary_Correct(
	JNIEnv *env,
	jobject thisObj,
	// INPUT
	jbyteArray jSampleCode1,
	jbyteArray jSampleCode2,
	jbyteArray jSampleCode3,
	// OUTPUT
	jbyteArray jCorrectedSampleCode
)
{
	int ValToRet = GBFRSW_SUCCESS;
	unsigned char *PtrToSource1 = NULL;
	unsigned char *PtrToSource2 = NULL;
	unsigned char *PtrToSource3 = NULL;

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	bool AtLeastOneNotNull = false;
	if (jSampleCode1 != NULL)
	{
		AtLeastOneNotNull = true;
		PtrToSource1 = new unsigned char[env->GetArrayLength(jSampleCode1)];
		env->GetByteArrayRegion(jSampleCode1, 0, env->GetArrayLength(jSampleCode1),
								reinterpret_cast<jbyte *>(PtrToSource1));
	}
	if (jSampleCode2 != NULL)
	{
		AtLeastOneNotNull = true;
		PtrToSource2 = new unsigned char[env->GetArrayLength(jSampleCode2)];
		env->GetByteArrayRegion(jSampleCode2, 0, env->GetArrayLength(jSampleCode2),
								reinterpret_cast<jbyte *>(PtrToSource2));
	}
	if (jSampleCode3 == NULL)
	{
		AtLeastOneNotNull = true;
		PtrToSource3 = new unsigned char[env->GetArrayLength(jSampleCode3)];
		env->GetByteArrayRegion(jSampleCode3, 0, env->GetArrayLength(jSampleCode3),
								reinterpret_cast<jbyte *>(PtrToSource3));
	}
	if (!AtLeastOneNotNull)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Correct: at least one input sample code should be not NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}
	if (jCorrectedSampleCode == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Correct: jCorrectedSampleCode is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (GbfrswDllPtr == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Correct: GbfrswDllPtr is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBFRSW_Correct == NULL)
	{
		ValToRet = GBFRSW_ERROR;
		sprintf(Gbfrsw_JNI_LastErrorString,
				"Correct: Ptr_GBFRSW_Enroll is NULL");
		a_printf ("%s\n", Gbfrsw_JNI_LastErrorString);
		return ValToRet;
	}

	///////////////////////////////////
	// convert java to C
	///////////////////////////////////
	/*************************
	 * source already done
	 */
	/*************************
	 * destination
	 */
	unsigned char *PtrToDestination = new unsigned char[env->GetArrayLength(jCorrectedSampleCode)];

	//////////////////////////////////
	// call function
	//////////////////////////////////
	ValToRet = Ptr_GBFRSW_Correct(
		PtrToSource1, PtrToSource2, PtrToSource3,
		PtrToDestination);
	if (PtrToSource1 != NULL)
		delete[] PtrToSource1;
	if (PtrToSource2 != NULL)
		delete[] PtrToSource2;
	if (PtrToSource3 != NULL)
		delete[] PtrToSource3;

	//////////////////////////////////
	// convert to Java
	//////////////////////////////////
	if (ValToRet == GBFRSW_SUCCESS)
	{
		/****************
		 * width, length,
		 * bits per pixel,
		 * ppi, lossyflag
		 */
		env->SetByteArrayRegion(
			jCorrectedSampleCode, 0, env->GetArrayLength(jCorrectedSampleCode),
			reinterpret_cast<jbyte *>(PtrToDestination));
	}
	else
	{
		SetGbfrswError("Ptr_GBFRSW_Correct");
	}
	delete[] PtrToDestination;

	return ValToRet;
}

}
