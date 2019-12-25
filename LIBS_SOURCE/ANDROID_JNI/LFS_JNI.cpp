//-------------------------------------------------------------//
//-------------------------------------------------------------//
// INCLUDE
//-------------------------------------------------------------//
//-------------------------------------------------------------//
#include <jni.h>
#include <string>
#include <dlfcn.h>
#include "GB_SystemConfiguration.h"
#include "lfs_dll.h"
#include <android/log.h>

//-------------------------------------------------------------//
//-------------------------------------------------------------//
// DEFINE
//-------------------------------------------------------------//
//-------------------------------------------------------------//
#define FreeLibrary(ptr) dlclose(ptr)
#define LoadLibrary(path) dlopen(path,RTLD_LAZY)
#define GetProcAddress(ptr, fname) dlsym(ptr,fname)
#define a_printf(...) __android_log_print(ANDROID_LOG_DEBUG, "LFS_LOG", __VA_ARGS__);

//-------------------------------------------------------------//
//-------------------------------------------------------------//
// GLOBALS
//-------------------------------------------------------------//
//-------------------------------------------------------------//
HMODULE LfsDllPtr = NULL;
char Lfs_JNI_LastErrorString[1000];

int (__stdcall *Ptr_get_minutiae)(
	MINUTIAE **ominutiae,
	unsigned char *idata,
	const int iw,
	const int ih,
	const int id,
	const double ppmm
);

void (__stdcall *Ptr_free_minutiae)(
	MINUTIAE *minutiae
);

