//-------------------------------------------------------------//
//-------------------------------------------------------------//
// INCLUDE
//-------------------------------------------------------------//
//-------------------------------------------------------------//
#include <jni.h>
#include <string>
#include <dlfcn.h>
#include "GB_SystemConfiguration.h"
#include "GBFINIMG.h"
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
#define a_printf(...) __android_log_print(ANDROID_LOG_DEBUG, "GBFINIMG_LOG", __VA_ARGS__);

//-------------------------------------------------------------//
//-------------------------------------------------------------//
// GLOBALS
//-------------------------------------------------------------//
//-------------------------------------------------------------//
HMODULE GbfinimgDllPtr = NULL;
char Gbfinimg_JNI_LastErrorString[1000];

unsigned char *GBFINIMG_MemoryBuffer = NULL;

void (__stdcall *Ptr_GBFINIMG_Slap4_MissingFingers)(
	unsigned char MissingFingerMask);

int (__stdcall *Ptr_GBFINIMG_GetFingerprintCounter)();

void (__stdcall *Ptr_GBFINIMG_CheckFingerRotation180Degree)(
	int *Probability,
	unsigned char *PatternDiagnostic);

void (__stdcall *Ptr_GBFINIMG_GetLicenseInfo)(
	GBFINIMG_LICENSE_USB_SCANNER_CHECK *ScannerBasedLicense);

int (__stdcall *Ptr_GBFINIMG_GetMissingDeltaNumber)(
	int SegmentIndex,
	int *MissingDeltaNumber,
	unsigned int *MissingDeltaMask);

int (__stdcall *Ptr_GBFINIMG_GetRolledFingerprintTopCornerFillingPercent)(
	int *LeftTopCornerFillingPercent,
	int *RightTopCornerFillingPercent);

int (__stdcall *Ptr_GBFINIMG_GetFullFingerMedialProximalJointBoundingBox)(
	int FingerIndex,
	int *MedialJoint_BoundingBoxL,
	int *MedialJoint_BoundingBoxR,
	int *MedialJoint_BoundingBoxT,
	int *MedialJoint_BoundingBoxB,
	int *MedialJoint_Completeness,
	int *ProximalJoint_BoundingBoxL,
	int *ProximalJoint_BoundingBoxR,
	int *ProximalJoint_BoundingBoxT,
	int *ProximalJoint_BoundingBoxB,
	int *ProximalJoint_Completeness);

void (__stdcall *Ptr_GBFINIMG_GetPalmPrintInterdigitalAreaBoundingBox)(
	int *BoundingBoxL,
	int *BoundingBoxR,
	int *BoundingBoxT,
	int *BoundingBoxB,
	int *Completeness,
	int *Quality);

void (__stdcall *Ptr_GBFINIMG_GetUpperPalmFingerPhalangesBoundingBox)(
	int *BoundingBoxL,
	int *BoundingBoxR,
	int *BoundingBoxT,
	int *BoundingBoxB,
	int *Completeness,
	int *Quality);

int (__stdcall *Ptr_GBFINIMG_SetJavaFD)(
	int *javafd,
	int device_count);

int (__stdcall *Ptr_GBFINIMG_ProcessImage)(
	unsigned char *InputImage,
	unsigned int InputSizeX,
	unsigned int InputSizeY,
	unsigned int InputImageType,

	unsigned int ImageProcessingOption,

	GBFINIMG_SEGMENT_IMAGE_DESCRIPTOR *SegmentImageList,
	unsigned int SegmentImageSizeX,
	unsigned int SegmentImageSizeY,

	unsigned int *DetectedSegmentImageNumber,
	unsigned int *DetectionFailureDiagnostic,

	GBFINIMG_SEGMENT_IMAGE_EXTERNAL_INFO *ExternalInfoList,
	unsigned int ExternalInfoListSize,

	unsigned char *TempBuffer,
	unsigned int TempBufferSize);

