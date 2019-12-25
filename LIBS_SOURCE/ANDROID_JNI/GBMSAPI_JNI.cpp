//-------------------------------------------------------------//
//-------------------------------------------------------------//
// INCLUDE
//-------------------------------------------------------------//
//-------------------------------------------------------------//
#include <jni.h>
#include <string>
#include <dlfcn.h>
#include "GB_SystemConfiguration.h"
#include "GBMSAPI_Defines.h"
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
#define a_printf(...) __android_log_print(ANDROID_LOG_DEBUG, "GBMSAPI_LOG", __VA_ARGS__);

//-------------------------------------------------------------//
//-------------------------------------------------------------//
// GLOBALS
//-------------------------------------------------------------//
//-------------------------------------------------------------//
HMODULE GBMSAPIDllPtr = NULL;

int (__stdcall *Ptr_GBMSAPI_LoadLibrary)() = NULL;

int (__stdcall *Ptr_GBMSAPI_UnloadLibrary)() = NULL;

int (__stdcall *Ptr_GBMSAPI_GetAttachedDeviceList)(
		GBMSAPI_DeviceInfoStruct AttachedDeviceList[GBMSAPI_MAX_PLUGGED_DEVICE_NUM],
		int *AttachedDeviceNumber,
		unsigned long *USBErrorCode
) = NULL;

void (__stdcall *Ptr_GBMSAPI_SetOpenedJavaFD)(
		int *openedJavaFd,
		int *bus_number,
		int *dev_address,
		int numberOfOpenedJavaFd
) = NULL;

int (__stdcall *Ptr_GBMSAPI_SetCurrentDevice)(
		unsigned char DeviceID,
		char *DeviceSerialNumber
) = NULL;

int (__stdcall *Ptr_GBMSAPI_GetCurrentDevice)(
		unsigned char *DeviceID,
		char *DeviceSerialNumber
) = NULL;

int (__stdcall *Ptr_GBMSAPI_GetScannableTypes)(
		unsigned int *ScannableTypesMask
) = NULL;

int (__stdcall *Ptr_GBMSAPI_GetDeviceFeatures)(
		unsigned int *DeviceFeatures
) = NULL;

int (__stdcall *Ptr_GBMSAPI_GetSupportedScanAreas)(
		unsigned int *SupportedScanAreas
) = NULL;

int (__stdcall *Ptr_GBMSAPI_GetSupportedScanAreasByDevice)(
		unsigned char DeviceID,
		unsigned int *SupportedScanAreas
) = NULL;

int (__stdcall *Ptr_GBMSAPI_GetAvailableImageInfo)(
		unsigned int ObjectTypeToScan,
		unsigned int *AvailableImageInfo
) = NULL;

int (__stdcall *Ptr_GBMSAPI_SetRollAreaStandard)(
		unsigned int RollAreaStandard
) = NULL;

int (__stdcall *Ptr_GBMSAPI_GetSupportedScanOptions)(
		unsigned int *SupportedScanOptions
) = NULL;

int (__stdcall *Ptr_GBMSAPI_GetOptionalExternalEquipment)(
		unsigned int *OptionalExternalEquipment
) = NULL;

int (__stdcall *Ptr_GBMSAPI_GetImageSize3)(
		unsigned int ObjectTypeToScan,
		unsigned int OptionMask,
		unsigned int ScanArea,
		unsigned int *FullResImageSizeX,
		unsigned int *FullResImageSizeY,
		unsigned int *PreviewImageSizeX,
		unsigned int *PreviewImageSizeY
) = NULL;

int (__stdcall *Ptr_GBMSAPI_GetMaxImageSize)(
		unsigned int *ImageMaxSizeX,
		unsigned int *ImageMaxSizeY
) = NULL;

int (__stdcall *Ptr_GBMSAPI_GetCalibrationImageSize2)(
		unsigned int ScanArea,
		unsigned int *ImageSizeX,
		unsigned int *ImageSizeY
) = NULL;

int (__stdcall *Ptr_GBMSAPI_GetFrameRateRange2)(
		unsigned char DeviceID,
		unsigned int FrameRateOptions,
		unsigned int ScanArea,
		double *MaxFrameRate,
		double *MinFrameRate,
		double *DefFrameRate
) = NULL;

void (__stdcall *Ptr_GBMSAPI_SetDiagnosticFilterForLCD)(
		unsigned int DiagnosticToBeFiltered
) = NULL;

int (__stdcall *Ptr_GBMSAPI_EnableAutoCaptureBlockForDetectedFakes)(
		unsigned char Flag
) = NULL;

int (__stdcall *Ptr_GBMSAPI_SetHardwareFakeFingerDetectionThreshold)(
		int FFDThreshold
) = NULL;

int (__stdcall *Ptr_GBMSAPI_HardwareFakeFingerDetection)(
		BOOL *FFDFlag,
		unsigned int *FFDDiagnostic
) = NULL;

int (__stdcall *Ptr_GBMSAPI_SetSoftwareFakeFingerDetectionThreshold)(
		int FFDThreshold
) = NULL;

int (__stdcall *Ptr_GBMSAPI_SoftwareFakeFingerDetection)(
		unsigned char *img, int sizeX, int sizeY,
		BOOL *FFDFlag
) = NULL;

int (__stdcall *Ptr_GBMSAPI_SetSelectImageTimeout)(
		unsigned int SelectImageTimeout
) = NULL;

int (__stdcall *Ptr_GBMSAPI_SetLowerHalfPalmCompletenessThreshold)(
		unsigned char CompletenessThreshold
) = NULL;

int (__stdcall *Ptr_GBMSAPI_SetFrameRate2)(
		unsigned int ScanArea,
		unsigned int FrameRateOptions,
		double FrameRate
) = NULL;

int (__stdcall *Ptr_GBMSAPI_EnableManualLEDControl)(
		BOOL Enable
) = NULL;

int (__stdcall *Ptr_GBMSAPI_EnableLEDColorSettingDuringAcquisition)(
		BOOL Enable
) = NULL;

int (__stdcall *Ptr_GBMSAPI_SetImageRotation)(
		int ImageRotation
) = NULL;

int (__stdcall *Ptr_GBMSAPI_EnableDrySkinImgEnhance)(
		unsigned char Enable
) = NULL;

int (__stdcall *Ptr_GBMSAPI_ROLL_EnableRollStripeAcquisition)(
		unsigned int ScanArea, BOOL Enable
) = NULL;

int (__stdcall *Ptr_GBMSAPI_ROLL_EnableBlockComposition)(
		int Enable
) = NULL;

int (__stdcall *Ptr_GBMSAPI_StartAcquisition2)(
		unsigned int ObjectToScan,
		unsigned int OptionMask,
		unsigned int ScanArea,
		GBMSAPI_AcquisitionEventsManagerCallback AcquisitionEventsManagerCallbackPtr,
		void *AcquisitionEventsManagerParameters,
		unsigned int DisplayOptionMask,
		unsigned char ContrastLimitToDisplayOnLCD,
		unsigned char CompletenessLimitToDisplayOnLCD
) = NULL;

int (__stdcall *Ptr_GBMSAPI_ROLL_StopPreview)() = NULL;

int (__stdcall *Ptr_GBMSAPI_StopAcquisition)() = NULL;

int (__stdcall *Ptr_GBMSAPI_SetClippingRegionSize)(
		unsigned int ClippingRegionWidth,
		unsigned int ClippingRegionHeight
) = NULL;

int (__stdcall *Ptr_GBMSAPI_ROLL_SetPreviewTimeout)(
		unsigned int Timeout
) = NULL;

int (__stdcall *Ptr_GBMSAPI_ROLL_SetArtefactCleaningDepth)(
		unsigned char Depth
) = NULL;

int (__stdcall *Ptr_GBMSAPI_VUI_LED_BlinkDuringAcquisition)(
		BOOL blink
) = NULL;

int (__stdcall *Ptr_GBMSAPI_GetDryWetFingerAreaPercent)(
		unsigned char *Dry, unsigned char *Wet
) = NULL;

int (__stdcall *Ptr_GBMSAPI_GetAutoCapturePhase)(
		unsigned int *AcqPhase
) = NULL;

int (__stdcall *Ptr_GBMSAPI_SetAutoCaptureBlocking)(
		BOOL blocking
) = NULL;

int (__stdcall *Ptr_GBMSAPI_GetFingerprintContrast)(
		unsigned char *Contrast
) = NULL;

int (__stdcall *Ptr_GBMSAPI_GetFingerprintSize)(
		unsigned int *Size
) = NULL;

int (__stdcall *Ptr_GBMSAPI_GetLowerHalfPalmCompleteness)(
		unsigned char *LHPCompleteness
) = NULL;

int (__stdcall *Ptr_GBMSAPI_GetClippingRegionPosition)(
		int *ClippingRegionPosX,
		int *ClippingRegionPosYY,
		unsigned int *ClippingRegionSizeX,
		unsigned int *ClippingRegionSizeY
) = NULL;

int (__stdcall *Ptr_GBMSAPI_ROLL_GetCurrentStripeCoord)(
		int *LeftX, int *RightX
) = NULL;

int (__stdcall *Ptr_GBMSAPI_UpdateBackgroundImage)(
		unsigned char *FrameToFinalize,
		unsigned int *Diagnostic
) = NULL;

int (__stdcall *Ptr_GBMSAPI_ImageFinalization)(
		unsigned char *FrameToFinalize
) = NULL;

int (__stdcall *Ptr_GBMSAPI_GetFrameStatistic)(
		unsigned long *AcquiredFrames,
		unsigned long *LostFrames
) = NULL;

int (__stdcall *Ptr_GBMSAPI_ROLL_GetCompositeImageInfo2)(
		unsigned int *RolledArtefactSize,
		unsigned char **MarkerFrame,
		unsigned char **NotWipedArtefactFrame,
		unsigned int *CompositeFingerprintSize,
		unsigned char *CompositeFingerprintContrast,
		unsigned int *FlatFingerprintSize,
		unsigned int *ImageSizeX,
		unsigned int *ImageSizeY
) = NULL;

int (__stdcall *Ptr_GBMSAPI_SetCalibrationImage2)(
		unsigned int ScanArea,
		unsigned char *CalibrationImage
) = NULL;

int (__stdcall *Ptr_GBMSAPI_GetCalibrationImage2)(
		unsigned int ScanArea,
		unsigned char *CalibrationImage,
		unsigned int *CalibrationDiagnostics
) = NULL;

int (__stdcall *Ptr_GBMSAPI_GetMultiScanAPIVersion)(
		unsigned char *VersionField1,
		unsigned char *VersionField2,
		unsigned char *VersionField3,
		unsigned char *VersionField4
) = NULL;

int (__stdcall *Ptr_GBMSAPI_GetUnderlyingLibraryDllVersion)(
		unsigned char *VersionField1,
		unsigned char *VersionField2,
		unsigned char *VersionField3,
		unsigned char *VersionField4,
		char *LowLevelDllName
) = NULL;

int (__stdcall *Ptr_GBMSAPI_GetUSBErrorCode)(
		unsigned long *USBErrorCode
) = NULL;

int (__stdcall *Ptr_GBMSAPI_GetScannerStatistics)(
		unsigned int *Counter,
		unsigned int *ProductionDate
) = NULL;

int (__stdcall *Ptr_GBMSAPI_Sound)(
		unsigned char Tone,
		unsigned char Duration,
		unsigned char Iterations
) = NULL;

int (__stdcall *Ptr_GBMSAPI_GetUserDataSize)(
		int *UserDataSize
) = NULL;

int (__stdcall *Ptr_GBMSAPI_ReadUserData)(
		unsigned int Address,
		unsigned int Size,
		void *Data
) = NULL;

int (__stdcall *Ptr_GBMSAPI_WriteUserData)(
		unsigned int Address,
		unsigned int Size,
		void *Data
) = NULL;

int (__stdcall *Ptr_GBMSAPI_SetFingerIndicatorVUIState)(
		unsigned int IndicatorMask,
		int Color,
		BOOL Blink
) = NULL;

int (__stdcall *Ptr_GBMSAPI_GetUsbLinkSpeed)(
		unsigned char *UsbLinkSpeed
) = NULL;

int (__stdcall *Ptr_GBMSAPI_GetheaterMeanTempRange)(
		float *Max, float *Default, float *Min
) = NULL;

int (__stdcall *Ptr_GBMSAPI_GetHeaterMeanTemp)(
		float *TempC
) = NULL;

int (__stdcall *Ptr_GBMSAPI_SetHeaterMeanTemp)(
		float TempC
) = NULL;

int (__stdcall *Ptr_GBMSAPI_CheckHeater)(
		int *Result
) = NULL;



//////////////////////////////////////////////
// ACQUISITION CALLBACK PARAMETER
//////////////////////////////////////////////
/*********************************************
 * Object that will implement the callback
 * function.
 * The function MUST have this signature
 * public boolean AcquisitionEventsManagerCallback(
            int OccurredEventCode, int GetFrameErrorCode, int EventInfo,
            byte [] FramePtr,
            int FrameSizeX, int FrameSizeY,
            double CurrentFrameRate, double NominalFrameRate,
            int GB_Diagnostic,
            Object UserDefinedParameters
    )
 */
jobject GBMSAPIWrapperCallbackObject = NULL;
jclass GBMSAPIWrapperCallbackClass = NULL;
/********************************************
 * Java Environment to be stored when the
 * library is loaded
 */
JavaVM *GBMSAPIWrapperJvm = NULL;
//////////////////////////////////////////////
// LAST ERROR STRING
//////////////////////////////////////////////
char GBMSAPI_JNI_LastErrorString[1000];

//-------------------------------------------------------------//
//-------------------------------------------------------------//
// MACRO
//-------------------------------------------------------------//
//-------------------------------------------------------------//