//-------------------------------------------------------------//
//-------------------------------------------------------------//
// FUNCTIONS
//-------------------------------------------------------------//
//-------------------------------------------------------------//
#define LFS_SUCCESS	0
#define LFS_ERROR -1
extern "C"
{
JNIEXPORT jstring JNICALL Java_com_greenbit_lfs_LfsJavaWrapperLibrary_GetLastErrorString(
	JNIEnv *env,
	jobject /* this */) {
	return env->NewStringUTF(Lfs_JNI_LastErrorString);
}

JNIEXPORT jint JNICALL Java_com_greenbit_lfs_LfsJavaWrapperLibrary_Unload(
	JNIEnv *env,
	jobject /* this */)
{
	int ValToRet = LFS_SUCCESS;

	if (LfsDllPtr != NULL)
	{
		FreeLibrary(LfsDllPtr);
	}
	else
	{
		sprintf(Lfs_JNI_LastErrorString,
				"Unload: LfsDllPtr is NULL");
		a_printf ("%s\n", Lfs_JNI_LastErrorString);
	}

	Ptr_get_minutiae = NULL;
	Ptr_free_minutiae = NULL;

	LfsDllPtr = NULL;

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_lfs_LfsJavaWrapperLibrary_Load(
	JNIEnv *env,
	jobject /* this */)
{
	int ValToRet = LFS_SUCCESS;

	Dl_info pathInfo;
	dladdr((void *) Java_com_greenbit_lfs_LfsJavaWrapperLibrary_Load, &pathInfo);
	a_printf("Load: pathName = %s", pathInfo.dli_fname);

	//////////////////////////////
	// free library
	//////////////////////////////
	if (LfsDllPtr != NULL)
		Java_com_greenbit_lfs_LfsJavaWrapperLibrary_Unload(env, nullptr);

	//////////////////////////////
	// load library
	//////////////////////////////
	LfsDllPtr = LoadLibrary("libLfs.so");
	if (LfsDllPtr == NULL)
	{
		ValToRet = LFS_ERROR;
		sprintf(Lfs_JNI_LastErrorString,
				"Load: Unable to load LFS: %s", dlerror());
		a_printf ("%s\n", Lfs_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: LFS loaded\n");

	//////////////////////////////
	// get_minutiae
	//////////////////////////////
	Ptr_get_minutiae = (int (*)(
		MINUTIAE **ominutiae,
		unsigned char *idata,
		const int iw,
		const int ih,
		const int id,
		const double ppmm
	)) GetProcAddress(LfsDllPtr, "get_minutiae");
	if (Ptr_get_minutiae == NULL)
	{
		ValToRet = LFS_ERROR;
		sprintf(Lfs_JNI_LastErrorString,
				"Load: Unable to load Ptr_get_minutiae: %s", dlerror());
		a_printf ("%s\n", Lfs_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_get_minutiae loaded\n");

	//////////////////////////////
	// free_minutiae
	//////////////////////////////
	Ptr_free_minutiae = (void (*)(
		MINUTIAE *minutiae
	)) GetProcAddress(LfsDllPtr, "free_minutiae");
	if (Ptr_free_minutiae == NULL)
	{
		ValToRet = LFS_ERROR;
		sprintf(Lfs_JNI_LastErrorString,
				"Load: Unable to load Ptr_free_minutiae: %s", dlerror());
		a_printf ("%s\n", Lfs_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_free_minutiae loaded\n");

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_lfs_LfsJavaWrapperLibrary_GetMinutiae(
	JNIEnv *env,
	jobject thisObj,
	// INPUT
	jbyteArray jImageBuffer,
	jint jSx, jint jSy,
	jint jBitsPerPixel, jdouble jPixelsPerMillimeter,
	// OUTPUT
	jobjectArray jMinutiae,
	jobject jMinutiaeNum
)
{
	int ValToRet = LFS_SUCCESS;

	jclass MinutiaStructJava, IntForExchangeClassJava;
	jmethodID ptrToMinutiaStructSetFieldFunction, ptrToIntForExchangeSetFieldFunction;

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jSx <= 0)
	{
		ValToRet = LFS_ERROR;
		sprintf(Lfs_JNI_LastErrorString,
				"GetMinutiae: jSx must be > 0");
		a_printf ("%s\n", Lfs_JNI_LastErrorString);
		return ValToRet;
	}
	if (jSy <= 0)
	{
		ValToRet = LFS_ERROR;
		sprintf(Lfs_JNI_LastErrorString,
				"GetMinutiae: jSy must be > 0");
		a_printf ("%s\n", Lfs_JNI_LastErrorString);
		return ValToRet;
	}
	if (jImageBuffer == NULL) {
		ValToRet = LFS_ERROR;
		sprintf(Lfs_JNI_LastErrorString,
				"GetMinutiae: jImageBuffer is NULL");
		a_printf ("%s\n", Lfs_JNI_LastErrorString);
		return ValToRet;
	} else if ((env)->GetArrayLength(jImageBuffer) < (jSx * jSy)) {
		ValToRet = LFS_ERROR;
		sprintf(Lfs_JNI_LastErrorString,
				"GetMinutiae: jImageBuffer too small");
		a_printf ("%s\n", Lfs_JNI_LastErrorString);
		return ValToRet;
	}
	if (jMinutiae == NULL) {
		ValToRet = LFS_ERROR;
		sprintf(Lfs_JNI_LastErrorString,
				"GetMinutiae: jMinutiae is NULL");
		a_printf ("%s\n", Lfs_JNI_LastErrorString);
		return ValToRet;
	} else if ((env)->GetArrayLength(jMinutiae) < (1000)) {
		ValToRet = LFS_ERROR;
		sprintf(Lfs_JNI_LastErrorString,
				"GetMinutiae: jMinutiae too small");
		a_printf ("%s\n", Lfs_JNI_LastErrorString);
		return ValToRet;
	}
	if (jMinutiaeNum == NULL) {
		ValToRet = LFS_ERROR;
		sprintf(Lfs_JNI_LastErrorString,
				"GetMinutiae: jMinutiaeNum is NULL");
		a_printf ("%s\n", Lfs_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// load java class
	//////////////////////////////
	MinutiaStructJava = env->FindClass(
		"com/greenbit/lfs/LfsJavaWrapperDefinesMinutia");
	if (MinutiaStructJava == NULL) {
		ValToRet = LFS_ERROR;
		sprintf(Lfs_JNI_LastErrorString,
				"GetMinutiae: cannot find definition of LfsJavaWrapperDefinesMinutia class");
		a_printf ("%s\n", Lfs_JNI_LastErrorString);
		return ValToRet;
	}
	ptrToMinutiaStructSetFieldFunction = env->GetMethodID(MinutiaStructJava, "SetFields", "(IIIDI)V");
	if (ptrToMinutiaStructSetFieldFunction == NULL) {
		ValToRet = LFS_ERROR;
		sprintf(Lfs_JNI_LastErrorString,
				"GetMinutiae: cannot find definition of SetFields function into LfsJavaWrapperDefinesMinutia class");
		a_printf ("%s\n", Lfs_JNI_LastErrorString);
		return ValToRet;
	}

	IntForExchangeClassJava = env->FindClass(
		"com/greenbit/utils/GBJavaWrapperUtilIntForJavaToCExchange");
	if (IntForExchangeClassJava == NULL)
	{
		ValToRet = LFS_ERROR;
		sprintf(Lfs_JNI_LastErrorString,
				"GetMinutiae: cannot find definition of GBJavaWrapperUtilIntForJavaToCExchange class");
		a_printf ("%s\n", Lfs_JNI_LastErrorString);
		return ValToRet;
	}
	ptrToIntForExchangeSetFieldFunction = env->GetMethodID(IntForExchangeClassJava, "Set", "(I)V");
	if (ptrToIntForExchangeSetFieldFunction == NULL)
	{
		ValToRet = LFS_ERROR;
		sprintf(Lfs_JNI_LastErrorString,
				"GetMinutiae: cannot find definition of Set function into GBJavaWrapperUtilIntForJavaToCExchange class");
		a_printf ("%s\n", Lfs_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (LfsDllPtr == NULL)
	{
		ValToRet = LFS_ERROR;
		sprintf(Lfs_JNI_LastErrorString,
				"GetMinutiae: LfsDllPtr is NULL");
		a_printf ("%s\n", Lfs_JNI_LastErrorString);
		return ValToRet;
	}
	if (Ptr_get_minutiae == NULL)
	{
		ValToRet = LFS_ERROR;
		sprintf(Lfs_JNI_LastErrorString,
				"GetMinutiae: Ptr_get_minutiae is NULL");
		a_printf ("%s\n", Lfs_JNI_LastErrorString);
		return ValToRet;
	}
	if (Ptr_free_minutiae == NULL)
	{
		ValToRet = LFS_ERROR;
		sprintf(Lfs_JNI_LastErrorString,
				"GetMinutiae: Ptr_free_minutiae is NULL");
		a_printf ("%s\n", Lfs_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	unsigned char *ImageBuffer = new unsigned char [jSx * jSy];
	env->GetByteArrayRegion(jImageBuffer, 0, jSx * jSy,
							reinterpret_cast<jbyte *>(ImageBuffer));

	//////////////////////////////////
	// call function
	//////////////////////////////////
	MINUTIAE *ptrToMinutiae;
	ValToRet = Ptr_get_minutiae(&ptrToMinutiae,ImageBuffer,jSx,jSy,jBitsPerPixel,jPixelsPerMillimeter);
	//////////////////////////////
	// release cpp arrays
	//////////////////////////////
	delete[] ImageBuffer;

	//////////////////////////////////
	// convert to Java
	//////////////////////////////////
	if (ValToRet == LFS_SUCCESS)
	{
		for (int i = 0; i < ptrToMinutiae->num; i++) {
			jobject item = env->GetObjectArrayElement(jMinutiae, i);
			// void SetFields(int x, int y, int direct, double reliab, int type)
			env->CallVoidMethod(item, ptrToMinutiaStructSetFieldFunction,
								ptrToMinutiae->list[i]->x, ptrToMinutiae->list[i]->y,
								ptrToMinutiae->list[i]->direction,
								ptrToMinutiae->list[i]->reliability,
								ptrToMinutiae->list[i]->type
			);
		}
		env->CallVoidMethod(jMinutiaeNum, ptrToIntForExchangeSetFieldFunction, (int) (ptrToMinutiae->num));
		/********************
		 * release resources
		 *******************/
		Ptr_free_minutiae(ptrToMinutiae);
	}
	else
	{
		sprintf(Lfs_JNI_LastErrorString,
				"GetMinutiae: Ptr_get_minutiae returned %d", ValToRet);
		a_printf ("%s\n", Lfs_JNI_LastErrorString);
		ValToRet = LFS_ERROR;
		return ValToRet;
	}
	return ValToRet;
}

}