//-------------------------------------------------------------//
//-------------------------------------------------------------//
// FUNCTIONS
//-------------------------------------------------------------//
//-------------------------------------------------------------//
extern "C"
{
JNIEXPORT jstring JNICALL Java_com_greenbit_gbfinimg_GbfinimgJavaWrapperLibrary_GetLastErrorString(
	JNIEnv *env,
	jobject /* this */)
{
	return env->NewStringUTF(Gbfinimg_JNI_LastErrorString);
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbfinimg_GbfinimgJavaWrapperLibrary_Unload(
	JNIEnv *env,
	jobject /* this */)
{
	int ValToRet = GBFINIMG_NO_ERROR;

	if (GbfinimgDllPtr != NULL)
	{
		FreeLibrary(GbfinimgDllPtr);
	}
	else
	{
		sprintf(Gbfinimg_JNI_LastErrorString,
				"Unload: GbfinimgDllPtr is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
	}

	Ptr_GBFINIMG_Slap4_MissingFingers = NULL;
	Ptr_GBFINIMG_GetFingerprintCounter = NULL;
	Ptr_GBFINIMG_CheckFingerRotation180Degree = NULL;
	Ptr_GBFINIMG_GetLicenseInfo = NULL;
	Ptr_GBFINIMG_GetMissingDeltaNumber = NULL;
	Ptr_GBFINIMG_GetRolledFingerprintTopCornerFillingPercent = NULL;
	Ptr_GBFINIMG_GetFullFingerMedialProximalJointBoundingBox = NULL;
	Ptr_GBFINIMG_GetPalmPrintInterdigitalAreaBoundingBox = NULL;
	Ptr_GBFINIMG_GetUpperPalmFingerPhalangesBoundingBox = NULL;
	Ptr_GBFINIMG_SetJavaFD = NULL;
	Ptr_GBFINIMG_ProcessImage = NULL;

	GbfinimgDllPtr = NULL;

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbfinimg_GbfinimgJavaWrapperLibrary_Load(
	JNIEnv *env,
	jobject /* this */)
{
	int ValToRet = GBFINIMG_NO_ERROR;

	Dl_info pathInfo;
	dladdr((void *) Java_com_greenbit_gbfinimg_GbfinimgJavaWrapperLibrary_Load, &pathInfo);
	a_printf("Load: pathName = %s", pathInfo.dli_fname);

	//////////////////////////////
	// free library
	//////////////////////////////
	if (GbfinimgDllPtr != NULL)
		Java_com_greenbit_gbfinimg_GbfinimgJavaWrapperLibrary_Unload(env, nullptr);

	//////////////////////////////
	// load library
	//////////////////////////////
	GbfinimgDllPtr = LoadLibrary("libGBFINIMG.so");
	if (GbfinimgDllPtr == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"Load: Unable to load GBFINIMG: %s", dlerror());
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: GBFINIMG loaded\n");

	//////////////////////////////
	// GBFINIMG_Slap4_MissingFingers
	//////////////////////////////
	Ptr_GBFINIMG_Slap4_MissingFingers = (void (*)(
		unsigned char MissingFingerMask
	)) GetProcAddress(GbfinimgDllPtr, "GBFINIMG_Slap4_MissingFingers");
	if (Ptr_GBFINIMG_Slap4_MissingFingers == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"Load: Unable to load Ptr_GBFINIMG_Slap4_MissingFingers: %s", dlerror());
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GBFINIMG_Slap4_MissingFingers loaded\n");

	//////////////////////////////
	// GBFINIMG_GetFingerprintCounter
	//////////////////////////////
	Ptr_GBFINIMG_GetFingerprintCounter = (int (*)(
	)) GetProcAddress(GbfinimgDllPtr, "GBFINIMG_GetFingerprintCounter");
	if (Ptr_GBFINIMG_GetFingerprintCounter == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"Load: Unable to load Ptr_GBFINIMG_GetFingerprintCounter: %s", dlerror());
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GBFINIMG_GetFingerprintCounter loaded\n");

	//////////////////////////////
	// GBFINIMG_CheckFingerRotation180Degree
	//////////////////////////////
	Ptr_GBFINIMG_CheckFingerRotation180Degree = (void (*)(
		int *Probability,
		unsigned char *PatternDiagnostic
	)) GetProcAddress(GbfinimgDllPtr, "GBFINIMG_CheckFingerRotation180Degree");
	if (Ptr_GBFINIMG_CheckFingerRotation180Degree == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"Load: Unable to load Ptr_GBFINIMG_CheckFingerRotation180Degree: %s", dlerror());
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GBFINIMG_CheckFingerRotation180Degree loaded\n");

	//////////////////////////////
	// GBFINIMG_GetLicenseInfo
	//////////////////////////////
	Ptr_GBFINIMG_GetLicenseInfo = (void (*)(
		GBFINIMG_LICENSE_USB_SCANNER_CHECK *ScannerBasedLicense
	)) GetProcAddress(GbfinimgDllPtr, "GBFINIMG_GetLicenseInfo");
	if (Ptr_GBFINIMG_GetLicenseInfo == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"Load: Unable to load Ptr_GBFINIMG_GetLicenseInfo: %s", dlerror());
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GBFINIMG_GetLicenseInfo loaded\n");

	//////////////////////////////
	// GBFINIMG_GetMissingDeltaNumber
	//////////////////////////////
	Ptr_GBFINIMG_GetMissingDeltaNumber = (int (*)(
		int SegmentIndex,
		int *MissingDeltaNumber,
		unsigned int *MissingDeltaMask
	)) GetProcAddress(GbfinimgDllPtr, "GBFINIMG_GetMissingDeltaNumber");
	if (Ptr_GBFINIMG_GetMissingDeltaNumber == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"Load: Unable to load Ptr_GBFINIMG_GetMissingDeltaNumber: %s", dlerror());
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GBFINIMG_GetMissingDeltaNumber loaded\n");

	//////////////////////////////
	// GBFINIMG_GetRolledFingerprintTopCornerFillingPercent
	//////////////////////////////
	Ptr_GBFINIMG_GetRolledFingerprintTopCornerFillingPercent = (int (*)(
		int *LeftTopCornerFillingPercent,
		int *RightTopCornerFillingPercent
	)) GetProcAddress(GbfinimgDllPtr, "GBFINIMG_GetRolledFingerprintTopCornerFillingPercent");
	if (Ptr_GBFINIMG_GetRolledFingerprintTopCornerFillingPercent == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"Load: Unable to load Ptr_GBFINIMG_GetRolledFingerprintTopCornerFillingPercent: %s",
				dlerror());
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GBFINIMG_GetRolledFingerprintTopCornerFillingPercent loaded\n");

	//////////////////////////////
	// GBFINIMG_GetFullFingerMedialProximalJointBoundingBox
	//////////////////////////////
	Ptr_GBFINIMG_GetFullFingerMedialProximalJointBoundingBox = (int (*)(
		int FingerIndex,
		int *MedialJoint_BoundingBoxL,
		int *MedialJoint_BoundingBoxR,
		int *MedialJoint_BoundingBoxT,
		int *MedialJoint_BoundingBoxB,
		int *MedialJoint_Completeness,
		int *ProximalJoint_BoundingBoxL,
		int *ProximalJoint_BoundingBoxR,
		int *ProximalJoint_BoundingBoxT,
		int *ProximalJoint_BoundingBoxB,
		int *ProximalJoint_Completeness
	)) GetProcAddress(GbfinimgDllPtr, "GBFINIMG_GetFullFingerMedialProximalJointBoundingBox");
	if (Ptr_GBFINIMG_GetFullFingerMedialProximalJointBoundingBox == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"Load: Unable to load Ptr_GBFINIMG_GetFullFingerMedialProximalJointBoundingBox: %s",
				dlerror());
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GBFINIMG_GetFullFingerMedialProximalJointBoundingBox loaded\n");

	//////////////////////////////
	// GBFINIMG_GetPalmPrintInterdigitalAreaBoundingBox
	//////////////////////////////
	Ptr_GBFINIMG_GetPalmPrintInterdigitalAreaBoundingBox = (void (*)(
		int *BoundingBoxL,
		int *BoundingBoxR,
		int *BoundingBoxT,
		int *BoundingBoxB,
		int *Completeness,
		int *Quality
	)) GetProcAddress(GbfinimgDllPtr, "GBFINIMG_GetPalmPrintInterdigitalAreaBoundingBox");
	if (Ptr_GBFINIMG_GetPalmPrintInterdigitalAreaBoundingBox == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"Load: Unable to load Ptr_GBFINIMG_GetPalmPrintInterdigitalAreaBoundingBox: %s",
				dlerror());
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GBFINIMG_GetPalmPrintInterdigitalAreaBoundingBox loaded\n");

	//////////////////////////////
	// GBFINIMG_GetUpperPalmFingerPhalangesBoundingBox
	//////////////////////////////
	Ptr_GBFINIMG_GetUpperPalmFingerPhalangesBoundingBox = (void (*)(
		int *BoundingBoxL,
		int *BoundingBoxR,
		int *BoundingBoxT,
		int *BoundingBoxB,
		int *Completeness,
		int *Quality
	)) GetProcAddress(GbfinimgDllPtr, "GBFINIMG_GetUpperPalmFingerPhalangesBoundingBox");
	if (Ptr_GBFINIMG_GetUpperPalmFingerPhalangesBoundingBox == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"Load: Unable to load Ptr_GBFINIMG_GetUpperPalmFingerPhalangesBoundingBox: %s",
				dlerror());
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GBFINIMG_GetUpperPalmFingerPhalangesBoundingBox loaded\n");

	//////////////////////////////
	// GBFINIMG_SetJavaFD
	//////////////////////////////
	Ptr_GBFINIMG_SetJavaFD = (int (*)(
		int *javafd, int javafdcount
	)) GetProcAddress(GbfinimgDllPtr, "GBFINIMG_SetJavaFD");
	if (Ptr_GBFINIMG_SetJavaFD == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"Load: Unable to load Ptr_GBFINIMG_SetJavaFD: %s", dlerror());
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GBFINIMG_SetJavaFD loaded\n");

	//////////////////////////////
	// GBFINIMG_ProcessImage
	//////////////////////////////
	Ptr_GBFINIMG_ProcessImage = (int (*)(
		unsigned char *InputImage,
		unsigned int InputSizeX,
		unsigned int InputSizeY,
		unsigned int InputImageType,
		unsigned int ImageProcessingOption,
		GBFINIMG_SEGMENT_IMAGE_DESCRIPTOR *SegmentImageList,
		unsigned int SegmentImageSizeX,
		unsigned int SegmentImageSizeY,
		unsigned int *DetectedSegmentImageNumber,
		unsigned int *DetectionFailureDiagnostic,
		GBFINIMG_SEGMENT_IMAGE_EXTERNAL_INFO *ExternalInfoList,
		unsigned int ExternalInfoListSize,
		unsigned char *TempBuffer,
		unsigned int TempBufferSize
	)) GetProcAddress(GbfinimgDllPtr, "GBFINIMG_ProcessImage");
	if (Ptr_GBFINIMG_ProcessImage == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"Load: Unable to load Ptr_GBFINIMG_ProcessImage: %s", dlerror());
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_GBFINIMG_ProcessImage loaded\n");

	return ValToRet;
}

bool GBFINIMG_JNI_CheckSegmentDescriptorSizeVsImageType(int SegmSize, unsigned int ImageType)
{
	if (ImageType == GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_4 ||
		ImageType == GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_4 ||
		ImageType == GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_HALF_PALM_UPPER ||
		ImageType == GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_HALF_PALM_UPPER)
	{
		return (SegmSize >= 4);
	}
	if (ImageType == GBFINIMG_INPUT_IMAGE_TYPE_THUMBS_2 ||
		ImageType == GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_2 ||
		ImageType == GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_2)
	{
		return (SegmSize >= 2);
	}
	if (ImageType == GBFINIMG_INPUT_IMAGE_TYPE_SINGLE_FINGER ||
		ImageType == GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_HALF_PALM ||
		ImageType == GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_HALF_PALM ||
		ImageType == GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_WRITER_PALM ||
		ImageType == GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_WRITER_PALM ||
		ImageType == GBFINIMG_INPUT_IMAGE_TYPE_SINGLE_FINGER_FLAT ||
		ImageType == GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_ROLLED_THENAR ||
		ImageType == GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_ROLLED_THENAR ||
		ImageType == GBFINIMG_INPUT_IMAGE_TYPE_ROLLED_JOINT_FINGER_FV1 ||
		ImageType == GBFINIMG_INPUT_IMAGE_TYPE_FLAT_JOINT_FINGER_FV2 ||
		ImageType == GBFINIMG_INPUT_IMAGE_TYPE_FLAT_JOINT_FINGER_FV3 ||
		ImageType == GBFINIMG_INPUT_IMAGE_TYPE_FLAT_JOINT_FINGER_FV4 ||
		ImageType == GBFINIMG_INPUT_IMAGE_TYPE_ROLLED_TIP)
	{
		return (SegmSize >= 1);
	}
	return false;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbfinimg_GbfinimgJavaWrapperLibrary_ProcessImage(
	JNIEnv *env,
	jobject thisObj,
	// INPUT
	jbyteArray jInputImage,
	jint jInputSizeX,
	jint jInputSizeY,
	jint jInputImageType,
	jint jImageProcessingOption,
	jint jSegmentImageSizeX,
	jint jSegmentImageSizeY,
	jobjectArray jExternalInfoList,
	jint jExternalInfoListSize,
	// OUTPUT
	jobjectArray jSegmentImageList,
	jobject jDetectedSegmentImageNumber,
	jobject jDetectionFailureDiagnostic
)
{
	int ValToRet = GBFINIMG_NO_ERROR;

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	/********************************
	 * Input size
	 *******************************/
	if (jInputSizeX <= 0)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"ProcessImage: jInputSizeX <= 0");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jInputSizeX > GBFINIMG_MAX_INPUT_SIZE_X)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"ProcessImage: jInputSizeX > GBFINIMG_MAX_INPUT_SIZE_X");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jInputSizeY > GBFINIMG_MAX_INPUT_SIZE_Y)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"ProcessImage: jInputSizeY > GBFINIMG_MAX_INPUT_SIZE_Y");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jInputSizeY <= 0)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"ProcessImage: jInputSizeY <= 0");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jInputImage == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"ProcessImage: jInputImage is null");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	if ((env)->GetArrayLength(jInputImage) < (jInputSizeX * jInputSizeY))
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"ProcessImage: jInputImage too small");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	/********************************
	 * Segment size
	 *******************************/
	if (jSegmentImageSizeX <= 0)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"ProcessImage: jSegmentImageSizeX <= 0");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jSegmentImageSizeX > jInputSizeX)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"ProcessImage: jSegmentImageSizeX > jInputSizeX");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jSegmentImageSizeY <= 0)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"ProcessImage: jSegmentImageSizeY <= 0");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jSegmentImageSizeY > jInputSizeY)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"ProcessImage: jSegmentImageSizeY > jInputSizeY");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	/********************************
	 * Segment Number and image type
	 *******************************/
	if (jSegmentImageList == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"ProcessImage: jSegmentImageList is null");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	if (!GBFINIMG_JNI_CheckSegmentDescriptorSizeVsImageType(
		(env)->GetArrayLength(jSegmentImageList),
		jInputImageType
	))
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"ProcessImage: Segments array size does not match image type");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	/********************************
	 * Output integers
	 *******************************/
	if (jDetectedSegmentImageNumber == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"ProcessImage: jDetectedSegmentImageNumber is null");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jDetectionFailureDiagnostic == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"ProcessImage: jDetectionFailureDiagnostic is null");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	/**********************************
	 * External info list
	 */
	bool EvaluateExternalInfo = false;
	if (jExternalInfoList != NULL)
	{
		if ((env)->GetArrayLength(jExternalInfoList) < jExternalInfoListSize)
		{
			ValToRet = GBFINIMG_ERROR_PARAMETER;
			sprintf(Gbfinimg_JNI_LastErrorString,
					"ProcessImage: jExternalInfoList size < jExternalInfoListSize");
			a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
			return ValToRet;
		}
		else
			EvaluateExternalInfo = true;
	}

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	/**********************************
	 * External info list
	 */
	GBFINIMG_SEGMENT_IMAGE_EXTERNAL_INFO *ExternalInfoList = NULL;
	if (EvaluateExternalInfo)
	{
		ExternalInfoList = new GBFINIMG_SEGMENT_IMAGE_EXTERNAL_INFO[jExternalInfoListSize];
		// copy
		for (int i = 0; i < jExternalInfoListSize; i++)
		{
			jobject obj = (jobject) env->GetObjectArrayElement(jExternalInfoList, i);
			jclass cls = env->GetObjectClass(obj);
			jfieldID fingerCenterXFieldId = env->GetFieldID(cls, "FingerCenterX", "I");
			jfieldID fingerCenterYFieldId = env->GetFieldID(cls, "FingerCenterY", "I");
			jfieldID handFieldId = env->GetFieldID(cls, "Hand", "I");
			jfieldID fingerFieldId = env->GetFieldID(cls, "Finger", "I");
			jint jFingerCenterX = env->GetIntField(obj, fingerCenterXFieldId);
			jint jFingerCenterY = env->GetIntField(obj, fingerCenterYFieldId);
			jint jHand = env->GetIntField(obj, handFieldId);
			jint jFinger = env->GetIntField(obj, fingerFieldId);
			ExternalInfoList[i].Finger = jFinger;
			ExternalInfoList[i].FingerCenterX = jFingerCenterX;
			ExternalInfoList[i].FingerCenterY = jFingerCenterY;
			ExternalInfoList[i].Hand = jHand;
		}
	}
	/*******************************
	 * Segment list size
	 */
	int SegmentListSize = (env)->GetArrayLength(jSegmentImageList);
	/*******************************************
	 * INPUT IMAGE
	 */
	unsigned char *InputImage = new unsigned char[jInputSizeX * jInputSizeY];
	env->GetByteArrayRegion(jInputImage, 0, jInputSizeX * jInputSizeY,
							reinterpret_cast<jbyte *>(InputImage));

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (GbfinimgDllPtr == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"ProcessImage: GbfinimgDllPtr is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBFINIMG_ProcessImage == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"ProcessImage: Ptr_GBFINIMG_ProcessImage is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// call function
	//////////////////////////////////
	GBFINIMG_SEGMENT_IMAGE_DESCRIPTOR *SegmentImageList = new GBFINIMG_SEGMENT_IMAGE_DESCRIPTOR[SegmentListSize];
	for (int i = 0; i < SegmentListSize; i++)
	{
		SegmentImageList[i].SegmentImagePtr = new unsigned char[jSegmentImageSizeX * jSegmentImageSizeY];
	}
	unsigned int DetectedSegmentImageNumber, DetectionFailureDiagnostic;
	unsigned char *TempBuffer = new unsigned char[GBFINIMG_MEMORY_BUFFER_SIZE];

	ValToRet = Ptr_GBFINIMG_ProcessImage(
		InputImage, jInputSizeX, jInputSizeY,
		jInputImageType, jImageProcessingOption,
		SegmentImageList, jSegmentImageSizeX, jSegmentImageSizeY,
		&DetectedSegmentImageNumber, &DetectionFailureDiagnostic,
		ExternalInfoList, jExternalInfoListSize,
		TempBuffer, GBFINIMG_MEMORY_BUFFER_SIZE);

	if (ValToRet == GBFINIMG_NO_ERROR)
	{
		// copy values
		/*******************************************
		 * SEGMENTS
		 ******************************************/
		for (int i = 0; i < SegmentListSize; i++)
		{
			/**********************
			 * get object and
			 * class
			 */
			jobject obj = (jobject) env->GetObjectArrayElement(jSegmentImageList, i);
			jclass cls = env->GetObjectClass(obj);
			/***********************
			 * get segment buffer
			 * field
			 */
			jfieldID segmBuffFieldId = env->GetFieldID(cls, "SegmentImage", "[B");
			jbyteArray segmBuffArray = (jbyteArray)env->GetObjectField (obj, segmBuffFieldId);
			env->SetByteArrayRegion(segmBuffArray,
									0, jSegmentImageSizeX * jSegmentImageSizeY,
									reinterpret_cast<jbyte *>(SegmentImageList[i].SegmentImagePtr));
			/*****************************
			 * finger center
			 */
			jfieldID fingerCenterXFieldId = env->GetFieldID(cls, "FingerCenterX", "I");
			jfieldID fingerCenterYFieldId = env->GetFieldID(cls, "FingerCenterY", "I");
			env->SetIntField(obj, fingerCenterXFieldId, SegmentImageList[i].FingerCenterX);
			env->SetIntField(obj, fingerCenterYFieldId, SegmentImageList[i].FingerCenterY);
			/*****************************
			 * finger and hand id
			 */
			jfieldID fingerNameFieldId = env->GetFieldID(cls, "Finger", "I");
			jfieldID handNameFieldId = env->GetFieldID(cls, "Hand", "I");
			env->SetIntField(obj, fingerNameFieldId, (int)SegmentImageList[i].Finger);
			env->SetIntField(obj, handNameFieldId, (int)SegmentImageList[i].Hand);
			/*****************************
			 * Quality
			 */
			jfieldID qualityFieldId = env->GetFieldID(cls, "Quality", "I");
			env->SetIntField(obj, qualityFieldId, SegmentImageList[i].Quality);
			/*****************************
			 * Bounding boxes
			 */
			jfieldID boundingBoxLFieldId = env->GetFieldID(cls, "BoundingBoxL", "I");
			jfieldID boundingBoxRFieldId = env->GetFieldID(cls, "BoundingBoxR", "I");
			jfieldID boundingBoxTFieldId = env->GetFieldID(cls, "BoundingBoxT", "I");
			jfieldID boundingBoxBFieldId = env->GetFieldID(cls, "BoundingBoxB", "I");
			env->SetIntField(obj, boundingBoxLFieldId, SegmentImageList[i].BoundingBoxL);
			env->SetIntField(obj, boundingBoxRFieldId, SegmentImageList[i].BoundingBoxR);
			env->SetIntField(obj, boundingBoxTFieldId, SegmentImageList[i].BoundingBoxT);
			env->SetIntField(obj, boundingBoxBFieldId, SegmentImageList[i].BoundingBoxB);
			/*****************************
			 * pattern parameters
			 */
			jfieldID patternValidityFieldId = env->GetFieldID(cls, "PatternValidity", "I");
			jfieldID patternCompletenessFieldId = env->GetFieldID(cls, "PatternCompleteness", "I");
			env->SetIntField(obj, patternValidityFieldId, SegmentImageList[i].PatternValidity);
			env->SetIntField(obj, patternCompletenessFieldId, SegmentImageList[i].PatternCompleteness);
			/*****************************
			 * size and inclination
			 */
			jfieldID sizeFieldId = env->GetFieldID(cls, "Size", "I");
			jfieldID inclinationFieldId = env->GetFieldID(cls, "Inclination", "I");
			env->SetIntField(obj, sizeFieldId, SegmentImageList[i].Size);
			env->SetIntField(obj, inclinationFieldId, SegmentImageList[i].Inclination);
		}
		GBANUTIL_GetJIntExchangeFromCInt(DetectedSegmentImageNumber,jDetectedSegmentImageNumber,env);
		GBANUTIL_GetJIntExchangeFromCInt(DetectionFailureDiagnostic,jDetectionFailureDiagnostic,env);
	}
	else
	{
		sprintf(Gbfinimg_JNI_LastErrorString,
				"ProcessImage: Ptr_GBFINIMG_ProcessImage returned %d", ValToRet);
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbfinimg_GbfinimgJavaWrapperLibrary_SetOpenedJavaFD(
	JNIEnv *env,
	jobject thisObj,
	// INPUT
	jintArray openedJavaFd,
	jint numberOfOpenedJavaFD
)
{
	int ValToRet = GBFINIMG_NO_ERROR;

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (numberOfOpenedJavaFD < 0)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"SetOpenedJavaFD: numberOfOpenedJavaFD less than 0");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	else if (numberOfOpenedJavaFD == 0)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"SetOpenedJavaFD: numberOfOpenedJavaFD = 0, no operations to perform");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	if (openedJavaFd == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"SetOpenedJavaFD: openedJavaFd is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	else if ((env)->GetArrayLength(openedJavaFd) < numberOfOpenedJavaFD)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"SetOpenedJavaFD: openedJavaFd length too small");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
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
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"SetOpenedJavaFD: cannot get elements from openedJavaFd");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	int *cppOpenedJavaFD = new int[numberOfOpenedJavaFD];
	for (int i = 0; i < numberOfOpenedJavaFD; i++)
	{
		cppOpenedJavaFD[i] = jintOpenedJavaFD[i];
	}
	(env)->ReleaseIntArrayElements(openedJavaFd, jintOpenedJavaFD, 0);

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (GbfinimgDllPtr == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"SetOpenedJavaFD: GbfinimgDllPtr is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBFINIMG_SetJavaFD == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"SetOpenedJavaFD: Ptr_GBFINIMG_SetJavaFD is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// call function
	//////////////////////////////////
	ValToRet = Ptr_GBFINIMG_SetJavaFD(
		cppOpenedJavaFD, numberOfOpenedJavaFD);
	//////////////////////////////
	// release cpp arrays
	//////////////////////////////
	delete[] cppOpenedJavaFD;

	//////////////////////////////////
	// convert to Java
	//////////////////////////////////

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_gbfinimg_GbfinimgJavaWrapperLibrary_GetUpperPalmFingerPhalangesBoundingBox(
	JNIEnv *env,
	jobject thisObj,
	// OUTPUT
	jobject jBoundingBoxL,
	jobject jBoundingBoxR,
	jobject jBoundingBoxT,
	jobject jBoundingBoxB,
	jobject jCompleteness,
	jobject jQuality
)
{
	int ValToRet = GBFINIMG_NO_ERROR;

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jBoundingBoxL == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetUpperPalmFingerPhalangesBoundingBox: jBoundingBoxL is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jBoundingBoxR == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetUpperPalmFingerPhalangesBoundingBox: jBoundingBoxR is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jBoundingBoxT == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetUpperPalmFingerPhalangesBoundingBox: jBoundingBoxT is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jBoundingBoxB == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetUpperPalmFingerPhalangesBoundingBox: jBoundingBoxB is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jCompleteness == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetUpperPalmFingerPhalangesBoundingBox: jCompleteness is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jQuality == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetUpperPalmFingerPhalangesBoundingBox: jProximalJoint_BoundingBoxL is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (GbfinimgDllPtr == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetUpperPalmFingerPhalangesBoundingBox: GbfinimgDllPtr is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBFINIMG_GetUpperPalmFingerPhalangesBoundingBox == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetUpperPalmFingerPhalangesBoundingBox: Ptr_GBFINIMG_GetUpperPalmFingerPhalangesBoundingBox is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}

	///////////////////////////////////
	// convert java to C
	///////////////////////////////////

	//////////////////////////////////
	// call function
	//////////////////////////////////
	int BoundingBoxL, BoundingBoxR, BoundingBoxT, BoundingBoxB, Completeness, Quality;
	Ptr_GBFINIMG_GetUpperPalmFingerPhalangesBoundingBox(
		&BoundingBoxL, &BoundingBoxR, &BoundingBoxT,
		&BoundingBoxB, &Completeness, &Quality
	);

	//////////////////////////////////
	// convert to Java
	//////////////////////////////////
	GBANUTIL_GetJIntExchangeFromCInt((int) (BoundingBoxL),jBoundingBoxL,env);
	GBANUTIL_GetJIntExchangeFromCInt((int) (BoundingBoxR),jBoundingBoxR,env);
	GBANUTIL_GetJIntExchangeFromCInt((int) (BoundingBoxT),jBoundingBoxT,env);
	GBANUTIL_GetJIntExchangeFromCInt((int) (BoundingBoxB),jBoundingBoxB,env);
	GBANUTIL_GetJIntExchangeFromCInt((int) (Completeness),jCompleteness,env);
	GBANUTIL_GetJIntExchangeFromCInt((int) (Quality),jQuality,env);

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_gbfinimg_GbfinimgJavaWrapperLibrary_GetPalmPrintInterdigitalAreaBoundingBox(
	JNIEnv *env,
	jobject thisObj,
	// OUTPUT
	jobject jBoundingBoxL,
	jobject jBoundingBoxR,
	jobject jBoundingBoxT,
	jobject jBoundingBoxB,
	jobject jCompleteness,
	jobject jQuality
)
{
	int ValToRet = GBFINIMG_NO_ERROR;

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jBoundingBoxL == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetPalmPrintInterdigitalAreaBoundingBox: jBoundingBoxL is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jBoundingBoxR == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetPalmPrintInterdigitalAreaBoundingBox: jBoundingBoxR is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jBoundingBoxT == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetPalmPrintInterdigitalAreaBoundingBox: jBoundingBoxT is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jBoundingBoxB == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetPalmPrintInterdigitalAreaBoundingBox: jBoundingBoxB is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jCompleteness == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetPalmPrintInterdigitalAreaBoundingBox: jCompleteness is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jQuality == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetPalmPrintInterdigitalAreaBoundingBox: jProximalJoint_BoundingBoxL is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (GbfinimgDllPtr == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetPalmPrintInterdigitalAreaBoundingBox: GbfinimgDllPtr is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBFINIMG_GetPalmPrintInterdigitalAreaBoundingBox == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetPalmPrintInterdigitalAreaBoundingBox: Ptr_GBFINIMG_GetPalmPrintInterdigitalAreaBoundingBox is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}

	///////////////////////////////////
	// convert java to C
	///////////////////////////////////

	//////////////////////////////////
	// call function
	//////////////////////////////////
	int BoundingBoxL, BoundingBoxR, BoundingBoxT, BoundingBoxB, Completeness, Quality;
	Ptr_GBFINIMG_GetPalmPrintInterdigitalAreaBoundingBox(
		&BoundingBoxL, &BoundingBoxR, &BoundingBoxT,
		&BoundingBoxB, &Completeness, &Quality
	);

	//////////////////////////////////
	// convert to Java
	//////////////////////////////////
	GBANUTIL_GetJIntExchangeFromCInt((int) (BoundingBoxL),jBoundingBoxL,env);
	GBANUTIL_GetJIntExchangeFromCInt((int) (BoundingBoxR),jBoundingBoxR,env);
	GBANUTIL_GetJIntExchangeFromCInt((int) (BoundingBoxT),jBoundingBoxT,env);
	GBANUTIL_GetJIntExchangeFromCInt((int) (BoundingBoxB),jBoundingBoxB,env);
	GBANUTIL_GetJIntExchangeFromCInt((int) (Completeness),jCompleteness,env);
	GBANUTIL_GetJIntExchangeFromCInt((int) (Quality),jQuality,env);

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_gbfinimg_GbfinimgJavaWrapperLibrary_GetFullFingerMedialProximalJointBoundingBox(
	JNIEnv *env,
	jobject thisObj,
	// INPUT
	jint jSegmentIndex,
	// OUTPUT
	jobject jMedialJoint_BoundingBoxL,
	jobject jMedialJoint_BoundingBoxR,
	jobject jMedialJoint_BoundingBoxT,
	jobject jMedialJoint_BoundingBoxB,
	jobject jMedialJoint_Completeness,
	jobject jProximalJoint_BoundingBoxL,
	jobject jProximalJoint_BoundingBoxR,
	jobject jProximalJoint_BoundingBoxT,
	jobject jProximalJoint_BoundingBoxB,
	jobject jProximalJoint_Completeness
)
{
	int ValToRet = GBFINIMG_NO_ERROR;

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jMedialJoint_BoundingBoxL == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetFullFingerMedialProximalJointBoundingBox: jMedialJoint_BoundingBoxL is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jMedialJoint_BoundingBoxR == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetFullFingerMedialProximalJointBoundingBox: jMedialJoint_BoundingBoxR is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jMedialJoint_BoundingBoxT == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetFullFingerMedialProximalJointBoundingBox: jMedialJoint_BoundingBoxT is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jMedialJoint_BoundingBoxB == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetFullFingerMedialProximalJointBoundingBox: jMedialJoint_BoundingBoxB is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jMedialJoint_Completeness == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetFullFingerMedialProximalJointBoundingBox: jMedialJoint_Completeness is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jProximalJoint_BoundingBoxL == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetFullFingerMedialProximalJointBoundingBox: jProximalJoint_BoundingBoxL is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jProximalJoint_BoundingBoxR == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetFullFingerMedialProximalJointBoundingBox: jProximalJoint_BoundingBoxR is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jProximalJoint_BoundingBoxT == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetFullFingerMedialProximalJointBoundingBox: jProximalJoint_BoundingBoxT is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jProximalJoint_BoundingBoxB == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetFullFingerMedialProximalJointBoundingBox: jProximalJoint_BoundingBoxB is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jProximalJoint_Completeness == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetFullFingerMedialProximalJointBoundingBox: jProximalJoint_Completeness is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (GbfinimgDllPtr == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetFullFingerMedialProximalJointBoundingBox: GbfinimgDllPtr is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBFINIMG_GetFullFingerMedialProximalJointBoundingBox == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetFullFingerMedialProximalJointBoundingBox: Ptr_GBFINIMG_GetFullFingerMedialProximalJointBoundingBox is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}

	///////////////////////////////////
	// convert java to C
	///////////////////////////////////

	//////////////////////////////////
	// call function
	//////////////////////////////////
	int MedialJoint_BoundingBoxL, MedialJoint_BoundingBoxR, MedialJoint_BoundingBoxT, MedialJoint_BoundingBoxB;
	int MedialJoint_Completeness;
	int ProximalJoint_BoundingBoxL, ProximalJoint_BoundingBoxR, ProximalJoint_BoundingBoxT, ProximalJoint_BoundingBoxB;
	int ProximalJoint_Completeness;
	ValToRet = Ptr_GBFINIMG_GetFullFingerMedialProximalJointBoundingBox(
		jSegmentIndex,
		&MedialJoint_BoundingBoxL, &MedialJoint_BoundingBoxR,
		&MedialJoint_BoundingBoxT, &MedialJoint_BoundingBoxB,
		&MedialJoint_Completeness,
		&ProximalJoint_BoundingBoxL, &ProximalJoint_BoundingBoxR,
		&ProximalJoint_BoundingBoxT, &ProximalJoint_BoundingBoxB,
		&ProximalJoint_Completeness
	);

	//////////////////////////////////
	// convert to Java
	//////////////////////////////////
	GBANUTIL_GetJIntExchangeFromCInt((int) (MedialJoint_BoundingBoxL),jMedialJoint_BoundingBoxL,env);
	GBANUTIL_GetJIntExchangeFromCInt((int) (MedialJoint_BoundingBoxR),jMedialJoint_BoundingBoxR,env);
	GBANUTIL_GetJIntExchangeFromCInt((int) (MedialJoint_BoundingBoxT),jMedialJoint_BoundingBoxT,env);
	GBANUTIL_GetJIntExchangeFromCInt((int) (MedialJoint_BoundingBoxB),jMedialJoint_BoundingBoxB,env);
	GBANUTIL_GetJIntExchangeFromCInt((int) (MedialJoint_Completeness),jMedialJoint_Completeness,env);
	GBANUTIL_GetJIntExchangeFromCInt((int) (ProximalJoint_BoundingBoxL),jProximalJoint_BoundingBoxL,env);
	GBANUTIL_GetJIntExchangeFromCInt((int) (ProximalJoint_BoundingBoxR),jProximalJoint_BoundingBoxR,env);
	GBANUTIL_GetJIntExchangeFromCInt((int) (ProximalJoint_BoundingBoxT),jProximalJoint_BoundingBoxT,env);
	GBANUTIL_GetJIntExchangeFromCInt((int) (ProximalJoint_BoundingBoxB),jProximalJoint_BoundingBoxB,env);
	GBANUTIL_GetJIntExchangeFromCInt((int) (ProximalJoint_Completeness),jProximalJoint_Completeness,env);

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_gbfinimg_GbfinimgJavaWrapperLibrary_GetRolledFingerprintTopCornerFillingPercent(
	JNIEnv *env,
	jobject thisObj,
	// OUTPUT
	jobject jLeftTopCornerFillingPercent,
	jobject jRightTopCornerFillingPercent
)
{
	int ValToRet = GBFINIMG_NO_ERROR;

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jLeftTopCornerFillingPercent == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetRolledFingerprintTopCornerFillingPercent: jLeftTopCornerFillingPercent is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jRightTopCornerFillingPercent == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetRolledFingerprintTopCornerFillingPercent: jRightTopCornerFillingPercent is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (GbfinimgDllPtr == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetRolledFingerprintTopCornerFillingPercent: GbfinimgDllPtr is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBFINIMG_GetRolledFingerprintTopCornerFillingPercent == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetRolledFingerprintTopCornerFillingPercent: Ptr_GBFINIMG_GetRolledFingerprintTopCornerFillingPercent is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}

	///////////////////////////////////
	// convert java to C
	///////////////////////////////////

	//////////////////////////////////
	// call function
	//////////////////////////////////
	int LeftTopCornerFillingPercent, RightTopCornerFillingPercent;
	ValToRet = Ptr_GBFINIMG_GetRolledFingerprintTopCornerFillingPercent(
		&LeftTopCornerFillingPercent, &RightTopCornerFillingPercent
	);

	//////////////////////////////////
	// convert to Java
	//////////////////////////////////
	GBANUTIL_GetJIntExchangeFromCInt((int) (LeftTopCornerFillingPercent),jLeftTopCornerFillingPercent,env);
	GBANUTIL_GetJIntExchangeFromCInt((int) (RightTopCornerFillingPercent),jRightTopCornerFillingPercent,env);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbfinimg_GbfinimgJavaWrapperLibrary_GetMissingDeltaNumber(
	JNIEnv *env,
	jobject thisObj,
	// INPUT
	jint jSegmentIndex,
	// OUTPUT
	jobject jMissingDeltaNumber,
	jobject jMissingDeltaMask
)
{
	int ValToRet = GBFINIMG_NO_ERROR;

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jMissingDeltaNumber == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetMissingDeltaNumber: jMissingDeltaNumber is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jMissingDeltaMask == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetMissingDeltaNumber: jMissingDeltaMask is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (GbfinimgDllPtr == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetMissingDeltaNumber: GbfinimgDllPtr is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBFINIMG_GetMissingDeltaNumber == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetMissingDeltaNumber: Ptr_GBFINIMG_GetMissingDeltaNumber is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}

	///////////////////////////////////
	// convert java to C
	///////////////////////////////////

	//////////////////////////////////
	// call function
	//////////////////////////////////
	int MissingDeltaNumber;
	unsigned int MissingDeltaMask;
	ValToRet = Ptr_GBFINIMG_GetMissingDeltaNumber(
		jSegmentIndex,
		&MissingDeltaNumber, &MissingDeltaMask
	);

	//////////////////////////////////
	// convert to Java
	//////////////////////////////////
	GBANUTIL_GetJIntExchangeFromCInt((int) (MissingDeltaNumber),jMissingDeltaNumber,env);
	GBANUTIL_GetJIntExchangeFromCInt((int) (MissingDeltaMask),jMissingDeltaMask,env);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbfinimg_GbfinimgJavaWrapperLibrary_Slap4MissingFingers(
	JNIEnv *env,
	jobject thisObj,
	// INPUT
	jint jMissingFingerMask
)
{
	int ValToRet = GBFINIMG_NO_ERROR;

	//////////////////////////////////
	// check parameters
	//////////////////////////////////

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (GbfinimgDllPtr == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"Slap4MissingFingers: GbfinimgDllPtr is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBFINIMG_Slap4_MissingFingers == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"Slap4MissingFingers: Ptr_GBFINIMG_Slap4_MissingFingers is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// load class for java
	//////////////////////////////////

	///////////////////////////////////
	// convert java to C
	///////////////////////////////////

	//////////////////////////////////
	// call function
	//////////////////////////////////
	Ptr_GBFINIMG_Slap4_MissingFingers(
		(unsigned char) jMissingFingerMask
	);

	//////////////////////////////////
	// convert to Java
	//////////////////////////////////
	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbfinimg_GbfinimgJavaWrapperLibrary_GetLicenseInfo(
	JNIEnv *env,
	jobject thisObj,
	// OUTPUT
	jobject jResult,
	jobject jUsbDriverErrorCodeDetail
)
{
	int ValToRet = GBFINIMG_NO_ERROR;

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jResult == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetLicenseInfo: jResult is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jUsbDriverErrorCodeDetail == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetLicenseInfo: jUsbDriverErrorCodeDetail is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (GbfinimgDllPtr == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetLicenseInfo: GbfinimgDllPtr is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBFINIMG_GetLicenseInfo == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetLicenseInfo: Ptr_GBFINIMG_GetLicenseInfo is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}

	///////////////////////////////////
	// convert java to C
	///////////////////////////////////

	//////////////////////////////////
	// call function
	//////////////////////////////////
	GBFINIMG_LICENSE_USB_SCANNER_CHECK ScannerBasedLicense;
	Ptr_GBFINIMG_GetLicenseInfo(
		&ScannerBasedLicense
	);

	//////////////////////////////////
	// convert to Java
	//////////////////////////////////
	GBANUTIL_GetJIntExchangeFromCInt((int) (ScannerBasedLicense.Result),jResult,env);
	GBANUTIL_GetJIntExchangeFromCInt((int) (ScannerBasedLicense.UsbDriverErrorCodeDetail),jUsbDriverErrorCodeDetail,env);

	return ValToRet;
}

JNIEXPORT jint JNICALL
Java_com_greenbit_gbfinimg_GbfinimgJavaWrapperLibrary_CheckFingerRotation180Degree(
	JNIEnv *env,
	jobject thisObj,
	// OUTPUT
	jobject jProbability,
	jobject jPatternDiagnostic
)
{
	int ValToRet = GBFINIMG_NO_ERROR;

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jProbability == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"CheckFingerRotation180Degree: jProbability is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jPatternDiagnostic == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"CheckFingerRotation180Degree: jPatternDiagnostic is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (GbfinimgDllPtr == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"CheckFingerRotation180Degree: GbfinimgDllPtr is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBFINIMG_CheckFingerRotation180Degree == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"CheckFingerRotation180Degree: Ptr_GBFINIMG_CheckFingerRotation180Degree is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}

	///////////////////////////////////
	// convert java to C
	///////////////////////////////////

	//////////////////////////////////
	// call function
	//////////////////////////////////
	int Probability;
	unsigned char PatternDiagnostic;
	Ptr_GBFINIMG_CheckFingerRotation180Degree(
		&Probability, &PatternDiagnostic
	);

	//////////////////////////////////
	// convert to Java
	//////////////////////////////////
	GBANUTIL_GetJIntExchangeFromCInt((int) (Probability),jProbability,env);
	GBANUTIL_GetJIntExchangeFromCInt((int) (PatternDiagnostic),jPatternDiagnostic,env);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_gbfinimg_GbfinimgJavaWrapperLibrary_GetFingerprintCounter(
	JNIEnv *env,
	jobject thisObj,
	// OUTPUT
	jobject jFingerprintNumber
)
{
	int ValToRet = GBFINIMG_NO_ERROR;

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jFingerprintNumber == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetFingerprintCounter: jFingerprintNumber is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (GbfinimgDllPtr == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetFingerprintCounter: GbfinimgDllPtr is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBFINIMG_GetFingerprintCounter == NULL)
	{
		ValToRet = GBFINIMG_ERROR_PARAMETER;
		sprintf(Gbfinimg_JNI_LastErrorString,
				"GetFingerprintCounter: Ptr_GBFINIMG_GetFingerprintCounter is NULL");
		a_printf ("%s\n", Gbfinimg_JNI_LastErrorString);
		return ValToRet;
	}

	///////////////////////////////////
	// convert java to C
	///////////////////////////////////

	//////////////////////////////////
	// call function
	//////////////////////////////////
	int fingerNum = Ptr_GBFINIMG_GetFingerprintCounter();

	//////////////////////////////////
	// convert to Java
	//////////////////////////////////
	GBANUTIL_GetJIntExchangeFromCInt((int) (fingerNum),jFingerprintNumber,env);

	return ValToRet;
}

}