//-------------------------------------------------------------//
//-------------------------------------------------------------//
// FUNCTIONS
//-------------------------------------------------------------//
//-------------------------------------------------------------//
extern "C"
{
JNIEXPORT jstring JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_GetLastErrorString(
		JNIEnv *env,
		jobject /* this */)
{
	return env->NewStringUTF(GBMSAPI_JNI_LastErrorString);
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_Unload(
		JNIEnv *env,
		jobject /* this */)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr != NULL)
	{
		if (Ptr_GBMSAPI_UnloadLibrary != NULL) ValToRet = Ptr_GBMSAPI_UnloadLibrary();
		else
		{
			ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
			sprintf(GBMSAPI_JNI_LastErrorString,
			        "GBMSAPIJavaWrapper_Unload: Ptr_GBMSAPI_UnloadLibrary is NULL");
			a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		}
		FreeLibrary(GBMSAPIDllPtr);
	}
	else
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Unload: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
	}

	Ptr_GBMSAPI_LoadLibrary = NULL;
	Ptr_GBMSAPI_UnloadLibrary = NULL;
	Ptr_GBMSAPI_GetAttachedDeviceList = NULL;
	Ptr_GBMSAPI_SetOpenedJavaFD = NULL;
	Ptr_GBMSAPI_SetCurrentDevice = NULL;
	Ptr_GBMSAPI_GetCurrentDevice = NULL;
	Ptr_GBMSAPI_GetScannableTypes = NULL;
	Ptr_GBMSAPI_GetSupportedScanAreas = NULL;
	Ptr_GBMSAPI_GetSupportedScanAreasByDevice = NULL;
	Ptr_GBMSAPI_GetAvailableImageInfo = NULL;
	Ptr_GBMSAPI_GetDeviceFeatures = NULL;
	Ptr_GBMSAPI_GetSupportedScanOptions = NULL;
	Ptr_GBMSAPI_GetOptionalExternalEquipment = NULL;
	Ptr_GBMSAPI_SetRollAreaStandard = NULL;
	Ptr_GBMSAPI_GetImageSize3 = NULL;
	Ptr_GBMSAPI_GetMaxImageSize = NULL;
	Ptr_GBMSAPI_GetCalibrationImageSize2 = NULL;
	Ptr_GBMSAPI_GetFrameRateRange2 = NULL;
	Ptr_GBMSAPI_SetDiagnosticFilterForLCD = NULL;
	Ptr_GBMSAPI_EnableAutoCaptureBlockForDetectedFakes = NULL;
	Ptr_GBMSAPI_SetHardwareFakeFingerDetectionThreshold = NULL;
	Ptr_GBMSAPI_HardwareFakeFingerDetection = NULL;
	Ptr_GBMSAPI_SetSoftwareFakeFingerDetectionThreshold = NULL;
	Ptr_GBMSAPI_SoftwareFakeFingerDetection = NULL;
	Ptr_GBMSAPI_SetSelectImageTimeout = NULL;
	Ptr_GBMSAPI_SetLowerHalfPalmCompletenessThreshold = NULL;
	Ptr_GBMSAPI_SetFrameRate2 = NULL;
	Ptr_GBMSAPI_EnableManualLEDControl = NULL;
	Ptr_GBMSAPI_EnableLEDColorSettingDuringAcquisition = NULL;
	Ptr_GBMSAPI_SetImageRotation = NULL;
	Ptr_GBMSAPI_EnableDrySkinImgEnhance = NULL;
	Ptr_GBMSAPI_ROLL_EnableRollStripeAcquisition = NULL;
	Ptr_GBMSAPI_ROLL_EnableBlockComposition = NULL;
	Ptr_GBMSAPI_StartAcquisition2 = NULL;
	Ptr_GBMSAPI_ROLL_StopPreview = NULL;
	Ptr_GBMSAPI_StopAcquisition = NULL;
	Ptr_GBMSAPI_SetClippingRegionSize = NULL;
	Ptr_GBMSAPI_ROLL_SetPreviewTimeout = NULL;
	Ptr_GBMSAPI_ROLL_SetArtefactCleaningDepth = NULL;
	Ptr_GBMSAPI_VUI_LED_BlinkDuringAcquisition = NULL;
	Ptr_GBMSAPI_GetDryWetFingerAreaPercent = NULL;
	Ptr_GBMSAPI_GetAutoCapturePhase = NULL;
	Ptr_GBMSAPI_SetAutoCaptureBlocking = NULL;
	Ptr_GBMSAPI_GetFingerprintContrast = NULL;
	Ptr_GBMSAPI_GetFingerprintSize = NULL;
	Ptr_GBMSAPI_GetLowerHalfPalmCompleteness = NULL;
	Ptr_GBMSAPI_GetClippingRegionPosition = NULL;
	Ptr_GBMSAPI_ROLL_GetCurrentStripeCoord = NULL;
	Ptr_GBMSAPI_UpdateBackgroundImage = NULL;
	Ptr_GBMSAPI_ImageFinalization = NULL;
	Ptr_GBMSAPI_GetFrameStatistic = NULL;
	Ptr_GBMSAPI_ROLL_GetCompositeImageInfo2 = NULL;
	Ptr_GBMSAPI_SetCalibrationImage2 = NULL;
	Ptr_GBMSAPI_GetCalibrationImage2 = NULL;
	Ptr_GBMSAPI_GetMultiScanAPIVersion = NULL;
	Ptr_GBMSAPI_GetUnderlyingLibraryDllVersion = NULL;
	Ptr_GBMSAPI_GetUSBErrorCode = NULL;
	Ptr_GBMSAPI_GetScannerStatistics = NULL;
	Ptr_GBMSAPI_Sound = NULL;
	Ptr_GBMSAPI_GetUserDataSize = NULL;
	Ptr_GBMSAPI_ReadUserData = NULL;
	Ptr_GBMSAPI_WriteUserData = NULL;
	Ptr_GBMSAPI_SetFingerIndicatorVUIState = NULL;
	Ptr_GBMSAPI_GetUsbLinkSpeed = NULL;
	Ptr_GBMSAPI_GetheaterMeanTempRange = NULL;
	Ptr_GBMSAPI_GetHeaterMeanTemp = NULL;
	Ptr_GBMSAPI_SetHeaterMeanTemp = NULL;
	Ptr_GBMSAPI_CheckHeater = NULL;

	GBMSAPIDllPtr = NULL;
	GBMSAPIWrapperJvm = NULL;

	return ValToRet;
}

