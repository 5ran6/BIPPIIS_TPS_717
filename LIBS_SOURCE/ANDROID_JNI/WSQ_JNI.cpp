//-------------------------------------------------------------//
//-------------------------------------------------------------//
// INCLUDE
//-------------------------------------------------------------//
//-------------------------------------------------------------//
#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include "GB_SystemConfiguration.h"
#include "WSQpack.H"
#include <android/log.h>

//-------------------------------------------------------------//
//-------------------------------------------------------------//
// DEFINE
//-------------------------------------------------------------//
//-------------------------------------------------------------//
#define FreeLibrary(ptr) dlclose(ptr)
#define LoadLibrary(path) dlopen(path,RTLD_LAZY)
#define GetProcAddress(ptr, fname) dlsym(ptr,fname)
#define a_printf(...) __android_log_print(ANDROID_LOG_DEBUG, "WSQ_LOG", __VA_ARGS__);

#define WSQPACK_FAIL	-1

//-------------------------------------------------------------//
//-------------------------------------------------------------//
// GLOBALS
//-------------------------------------------------------------//
//-------------------------------------------------------------//
HMODULE WsqDllPtr = NULL;
char Wsq_JNI_LastErrorString[1000];

int (__stdcall *Ptr_WSQPACK_Compress)(
	unsigned char **dest, int *destLen, float r_bitrate,
	unsigned char *source, int width, int height, int depth, int ppi,
	char *comment_text) = NULL;

int (__stdcall *Ptr_WSQPACK_Uncompress)(
	unsigned char **dest,int *width,int *height,int *depth,int *ppi,int *lossyflag,
	unsigned char *source,int sourceLen) = NULL;

void (__stdcall *Ptr_WSQPACK_Free)(unsigned char *dest) = NULL;

//-------------------------------------------------------------//
//-------------------------------------------------------------//
// FUNCTIONS
//-------------------------------------------------------------//
//-------------------------------------------------------------//
extern "C"
{
JNIEXPORT jstring JNICALL Java_com_greenbit_wsq_WsqJavaWrapperLibrary_GetLastErrorString(
	JNIEnv *env,
	jobject /* this */)
{
	return env->NewStringUTF(Wsq_JNI_LastErrorString);
}

JNIEXPORT jint JNICALL Java_com_greenbit_wsq_WsqJavaWrapperLibrary_Unload(
	JNIEnv *env,
	jobject /* this */)
{
	int ValToRet = WSQPACK_OK;

	if (WsqDllPtr != NULL) {
		FreeLibrary(WsqDllPtr);
	} else {
		sprintf(Wsq_JNI_LastErrorString,
				"Unload: WsqDllPtr is NULL");
		a_printf ("%s\n", Wsq_JNI_LastErrorString);
	}

	Ptr_WSQPACK_Compress = NULL;
	Ptr_WSQPACK_Uncompress = NULL;
	Ptr_WSQPACK_Free = NULL;

	WsqDllPtr = NULL;

	return ValToRet;
}

jint WsqUtil_Compress(
	JNIEnv *env,
	jobject thisObj,
	jdouble jBitrate,
	jbyteArray jSource, jint jW, jint jH, jint jBitsPerPixel,jint jPpi,
	jstring jComment,
	int *pDestLen,
	unsigned char **pPtrToDestination
) {
	int ValToRet = WSQPACK_OK;

	unsigned char *ptrToSource, *ptrToDestination;
	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (WsqDllPtr == NULL) {
		ValToRet = WSQPACK_FAIL;
		sprintf(Wsq_JNI_LastErrorString,
				"WsqUtil_Compress: WsqDllPtr is NULL");
		a_printf ("%s\n", Wsq_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_WSQPACK_Compress == NULL) {
		ValToRet = WSQPACK_FAIL;
		sprintf(Wsq_JNI_LastErrorString,
				"WsqUtil_Compress: Ptr_WSQPACK_Compress is NULL");
		a_printf ("%s\n", Wsq_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_WSQPACK_Free == NULL) {
		ValToRet = WSQPACK_FAIL;
		sprintf(Wsq_JNI_LastErrorString,
				"WsqUtil_Compress: Ptr_WSQPACK_Free is NULL");
		a_printf ("%s\n", Wsq_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jSource == NULL) {
		ValToRet = WSQPACK_FAIL;
		sprintf(Wsq_JNI_LastErrorString,
				"WsqUtil_Compress: jSource is NULL");
		a_printf ("%s\n", Wsq_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetArrayLength(jSource) < (jW * jH * (jBitsPerPixel / 8))) {
		ValToRet = WSQPACK_FAIL;
		sprintf(Wsq_JNI_LastErrorString,
				"WsqUtil_Compress: Source FrameLen not enough");
		a_printf ("%s\n", Wsq_JNI_LastErrorString);
		return ValToRet;
	}

	///////////////////////////////////
	// convert java to C
	///////////////////////////////////
	ptrToSource = new unsigned char[jW * jH * (jBitsPerPixel / 8)];
	env->GetByteArrayRegion(jSource, 0, jW * jH * (jBitsPerPixel / 8),
							reinterpret_cast<jbyte *>(ptrToSource));

	// comment
	const char *constComment;
	constComment = (env)->GetStringUTFChars(jComment, 0);
	char *comment = new char[(env)->GetStringUTFLength(jComment) + 1];
	memcpy(comment, constComment, (env)->GetStringUTFLength(jComment));
	comment[(env)->GetStringUTFLength(jComment)] = 0;
	env->ReleaseStringUTFChars(jComment, constComment);

	//////////////////////////////////
	// call function
	//////////////////////////////////
	ValToRet = Ptr_WSQPACK_Compress(
		&ptrToDestination, pDestLen, (float) jBitrate,
		ptrToSource, jW, jH, jBitsPerPixel, jPpi,
		comment
	);

	delete[] ptrToSource;

	if (pPtrToDestination != NULL) *pPtrToDestination = ptrToDestination;
	else Ptr_WSQPACK_Free(ptrToDestination);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_wsq_WsqJavaWrapperLibrary_Compress(
	JNIEnv *env,
	jobject thisObj,
	jbyteArray jDestination,jdouble jBitrate,
	jbyteArray jSource, jint jW, jint jH, jint jBitsPerPixel,jint jPpi,
	jstring jComment)
{
	unsigned char *ptrToDestination;
	int destLen;
	int ValToRet = WsqUtil_Compress(
		env,thisObj,
		jBitrate,jSource, jW, jH,
		jBitsPerPixel, jPpi, jComment, &destLen, &ptrToDestination
	);

	if (jDestination == NULL) {
		ValToRet = WSQPACK_FAIL;
		sprintf(Wsq_JNI_LastErrorString,
				"Compress: jDestination is NULL");
		a_printf ("%s\n", Wsq_JNI_LastErrorString);
		return ValToRet;
	}

	if (ValToRet == WSQPACK_OK)
	{
		//////////////////////////////////
		// convert to Java
		//////////////////////////////////
		if (env->GetArrayLength(jDestination) < destLen) {
			ValToRet = WSQPACK_FAIL;
			sprintf(Wsq_JNI_LastErrorString,
					"Compress: destination length %d not enough, should be at least %d",env->GetArrayLength(jDestination), destLen);
			a_printf ("%s\n", Wsq_JNI_LastErrorString);
			Ptr_WSQPACK_Free(ptrToDestination);
			return ValToRet;
		}
		env->SetByteArrayRegion(jDestination, 0, destLen,
								reinterpret_cast<jbyte *>(ptrToDestination));
		Ptr_WSQPACK_Free(ptrToDestination);
	}
	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_wsq_WsqJavaWrapperLibrary_GetCompressedFileSize(
	JNIEnv *env,
	jobject thisObj,
	jdouble jBitrate,
	jbyteArray jSource, jint jW, jint jH, jint jBitsPerPixel,jint jPpi,
	jstring jComment,
	jobject jCompressedFileSize)
{
	int destLen;
	int ValToRet = WsqUtil_Compress(
		env,thisObj,
		jBitrate,jSource, jW, jH,
		jBitsPerPixel, jPpi, jComment, &destLen, NULL
	);
	jclass DeviceInfoJava;
	jmethodID ptrToSetFieldFunction;

	////////////////////////////////
	// load class for java
	////////////////////////////////
	DeviceInfoJava = env->FindClass(
		"com/greenbit/utils/GBJavaWrapperUtilIntForJavaToCExchange");
	if (DeviceInfoJava == NULL) {
		ValToRet = WSQPACK_FAIL;
		sprintf(Wsq_JNI_LastErrorString,
				"GetCompressedFileSize: cannot find definition of GBJavaWrapperUtilIntForJavaToCExchange class");
		a_printf ("%s\n", Wsq_JNI_LastErrorString);
		return ValToRet;
	}
	ptrToSetFieldFunction = env->GetMethodID(DeviceInfoJava, "Set", "(I)V");
	if (ptrToSetFieldFunction == NULL) {
		ValToRet = WSQPACK_FAIL;
		sprintf(Wsq_JNI_LastErrorString,
				"GetCompressedFileSize: cannot find definition of Set function into GBJavaWrapperUtilIntForJavaToCExchange class");
		a_printf ("%s\n", Wsq_JNI_LastErrorString);
		return ValToRet;
	}

	if (ValToRet == WSQPACK_OK)
	{
		//////////////////////////////////
		// convert to Java
		//////////////////////////////////
		env->CallVoidMethod(jCompressedFileSize, ptrToSetFieldFunction,(int) (destLen));
	}
	return ValToRet;
}

jint WsqUtil_Uncompress(
	JNIEnv *env,
	jobject thisObj,
	unsigned char **pPtrToDestination,
	int *pWidth, int *pHeight,
	int *pBitsPerPixel, int *pPpi,
	int *pLossyFlag,
	jbyteArray jSource) {
	int ValToRet = WSQPACK_OK;

	unsigned char *ptrToSource, *ptrToDestination;
	//////////////////////////////////
	// check functions
	//////////////////////////////////
	if (WsqDllPtr == NULL) {
		ValToRet = WSQPACK_FAIL;
		sprintf(Wsq_JNI_LastErrorString,
				"WsqUtil_Uncompress: WsqDllPtr is NULL");
		a_printf ("%s\n", Wsq_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_WSQPACK_Uncompress == NULL) {
		ValToRet = WSQPACK_FAIL;
		sprintf(Wsq_JNI_LastErrorString,
				"WsqUtil_Uncompress: Ptr_WSQPACK_Uncompress is NULL");
		a_printf ("%s\n", Wsq_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_WSQPACK_Free == NULL) {
		ValToRet = WSQPACK_FAIL;
		sprintf(Wsq_JNI_LastErrorString,
				"WsqUtil_Uncompress: Ptr_WSQPACK_Free is NULL");
		a_printf ("%s\n", Wsq_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jSource == NULL) {
		ValToRet = WSQPACK_FAIL;
		sprintf(Wsq_JNI_LastErrorString,
				"WsqUtil_Uncompress: jSource is NULL");
		a_printf ("%s\n", Wsq_JNI_LastErrorString);
		return ValToRet;
	}
	if (env->GetArrayLength(jSource) <= 0) {
		ValToRet = WSQPACK_FAIL;
		sprintf(Wsq_JNI_LastErrorString,
				"WsqUtil_Uncompress: Source FrameLen not enough");
		a_printf ("%s\n", Wsq_JNI_LastErrorString);
		return ValToRet;
	}

	///////////////////////////////////
	// convert java to C
	///////////////////////////////////
	/************
	 * source
	 */
	int sourceLen = env->GetArrayLength(jSource);
	ptrToSource = new unsigned char[sourceLen];
	env->GetByteArrayRegion(jSource, 0, sourceLen, reinterpret_cast<jbyte *>(ptrToSource));

	//////////////////////////////////
	// call function
	//////////////////////////////////
	ValToRet = Ptr_WSQPACK_Uncompress(
		&ptrToDestination, pWidth, pHeight, pBitsPerPixel, pPpi, pLossyFlag,
		ptrToSource, sourceLen
	);
	a_printf("WsqUtil_Uncompress: pWidth = %d",(*pWidth));
	a_printf("WsqUtil_Uncompress: pHeight = %d",(*pHeight));
	a_printf("WsqUtil_Uncompress: pBitsPerPixel = %d",(*pBitsPerPixel));
	a_printf("WsqUtil_Uncompress: pPpi = %d",(*pPpi));
	a_printf("WsqUtil_Uncompress: pLossyFlag = %d",(*pLossyFlag));

	/********************
	 * delete no more
	 * useful allocated
	 * resources
	 */
	delete[] ptrToSource;
	if (pPtrToDestination != NULL) *pPtrToDestination = ptrToDestination;
	else Ptr_WSQPACK_Free(ptrToDestination);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_wsq_WsqJavaWrapperLibrary_Uncompress(
	JNIEnv *env,
	jobject thisObj,
	jbyteArray jDestination,
	jobject jWidth, jobject jHeight,
	jobject jBitsPerPixel, jobject jPpi,
	jobject jLossyFlag,
	jbyteArray jSource)
{
	int ValToRet = WSQPACK_OK;

	unsigned char *ptrToDestination;
	jclass DeviceInfoJava;
	jmethodID ptrToSetFieldFunction;

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jDestination == NULL) {
		ValToRet = WSQPACK_FAIL;
		sprintf(Wsq_JNI_LastErrorString,
				"Uncompress: jDestination is NULL");
		a_printf ("%s\n", Wsq_JNI_LastErrorString);
		return ValToRet;
	}
	if (jWidth == NULL) {
		ValToRet = WSQPACK_FAIL;
		sprintf(Wsq_JNI_LastErrorString,
				"Uncompress: jWidth is NULL");
		a_printf ("%s\n", Wsq_JNI_LastErrorString);
		return ValToRet;
	}
	if (jHeight == NULL) {
		ValToRet = WSQPACK_FAIL;
		sprintf(Wsq_JNI_LastErrorString,
				"Uncompress: jHeight is NULL");
		a_printf ("%s\n", Wsq_JNI_LastErrorString);
		return ValToRet;
	}
	if (jBitsPerPixel == NULL) {
		ValToRet = WSQPACK_FAIL;
		sprintf(Wsq_JNI_LastErrorString,
				"Uncompress: jBitsPerPixel is NULL");
		a_printf ("%s\n", Wsq_JNI_LastErrorString);
		return ValToRet;
	}
	if (jPpi == NULL) {
		ValToRet = WSQPACK_FAIL;
		sprintf(Wsq_JNI_LastErrorString,
				"Uncompress: jPpi is NULL");
		a_printf ("%s\n", Wsq_JNI_LastErrorString);
		return ValToRet;
	}
	if (jLossyFlag == NULL) {
		ValToRet = WSQPACK_FAIL;
		sprintf(Wsq_JNI_LastErrorString,
				"Uncompress: jLossyFlag is NULL");
		a_printf ("%s\n", Wsq_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// load class for java
	//////////////////////////////////
	DeviceInfoJava = env->FindClass(
		"com/greenbit/utils/GBJavaWrapperUtilIntForJavaToCExchange");
	if (DeviceInfoJava == NULL) {
		ValToRet = WSQPACK_FAIL;
		sprintf(Wsq_JNI_LastErrorString,
				"Uncompress: cannot find definition of GBJavaWrapperUtilIntForJavaToCExchange class");
		a_printf ("%s\n", Wsq_JNI_LastErrorString);
		return ValToRet;
	}
	ptrToSetFieldFunction = env->GetMethodID(DeviceInfoJava, "Set", "(I)V");
	if (ptrToSetFieldFunction == NULL) {
		ValToRet = WSQPACK_FAIL;
		sprintf(Wsq_JNI_LastErrorString,
				"Uncompress: cannot find definition of Set function into GBJavaWrapperUtilIntForJavaToCExchange class");
		a_printf ("%s\n", Wsq_JNI_LastErrorString);
		return ValToRet;
	}

	///////////////////////////////////
	// convert java to C
	///////////////////////////////////

	//////////////////////////////////
	// call function
	//////////////////////////////////
	int width, height, depth, ppi, lossyflag;
	ValToRet = WsqUtil_Uncompress(
		env, thisObj,&ptrToDestination,
		&width, &height, &depth, &ppi,&lossyflag, jSource);

	//////////////////////////////////
	// convert to Java
	//////////////////////////////////
	if (ValToRet == WSQPACK_OK)
	{
		/****************
		 * destination
		 */
		int destLen = width * height * (depth/8);
		if (env->GetArrayLength(jDestination) < destLen) {
			ValToRet = WSQPACK_FAIL;
			sprintf(Wsq_JNI_LastErrorString,
					"Uncompress: destination length %d not enough, should be at least %d",env->GetArrayLength(jDestination), destLen);
			a_printf ("%s\n", Wsq_JNI_LastErrorString);
			// free allocated resources
			Ptr_WSQPACK_Free(ptrToDestination);
			return ValToRet;
		}
		env->SetByteArrayRegion(jDestination, 0, destLen,
								reinterpret_cast<jbyte *>(ptrToDestination));
		// free allocated resources
		Ptr_WSQPACK_Free(ptrToDestination);

		/****************
		 * width, length,
		 * bits per pixel,
		 * ppi, lossyflag
		 */
		env->CallVoidMethod(jWidth, ptrToSetFieldFunction,(int) (width));
		env->CallVoidMethod(jHeight, ptrToSetFieldFunction,(int) (height));
		env->CallVoidMethod(jBitsPerPixel, ptrToSetFieldFunction,(int) (depth));
		env->CallVoidMethod(jPpi, ptrToSetFieldFunction,(int) (ppi));
		env->CallVoidMethod(jLossyFlag, ptrToSetFieldFunction,(int) (lossyflag));
	}
	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_wsq_WsqJavaWrapperLibrary_GetUncompressedOutputParameters(
	JNIEnv *env,
	jobject thisObj,
	jobject jWidth, jobject jHeight,
	jobject jBitsPerPixel, jobject jPpi,
	jobject jLossyFlag,
	jbyteArray jSource)
{
	int ValToRet = WSQPACK_OK;

	jclass DeviceInfoJava;
	jmethodID ptrToSetFieldFunction;

	//////////////////////////////////
	// check parameters
	//////////////////////////////////
	if (jWidth == NULL) {
		ValToRet = WSQPACK_FAIL;
		sprintf(Wsq_JNI_LastErrorString,
				"GetUncompressedParameters: jWidth is NULL");
		a_printf ("%s\n", Wsq_JNI_LastErrorString);
		return ValToRet;
	}
	if (jHeight == NULL) {
		ValToRet = WSQPACK_FAIL;
		sprintf(Wsq_JNI_LastErrorString,
				"GetUncompressedParameters: jHeight is NULL");
		a_printf ("%s\n", Wsq_JNI_LastErrorString);
		return ValToRet;
	}
	if (jBitsPerPixel == NULL) {
		ValToRet = WSQPACK_FAIL;
		sprintf(Wsq_JNI_LastErrorString,
				"GetUncompressedParameters: jBitsPerPixel is NULL");
		a_printf ("%s\n", Wsq_JNI_LastErrorString);
		return ValToRet;
	}
	if (jPpi == NULL) {
		ValToRet = WSQPACK_FAIL;
		sprintf(Wsq_JNI_LastErrorString,
				"GetUncompressedParameters: jPpi is NULL");
		a_printf ("%s\n", Wsq_JNI_LastErrorString);
		return ValToRet;
	}
	if (jLossyFlag == NULL) {
		ValToRet = WSQPACK_FAIL;
		sprintf(Wsq_JNI_LastErrorString,
				"GetUncompressedParameters: jLossyFlag is NULL");
		a_printf ("%s\n", Wsq_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////////
	// load class for java
	//////////////////////////////////
	DeviceInfoJava = env->FindClass(
		"com/greenbit/utils/GBJavaWrapperUtilIntForJavaToCExchange");
	if (DeviceInfoJava == NULL) {
		ValToRet = WSQPACK_FAIL;
		sprintf(Wsq_JNI_LastErrorString,
				"GetUncompressedParameters: cannot find definition of GBJavaWrapperUtilIntForJavaToCExchange class");
		a_printf ("%s\n", Wsq_JNI_LastErrorString);
		return ValToRet;
	}
	ptrToSetFieldFunction = env->GetMethodID(DeviceInfoJava, "Set", "(I)V");
	if (ptrToSetFieldFunction == NULL) {
		ValToRet = WSQPACK_FAIL;
		sprintf(Wsq_JNI_LastErrorString,
				"GetUncompressedParameters: cannot find definition of Set function into GBJavaWrapperUtilIntForJavaToCExchange class");
		a_printf ("%s\n", Wsq_JNI_LastErrorString);
		return ValToRet;
	}

	///////////////////////////////////
	// convert java to C
	///////////////////////////////////

	//////////////////////////////////
	// call function
	//////////////////////////////////
	int width, height, depth, ppi, lossyflag;
	ValToRet = WsqUtil_Uncompress(
		env, thisObj,NULL,
		&width, &height, &depth, &ppi,&lossyflag, jSource);

	//////////////////////////////////
	// convert to Java
	//////////////////////////////////
	if (ValToRet == WSQPACK_OK)
	{
		/****************
		 * width, length,
		 * bits per pixel,
		 * ppi, lossyflag
		 */
		env->CallVoidMethod(jWidth, ptrToSetFieldFunction,(int) (width));
		env->CallVoidMethod(jHeight, ptrToSetFieldFunction,(int) (height));
		env->CallVoidMethod(jBitsPerPixel, ptrToSetFieldFunction,(int) (depth));
		env->CallVoidMethod(jPpi, ptrToSetFieldFunction,(int) (ppi));
		env->CallVoidMethod(jLossyFlag, ptrToSetFieldFunction,(int) (lossyflag));
	}
	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_wsq_WsqJavaWrapperLibrary_Load(
	JNIEnv *env,
	jobject /* this */)
{
	int ValToRet = WSQPACK_OK;

	Dl_info pathInfo;
	dladdr((void *)Java_com_greenbit_wsq_WsqJavaWrapperLibrary_Load,
	&pathInfo);
	a_printf("Load: pathName = %s", pathInfo.dli_fname);

	//////////////////////////////
	// free library
	//////////////////////////////
	if (WsqDllPtr != NULL)
		Java_com_greenbit_wsq_WsqJavaWrapperLibrary_Unload(env, nullptr);

	//////////////////////////////
	// load library
	//////////////////////////////
	WsqDllPtr = LoadLibrary("libWSQPack.so");
	if (WsqDllPtr == NULL) {
		ValToRet = WSQPACK_FAIL;
		sprintf(Wsq_JNI_LastErrorString,
				"Load: Unable to load WSQPack: %s", dlerror());
		a_printf ("%s\n", Wsq_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: WSQPack loaded\n");

	//////////////////////////////
	// WSQPACK_Compress
	//////////////////////////////
	Ptr_WSQPACK_Compress = (int (*)(
		unsigned char **dest, int *destLen, float r_bitrate,
		unsigned char *source, int width, int height, int depth, int ppi,
		char *comment_text
	)) GetProcAddress(WsqDllPtr, "WSQPACK_Compress");
	if (Ptr_WSQPACK_Compress == NULL) {
		ValToRet = WSQPACK_FAIL;
		sprintf(Wsq_JNI_LastErrorString,
				"Load: Unable to load Ptr_WSQPACK_Compress: %s", dlerror());
		a_printf ("%s\n", Wsq_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_WSQPACK_Compress loaded\n");

	//////////////////////////////
	// WSQPACK_Uncompress
	//////////////////////////////
	Ptr_WSQPACK_Uncompress = (int (*)(
		unsigned char **dest,int *width,int *height,int *depth,int *ppi,int *lossyflag,
		unsigned char *source,int sourceLen
	)) GetProcAddress(WsqDllPtr, "WSQPACK_Uncompress");
	if (Ptr_WSQPACK_Uncompress == NULL) {
		ValToRet = WSQPACK_FAIL;
		sprintf(Wsq_JNI_LastErrorString,
				"Load: Unable to load Ptr_WSQPACK_Uncompress: %s", dlerror());
		a_printf ("%s\n", Wsq_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_WSQPACK_Uncompress loaded\n");

	//////////////////////////////
	// WSQPACK_Free
	//////////////////////////////
	Ptr_WSQPACK_Free = (void (*)(unsigned char *dest)) GetProcAddress(WsqDllPtr, "WSQPACK_Free");
	if (Ptr_WSQPACK_Compress == NULL) {
		ValToRet = WSQPACK_FAIL;
		sprintf(Wsq_JNI_LastErrorString,
				"Load: Unable to load Ptr_WSQPACK_Free: %s", dlerror());
		a_printf ("%s\n", Wsq_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: Ptr_WSQPACK_Free loaded\n");
	return ValToRet;
}

} // extern "C"