int GBMSAPI_JAVA_WRAPPER_AcquisitionEventsManagerCallback(
		unsigned int OccurredEventCode,
		int GetFrameErrorCode,
		unsigned int EventInfo,
		unsigned char *FramePtr,
		int FrameSizeX,
		int FrameSizeY,
		double CurrentFrameRate,
		double NominalFrameRate,
		unsigned int GB_Diagnostic,
		void *UserDefinedParameters
)
{
	int ret = TRUE;

	a_printf ("GBMSAPI_JAVA_WRAPPER_AcquisitionEventsManagerCallback Called\n");

	if (GBMSAPIWrapperJvm == NULL)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPI_JAVA_WRAPPER_AcquisitionEventsManagerCallback: GBMSAPIWrapperJvm is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return FALSE;
	}

	if (GBMSAPIWrapperCallbackObject != NULL)
	{
		JNIEnv *env;
		jmethodID jCallbackFunction;

		jint jRetVal = GBMSAPIWrapperJvm->AttachCurrentThread(&env, NULL);
		if (jRetVal != 0)
		{
			sprintf(GBMSAPI_JNI_LastErrorString,
			        "GBMSAPI_JAVA_WRAPPER_AcquisitionEventsManagerCallback: failed to attach current Thread to env");
			a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
			return FALSE;
		}
		jCallbackFunction = env->GetMethodID(GBMSAPIWrapperCallbackClass,
		                                     "AcquisitionEventsManagerCallback",
		                                     "(III[BIIDDILjava/lang/Object;)Z");
		if (jCallbackFunction == NULL)
		{
			sprintf(GBMSAPI_JNI_LastErrorString,
			        "GBMSAPI_JAVA_WRAPPER_AcquisitionEventsManagerCallback: jCallbackFunction not found");
			a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
			return FALSE;
		}
		/*        public boolean AcquisitionEventsManagerCallback(
                int OccurredEventCode, int GetFrameErrorCode, int EventInfo,
                byte [] FramePtr,
                int FrameSizeX, int FrameSizeY,
                double CurrentFrameRate, double NominalFrameRate,
                int GB_Diagnostic,
                Object UserDefinedParameters
        )*/
		jbyteArray jframe = (env)->NewByteArray(FrameSizeX * FrameSizeY);
		if (jframe == NULL)
		{
			sprintf(GBMSAPI_JNI_LastErrorString,
			        "GBMSAPI_JAVA_WRAPPER_AcquisitionEventsManagerCallback: cannot create wrapperFrame, not enough memory");
			a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
			return FALSE;
		}
		FILE *fp = fopen("/storage/emulated/0/Greenbit/ImageFromSDK.raw", "w");
		if (fp == NULL)
		{
			a_printf ("IMPOSSIBLE TO OPEN raw\n");
		}
		else
		{
			fwrite(FramePtr, FrameSizeX * FrameSizeY, 1, fp);
			fclose(fp);
		}
		if (FramePtr != NULL)
			(env)->SetByteArrayRegion(jframe, 0, FrameSizeX * FrameSizeY, (jbyte *) FramePtr);

		jboolean jret = env->CallBooleanMethod(
				GBMSAPIWrapperCallbackObject, jCallbackFunction,
				(jint) OccurredEventCode, (jint) GetFrameErrorCode, (jint) EventInfo,
				jframe,
				(jint) FrameSizeX, (jint) FrameSizeY,
				(jdouble) CurrentFrameRate, (jdouble) NominalFrameRate,
				(jint) GB_Diagnostic,
				UserDefinedParameters
		);
		ret = (jret == JNI_TRUE) ? TRUE : FALSE;

		GBMSAPIWrapperJvm->DetachCurrentThread();
	}

	return ret;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_StartAcquisition(
		JNIEnv *env,
		jobject thisObj,
		jint jObjToScan, jint jOptionMask, jint jScanArea,
		jobject jObjThatImplementsCallback,
		jobject jAcquisitionParameters,
		jint jDisplayOptionMask,
		jint jContrastLimitLCD, jint jCompletenessLimitLCD)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;
	jmethodID jCallbackFunction;

	a_printf ("Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_StartAcquisition Called\n");

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "StartAcquisition: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_StartAcquisition2 == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "StartAcquisition: Ptr_GBMSAPI_StartAcquisition2 is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters and
	// get C parameters from
	// java ones
	//////////////////////////////
	// if the object that implements callback is not null, then also its class name
	// MUST be NOT null
	if (jObjThatImplementsCallback == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "StartAcquisition: jObjThatImplementsCallback is null");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	// check that class effectively implements the callback method
	// now find the class
	jclass tmp = env->FindClass("com/greenbit/gbmsapi/IGbmsapiAcquisitionManagerCallback");
	GBMSAPIWrapperCallbackClass = (jclass) env->NewGlobalRef(tmp);
	if (GBMSAPIWrapperCallbackClass == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "StartAcquisition: cannot find definition of com/greenbit/gbmsapi/IGbmsapiAcquisitionManagerCallback class");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	jCallbackFunction = env->GetMethodID(GBMSAPIWrapperCallbackClass,
	                                     "AcquisitionEventsManagerCallback",
	                                     "(III[BIIDDILjava/lang/Object;)Z");
	if (jCallbackFunction == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "StartAcquisition: cannot find definition of AcquisitionEventsManagerCallback function into jObjThatImplementsCallbackClass class");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		// reset GBMSAPIWrapperCallbackObjectTypeString
		return ValToRet;
	}
	GBMSAPIWrapperCallbackObject = env->NewGlobalRef(jObjThatImplementsCallback);

	//////////////////////////////
	// call function from C
	//////////////////////////////
	ValToRet = Ptr_GBMSAPI_StartAcquisition2(
			(unsigned int) jObjToScan, (unsigned int) jOptionMask, (unsigned int) jScanArea,
			GBMSAPI_JAVA_WRAPPER_AcquisitionEventsManagerCallback,
			jAcquisitionParameters,
			(unsigned int) jDisplayOptionMask,
			(unsigned char) jContrastLimitLCD, (unsigned char) jCompletenessLimitLCD
	);
	if (ValToRet != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "StartAcquisition: Ptr_GBMSAPI_StartAcquisition2 returned %d",
		        ValToRet);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_SetClippingRegionSize(
		JNIEnv *env,
		jobject thisObj,
		jint jClippingRegionWidth, jint jClippingRegionHeigth)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetClippingRegionSize: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_SetClippingRegionSize == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetClippingRegionSize: Ptr_GBMSAPI_SetClippingRegionSize is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	ValToRet = Ptr_GBMSAPI_SetClippingRegionSize(
			(unsigned int) jClippingRegionWidth,
			(unsigned int) jClippingRegionHeigth
	);
	if (ValToRet != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetClippingRegionSize: Ptr_GBMSAPI_SetClippingRegionSize returned %d",
		        ValToRet);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_RollSetPreviewTimeout(
		JNIEnv *env,
		jobject thisObj,
		jint jTimeout)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "RollSetPreviewTimeout: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_ROLL_SetPreviewTimeout == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "RollSetPreviewTimeout: Ptr_GBMSAPI_ROLL_SetPreviewTimeout is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	ValToRet = Ptr_GBMSAPI_ROLL_SetPreviewTimeout(
			(unsigned int) jTimeout
	);
	if (ValToRet != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "RollSetPreviewTimeout: Ptr_GBMSAPI_ROLL_SetPreviewTimeout returned %d",
		        ValToRet);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_RollSetArtefactCleaningDepth(
		JNIEnv *env,
		jobject thisObj,
		jint jArtefactCleaningDepth)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "RollSetArtefactCleaningDepth: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_ROLL_SetArtefactCleaningDepth == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "RollSetArtefactCleaningDepth: Ptr_GBMSAPI_ROLL_SetArtefactCleaningDepth is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	ValToRet = Ptr_GBMSAPI_ROLL_SetArtefactCleaningDepth(
			(unsigned char) jArtefactCleaningDepth
	);
	if (ValToRet != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "RollSetArtefactCleaningDepth: Ptr_GBMSAPI_ROLL_SetArtefactCleaningDepth returned %d",
		        ValToRet);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_Sound(
		JNIEnv *env,
		jobject thisObj,
		jint jTone,
		jint jDuration,
		jint jIterations)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "Sound: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_Sound == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "Sound: Ptr_GBMSAPI_Sound is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	ValToRet = Ptr_GBMSAPI_Sound(
			(unsigned char) jTone,
			(unsigned char) jDuration,
			(unsigned char) jIterations
	);
	if (ValToRet != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "Sound: Ptr_GBMSAPI_Sound returned %d",
		        ValToRet);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_SetFingerIndicatorVUIState(
		JNIEnv *env,
		jobject thisObj,
		jint jIndicatorMask,
		jint jColor,
		jboolean jBlink)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetFingerIndicatorVUIState: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_SetFingerIndicatorVUIState == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetFingerIndicatorVUIState: Ptr_GBMSAPI_SetFingerIndicatorVUIState is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	ValToRet = Ptr_GBMSAPI_SetFingerIndicatorVUIState(
			(unsigned int) jIndicatorMask,
			(int) jColor,
			jBlink ? TRUE : FALSE
	);
	if (ValToRet != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetFingerIndicatorVUIState: Ptr_GBMSAPI_SetFingerIndicatorVUIState returned %d",
		        ValToRet);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_VuiLedBlinkDuringAcquisition(
		JNIEnv *env,
		jobject thisObj,
		jboolean jBlink)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "VuiLedBlinkDuringAcquisition: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_VUI_LED_BlinkDuringAcquisition == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "VuiLedBlinkDuringAcquisition: Ptr_GBMSAPI_VUI_LED_BlinkDuringAcquisition is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	BOOL blink = jBlink ? TRUE : FALSE;
	ValToRet = Ptr_GBMSAPI_VUI_LED_BlinkDuringAcquisition(
			blink
	);
	if (ValToRet != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "VuiLedBlinkDuringAcquisition: Ptr_GBMSAPI_VUI_LED_BlinkDuringAcquisition returned %d",
		        ValToRet);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_SetAutoCaptureBlocking(
		JNIEnv *env,
		jobject thisObj,
		jboolean jBlocking)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetAutoCaptureBlocking: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_SetAutoCaptureBlocking == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetAutoCaptureBlocking: Ptr_GBMSAPI_SetAutoCaptureBlocking is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	BOOL block = jBlocking ? TRUE : FALSE;
	ValToRet = Ptr_GBMSAPI_SetAutoCaptureBlocking(
			block
	);
	if (ValToRet != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetAutoCaptureBlocking: Ptr_GBMSAPI_SetAutoCaptureBlocking returned %d",
		        ValToRet);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_StopAcquisition(
		JNIEnv *env,
		jobject thisObj)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;
	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "StopAcquisition: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_StopAcquisition == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "StopAcquisition: Ptr_GBMSAPI_StopAcquisition is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	ValToRet = Ptr_GBMSAPI_StopAcquisition();

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_RollStopPreview(
		JNIEnv *env,
		jobject thisObj)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;
	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "RollStopPreview: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_ROLL_StopPreview == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "RollStopPreview: Ptr_GBMSAPI_ROLL_StopPreview is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	ValToRet = Ptr_GBMSAPI_ROLL_StopPreview();

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_SetRollAreaStandard(
		JNIEnv *env,
		jobject thisObj,
		jint jRollAreaStandard
)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;
	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetRollAreaStandard: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_SetRollAreaStandard == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetRollAreaStandard: Ptr_GBMSAPI_SetRollAreaStandard is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	ValToRet = Ptr_GBMSAPI_SetRollAreaStandard((unsigned int) jRollAreaStandard);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_ImageFinalization(
		JNIEnv *env,
		jobject thisObj,
		jbyteArray ImageFrame
)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;
	unsigned char *ptrToFrame;
	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "ImageFinalization: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_ImageFinalization == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "ImageFinalization: Ptr_GBMSAPI_ImageFinalization is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (ImageFrame == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "ImageFinalization: ImageFrame is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetArrayLength(ImageFrame) <= 0)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "ImageFinalization: FrameLen <= 0");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	ptrToFrame = new unsigned char[env->GetArrayLength(ImageFrame)];
	if (ptrToFrame == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_MEMORY_ALLOCATION;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "ImageFinalization: Not enough memory for ptrToFrame");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	env->GetByteArrayRegion(ImageFrame, 0, env->GetArrayLength(ImageFrame),
	                        reinterpret_cast<jbyte *>(ptrToFrame));

	ValToRet = Ptr_GBMSAPI_ImageFinalization(ptrToFrame);
	// copy array again
	env->SetByteArrayRegion(ImageFrame, 0, env->GetArrayLength(ImageFrame),
	                        reinterpret_cast<jbyte *>(ptrToFrame));
	delete[] ptrToFrame;
	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_SetCalibrationImage(
		JNIEnv *env,
		jobject thisObj,
		jint jScanArea,
		jbyteArray ImageFrame
)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;
	unsigned char *ptrToFrame;
	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetCalibrationImage: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_SetCalibrationImage2 == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetCalibrationImage: Ptr_GBMSAPI_SetCalibrationImage2 is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (ImageFrame == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetCalibrationImage: ImageFrame is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetArrayLength(ImageFrame) <= 0)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetCalibrationImage: FrameLen <= 0");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	ptrToFrame = new unsigned char[env->GetArrayLength(ImageFrame)];
	if (ptrToFrame == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_MEMORY_ALLOCATION;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetCalibrationImage: Not enough memory for ptrToFrame");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	env->GetByteArrayRegion(ImageFrame, 0, env->GetArrayLength(ImageFrame),
	                        reinterpret_cast<jbyte *>(ptrToFrame));

	ValToRet = Ptr_GBMSAPI_SetCalibrationImage2(
			(unsigned int) jScanArea,
			ptrToFrame);

	delete[] ptrToFrame;
	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_WriteUserData(
		JNIEnv *env,
		jobject thisObj,
		jint jAddress,
		jint jSize,
		jbyteArray jDataBuffer
)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;
	unsigned char *ptrToFrame;
	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "WriteUserData: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_WriteUserData == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "WriteUserData: Ptr_GBMSAPI_WriteUserData is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jDataBuffer == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "WriteUserData: jDataBuffer is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetArrayLength(jDataBuffer) < jSize)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "WriteUserData: jDataBuffer < Size");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	ptrToFrame = new unsigned char[env->GetArrayLength(jDataBuffer)];
	if (ptrToFrame == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_MEMORY_ALLOCATION;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "WriteUserData: Not enough memory for ptrToFrame");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	env->GetByteArrayRegion(jDataBuffer, 0, env->GetArrayLength(jDataBuffer),
	                        reinterpret_cast<jbyte *>(ptrToFrame));

	ValToRet = Ptr_GBMSAPI_WriteUserData(
			(unsigned int) jAddress,
			(unsigned int) jSize,
			ptrToFrame);

	delete[] ptrToFrame;
	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_SetOpenedJavaFD(
		JNIEnv *env,
		jobject thisObj,
		jintArray openedJavaFd, jintArray bus_number, jintArray devAddress,
		jint numberOfOpenedJavaFD)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetOpenedJavaFD: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_SetOpenedJavaFD == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetOpenedJavaFD: Ptr_GBMSAPI_SetOpenedJavaFD is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (numberOfOpenedJavaFD < 0)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetOpenedJavaFD: numberOfOpenedJavaFD less than 0");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	else if (numberOfOpenedJavaFD == 0)
	{
		ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetOpenedJavaFD: numberOfOpenedJavaFD = 0, no operations to perform");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (openedJavaFd == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetOpenedJavaFD: openedJavaFd is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	else if ((env)->GetArrayLength(openedJavaFd) < numberOfOpenedJavaFD)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetOpenedJavaFD: openedJavaFd length too small");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (bus_number == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetOpenedJavaFD: bus_number is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	else if ((env)->GetArrayLength(bus_number) < numberOfOpenedJavaFD)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetOpenedJavaFD: bus_number length too small");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (devAddress == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetOpenedJavaFD: devAddress is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	else if ((env)->GetArrayLength(devAddress) < numberOfOpenedJavaFD)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetOpenedJavaFD: devAddress length too small");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	// openedJavaFD
	jint *jintOpenedJavaFD = (env)->GetIntArrayElements(openedJavaFd, NULL);
	if (jintOpenedJavaFD == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_MEMORY_ALLOCATION;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetOpenedJavaFD: cannot get elements from openedJavaFd");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	int *cppOpenedJavaFD = new int[numberOfOpenedJavaFD];
	if (cppOpenedJavaFD == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_MEMORY_ALLOCATION;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetOpenedJavaFD: cannot alloc space for cppOpenedJavaFD");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		(env)->ReleaseIntArrayElements(openedJavaFd, jintOpenedJavaFD, 0);
		return ValToRet;
	}
	for (int i = 0; i < numberOfOpenedJavaFD; i++)
	{
		cppOpenedJavaFD[i] = jintOpenedJavaFD[i];
	}
	(env)->ReleaseIntArrayElements(openedJavaFd, jintOpenedJavaFD, 0);
	// bus_number
	jint *jintBus_number = (env)->GetIntArrayElements(bus_number, NULL);
	if (jintBus_number == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_MEMORY_ALLOCATION;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetOpenedJavaFD: cannot get elements from bus_number");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	int *cppBus_number = new int[numberOfOpenedJavaFD];
	if (cppBus_number == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_MEMORY_ALLOCATION;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetOpenedJavaFD: cannot alloc space for cppBus_number");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		(env)->ReleaseIntArrayElements(bus_number, jintBus_number, 0);
		delete[] cppOpenedJavaFD;
		return ValToRet;
	}
	for (int i = 0; i < numberOfOpenedJavaFD; i++)
	{
		cppBus_number[i] = jintBus_number[i];
	}
	(env)->ReleaseIntArrayElements(bus_number, jintBus_number, 0);
	// bus_number
	jint *jintDevAddress = (env)->GetIntArrayElements(devAddress, NULL);
	if (jintDevAddress == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_MEMORY_ALLOCATION;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetOpenedJavaFD: cannot get elements from devAddress");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	int *cppDevAddress = new int[numberOfOpenedJavaFD];
	if (cppDevAddress == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_MEMORY_ALLOCATION;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetOpenedJavaFD: cannot alloc space for cppDevAddress");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		(env)->ReleaseIntArrayElements(devAddress, jintDevAddress, 0);
		delete[] cppOpenedJavaFD;
		delete[] cppBus_number;
		return ValToRet;
	}
	for (int i = 0; i < numberOfOpenedJavaFD; i++)
	{
		cppDevAddress[i] = jintDevAddress[i];
	}
	(env)->ReleaseIntArrayElements(devAddress, jintDevAddress, 0);

	//////////////////////////////
	// call function from C
	//////////////////////////////
	Ptr_GBMSAPI_SetOpenedJavaFD(cppOpenedJavaFD, cppBus_number, cppDevAddress,
	                            numberOfOpenedJavaFD);
	a_printf ("SetOpenedJavaFD: Ptr_GBMSAPI_SetOpenedJavaFD called succesfully\n");
	//////////////////////////////
	// release cpp arrays
	//////////////////////////////
	delete[] cppOpenedJavaFD;
	delete[] cppBus_number;
	delete[] cppDevAddress;
	a_printf ("SetOpenedJavaFD: resources released \n");

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_GetMaxImageSize(
		JNIEnv *env,
		jobject thisObj,
		jobject jMaxImageSize)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;
	jclass jMaxImageSizeClass;
	jmethodID jPtrToSetSizeFunction;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetMaxImageSize: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_GetMaxImageSize == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetMaxImageSize: Ptr_GBMSAPI_GetMaxImageSize is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (jMaxImageSize == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetMaxImageSize: jMaxImageSize is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	jMaxImageSizeClass = env->FindClass(
			"com/greenbit/gbmsapi/GBMSAPIJavaWrapperDefinesImageSize");
	if (jMaxImageSizeClass == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetMaxImageSize: cannot find definition of GBMSAPIJavaWrapperDefinesImageSize class");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	jPtrToSetSizeFunction = env->GetMethodID(jMaxImageSizeClass, "SetSize", "(II)V");
	if (jPtrToSetSizeFunction == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetMaxImageSize: cannot find definition of SetSize function into GBMSAPIJavaWrapperDefinesImageSize class");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	unsigned int Sx, Sy;
	ValToRet = Ptr_GBMSAPI_GetMaxImageSize(&Sx, &Sy);
	if (ValToRet != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetMaxImageSize: Ptr_GBMSAPI_GetMaxImageSize returned %d",
		        ValToRet);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// copy values to array
	//////////////////////////////
	env->CallVoidMethod(jMaxImageSize, jPtrToSetSizeFunction, (int) Sx, (int) Sy);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_GetCalibrationImageSize(
		JNIEnv *env,
		jobject thisObj,
		jint jScanArea,
		jobject jCalibrationImageSize)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;
	jclass jMaxImageSizeClass;
	jmethodID jPtrToSetSizeFunction;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetCalibrationImageSize: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_GetCalibrationImageSize2 == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetCalibrationImageSize: Ptr_GBMSAPI_GetCalibrationImageSize2 is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (jCalibrationImageSize == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetCalibrationImageSize: jCalibrationImageSize is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	jMaxImageSizeClass = env->FindClass(
			"com/greenbit/gbmsapi/GBMSAPIJavaWrapperDefinesImageSize");
	if (jMaxImageSizeClass == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetCalibrationImageSize: cannot find definition of GBMSAPIJavaWrapperDefinesImageSize class");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	jPtrToSetSizeFunction = env->GetMethodID(jMaxImageSizeClass, "SetSize", "(II)V");
	if (jPtrToSetSizeFunction == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetCalibrationImageSize: cannot find definition of SetSize function into GBMSAPIJavaWrapperDefinesImageSize class");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	unsigned int Sx, Sy;
	ValToRet = Ptr_GBMSAPI_GetCalibrationImageSize2(
			(unsigned int) jScanArea,
			&Sx, &Sy);
	if (ValToRet != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetCalibrationImageSize: Ptr_GBMSAPI_GetCalibrationImageSize2 returned %d",
		        ValToRet);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// copy values to array
	//////////////////////////////
	env->CallVoidMethod(jCalibrationImageSize, jPtrToSetSizeFunction, (int) Sx, (int) Sy);

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_SetDiagnosticFilterForLCD(
		JNIEnv *env,
		jobject thisObj,
		jint jDiagnosticToBeFiltered)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetDiagnosticFilterForLCD: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_SetDiagnosticFilterForLCD == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetDiagnosticFilterForLCD: Ptr_GBMSAPI_SetDiagnosticFilterForLCD is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	Ptr_GBMSAPI_SetDiagnosticFilterForLCD((unsigned int) jDiagnosticToBeFiltered);

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_EnableAutoCaptureBlockForDetectedFakes(
		JNIEnv *env,
		jobject thisObj,
		jboolean jFlag)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "EnableAutoCaptureBlockForDetectedFakes: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_EnableAutoCaptureBlockForDetectedFakes == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "EnableAutoCaptureBlockForDetectedFakes: Ptr_GBMSAPI_EnableAutoCaptureBlockForDetectedFakes is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	Ptr_GBMSAPI_EnableAutoCaptureBlockForDetectedFakes((unsigned char) jFlag);

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_SetHardwareFakeFingerDetectionThreshold(
		JNIEnv *env,
		jobject thisObj,
		jint jFFDThreshold)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetHardwareFakeFingerDetectionThreshold: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_SetHardwareFakeFingerDetectionThreshold == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetHardwareFakeFingerDetectionThreshold: Ptr_GBMSAPI_SetHardwareFakeFingerDetectionThreshold is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	Ptr_GBMSAPI_SetHardwareFakeFingerDetectionThreshold((unsigned char) jFFDThreshold);

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_SetSoftwareFakeFingerDetectionThreshold(
		JNIEnv *env,
		jobject thisObj,
		jint jFFDThreshold)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetSoftwareFakeFingerDetectionThreshold: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_SetSoftwareFakeFingerDetectionThreshold == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetSoftwareFakeFingerDetectionThreshold: Ptr_GBMSAPI_SetSoftwareFakeFingerDetectionThreshold is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	Ptr_GBMSAPI_SetSoftwareFakeFingerDetectionThreshold((unsigned char) jFFDThreshold);

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_HardwareFakeFingerDetection(
		JNIEnv *env,
		jobject thisObj,
		jobject jFfdFlag,
		jobject jFfdDiagnostic)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;
	jclass jBoolPtrClass, jIntPtrClass;
	jmethodID jPtrToSetBoolFunction, jPtrToSetIntFunction;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "HardwareFakeFingerDetection: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_HardwareFakeFingerDetection == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "HardwareFakeFingerDetection: Ptr_GBMSAPI_HardwareFakeFingerDetection is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (jFfdFlag == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "HardwareFakeFingerDetection: jFfdFlag is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFfdDiagnostic == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "HardwareFakeFingerDetection: jFfdDiagnostic is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	jBoolPtrClass = env->FindClass(
			"com/greenbit/utils/GBJavaWrapperUtilBOOLForJavaToCExchange");
	if (jBoolPtrClass == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "HardwareFakeFingerDetection: cannot find definition of GBJavaWrapperUtilBOOLForJavaToCExchange class");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	jPtrToSetBoolFunction = env->GetMethodID(jBoolPtrClass, "SetFromInt", "(I)V");
	if (jPtrToSetBoolFunction == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "HardwareFakeFingerDetection: cannot find definition of SetFromInt function into GBJavaWrapperUtilBOOLForJavaToCExchange class");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	jIntPtrClass = env->FindClass(
			"com/greenbit/utils/GBJavaWrapperUtilIntForJavaToCExchange");
	if (jIntPtrClass == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "HardwareFakeFingerDetection: cannot find definition of GBJavaWrapperUtilIntForJavaToCExchange class");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	jPtrToSetIntFunction = env->GetMethodID(jIntPtrClass, "Set", "(I)V");
	if (jPtrToSetIntFunction == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "HardwareFakeFingerDetection: cannot find definition of Set function into GBJavaWrapperUtilIntForJavaToCExchange class");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	unsigned int FfdDiagnostic;
	BOOL FfdFlag;
	ValToRet = Ptr_GBMSAPI_HardwareFakeFingerDetection(&FfdFlag, &FfdDiagnostic);
	if (ValToRet != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "HardwareFakeFingerDetection: Ptr_GBMSAPI_HardwareFakeFingerDetection returned %d",
		        ValToRet);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// copy values to array
	//////////////////////////////
	env->CallVoidMethod(jBoolPtrClass, jPtrToSetBoolFunction, (int) FfdFlag);
	env->CallVoidMethod(jIntPtrClass, jPtrToSetIntFunction, (int) FfdDiagnostic);

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_SoftwareFakeFingerDetection(
		JNIEnv *env,
		jobject thisObj,
		jbyteArray jImageFrame,
		jint jSizeX, jint jSizeY,
		jobject jFfdFlag)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;
	jclass jBoolPtrClass;
	jmethodID jPtrToSetBoolFunction;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SoftwareFakeFingerDetection: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_SoftwareFakeFingerDetection == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SoftwareFakeFingerDetection: Ptr_GBMSAPI_SoftwareFakeFingerDetection is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (jFfdFlag == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SoftwareFakeFingerDetection: jFfdFlag is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	if (jImageFrame == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SoftwareFakeFingerDetection: jImageFrame is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetArrayLength(jImageFrame) <= 0)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SoftwareFakeFingerDetection: FrameLen <= 0");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	jBoolPtrClass = env->FindClass(
			"com/greenbit/utils/GBJavaWrapperUtilBOOLForJavaToCExchange");
	if (jBoolPtrClass == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SoftwareFakeFingerDetection: cannot find definition of GBJavaWrapperUtilBOOLForJavaToCExchange class");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	jPtrToSetBoolFunction = env->GetMethodID(jBoolPtrClass, "SetFromInt", "(I)V");
	if (jPtrToSetBoolFunction == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SoftwareFakeFingerDetection: cannot find definition of SetFromInt function into GBJavaWrapperUtilBOOLForJavaToCExchange class");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}


	unsigned char *ptrToFrame;
	ptrToFrame = new unsigned char[env->GetArrayLength(jImageFrame)];
	if (ptrToFrame == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_MEMORY_ALLOCATION;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "ImageFinalization: Not enough memory for ptrToFrame");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	env->GetByteArrayRegion(jImageFrame, 0, env->GetArrayLength(jImageFrame),
	                        reinterpret_cast<jbyte *>(ptrToFrame));

	//////////////////////////////
	// call function from C
	//////////////////////////////
	BOOL FfdFlag;
	ValToRet = Ptr_GBMSAPI_SoftwareFakeFingerDetection(
			ptrToFrame, (int) jSizeX, (int) jSizeY,
			&FfdFlag);
	if (ValToRet != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SoftwareFakeFingerDetection: Ptr_GBMSAPI_SoftwareFakeFingerDetection returned %d",
		        ValToRet);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// copy values to array
	//////////////////////////////
	env->CallVoidMethod(jBoolPtrClass, jPtrToSetBoolFunction, (int) FfdFlag);

	//////////////////////////////
	// clean memory
	//////////////////////////////
	delete[] ptrToFrame;
	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_SetSelectImageTimeout(
		JNIEnv *env,
		jobject thisObj,
		jint jSelectImageTimeout)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetSelectImageTimeout: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_SetSelectImageTimeout == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetSelectImageTimeout: Ptr_GBMSAPI_SetSelectImageTimeout is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	Ptr_GBMSAPI_SetSelectImageTimeout((unsigned int) jSelectImageTimeout);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_EnableManualLEDControl(
		JNIEnv *env,
		jobject thisObj,
		jboolean jEnable)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "EnableManualLEDControl: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_EnableManualLEDControl == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "EnableManualLEDControl: Ptr_GBMSAPI_EnableManualLEDControl is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	Ptr_GBMSAPI_EnableManualLEDControl(jEnable ? TRUE : FALSE);

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_RollEnableBlockComposition(
		JNIEnv *env,
		jobject thisObj,
		jboolean jEnable)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "RollEnableBlockComposition: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_ROLL_EnableBlockComposition == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "RollEnableBlockComposition: Ptr_GBMSAPI_ROLL_EnableBlockComposition is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	Ptr_GBMSAPI_ROLL_EnableBlockComposition(jEnable ? 1 : 0);

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_RollEnableRollStripeAcquisition(
		JNIEnv *env,
		jobject thisObj,
		jint jScanArea,
		jboolean jEnable)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "RollEnableRollStripeAcquisition: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_ROLL_EnableRollStripeAcquisition == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "RollEnableRollStripeAcquisition: Ptr_GBMSAPI_ROLL_EnableRollStripeAcquisition is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	Ptr_GBMSAPI_ROLL_EnableRollStripeAcquisition((unsigned int) jScanArea, jEnable ? TRUE : FALSE);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_EnableDrySkinImgEnhance(
		JNIEnv *env,
		jobject thisObj,
		jboolean jEnable)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "EnableDrySkinImgEnhance: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_EnableDrySkinImgEnhance == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "EnableDrySkinImgEnhance: Ptr_GBMSAPI_EnableDrySkinImgEnhance is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	Ptr_GBMSAPI_EnableDrySkinImgEnhance(jEnable ? 1 : 0);

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_EnableLEDColorSettingDuringAcquisition(
		JNIEnv *env,
		jobject thisObj,
		jboolean jEnable)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "EnableLEDColorSettingDuringAcquisition: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_EnableLEDColorSettingDuringAcquisition == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "EnableLEDColorSettingDuringAcquisition: Ptr_GBMSAPI_EnableLEDColorSettingDuringAcquisition is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	Ptr_GBMSAPI_EnableLEDColorSettingDuringAcquisition(jEnable ? TRUE : FALSE);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_SetImageRotation(
		JNIEnv *env,
		jobject thisObj,
		jint jImageRotation)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetImageRotation: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_SetImageRotation == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetImageRotation: Ptr_GBMSAPI_SetImageRotation is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	Ptr_GBMSAPI_SetImageRotation((int) jImageRotation);

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_SetLowerHalfPalmCompletenessThreshold(
		JNIEnv *env,
		jobject thisObj,
		jint jCompletenessThreshold)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetLowerHalfPalmCompletenessThreshold: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_SetLowerHalfPalmCompletenessThreshold == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetLowerHalfPalmCompletenessThreshold: Ptr_GBMSAPI_SetLowerHalfPalmCompletenessThreshold is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	Ptr_GBMSAPI_SetLowerHalfPalmCompletenessThreshold((unsigned char) jCompletenessThreshold);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_SetFrameRate(
		JNIEnv *env,
		jobject thisObj,
		jint jScanArea,
		jint FrameRateOptions,
		jdouble FrameRate
)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetFrameRate: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_SetFrameRate2 == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetFrameRate: Ptr_GBMSAPI_SetFrameRate2 is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	Ptr_GBMSAPI_SetFrameRate2(
			(unsigned int) jScanArea,
			(unsigned int) FrameRateOptions,
			(double) FrameRate);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_GetImageSize(
		JNIEnv *env,
		jobject thisObj,
		jint jObjectTypeToScan,
		jint jOptionMask,
		jint jScanArea,
		jobject jFullResImageSize,
		jobject jPreviewImageSize
)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;
	jclass jMaxImageSizeClass;
	jmethodID jPtrToSetSizeFunction;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetImageSize: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_GetImageSize3 == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetImageSize: Ptr_GBMSAPI_GetImageSize3 is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (jFullResImageSize == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetImageSize: jFullResImageSize is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	if (jPreviewImageSize == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetImageSize: jPreviewImageSize is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	jMaxImageSizeClass = env->FindClass(
			"com/greenbit/gbmsapi/GBMSAPIJavaWrapperDefinesImageSize");
	if (jMaxImageSizeClass == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetImageSize: cannot find definition of GBMSAPIJavaWrapperDefinesImageSize class");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	jPtrToSetSizeFunction = env->GetMethodID(jMaxImageSizeClass, "SetSize", "(II)V");
	if (jPtrToSetSizeFunction == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetImageSize: cannot find definition of SetSize function into GBMSAPIJavaWrapperDefinesImageSize class");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	unsigned int Sx, Sy, Psx, Psy;
	ValToRet = Ptr_GBMSAPI_GetImageSize3(
			(unsigned int) jObjectTypeToScan,
			(unsigned int) jOptionMask,
			(unsigned int) jScanArea,
			&Sx, &Sy, &Psx, &Psy);
	if (ValToRet != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetImageSize: Ptr_GBMSAPI_GetImageSize3 returned %d",
		        ValToRet);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// copy values to array
	//////////////////////////////
	env->CallVoidMethod(jFullResImageSize, jPtrToSetSizeFunction, (int) Sx, (int) Sy);
	env->CallVoidMethod(jPreviewImageSize, jPtrToSetSizeFunction, (int) Psx, (int) Psy);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_GetCurrentDevice(
		JNIEnv *env,
		jobject thisObj,
		jobject jCurrentDevice)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;
	jclass DeviceInfoJava;
	jmethodID ptrToSetFieldFunction;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetCurrentDevice: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_GetCurrentDevice == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetCurrentDevice: Ptr_GBMSAPI_GetCurrentDevice is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (jCurrentDevice == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetCurrentDevice: jCurrentDevice is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	DeviceInfoJava = env->FindClass(
			"com/greenbit/gbmsapi/GBMSAPIJavaWrapperDefinesDeviceInfoStruct");
	if (DeviceInfoJava == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetCurrentDevice: cannot find definition of GBMSAPIJavaWrapperDefinesDeviceInfoStruct class");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	ptrToSetFieldFunction = env->GetMethodID(DeviceInfoJava, "SetFields", "(ILjava/lang/String;)V");
	if (ptrToSetFieldFunction == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetCurrentDevice: cannot find definition of SetFields function into GBMSAPIJavaWrapperDefinesDeviceInfoStruct class");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	unsigned char DeviceID;
	char DeviceSerialNumber[GBMSAPI_MAX_SN_LENGHT + 1];

	ValToRet = Ptr_GBMSAPI_GetCurrentDevice(&DeviceID, DeviceSerialNumber);
	if (ValToRet != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetCurrentDevice: Ptr_GBMSAPI_GetCurrentDevice returned %d", ValToRet);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	DeviceSerialNumber[GBMSAPI_MAX_SN_LENGHT] = 0;

	env->CallVoidMethod(jCurrentDevice, ptrToSetFieldFunction,
	                    (int) (DeviceID),
	                    env->NewStringUTF(DeviceSerialNumber));

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_GetScannableTypes(
		JNIEnv *env,
		jobject thisObj,
		jobject jScannableTypes)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetScannableTypes: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_GetScannableTypes == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetScannableTypes: Ptr_GBMSAPI_GetScannableTypes is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (jScannableTypes == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetScannableTypes: jScannableTypes is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	unsigned int ScannableTypes;

	ValToRet = Ptr_GBMSAPI_GetScannableTypes(&ScannableTypes);
	if (ValToRet != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetScannableTypes: Ptr_GBMSAPI_GetScannableTypes returned %d", ValToRet);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	GBANUTIL_GetJIntExchangeFromCInt((int) (ScannableTypes), jScannableTypes, env);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_GetDeviceFeatures(
		JNIEnv *env,
		jobject thisObj,
		jobject jDeviceFeatures)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetDeviceFeatures: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_GetDeviceFeatures == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetDeviceFeatures: Ptr_GBMSAPI_GetDeviceFeatures is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (jDeviceFeatures == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetDeviceFeatures: jDeviceFeatures is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	unsigned int DeviceFeatures;

	ValToRet = Ptr_GBMSAPI_GetDeviceFeatures(&DeviceFeatures);
	if (ValToRet != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetDeviceFeatures: Ptr_GBMSAPI_GetDeviceFeatures returned %d", ValToRet);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	GBANUTIL_GetJIntExchangeFromCInt((int) (DeviceFeatures), jDeviceFeatures, env);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_GetSupportedScanOptions(
		JNIEnv *env,
		jobject thisObj,
		jobject jScanOptions)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetSupportedScanOptions: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_GetSupportedScanOptions == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetSupportedScanOptions: Ptr_GBMSAPI_GetSupportedScanOptions is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (jScanOptions == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetSupportedScanOptions: jScanOptions is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	unsigned int scanOptions;

	ValToRet = Ptr_GBMSAPI_GetSupportedScanOptions(&scanOptions);
	if (ValToRet != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetSupportedScanOptions: Ptr_GBMSAPI_GetSupportedScanOptions returned %d",
		        ValToRet);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	GBANUTIL_GetJIntExchangeFromCInt((int) (scanOptions), jScanOptions, env);

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_GetOptionalExternalEquipment(
		JNIEnv *env,
		jobject thisObj,
		jobject jOptionalExternalEquipment)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetOptionalExternalEquipment: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_GetOptionalExternalEquipment == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetOptionalExternalEquipment: Ptr_GBMSAPI_GetOptionalExternalEquipment is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (jOptionalExternalEquipment == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetOptionalExternalEquipment: jOptionalExternalEquipment is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	unsigned int optExtEquip;

	ValToRet = Ptr_GBMSAPI_GetOptionalExternalEquipment(&optExtEquip);
	if (ValToRet != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetOptionalExternalEquipment: Ptr_GBMSAPI_GetOptionalExternalEquipment returned %d",
		        ValToRet);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	GBANUTIL_GetJIntExchangeFromCInt((int) (optExtEquip), jOptionalExternalEquipment, env);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_GetFrameRateRange(
		JNIEnv *env,
		jobject thisObj,
		jint DeviceID,
		jint FrameRateOptions,
		jint ScanArea,
		jobject jMaxFrameRate,
		jobject jMinFrameRate,
		jobject jDefaultFrameRate
)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetFrameRateRange: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_GetFrameRateRange2 == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetFrameRateRange: Ptr_GBMSAPI_GetFrameRateRange2 is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (jMaxFrameRate == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetFrameRateRange: jMaxFrameRate is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	if (jMinFrameRate == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetFrameRateRange: jMinFrameRate is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	if (jDefaultFrameRate == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetFrameRateRange: jDefaultFrameRate is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	double Max, Min, Def;

	ValToRet = Ptr_GBMSAPI_GetFrameRateRange2(
			(unsigned char) DeviceID, (unsigned int) FrameRateOptions, (unsigned int) ScanArea,
			&Max, &Min, &Def
	);
	if (ValToRet != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetOptionalExternalEquipment: Ptr_GBMSAPI_GetFrameRateRange2 returned %d",
		        ValToRet);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	GBANUTIL_GetJDoubleExchangeFromCDouble((double)Max, jMaxFrameRate, env);
	GBANUTIL_GetJDoubleExchangeFromCDouble((double)Min, jMinFrameRate, env);
	GBANUTIL_GetJDoubleExchangeFromCDouble((double)Def, jDefaultFrameRate, env);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_GetSupportedScanAreas(
		JNIEnv *env,
		jobject thisObj,
		jobject jScanAreas)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetSupportedScanAreas: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_GetSupportedScanAreas == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetSupportedScanAreas: Ptr_GBMSAPI_GetSupportedScanAreas is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (jScanAreas == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetSupportedScanAreas: jScanAreas is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	unsigned int ScanAreas;

	ValToRet = Ptr_GBMSAPI_GetSupportedScanAreas(&ScanAreas);
	if (ValToRet != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetSupportedScanAreas: Ptr_GBMSAPI_GetSupportedScanAreas returned %d", ValToRet);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	GBANUTIL_GetJIntExchangeFromCInt((int) (ScanAreas),jScanAreas,env);

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_GetSupportedScanAreasByDevice(
		JNIEnv *env,
		jobject thisObj,
		jint jDeviceId,
		jobject jScanAreas)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetSupportedScanAreasByDevice: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_GetSupportedScanAreasByDevice == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetSupportedScanAreasByDevice: Ptr_GBMSAPI_GetSupportedScanAreasByDevice is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (jScanAreas == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetSupportedScanAreasByDevice: jScanAreas is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	unsigned int ScanAreas;
	unsigned char DeviceID = (unsigned char) jDeviceId;

	ValToRet = Ptr_GBMSAPI_GetSupportedScanAreasByDevice(DeviceID, &ScanAreas);
	if (ValToRet != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetSupportedScanAreasByDevice: Ptr_GBMSAPI_GetSupportedScanAreasByDevice returned %d",
		        ValToRet);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	GBANUTIL_GetJIntExchangeFromCInt((int) (ScanAreas),jScanAreas,env);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_GetAvailableImageInfo(
		JNIEnv *env,
		jobject thisObj,
		jint jObjTypeToScan,
		jobject jAvailableImageInfo)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetAvailableImageInfo: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_GetAvailableImageInfo == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetAvailableImageInfo: Ptr_GBMSAPI_GetAvailableImageInfo is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (jAvailableImageInfo == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetAvailableImageInfo: jAvailableImageInfo is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	unsigned int AvailableImageInfo;
	unsigned int ObjectTypeToScan = (unsigned int) jObjTypeToScan;

	ValToRet = Ptr_GBMSAPI_GetAvailableImageInfo(ObjectTypeToScan, &AvailableImageInfo);
	if (ValToRet != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetAvailableImageInfo: Ptr_GBMSAPI_GetAvailableImageInfo returned %d", ValToRet);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	GBANUTIL_GetJIntExchangeFromCInt((int) (AvailableImageInfo),jAvailableImageInfo,env);

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_GetDryWetFingerAreaPercent(
		JNIEnv *env,
		jobject thisObj,
		jobject jDry, jobject jWet)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetDryWetFingerAreaPercent: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_GetDryWetFingerAreaPercent == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetDryWetFingerAreaPercent: Ptr_GBMSAPI_GetDryWetFingerAreaPercent is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (jDry == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetDryWetFingerAreaPercent: jDry is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	if (jWet == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetDryWetFingerAreaPercent: jWet is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	unsigned char Dry, Wet;

	ValToRet = Ptr_GBMSAPI_GetDryWetFingerAreaPercent(&Dry, &Wet);
	if (ValToRet != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetDryWetFingerAreaPercent: Ptr_GBMSAPI_GetDryWetFingerAreaPercent returned %d",
		        ValToRet);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	GBANUTIL_GetJIntExchangeFromCInt((int) (Dry),jDry,env);
	GBANUTIL_GetJIntExchangeFromCInt((int) (Wet),jWet,env);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_GetFingerprintContrast(
		JNIEnv *env,
		jobject thisObj,
		jobject jContrast)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetFingerprintContrast: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_GetFingerprintContrast == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetFingerprintContrast: Ptr_GBMSAPI_GetFingerprintContrast is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (jContrast == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetFingerprintContrast: jContrast is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	unsigned char contrast;

	ValToRet = Ptr_GBMSAPI_GetFingerprintContrast(&contrast);
	if (ValToRet != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetFingerprintContrast: Ptr_GBMSAPI_GetFingerprintContrast returned %d", ValToRet);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	GBANUTIL_GetJIntExchangeFromCInt((int) (contrast),jContrast,env);

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_GetLowerHalfPalmCompleteness(
		JNIEnv *env,
		jobject thisObj,
		jobject jLHPComp)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetLowerHalfPalmCompleteness: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_GetLowerHalfPalmCompleteness == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetLowerHalfPalmCompleteness: Ptr_GBMSAPI_GetLowerHalfPalmCompleteness is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (jLHPComp == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetLowerHalfPalmCompleteness: jLHPComp is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	unsigned char contrast;

	ValToRet = Ptr_GBMSAPI_GetLowerHalfPalmCompleteness(&contrast);
	if (ValToRet != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetLowerHalfPalmCompleteness: Ptr_GBMSAPI_GetLowerHalfPalmCompleteness returned %d",
		        ValToRet);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	GBANUTIL_GetJIntExchangeFromCInt((int) (contrast),jLHPComp,env);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_GetAutoCapturePhase(
		JNIEnv *env,
		jobject thisObj,
		jobject jAcqPhase)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetAutoCapturePhase: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_GetAutoCapturePhase == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetAutoCapturePhase: Ptr_GBMSAPI_GetAutoCapturePhase is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (jAcqPhase == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetAutoCapturePhase: jAcqPhase is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	unsigned int AcqPhase;

	ValToRet = Ptr_GBMSAPI_GetAutoCapturePhase(&AcqPhase);
	if (ValToRet != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetAutoCapturePhase: Ptr_GBMSAPI_GetAutoCapturePhase returned %d", ValToRet);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	GBANUTIL_GetJIntExchangeFromCInt((int) (AcqPhase),jAcqPhase,env);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_GetFingerprintSize(
		JNIEnv *env,
		jobject thisObj,
		jobject jFingerSize)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetFingerprintSize: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_GetFingerprintSize == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetFingerprintSize: Ptr_GBMSAPI_GetFingerprintSize is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (jFingerSize == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetFingerprintSize: jFingerSize is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	unsigned int FingerSize;

	ValToRet = Ptr_GBMSAPI_GetFingerprintSize(&FingerSize);
	if (ValToRet != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetFingerprintSize: Ptr_GBMSAPI_GetFingerprintSize returned %d", ValToRet);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	GBANUTIL_GetJIntExchangeFromCInt((int) (FingerSize),jFingerSize,env);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_GetScannerStatistics(
		JNIEnv *env,
		jobject thisObj,
		jobject jCounter,
		jobject jProductionDate)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetScannerStatistics: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_GetScannerStatistics == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetScannerStatistics: Ptr_GBMSAPI_GetScannerStatistics is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (jCounter == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetScannerStatistics: jCounter is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	if (jProductionDate == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetScannerStatistics: jProductionDate is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	unsigned int Counter, ProductionDate;

	ValToRet = Ptr_GBMSAPI_GetScannerStatistics(&Counter, &ProductionDate);
	if (ValToRet != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetScannerStatistics: Ptr_GBMSAPI_GetScannerStatistics returned %d", ValToRet);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	GBANUTIL_GetJIntExchangeFromCInt((int) (Counter),jCounter,env);
	GBANUTIL_GetJIntExchangeFromCInt((int) (ProductionDate),jProductionDate,env);

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_GetClippingRegionPosition(
		JNIEnv *env,
		jobject thisObj,
		jobject jClippingRegionPosX,
		jobject jClippingRegionPosYY,
		jobject jClippingRegionSizeX,
		jobject jClippingRegionSizeY)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetClippingRegionPosition: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_GetClippingRegionPosition == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetClippingRegionPosition: Ptr_GBMSAPI_GetClippingRegionPosition is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (jClippingRegionPosX == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetClippingRegionPosition: jClippingRegionPosX is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	if (jClippingRegionPosYY == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetClippingRegionPosition: jClippingRegionPosYY is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	if (jClippingRegionSizeX == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetClippingRegionPosition: jClippingRegionSizeX is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	if (jClippingRegionSizeY == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetClippingRegionPosition: jClippingRegionSizeY is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	int ClippingRegionPosX, ClippingRegionPosYY;
	unsigned int ClippingRegionSizeX, ClippingRegionSizeY;

	ValToRet = Ptr_GBMSAPI_GetClippingRegionPosition(
			&ClippingRegionPosX, &ClippingRegionPosYY, &ClippingRegionSizeX, &ClippingRegionSizeY);
	if (ValToRet != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetClippingRegionPosition: Ptr_GBMSAPI_GetClippingRegionPosition returned %d",
		        ValToRet);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	GBANUTIL_GetJIntExchangeFromCInt((int) (ClippingRegionPosX),jClippingRegionPosX,env);
	GBANUTIL_GetJIntExchangeFromCInt((int) (ClippingRegionPosYY),jClippingRegionPosYY,env);
	GBANUTIL_GetJIntExchangeFromCInt((int) (ClippingRegionSizeX),jClippingRegionSizeX,env);
	GBANUTIL_GetJIntExchangeFromCInt((int) (ClippingRegionSizeY),jClippingRegionSizeY,env);

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_RollGetCurrentStripeCoord(
		JNIEnv *env,
		jobject thisObj,
		jobject jLeftX,
		jobject jRightX)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "RollGetCurrentStripeCoord: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_ROLL_GetCurrentStripeCoord == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "RollGetCurrentStripeCoord: Ptr_GBMSAPI_ROLL_GetCurrentStripeCoord is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (jLeftX == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "RollGetCurrentStripeCoord: jLeftX is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	if (jRightX == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "RollGetCurrentStripeCoord: jRightX is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	int LeftX, RightX;

	ValToRet = Ptr_GBMSAPI_ROLL_GetCurrentStripeCoord(
			&LeftX, &RightX);
	if (ValToRet != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "RollGetCurrentStripeCoord: Ptr_GBMSAPI_ROLL_GetCurrentStripeCoord returned %d",
		        ValToRet);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	GBANUTIL_GetJIntExchangeFromCInt((int) (LeftX),jLeftX,env);
	GBANUTIL_GetJIntExchangeFromCInt((int) (RightX),jRightX,env);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_GetMultiScanAPIVersion(
		JNIEnv *env,
		jobject thisObj,
		jobject jVer1,
		jobject jVer2,
		jobject jVer3,
		jobject jVer4)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetMultiScanAPIVersion: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_ROLL_GetCurrentStripeCoord == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetMultiScanAPIVersion: Ptr_GBMSAPI_GetMultiScanAPIVersion is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (jVer1 == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetMultiScanAPIVersion: jVer1 is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	if (jVer2 == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetMultiScanAPIVersion: jVer2 is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	if (jVer3 == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetMultiScanAPIVersion: jVer3 is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	if (jVer4 == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetMultiScanAPIVersion: jVer4 is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	unsigned char ver1, ver2, ver3, ver4;

	ValToRet = Ptr_GBMSAPI_GetMultiScanAPIVersion(
			&ver1, &ver2, &ver3, &ver4);
	if (ValToRet != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetMultiScanAPIVersion: Ptr_GBMSAPI_GetMultiScanAPIVersion returned %d", ValToRet);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	GBANUTIL_GetJIntExchangeFromCInt((int) (ver1),jVer1,env);
	GBANUTIL_GetJIntExchangeFromCInt((int) (ver2),jVer2,env);
	GBANUTIL_GetJIntExchangeFromCInt((int) (ver3),jVer3,env);
	GBANUTIL_GetJIntExchangeFromCInt((int) (ver4),jVer4,env);

	return ValToRet;
}

JNIEXPORT jstring JNICALL
Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_GetUnderlyingLibraryDllVersion(
		JNIEnv *env,
		jobject thisObj,
		jobject jVer1,
		jobject jVer2,
		jobject jVer3,
		jobject jVer4)
{
	int RetVal = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetUnderlyingLibraryDllVersion: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return NULL;
	}

	if (Ptr_GBMSAPI_GetUnderlyingLibraryDllVersion == NULL)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetUnderlyingLibraryDllVersion: Ptr_GBMSAPI_GetUnderlyingLibraryDllVersion is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return NULL;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (jVer1 == NULL)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetUnderlyingLibraryDllVersion: jVer1 is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return NULL;
	}
	if (jVer2 == NULL)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetUnderlyingLibraryDllVersion: jVer2 is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return NULL;
	}
	if (jVer3 == NULL)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetUnderlyingLibraryDllVersion: jVer3 is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return NULL;
	}
	if (jVer4 == NULL)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetUnderlyingLibraryDllVersion: jVer4 is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return NULL;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	unsigned char ver1, ver2, ver3, ver4;
	char LowLevelDllName[1000];

	RetVal = Ptr_GBMSAPI_GetUnderlyingLibraryDllVersion(
			&ver1, &ver2, &ver3, &ver4, LowLevelDllName);
	if (RetVal != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetUnderlyingLibraryDllVersion: Ptr_GBMSAPI_GetUnderlyingLibraryDllVersion returned %d",
		        RetVal);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return NULL;
	}

	GBANUTIL_GetJIntExchangeFromCInt((int) (ver1),jVer1,env);
	GBANUTIL_GetJIntExchangeFromCInt((int) (ver2),jVer2,env);
	GBANUTIL_GetJIntExchangeFromCInt((int) (ver3),jVer3,env);
	GBANUTIL_GetJIntExchangeFromCInt((int) (ver4),jVer4,env);

	return env->NewStringUTF(LowLevelDllName);
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_GetFrameStatistic(
		JNIEnv *env,
		jobject thisObj,
		jobject jAcquiredFrames,
		jobject jLostFrames)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetFrameStatistic: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_GetFrameStatistic == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetFrameStatistic: Ptr_GBMSAPI_GetFrameStatistic is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (jAcquiredFrames == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetFrameStatistic: jAcquiredFrames is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	if (jLostFrames == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetFrameStatistic: jLostFrames is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	unsigned long AcquiredFrames, LostFrames;

	ValToRet = Ptr_GBMSAPI_GetFrameStatistic(
			&AcquiredFrames, &LostFrames);
	if (ValToRet != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetFrameStatistic: Ptr_GBMSAPI_GetFrameStatistic returned %d", ValToRet);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	GBANUTIL_GetJIntExchangeFromCInt((int) (AcquiredFrames),jAcquiredFrames,env);
	GBANUTIL_GetJIntExchangeFromCInt((int) (LostFrames),jLostFrames,env);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_GetUSBErrorCode(
		JNIEnv *env,
		jobject thisObj,
		jobject jUsbErrorCode)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetUSBErrorCode: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_GetUSBErrorCode == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetUSBErrorCode: Ptr_GBMSAPI_GetUSBErrorCode is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (jUsbErrorCode == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetUSBErrorCode: jLostFrames is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	unsigned long UsbErrorCode;

	ValToRet = Ptr_GBMSAPI_GetUSBErrorCode(&UsbErrorCode);
	if (ValToRet != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetUSBErrorCode: v returned %d", ValToRet);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	GBANUTIL_GetJIntExchangeFromCInt((int) (UsbErrorCode),jUsbErrorCode,env);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_GetUserDataSize(
		JNIEnv *env,
		jobject thisObj,
		jobject jUserDataSize)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetUserDataSize: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_GetUserDataSize == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetUserDataSize: Ptr_GBMSAPI_GetUserDataSize is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (jUserDataSize == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "jUserDataSize: jLostFrames is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	int UserDataSize;

	ValToRet = Ptr_GBMSAPI_GetUserDataSize(&UserDataSize);
	if (ValToRet != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetUserDataSize: Ptr_GBMSAPI_GetUserDataSize returned %d", ValToRet);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	GBANUTIL_GetJIntExchangeFromCInt((int) (UserDataSize),jUserDataSize,env);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_GetUsbLinkSpeed(
		JNIEnv *env,
		jobject thisObj,
		jobject jUsbLinkSpeed)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetUsbLinkSpeed: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_GetUsbLinkSpeed == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetUsbLinkSpeed: Ptr_GBMSAPI_GetUsbLinkSpeed is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (jUsbLinkSpeed == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetUsbLinkSpeed: jUsbLinkSpeed is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	unsigned char UsbLinkSpeed;

	ValToRet = Ptr_GBMSAPI_GetUsbLinkSpeed(&UsbLinkSpeed);
	if (ValToRet != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetUsbLinkSpeed: Ptr_GBMSAPI_GetUsbLinkSpeed returned %d", ValToRet);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	GBANUTIL_GetJIntExchangeFromCInt((int) (UsbLinkSpeed),jUsbLinkSpeed,env);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_CheckHeater(
		JNIEnv *env,
		jobject thisObj,
		jobject jResult)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "CheckHeater: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_CheckHeater == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "CheckHeater: Ptr_GBMSAPI_CheckHeater is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (jResult == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "CheckHeater: jResult is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	int Result;

	ValToRet = Ptr_GBMSAPI_CheckHeater(&Result);
	if (ValToRet != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "CheckHeater: Ptr_GBMSAPI_CheckHeater returned %d", ValToRet);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	GBANUTIL_GetJIntExchangeFromCInt((int) (Result),jResult,env);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_GetheaterMeanTempRange(
		JNIEnv *env,
		jobject thisObj,
		jobject jMax,
		jobject jDefault,
		jobject jMin)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetheaterMeanTempRange: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_GetheaterMeanTempRange == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetheaterMeanTempRange: Ptr_GBMSAPI_GetheaterMeanTempRange is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (jMax == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetheaterMeanTempRange: jMax is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	if (jMin == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetheaterMeanTempRange: jMin is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	if (jDefault == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetheaterMeanTempRange: jDefault is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	float Max, Def, Min;

	ValToRet = Ptr_GBMSAPI_GetheaterMeanTempRange(&Max, &Def, & Min);
	if (ValToRet != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetheaterMeanTempRange: Ptr_GBMSAPI_GetheaterMeanTempRange returned %d", ValToRet);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	////////////////////////////////
	// set values
	////////////////////////////////
	GBANUTIL_GetJDoubleExchangeFromCDouble((double)Max,jMax,env);
	GBANUTIL_GetJDoubleExchangeFromCDouble((double)Min,jMin,env);
	GBANUTIL_GetJDoubleExchangeFromCDouble((double)Def,jDefault,env);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_GetHeaterMeanTemp(
		JNIEnv *env,
		jobject thisObj,
		jobject jTempC)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetHeaterMeanTemp: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_GetHeaterMeanTemp == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetHeaterMeanTemp: Ptr_GBMSAPI_GetHeaterMeanTemp is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (jTempC == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetHeaterMeanTemp: jTempC is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	float TempC;

	ValToRet = Ptr_GBMSAPI_GetHeaterMeanTemp(&TempC);
	if (ValToRet != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetHeaterMeanTemp: Ptr_GBMSAPI_GetHeaterMeanTemp returned %d", ValToRet);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	////////////////////////////////
	// set values
	////////////////////////////////
	GBANUTIL_GetJDoubleExchangeFromCDouble((double)TempC,jTempC,env);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_SetHeaterMeanTemp(
		JNIEnv *env,
		jobject thisObj,
		jdouble jTempC)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetHeaterMeanTemp: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_SetHeaterMeanTemp == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetHeaterMeanTemp: Ptr_GBMSAPI_SetHeaterMeanTemp is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////

	//////////////////////////////
	// call function from C
	//////////////////////////////
	ValToRet = Ptr_GBMSAPI_SetHeaterMeanTemp((float)jTempC);
	if (ValToRet != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetHeaterMeanTemp: Ptr_GBMSAPI_SetHeaterMeanTemp returned %d", ValToRet);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	////////////////////////////////
	// set values
	////////////////////////////////

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_UpdateBackgroundImage(
		JNIEnv *env,
		jobject thisObj,
		jbyteArray ImageFrame,
		jobject jDiagnostic
)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;
	unsigned char *ptrToFrame;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "UpdateBackgroundImage: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_UpdateBackgroundImage == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "UpdateBackgroundImage: Ptr_GBMSAPI_UpdateBackgroundImage is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (ImageFrame == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "UpdateBackgroundImage: ImageFrame is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetArrayLength(ImageFrame) <= 0)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "UpdateBackgroundImage: FrameLen <= 0");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	if (jDiagnostic == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "UpdateBackgroundImage: jDiagnostic is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	ptrToFrame = new unsigned char[env->GetArrayLength(ImageFrame)];
	if (ptrToFrame == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_MEMORY_ALLOCATION;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "UpdateBackgroundImage: Not enough memory for ptrToFrame");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	env->GetByteArrayRegion(ImageFrame, 0, env->GetArrayLength(ImageFrame),
	                        reinterpret_cast<jbyte *>(ptrToFrame));

	//////////////////////////////////
	// call function
	//////////////////////////////////
	unsigned int Diagnostic;

	ValToRet = Ptr_GBMSAPI_UpdateBackgroundImage(ptrToFrame, &Diagnostic);

	GBANUTIL_GetJIntExchangeFromCInt((int)Diagnostic,jDiagnostic,env);

	delete[] ptrToFrame;
	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_GetCalibrationImage(
		JNIEnv *env,
		jobject thisObj,
		jint jScanArea,
		jbyteArray ImageFrame,
		jobject jDiagnostic
)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;
	unsigned char *ptrToFrame;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetCalibrationImage: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_GetCalibrationImage2 == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetCalibrationImage: Ptr_GBMSAPI_GetCalibrationImage2 is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (ImageFrame == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetCalibrationImage: ImageFrame is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetArrayLength(ImageFrame) <= 0)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetCalibrationImage: FrameLen <= 0");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	if (jDiagnostic == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetCalibrationImage: jDiagnostic is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	ptrToFrame = new unsigned char[env->GetArrayLength(ImageFrame)];
	if (ptrToFrame == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_MEMORY_ALLOCATION;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetCalibrationImage: Not enough memory for ptrToFrame");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	env->GetByteArrayRegion(ImageFrame, 0, env->GetArrayLength(ImageFrame),
	                        reinterpret_cast<jbyte *>(ptrToFrame));

	//////////////////////////////////
	// call function
	//////////////////////////////////
	unsigned int Diagnostic;

	ValToRet = Ptr_GBMSAPI_GetCalibrationImage2((unsigned int) jScanArea, ptrToFrame, &Diagnostic);
	// copy array again
	env->SetByteArrayRegion(ImageFrame, 0, env->GetArrayLength(ImageFrame),
	                        reinterpret_cast<jbyte *>(ptrToFrame));
	GBANUTIL_GetJIntExchangeFromCInt((int)Diagnostic,jDiagnostic,env);

	delete[] ptrToFrame;
	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_ReadUserData(
		JNIEnv *env,
		jobject thisObj,
		jint jAddress,
		jint jSize,
		jbyteArray jDataBuffer
)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;
	unsigned char *ptrTodata;
	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "ReadUserData: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_ReadUserData == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "ReadUserData: Ptr_GBMSAPI_ReadUserData is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jDataBuffer == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "ReadUserData: jDataBuffer is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetArrayLength(jDataBuffer) < jSize)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "ReadUserData: jDataBuffer <= Size");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	ptrTodata = new unsigned char[jSize];
	if (ptrTodata == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_MEMORY_ALLOCATION;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "ReadUserData: Not enough memory for ptrTodata");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// call function
	//////////////////////////////////
	ValToRet = Ptr_GBMSAPI_ReadUserData((unsigned int) jAddress, (unsigned int) jSize, ptrTodata);
	// copy array again
	env->SetByteArrayRegion(jDataBuffer, 0, jSize, reinterpret_cast<jbyte *>(ptrTodata));
	delete[] ptrTodata;
	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_RollGetCompositeImageInfo(
		JNIEnv *env,
		jobject thisObj,
		jobject jRolledArtefactSize,
		jbyteArray jMarkerFrame,
		jbyteArray jNotWipedArtefactFrame,
		jobject jCompositeFingerprintSize,
		jobject jCompositeFingerprintContrast,
		jobject jFlatFingerprintSize,
		jobject jSizeX,
		jobject jSizeY
)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;
	unsigned char *ptrToMarkerFrame, *ptrToNotWipedArtefactFrame;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "RollGetCompositeImageInfo: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_ROLL_GetCompositeImageInfo2 == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "RollGetCompositeImageInfo: Ptr_GBMSAPI_ROLL_GetCompositeImageInfo2 is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jMarkerFrame == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "RollGetCompositeImageInfo: jMarkerFrame is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetArrayLength(jMarkerFrame) <= 0)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "RollGetCompositeImageInfo: jMarkerFrame FrameLen <= 0");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	if (jNotWipedArtefactFrame == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "RollGetCompositeImageInfo: jNotWipedArtefactFrame is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetArrayLength(jNotWipedArtefactFrame) <= 0)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "RollGetCompositeImageInfo: jNotWipedArtefactFrame FrameLen <= 0");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	if (jRolledArtefactSize == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "RollGetCompositeImageInfo: jRolledArtefactSize is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	if (jCompositeFingerprintSize == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "RollGetCompositeImageInfo: jCompositeFingerprintSize is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	if (jCompositeFingerprintContrast == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "RollGetCompositeImageInfo: jCompositeFingerprintContrast is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	if (jFlatFingerprintSize == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "RollGetCompositeImageInfo: jFlatFingerprintSize is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	if (jSizeX == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "RollGetCompositeImageInfo: jSizeX is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	if (jSizeY == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "RollGetCompositeImageInfo: jSizeY is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// call function
	//////////////////////////////////
	unsigned int RolledArtefactSize, CompositeFingerprintSize, FlatFingerprintSize,
			ImageSizeX, ImageSizeY;
	unsigned char CompositeFingerprintContrast;

	ValToRet = Ptr_GBMSAPI_ROLL_GetCompositeImageInfo2(
			&RolledArtefactSize,
			&ptrToMarkerFrame,
			&ptrToNotWipedArtefactFrame,
			&CompositeFingerprintSize,
			&CompositeFingerprintContrast,
			&FlatFingerprintSize,
			&ImageSizeX, &ImageSizeY
	);

	// copy array again
	env->SetByteArrayRegion(jMarkerFrame, 0, env->GetArrayLength(jMarkerFrame),
	                        reinterpret_cast<jbyte *>(ptrToMarkerFrame));
	env->SetByteArrayRegion(jNotWipedArtefactFrame, 0, env->GetArrayLength(jNotWipedArtefactFrame),
	                        reinterpret_cast<jbyte *>(ptrToNotWipedArtefactFrame));

	GBANUTIL_GetJIntExchangeFromCInt((int)RolledArtefactSize,jRolledArtefactSize,env);
	GBANUTIL_GetJIntExchangeFromCInt((int)CompositeFingerprintSize,jCompositeFingerprintSize,env);
	GBANUTIL_GetJIntExchangeFromCInt((int)CompositeFingerprintContrast,jCompositeFingerprintContrast,env);
	GBANUTIL_GetJIntExchangeFromCInt((int)FlatFingerprintSize,jFlatFingerprintSize,env);
	GBANUTIL_GetJIntExchangeFromCInt((int)ImageSizeX,jSizeX,env);
	GBANUTIL_GetJIntExchangeFromCInt((int)ImageSizeY,jSizeY,env);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_SetCurrentDevice(
		JNIEnv *env,
		jobject thisObj,
		jint jDeviceID,
		jstring jDeviceSerialNumber)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetCurrentDevice: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_SetCurrentDevice == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetCurrentDevice: Ptr_GBMSAPI_SetCurrentDevice is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jDeviceSerialNumber == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "SetCurrentDevice: attachedDeviceListArray is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	// device ID
	unsigned char deviceID = (unsigned char) jDeviceID;
	// serial number
	const char *constDevSerNum;
	constDevSerNum = (env)->GetStringUTFChars(jDeviceSerialNumber, 0);
	char *DevSerNum = new char[(env)->GetStringUTFLength(jDeviceSerialNumber)];
	memcpy(DevSerNum, constDevSerNum, (env)->GetStringUTFLength(jDeviceSerialNumber));
	env->ReleaseStringUTFChars(jDeviceSerialNumber, constDevSerNum);

	//////////////////////////////
	// call function from C
	//////////////////////////////
	ValToRet = Ptr_GBMSAPI_SetCurrentDevice(deviceID, DevSerNum);
	delete[] DevSerNum;
	if (ValToRet != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetAttachedDeviceList: Ptr_GBMSAPI_GetAttachedDeviceList returned %d",
		        ValToRet);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_GetAttachedDeviceList(
		JNIEnv *env,
		jobject thisObj,
		jobjectArray attachedDeviceListArray)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;
	jclass DeviceInfoJava;
	jmethodID ptrToSetFieldFunction;

	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetAttachedDeviceList: GBMSAPIDllPtr is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBMSAPI_GetAttachedDeviceList == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetAttachedDeviceList: Ptr_GBMSAPI_GetAttachedDeviceList is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (attachedDeviceListArray == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetAttachedDeviceList: attachedDeviceListArray is NULL");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	if ((env)->GetArrayLength(attachedDeviceListArray) < GBMSAPI_MAX_PLUGGED_DEVICE_NUM)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetAttachedDeviceList: attachedDeviceListArray length too small");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	DeviceInfoJava = env->FindClass(
			"com/greenbit/gbmsapi/GBMSAPIJavaWrapperDefinesDeviceInfoStruct");
	if (DeviceInfoJava == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetAttachedDeviceList: cannot find definition of GBMSAPIJavaWrapperDefinesDeviceInfoStruct class");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	ptrToSetFieldFunction = env->GetMethodID(DeviceInfoJava, "SetFields", "(ILjava/lang/String;)V");
	if (ptrToSetFieldFunction == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_PARAMETER;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetAttachedDeviceList: cannot find definition of SetFields function into GBMSAPIJavaWrapperDefinesDeviceInfoStruct class");
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// call function from C
	//////////////////////////////
	GBMSAPI_DeviceInfoStruct AttachedDeviceList[GBMSAPI_MAX_PLUGGED_DEVICE_NUM];
	int AttachedDeviceNumber;
	unsigned long UsbErrorCode;
	ValToRet = Ptr_GBMSAPI_GetAttachedDeviceList(AttachedDeviceList, &AttachedDeviceNumber,
	                                             &UsbErrorCode);
	if (ValToRet != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GetAttachedDeviceList: Ptr_GBMSAPI_GetAttachedDeviceList returned %d, UsbErrorCode = 0x%x",
		        ValToRet, (unsigned int) UsbErrorCode);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// copy values to array
	//////////////////////////////
	int i;
	for (i = 0; i < AttachedDeviceNumber; i++)
	{
		jobject item = env->GetObjectArrayElement(attachedDeviceListArray, i);
		env->CallVoidMethod(item, ptrToSetFieldFunction, (int) (AttachedDeviceList[i].DeviceID),
		                    env->NewStringUTF(AttachedDeviceList[i].DeviceSerialNumber));
	}
	for (; i < (env)->GetArrayLength(attachedDeviceListArray); i++)
	{
		char c[2] = "";
		jobject item = env->GetObjectArrayElement(attachedDeviceListArray, i);
		env->CallVoidMethod(item, ptrToSetFieldFunction, 0, env->NewStringUTF(c));
	}

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_Load(
		JNIEnv *env,
		jobject /* this */)
{
	int ValToRet = GBMSAPI_ERROR_CODE_NO_ERROR;

	//////////////////////////////
	// free library
	//////////////////////////////
	if (GBMSAPIDllPtr != NULL)
		Java_com_greenbit_gbmsapi_GBMSAPIJavaWrapperLibrary_Unload(env, nullptr);

	//////////////////////////////
	// load library
	//////////////////////////////
	GBMSAPIDllPtr = LoadLibrary("libGBMSAPI.so");
	if (GBMSAPIDllPtr == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load GBMSAPI: %s", dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: GBMSAPI loaded\n");

	//////////////////////////////
	// GBMSAPI_LoadLibrary
	//////////////////////////////
	Ptr_GBMSAPI_LoadLibrary = (int (*)()) GetProcAddress(GBMSAPIDllPtr, "GBMSAPI_LoadLibrary");
	if (Ptr_GBMSAPI_LoadLibrary == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_LoadLibrary: %s", dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_LoadLibrary loaded\n");

	// call loadlibrary
	ValToRet = Ptr_GBMSAPI_LoadLibrary();
	if (ValToRet != GBMSAPI_ERROR_CODE_NO_ERROR)
	{
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_LoadLibrary error = %d", ValToRet);
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_LoadLibrary called succesfully\n");

	//////////////////////////////
	// GBMSAPI_UnloadLibrary
	//////////////////////////////
	Ptr_GBMSAPI_UnloadLibrary = (int (*)()) GetProcAddress(GBMSAPIDllPtr, "GBMSAPI_UnloadLibrary");
	if (Ptr_GBMSAPI_UnloadLibrary == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_UnloadLibrary: %s", dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_UnloadLibrary loaded\n");

	//////////////////////////////
	// GBMSAPI_GetAttachedDeviceList
	//////////////////////////////
	Ptr_GBMSAPI_GetAttachedDeviceList = (int (*)(
			GBMSAPI_DeviceInfoStruct AttachedDeviceList[GBMSAPI_MAX_PLUGGED_DEVICE_NUM],
			int *AttachedDeviceNumber,
			unsigned long *USBErrorCode))
			GetProcAddress(GBMSAPIDllPtr, "GBMSAPI_GetAttachedDeviceList");
	if (Ptr_GBMSAPI_GetAttachedDeviceList == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_GetAttachedDeviceList: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_GetAttachedDeviceList loaded\n");

	//////////////////////////////
	// GBMSAPI_GetUsbLinkSpeed
	//////////////////////////////
	Ptr_GBMSAPI_GetUsbLinkSpeed = (int (*)(
			unsigned char *))
			GetProcAddress(GBMSAPIDllPtr, "GBMSAPI_GetUsbLinkSpeed");
	if (Ptr_GBMSAPI_GetUsbLinkSpeed == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_GetUsbLinkSpeed: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_GetUsbLinkSpeed loaded\n");

	//////////////////////////////
	// GBMSAPI_CheckHeater
	//////////////////////////////
	Ptr_GBMSAPI_CheckHeater = (int (*)(
			int *))
			GetProcAddress(GBMSAPIDllPtr, "GBMSAPI_CheckHeater");
	if (Ptr_GBMSAPI_CheckHeater == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_CheckHeater: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_CheckHeater loaded\n");

	//////////////////////////////
	// GBMSAPI_GetheaterMeanTempRange
	//////////////////////////////
	Ptr_GBMSAPI_GetheaterMeanTempRange = (int (*)(
			float *Max, float *Default, float *Min))
			GetProcAddress(GBMSAPIDllPtr, "GBMSAPI_GetheaterMeanTempRange");
	if (Ptr_GBMSAPI_GetheaterMeanTempRange == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_GetheaterMeanTempRange: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_GetheaterMeanTempRange loaded\n");

	//////////////////////////////
	// GBMSAPI_GetHeaterMeanTemp
	//////////////////////////////
	Ptr_GBMSAPI_GetHeaterMeanTemp = (int (*)(
			float *))
			GetProcAddress(GBMSAPIDllPtr, "GBMSAPI_GetHeaterMeanTemp");
	if (Ptr_GBMSAPI_GetHeaterMeanTemp == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_GetHeaterMeanTemp: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_GetHeaterMeanTemp loaded\n");

	//////////////////////////////
	// GBMSAPI_SetHeaterMeanTemp
	//////////////////////////////
	Ptr_GBMSAPI_SetHeaterMeanTemp = (int (*)(
			float))
			GetProcAddress(GBMSAPIDllPtr, "GBMSAPI_SetHeaterMeanTemp");
	if (Ptr_GBMSAPI_SetHeaterMeanTemp == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_SetHeaterMeanTemp: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_SetHeaterMeanTemp loaded\n");

	//////////////////////////////
	// GBMSAPI_SetOpenedJavaFD
	//////////////////////////////
	Ptr_GBMSAPI_SetOpenedJavaFD = (void (*)(
			int *, int *, int *, int))
			GetProcAddress(GBMSAPIDllPtr, "GBMSAPI_SetOpenedJavaFD");
	if (Ptr_GBMSAPI_SetOpenedJavaFD == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load GBMSAPI_SetOpenedJavaFD: %s", dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_SetOpenedJavaFD loaded\n");

	//////////////////////////////
	// GBMSAPI_SetCurrentDevice
	//////////////////////////////
	Ptr_GBMSAPI_SetCurrentDevice = (int (*)(unsigned char, char *)) GetProcAddress(GBMSAPIDllPtr,
	                                                                               "GBMSAPI_SetCurrentDevice");
	if (Ptr_GBMSAPI_SetCurrentDevice == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_SetCurrentDevice: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_SetCurrentDevice loaded\n");

	//////////////////////////////
	// GBMSAPI_GetCurrentDevice
	//////////////////////////////
	Ptr_GBMSAPI_GetCurrentDevice = (int (*)(unsigned char *, char *)) GetProcAddress(GBMSAPIDllPtr,
	                                                                                 "GBMSAPI_GetCurrentDevice");
	if (Ptr_GBMSAPI_GetCurrentDevice == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_GetCurrentDevice: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_GetCurrentDevice loaded\n");

	//////////////////////////////
	// GBMSAPI_GetScannableTypes
	//////////////////////////////
	Ptr_GBMSAPI_GetScannableTypes = (int (*)(unsigned int *)) GetProcAddress(GBMSAPIDllPtr,
	                                                                         "GBMSAPI_GetScannableTypes");
	if (Ptr_GBMSAPI_GetScannableTypes == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_GetScannableTypes: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_GetScannableTypes loaded\n");

	//////////////////////////////
	// GBMSAPI_GetDeviceFeatures
	//////////////////////////////
	Ptr_GBMSAPI_GetDeviceFeatures = (int (*)(unsigned int *)) GetProcAddress(GBMSAPIDllPtr,
	                                                                         "GBMSAPI_GetDeviceFeatures");
	if (Ptr_GBMSAPI_GetDeviceFeatures == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_GetDeviceFeatures: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_GetDeviceFeatures loaded\n");

	//////////////////////////////
	// GBMSAPI_GetSupportedScanAreas
	//////////////////////////////
	Ptr_GBMSAPI_GetSupportedScanAreas = (int (*)(unsigned int *)) GetProcAddress(GBMSAPIDllPtr,
	                                                                             "GBMSAPI_GetSupportedScanAreas");
	if (Ptr_GBMSAPI_GetSupportedScanAreas == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_GetSupportedScanAreas: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_GetSupportedScanAreas loaded\n");

	//////////////////////////////
	// GBMSAPI_GetSupportedScanAreasByDevice
	//////////////////////////////
	Ptr_GBMSAPI_GetSupportedScanAreasByDevice = (int (*)(unsigned char,
	                                                     unsigned int *)) GetProcAddress(
			GBMSAPIDllPtr,
			"GBMSAPI_GetSupportedScanAreasByDevice");
	if (Ptr_GBMSAPI_GetSupportedScanAreasByDevice == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_GetSupportedScanAreasByDevice: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_GetSupportedScanAreasByDevice loaded\n");

	//////////////////////////////
	// GBMSAPI_GetAvailableImageInfo
	//////////////////////////////
	Ptr_GBMSAPI_GetAvailableImageInfo = (int (*)(unsigned int, unsigned int *)) GetProcAddress(
			GBMSAPIDllPtr,
			"GBMSAPI_GetAvailableImageInfo");
	if (Ptr_GBMSAPI_GetAvailableImageInfo == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_GetAvailableImageInfo: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_GetAvailableImageInfo loaded\n");

	//////////////////////////////
	// GBMSAPI_SetRollAreaStandard
	//////////////////////////////
	Ptr_GBMSAPI_SetRollAreaStandard = (int (*)(unsigned int)) GetProcAddress(GBMSAPIDllPtr,
	                                                                         "GBMSAPI_SetRollAreaStandard");
	if (Ptr_GBMSAPI_SetRollAreaStandard == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_SetRollAreaStandard: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_SetRollAreaStandard loaded\n");

	//////////////////////////////
	// GBMSAPI_GetSupportedScanOptions
	//////////////////////////////
	Ptr_GBMSAPI_GetSupportedScanOptions = (int (*)(unsigned int *)) GetProcAddress(GBMSAPIDllPtr,
	                                                                               "GBMSAPI_GetSupportedScanOptions");
	if (Ptr_GBMSAPI_GetSupportedScanOptions == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_GetSupportedScanOptions: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_GetSupportedScanOptions loaded\n");

	//////////////////////////////
	// GBMSAPI_GetOptionalExternalEquipment
	//////////////////////////////
	Ptr_GBMSAPI_GetOptionalExternalEquipment = (int (*)(unsigned int *)) GetProcAddress(
			GBMSAPIDllPtr,
			"GBMSAPI_GetOptionalExternalEquipment");
	if (Ptr_GBMSAPI_GetOptionalExternalEquipment == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_GetOptionalExternalEquipment: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_GetOptionalExternalEquipment loaded\n");

	//////////////////////////////
	// GBMSAPI_GetImageSize3
	//////////////////////////////
	Ptr_GBMSAPI_GetImageSize3 = (int (*)(
			unsigned int, unsigned int, unsigned int,
			unsigned int *, unsigned int *,
			unsigned int *, unsigned int *)) GetProcAddress(
			GBMSAPIDllPtr, "GBMSAPI_GetImageSize3");
	if (Ptr_GBMSAPI_GetImageSize3 == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_GetImageSize3: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_GetImageSize3 loaded\n");

	//////////////////////////////
	// GBMSAPI_GetMaxImageSize
	//////////////////////////////
	Ptr_GBMSAPI_GetMaxImageSize = (int (*)(unsigned int *, unsigned int *)) GetProcAddress(
			GBMSAPIDllPtr, "GBMSAPI_GetMaxImageSize");
	if (Ptr_GBMSAPI_GetMaxImageSize == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_GetMaxImageSize: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_GetMaxImageSize loaded\n");

	//////////////////////////////
	// GBMSAPI_GetCalibrationImageSize2
	//////////////////////////////
	Ptr_GBMSAPI_GetCalibrationImageSize2 = (int (*)(unsigned int, unsigned int *,
	                                                unsigned int *)) GetProcAddress(
			GBMSAPIDllPtr, "GBMSAPI_GetCalibrationImageSize2");
	if (Ptr_GBMSAPI_GetCalibrationImageSize2 == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_GetCalibrationImageSize2: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_GetCalibrationImageSize2 loaded\n");

	//////////////////////////////////////////////////////
	// GBMSAPI_GetFrameRateRange2
	//////////////////////////////////////////////////////
	Ptr_GBMSAPI_GetFrameRateRange2 = (int (*)(
			unsigned char, unsigned int, unsigned int,
			double *, double *, double *
	))
			GetProcAddress(GBMSAPIDllPtr, "GBMSAPI_GetFrameRateRange2");
	if (Ptr_GBMSAPI_GetFrameRateRange2 == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_GetFrameRateRange2: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_GetFrameRateRange2 loaded\n");

	//////////////////////////////
	// GBMSAPI_SetDiagnosticFilterForLCD
	//////////////////////////////
	Ptr_GBMSAPI_SetDiagnosticFilterForLCD = (void (*)(unsigned int)) GetProcAddress(
			GBMSAPIDllPtr, "GBMSAPI_SetDiagnosticFilterForLCD");
	if (Ptr_GBMSAPI_SetDiagnosticFilterForLCD == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_SetDiagnosticFilterForLCD: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_SetDiagnosticFilterForLCD loaded\n");

	//////////////////////////////
	// GBMSAPI_EnableAutoCaptureBlockForDetectedFakes
	//////////////////////////////
	Ptr_GBMSAPI_EnableAutoCaptureBlockForDetectedFakes = (int (*)(unsigned char)) GetProcAddress(
			GBMSAPIDllPtr, "GBMSAPI_EnableAutoCaptureBlockForDetectedFakes");
	if (Ptr_GBMSAPI_EnableAutoCaptureBlockForDetectedFakes == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_EnableAutoCaptureBlockForDetectedFakes: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf (
			"GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_EnableAutoCaptureBlockForDetectedFakes loaded\n");

	//////////////////////////////
	// GBMSAPI_SetHardwareFakeFingerDetectionThreshold
	//////////////////////////////
	Ptr_GBMSAPI_SetHardwareFakeFingerDetectionThreshold = (int (*)(int)) GetProcAddress(
			GBMSAPIDllPtr, "GBMSAPI_SetHardwareFakeFingerDetectionThreshold");
	if (Ptr_GBMSAPI_SetHardwareFakeFingerDetectionThreshold == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_SetHardwareFakeFingerDetectionThreshold: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf (
			"GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_SetHardwareFakeFingerDetectionThreshold loaded\n");

	/////////////////////////////////////////
	// GBMSAPI_HardwareFakeFingerDetection
	/////////////////////////////////////////
	Ptr_GBMSAPI_HardwareFakeFingerDetection = (int (*)(BOOL *, unsigned int *)) GetProcAddress(
			GBMSAPIDllPtr, "GBMSAPI_HardwareFakeFingerDetection");
	if (Ptr_GBMSAPI_HardwareFakeFingerDetection == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_HardwareFakeFingerDetection: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_HardwareFakeFingerDetection loaded\n");

	//////////////////////////////////////////////////////
	// GBMSAPI_SetSoftwareFakeFingerDetectionThreshold
	//////////////////////////////////////////////////////
	Ptr_GBMSAPI_SetSoftwareFakeFingerDetectionThreshold = (int (*)(int)) GetProcAddress(
			GBMSAPIDllPtr, "GBMSAPI_SetSoftwareFakeFingerDetectionThreshold");
	if (Ptr_GBMSAPI_SetSoftwareFakeFingerDetectionThreshold == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_SetSoftwareFakeFingerDetectionThreshold: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf (
			"GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_SetSoftwareFakeFingerDetectionThreshold loaded\n");

	/////////////////////////////////////////
	// GBMSAPI_SoftwareFakeFingerDetection
	/////////////////////////////////////////
	Ptr_GBMSAPI_SoftwareFakeFingerDetection = (int (*)(
			unsigned char *, int, int,
			BOOL *)) GetProcAddress(
			GBMSAPIDllPtr, "GBMSAPI_SoftwareFakeFingerDetection");
	if (Ptr_GBMSAPI_SoftwareFakeFingerDetection == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_SoftwareFakeFingerDetection: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_SoftwareFakeFingerDetection loaded\n");

	//////////////////////////////////////////////////////
	// GBMSAPI_SetSelectImageTimeout
	//////////////////////////////////////////////////////
	Ptr_GBMSAPI_SetSelectImageTimeout = (int (*)(unsigned int)) GetProcAddress(
			GBMSAPIDllPtr, "GBMSAPI_SetSelectImageTimeout");
	if (Ptr_GBMSAPI_SetSelectImageTimeout == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_SetSelectImageTimeout: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_SetSelectImageTimeout loaded\n");

	//////////////////////////////////////////////////////
	// GBMSAPI_SetLowerHalfPalmCompletenessThreshold
	//////////////////////////////////////////////////////
	Ptr_GBMSAPI_SetLowerHalfPalmCompletenessThreshold = (int (*)(unsigned char)) GetProcAddress(
			GBMSAPIDllPtr, "GBMSAPI_SetLowerHalfPalmCompletenessThreshold");
	if (Ptr_GBMSAPI_SetLowerHalfPalmCompletenessThreshold == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_SetLowerHalfPalmCompletenessThreshold: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf (
			"GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_SetLowerHalfPalmCompletenessThreshold loaded\n");

	//////////////////////////////////////////////////////
	// GBMSAPI_SetFrameRate2
	//////////////////////////////////////////////////////
	Ptr_GBMSAPI_SetFrameRate2 = (int (*)(unsigned int, unsigned int, double)) GetProcAddress(
			GBMSAPIDllPtr, "GBMSAPI_SetFrameRate2");
	if (Ptr_GBMSAPI_SetFrameRate2 == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_SetFrameRate2: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_SetFrameRate2 loaded\n");

	//////////////////////////////////////////////////////
	// GBMSAPI_EnableManualLEDControl
	//////////////////////////////////////////////////////
	Ptr_GBMSAPI_EnableManualLEDControl = (int (*)(BOOL)) GetProcAddress(
			GBMSAPIDllPtr, "GBMSAPI_EnableManualLEDControl");
	if (Ptr_GBMSAPI_SetSelectImageTimeout == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_EnableManualLEDControl: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_EnableManualLEDControl loaded\n");

	//////////////////////////////////////////////////////
	// GBMSAPI_EnableLEDColorSettingDuringAcquisition
	//////////////////////////////////////////////////////
	Ptr_GBMSAPI_EnableLEDColorSettingDuringAcquisition = (int (*)(BOOL)) GetProcAddress(
			GBMSAPIDllPtr, "GBMSAPI_EnableLEDColorSettingDuringAcquisition");
	if (Ptr_GBMSAPI_SetSelectImageTimeout == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_EnableLEDColorSettingDuringAcquisition: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf (
			"GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_EnableLEDColorSettingDuringAcquisition loaded\n");

	//////////////////////////////////////////////////////
	// GBMSAPI_SetImageRotation
	//////////////////////////////////////////////////////
	Ptr_GBMSAPI_SetImageRotation = (int (*)(int)) GetProcAddress(
			GBMSAPIDllPtr, "GBMSAPI_SetImageRotation");
	if (Ptr_GBMSAPI_SetSelectImageTimeout == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_SetImageRotation: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_SetImageRotation loaded\n");

	//////////////////////////////////////////////////////
	// GBMSAPI_EnableDrySkinImgEnhance
	//////////////////////////////////////////////////////
	Ptr_GBMSAPI_EnableDrySkinImgEnhance = (int (*)(unsigned char)) GetProcAddress(
			GBMSAPIDllPtr, "GBMSAPI_EnableDrySkinImgEnhance");
	if (Ptr_GBMSAPI_SetSelectImageTimeout == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_EnableDrySkinImgEnhance: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_EnableDrySkinImgEnhance loaded\n");

	//////////////////////////////////////////////////////
	// GBMSAPI_ROLL_EnableRollStripeAcquisition
	//////////////////////////////////////////////////////
	Ptr_GBMSAPI_ROLL_EnableRollStripeAcquisition = (int (*)(
			unsigned int, BOOL)) GetProcAddress(
			GBMSAPIDllPtr, "GBMSAPI_ROLL_EnableRollStripeAcquisition");
	if (Ptr_GBMSAPI_SetSelectImageTimeout == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_ROLL_EnableRollStripeAcquisition: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_ROLL_EnableRollStripeAcquisition loaded\n");

	//////////////////////////////////////////////////////
	// GBMSAPI_ROLL_EnableBlockComposition
	//////////////////////////////////////////////////////
	Ptr_GBMSAPI_ROLL_EnableBlockComposition = (int (*)(int)) GetProcAddress(
			GBMSAPIDllPtr, "GBMSAPI_ROLL_EnableBlockComposition");
	if (Ptr_GBMSAPI_SetSelectImageTimeout == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_ROLL_EnableBlockComposition: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_ROLL_EnableBlockComposition loaded\n");

	//////////////////////////////
	// GBMSAPI_StartAcquisition2
	//////////////////////////////
	Ptr_GBMSAPI_StartAcquisition2 = (int (*)(
			unsigned int ObjectToScan,
			unsigned int OptionMask,
			unsigned int ScanArea,
			GBMSAPI_AcquisitionEventsManagerCallback AcquisitionEventsManagerCallbackPtr,
			void *AcquisitionEventsManagerParameters,
			unsigned int DisplayOptionMask,
			unsigned char ContrastLimitToDisplayOnLCD,
			unsigned char CompletenessLimitToDisplayOnLCD))
			GetProcAddress(
					GBMSAPIDllPtr, "GBMSAPI_StartAcquisition2");
	if (Ptr_GBMSAPI_StartAcquisition2 == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_StartAcquisition2: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_StartAcquisition2 loaded\n");

	//////////////////////////////
	// GBMSAPI_ROLL_StopPreview
	//////////////////////////////
	Ptr_GBMSAPI_ROLL_StopPreview = (int (*)()) GetProcAddress(GBMSAPIDllPtr,
	                                                          "GBMSAPI_ROLL_StopPreview");
	if (Ptr_GBMSAPI_ROLL_StopPreview == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_ROLL_StopPreview: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_ROLL_StopPreview loaded\n");

	//////////////////////////////
	// GBMSAPI_StopAcquisition
	//////////////////////////////
	Ptr_GBMSAPI_StopAcquisition = (int (*)()) GetProcAddress(GBMSAPIDllPtr,
	                                                         "GBMSAPI_StopAcquisition");
	if (Ptr_GBMSAPI_StopAcquisition == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_StopAcquisition: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_StopAcquisition loaded\n");

	//////////////////////////////
	// GBMSAPI_ImageFinalization
	//////////////////////////////
	Ptr_GBMSAPI_ImageFinalization = (int (*)(unsigned char *)) GetProcAddress(GBMSAPIDllPtr,
	                                                                          "GBMSAPI_ImageFinalization");
	if (Ptr_GBMSAPI_ImageFinalization == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_ImageFinalization: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_ImageFinalization loaded\n");

	//////////////////////////////
	// GBMSAPI_SetClippingRegionSize
	//////////////////////////////
	Ptr_GBMSAPI_SetClippingRegionSize = (int (*)(unsigned int, unsigned int))
			GetProcAddress(GBMSAPIDllPtr, "GBMSAPI_SetClippingRegionSize");
	if (Ptr_GBMSAPI_SetClippingRegionSize == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_SetClippingRegionSize: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_SetClippingRegionSize loaded\n");

	//////////////////////////////
	// GBMSAPI_ROLL_SetPreviewTimeout
	//////////////////////////////
	Ptr_GBMSAPI_ROLL_SetPreviewTimeout = (int (*)(unsigned int))
			GetProcAddress(GBMSAPIDllPtr, "GBMSAPI_ROLL_SetPreviewTimeout");
	if (Ptr_GBMSAPI_ROLL_SetPreviewTimeout == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_ROLL_SetPreviewTimeout: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_ROLL_SetPreviewTimeout loaded\n");

	//////////////////////////////
	// GBMSAPI_ROLL_SetArtefactCleaningDepth
	//////////////////////////////
	Ptr_GBMSAPI_ROLL_SetArtefactCleaningDepth = (int (*)(unsigned char))
			GetProcAddress(GBMSAPIDllPtr, "GBMSAPI_ROLL_SetArtefactCleaningDepth");
	if (Ptr_GBMSAPI_ROLL_SetArtefactCleaningDepth == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_ROLL_SetArtefactCleaningDepth: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_ROLL_SetArtefactCleaningDepth loaded\n");

	//////////////////////////////////////////
	// GBMSAPI_VUI_LED_BlinkDuringAcquisition
	//////////////////////////////////////////
	Ptr_GBMSAPI_VUI_LED_BlinkDuringAcquisition = (int (*)(BOOL))
			GetProcAddress(GBMSAPIDllPtr, "GBMSAPI_VUI_LED_BlinkDuringAcquisition");
	if (Ptr_GBMSAPI_VUI_LED_BlinkDuringAcquisition == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_VUI_LED_BlinkDuringAcquisition: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_VUI_LED_BlinkDuringAcquisition loaded\n");

	//////////////////////////////////////////
	// GBMSAPI_GetDryWetFingerAreaPercent
	//////////////////////////////////////////
	Ptr_GBMSAPI_GetDryWetFingerAreaPercent = (int (*)(unsigned char *, unsigned char *))
			GetProcAddress(GBMSAPIDllPtr, "GBMSAPI_GetDryWetFingerAreaPercent");
	if (Ptr_GBMSAPI_GetDryWetFingerAreaPercent == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_GetDryWetFingerAreaPercent: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_GetDryWetFingerAreaPercent loaded\n");

	//////////////////////////////////////////
	// GBMSAPI_GetAutoCapturePhase
	//////////////////////////////////////////
	Ptr_GBMSAPI_GetAutoCapturePhase = (int (*)(unsigned int *))
			GetProcAddress(GBMSAPIDllPtr, "GBMSAPI_GetAutoCapturePhase");
	if (Ptr_GBMSAPI_GetAutoCapturePhase == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_GetAutoCapturePhase: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_GetAutoCapturePhase loaded\n");

	//////////////////////////////////////////
	// GBMSAPI_SetAutoCaptureBlocking
	//////////////////////////////////////////
	Ptr_GBMSAPI_SetAutoCaptureBlocking = (int (*)(BOOL))
			GetProcAddress(GBMSAPIDllPtr, "GBMSAPI_SetAutoCaptureBlocking");
	if (Ptr_GBMSAPI_SetAutoCaptureBlocking == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_SetAutoCaptureBlocking: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_SetAutoCaptureBlocking loaded\n");

	//////////////////////////////////////////
	// GBMSAPI_GetFingerprintContrast
	//////////////////////////////////////////
	Ptr_GBMSAPI_GetFingerprintContrast = (int (*)(unsigned char *))
			GetProcAddress(GBMSAPIDllPtr, "GBMSAPI_GetFingerprintContrast");
	if (Ptr_GBMSAPI_GetFingerprintContrast == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_GetFingerprintContrast: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_GetFingerprintContrast loaded\n");

	//////////////////////////////////////////
	// GBMSAPI_GetFingerprintSize
	//////////////////////////////////////////
	Ptr_GBMSAPI_GetFingerprintSize = (int (*)(unsigned int *))
			GetProcAddress(GBMSAPIDllPtr, "GBMSAPI_GetFingerprintSize");
	if (Ptr_GBMSAPI_GetFingerprintSize == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_GetFingerprintSize: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_GetFingerprintSize loaded\n");

	//////////////////////////////////////////
	// GBMSAPI_GetLowerHalfPalmCompleteness
	//////////////////////////////////////////
	Ptr_GBMSAPI_GetLowerHalfPalmCompleteness = (int (*)(unsigned char *))
			GetProcAddress(GBMSAPIDllPtr, "GBMSAPI_GetLowerHalfPalmCompleteness");
	if (Ptr_GBMSAPI_GetLowerHalfPalmCompleteness == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_GetLowerHalfPalmCompleteness: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_GetLowerHalfPalmCompleteness loaded\n");

	//////////////////////////////////////////
	// GBMSAPI_GetClippingRegionPosition
	//////////////////////////////////////////
	Ptr_GBMSAPI_GetClippingRegionPosition = (int (*)(int *, int *, unsigned int *, unsigned int *))
			GetProcAddress(GBMSAPIDllPtr, "GBMSAPI_GetClippingRegionPosition");
	if (Ptr_GBMSAPI_GetClippingRegionPosition == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_GetClippingRegionPosition: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_GetClippingRegionPosition loaded\n");

	//////////////////////////////////////////
	// GBMSAPI_ROLL_GetCurrentStripeCoord
	//////////////////////////////////////////
	Ptr_GBMSAPI_ROLL_GetCurrentStripeCoord = (int (*)(int *, int *))
			GetProcAddress(GBMSAPIDllPtr, "GBMSAPI_ROLL_GetCurrentStripeCoord");
	if (Ptr_GBMSAPI_ROLL_GetCurrentStripeCoord == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_ROLL_GetCurrentStripeCoord: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_ROLL_GetCurrentStripeCoord loaded\n");

	//////////////////////////////////////////
	// GBMSAPI_UpdateBackgroundImage
	//////////////////////////////////////////
	Ptr_GBMSAPI_UpdateBackgroundImage = (int (*)(unsigned char *, unsigned int *))
			GetProcAddress(GBMSAPIDllPtr, "GBMSAPI_UpdateBackgroundImage");
	if (Ptr_GBMSAPI_UpdateBackgroundImage == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_UpdateBackgroundImage: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_UpdateBackgroundImage loaded\n");

	//////////////////////////////////////////
	// GBMSAPI_GetFrameStatistic
	//////////////////////////////////////////
	Ptr_GBMSAPI_GetFrameStatistic = (int (*)(unsigned long *, unsigned long *))
			GetProcAddress(GBMSAPIDllPtr, "GBMSAPI_GetFrameStatistic");
	if (Ptr_GBMSAPI_GetFrameStatistic == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_GetFrameStatistic: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_GetFrameStatistic loaded\n");

	//////////////////////////////////////////
	// GBMSAPI_ROLL_GetCompositeImageInfo2
	//////////////////////////////////////////
	Ptr_GBMSAPI_ROLL_GetCompositeImageInfo2 = (int (*)(unsigned int *, unsigned char **,
	                                                   unsigned char **, unsigned int *,
	                                                   unsigned char *, unsigned int *,
	                                                   unsigned int *, unsigned int *))
			GetProcAddress(GBMSAPIDllPtr, "GBMSAPI_ROLL_GetCompositeImageInfo2");
	if (Ptr_GBMSAPI_ROLL_GetCompositeImageInfo2 == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_ROLL_GetCompositeImageInfo2: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_ROLL_GetCompositeImageInfo2 loaded\n");

	//////////////////////////////////
	// GBMSAPI_SetCalibrationImage2
	//////////////////////////////////
	Ptr_GBMSAPI_SetCalibrationImage2 = (int (*)(unsigned int, unsigned char *)) GetProcAddress(
			GBMSAPIDllPtr,
			"GBMSAPI_SetCalibrationImage2");
	if (Ptr_GBMSAPI_SetCalibrationImage2 == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_SetCalibrationImage2: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_SetCalibrationImage2 loaded\n");

	//////////////////////////////////////////
	// GBMSAPI_GetCalibrationImage2
	//////////////////////////////////////////
	Ptr_GBMSAPI_GetCalibrationImage2 = (int (*)(unsigned int, unsigned char *, unsigned int *))
			GetProcAddress(GBMSAPIDllPtr, "GBMSAPI_UpdateBackgroundImage");
	if (Ptr_GBMSAPI_GetCalibrationImage2 == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load GBMSAPI_GetCalibrationImage2: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_GetCalibrationImage2 loaded\n");

	//////////////////////////////////////////
	// GBMSAPI_GetMultiScanAPIVersion
	//////////////////////////////////////////
	Ptr_GBMSAPI_GetMultiScanAPIVersion = (int (*)(
			unsigned char *, unsigned char *, unsigned char *, unsigned char *))
			GetProcAddress(GBMSAPIDllPtr, "GBMSAPI_GetMultiScanAPIVersion");
	if (Ptr_GBMSAPI_GetMultiScanAPIVersion == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_GetMultiScanAPIVersion: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_GetMultiScanAPIVersion loaded\n");

	//////////////////////////////////////////
	// GBMSAPI_GetUnderlyingLibraryDllVersion
	//////////////////////////////////////////
	Ptr_GBMSAPI_GetUnderlyingLibraryDllVersion = (int (*)(
			unsigned char *, unsigned char *, unsigned char *, unsigned char *,
			char *))
			GetProcAddress(GBMSAPIDllPtr, "GBMSAPI_GetUnderlyingLibraryDllVersion");
	if (Ptr_GBMSAPI_GetUnderlyingLibraryDllVersion == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_GetUnderlyingLibraryDllVersion: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_GetUnderlyingLibraryDllVersion loaded\n");

	//////////////////////////////////////////
	// GBMSAPI_GetUSBErrorCode
	//////////////////////////////////////////
	Ptr_GBMSAPI_GetUSBErrorCode = (int (*)(unsigned long *))
			GetProcAddress(GBMSAPIDllPtr, "GBMSAPI_GetUSBErrorCode");
	if (Ptr_GBMSAPI_GetUSBErrorCode == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_GetUSBErrorCode: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_GetUSBErrorCode loaded\n");

	//////////////////////////////////////////
	// GBMSAPI_GetScannerStatistics
	//////////////////////////////////////////
	Ptr_GBMSAPI_GetScannerStatistics = (int (*)(unsigned int *, unsigned int *))
			GetProcAddress(GBMSAPIDllPtr, "GBMSAPI_GetScannerStatistics");
	if (Ptr_GBMSAPI_GetScannerStatistics == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_GetScannerStatistics: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_GetScannerStatistics loaded\n");

	//////////////////////////////
	// GBMSAPI_Sound
	//////////////////////////////
	Ptr_GBMSAPI_Sound = (int (*)(unsigned char, unsigned char, unsigned char))
			GetProcAddress(GBMSAPIDllPtr, "GBMSAPI_Sound");
	if (Ptr_GBMSAPI_Sound == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_Sound: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_Sound loaded\n");

	//////////////////////////////////////////
	// GBMSAPI_GetUserDataSize
	//////////////////////////////////////////
	Ptr_GBMSAPI_GetUserDataSize = (int (*)(int *))
			GetProcAddress(GBMSAPIDllPtr, "GBMSAPI_GetUserDataSize");
	if (Ptr_GBMSAPI_GetUserDataSize == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_GetUserDataSize: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_GetUserDataSize loaded\n");

	//////////////////////////////////////////
	// GBMSAPI_ReadUserData
	//////////////////////////////////////////
	Ptr_GBMSAPI_ReadUserData = (int (*)(unsigned int, unsigned int, void *))
			GetProcAddress(GBMSAPIDllPtr, "GBMSAPI_ReadUserData");
	if (Ptr_GBMSAPI_ReadUserData == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_ReadUserData: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_ReadUserData loaded\n");

	//////////////////////////////////////////
	// GBMSAPI_WriteUserData
	//////////////////////////////////////////
	Ptr_GBMSAPI_WriteUserData = (int (*)(unsigned int, unsigned int, void *))
			GetProcAddress(GBMSAPIDllPtr, "GBMSAPI_WriteUserData");
	if (Ptr_GBMSAPI_WriteUserData == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_WriteUserData: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_WriteUserData loaded\n");

	//////////////////////////////
	// GBMSAPI_SetFingerIndicatorVUIState
	//////////////////////////////
	Ptr_GBMSAPI_SetFingerIndicatorVUIState = (int (*)(unsigned int, int, BOOL))
			GetProcAddress(GBMSAPIDllPtr, "GBMSAPI_SetFingerIndicatorVUIState");
	if (Ptr_GBMSAPI_Sound == NULL)
	{
		ValToRet = GBMSAPI_ERROR_CODE_GENERIC;
		sprintf(GBMSAPI_JNI_LastErrorString,
		        "GBMSAPIJavaWrapper_Load: Unable to load Ptr_GBMSAPI_SetFingerIndicatorVUIState: %s",
		        dlerror());
		a_printf ("%s\n", GBMSAPI_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSAPIJavaWrapper_Load: Ptr_GBMSAPI_SetFingerIndicatorVUIState loaded\n");

	//////////////////////////////
	// Load jvm and RETURN
	//////////////////////////////
	(env)->GetJavaVM(&GBMSAPIWrapperJvm);
	return ValToRet;
}

} // extern "C"
