//-------------------------------------------------------------//
//-------------------------------------------------------------//
// INCLUDE
//-------------------------------------------------------------//
//-------------------------------------------------------------//
#include <jni.h>
#include <string>
#include <dlfcn.h>
#include "GB_SystemConfiguration.h"
#include "GBJpeg.h"
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
#define a_printf(...) __android_log_print(ANDROID_LOG_DEBUG, "GBJPEG_LOG", __VA_ARGS__);

//-------------------------------------------------------------//
//-------------------------------------------------------------//
// GLOBALS
//-------------------------------------------------------------//
//-------------------------------------------------------------//
HMODULE GbjpegDllPtr = NULL;
char Gbjpeg_JNI_LastErrorString[1000];

int (__stdcall *Ptr_GBJPEG_JP2_Encode)(
		unsigned char **ImgBuffer,
		unsigned long *BuffLen,
		unsigned char *RawImage,
		unsigned long ImageWidth,
		unsigned long ImageHeight,
		unsigned char BitsPerPixel,
		unsigned short Rate) = NULL;

int (__stdcall *Ptr_GBJPEG_JP2_Encode_FP)(
		unsigned char **ImgBuffer,
		unsigned long *BuffLen,
		unsigned char *RawImage,
		unsigned long ImageWidth,
		unsigned long ImageHeight,
		unsigned char BitsPerPixel,
		double Rate) = NULL;

int (__stdcall *Ptr_GBJPEG_JP2_Decode)(
		unsigned char *ImgBuffer,
		unsigned long BuffLen,
		unsigned char **RawImage,
		unsigned long *ImageWidth,
		unsigned long *ImageHeight,
		unsigned char *BitsPerPixel) = NULL;

int (__stdcall *Ptr_GBJPEG_JPG_Encode)(
		unsigned char **ImgBuffer,
		unsigned long *BuffLen,
		unsigned char *RawImage,
		unsigned long ImageWidth,
		unsigned long ImageHeight,
		unsigned char BitsPerPixel,
		unsigned short DPI,
		unsigned short Q) = NULL;

int (__stdcall *Ptr_GBJPEG_JPG_Decode)(
		unsigned char *ImgBuffer,
		unsigned long BuffLen,
		unsigned char **RawImage,
		unsigned long *ImageWidth,
		unsigned long *ImageHeight,
		unsigned char *BitsPerPixel) = NULL;

void (__stdcall *Ptr_GBJPEG_Free)(void *Ptr) = NULL;

void (__stdcall *Ptr_GBJPEG_JP2_Set_Dpi)(int Dpi) = NULL;

//-------------------------------------------------------------//
//-------------------------------------------------------------//
// FUNCTIONS
//-------------------------------------------------------------//
//-------------------------------------------------------------//
extern "C"
{
JNIEXPORT jstring JNICALL Java_com_greenbit_jpeg_GbjpegJavaWrapperLibrary_GetLastErrorString(
		JNIEnv *env,
		jobject /* this */)
{
	return env->NewStringUTF(Gbjpeg_JNI_LastErrorString);
}

JNIEXPORT jint JNICALL Java_com_greenbit_jpeg_GbjpegJavaWrapperLibrary_Jp2GetParamsOfEncodedBuffer(
		JNIEnv *env,
		jobject /* this */,
		/***********
		 * input
		 */
		jbyteArray jSource,
		jint jSx,
		jint jSy,
		jint jBitsPerPixel,
		jint jRate,
		/**********
		 * output
		 */
		jobject jDestinationLen
)
{
	int ValToRet = GBJPEG_OK;

	//////////////////////////////
	// check lib
	//////////////////////////////
	if (GbjpegDllPtr == NULL)
	{
		ValToRet = GBJPEG_ERROR_INITIALIZATION;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2GetParamsOfEncodedBuffer: GbjpegDllPtr is NULL");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBJPEG_JP2_Encode == NULL)
	{
		ValToRet = GBJPEG_ERROR_INITIALIZATION;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2GetParamsOfEncodedBuffer: Ptr_GBJPEG_JP2_Encode is NULL");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (jSource == NULL)
	{
		ValToRet = GBJPEG_ERROR_INVALID_PARAMETERS;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2GetParamsOfEncodedBuffer: jSource is NULL");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jSx <= 0)
	{
		ValToRet = GBJPEG_ERROR_INVALID_PARAMETERS;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2GetParamsOfEncodedBuffer: jSx <= 0");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jSy <= 0)
	{
		ValToRet = GBJPEG_ERROR_INVALID_PARAMETERS;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2GetParamsOfEncodedBuffer: jSy <= 0");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}
	if ((env)->GetArrayLength(jSource) < (jSx * jSy))
	{
		ValToRet = GBJPEG_ERROR_INVALID_PARAMETERS;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2GetParamsOfEncodedBuffer: jSource size too small");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	jbyte *jbyteSource = (env)->GetByteArrayElements(jSource, NULL);
	unsigned char *Source = new unsigned char[jSx * jSy];
	for (int i = 0; i < jSx * jSy; i++)
	{
		Source[i] = jbyteSource[i];
	}
	(env)->ReleaseByteArrayElements(jSource, jbyteSource, 0);

	//////////////////////////////
	// call function from C
	//////////////////////////////
	unsigned char *Destination;
	unsigned long DestLen;
	ValToRet = Ptr_GBJPEG_JP2_Encode(
			&Destination, &DestLen,
			Source, (unsigned long) jSx, (unsigned long) jSy,
			(unsigned char) jBitsPerPixel, (unsigned short) jRate
	);
	if (ValToRet != GBJPEG_OK)
	{
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2GetParamsOfEncodedBuffer: Ptr_GBJPEG_JP2_Encode returned %d", ValToRet);
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// release values
	//////////////////////////////
	delete[] Source;
	if (Ptr_GBJPEG_Free != NULL) Ptr_GBJPEG_Free(Destination);

	//////////////////////////////
	// copy return values to
	// java
	//////////////////////////////
	// check if needed
	if ((jDestinationLen != NULL))
	{
		GBANUTIL_GetJIntExchangeFromCInt((int)DestLen,jDestinationLen,env);
	}

	return ValToRet;

}

JNIEXPORT jint JNICALL Java_com_greenbit_jpeg_GbjpegJavaWrapperLibrary_Jp2Encode(
		JNIEnv *env,
		jobject /* this */,
		/***********
		 * input
		 */
		jbyteArray jSource,
		jint jSx,
		jint jSy,
		jint jBitsPerPixel,
		jint jRate,
		/**********
		 * output
		 */
		jbyteArray jDestination
)
{
	int ValToRet = GBJPEG_OK;

	//////////////////////////////
	// check lib
	//////////////////////////////
	if (GbjpegDllPtr == NULL)
	{
		ValToRet = GBJPEG_ERROR_INITIALIZATION;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2Encode: GbjpegDllPtr is NULL");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBJPEG_JP2_Encode == NULL)
	{
		ValToRet = GBJPEG_ERROR_INITIALIZATION;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2Encode: Ptr_GBJPEG_JP2_Encode is NULL");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (jSource == NULL)
	{
		ValToRet = GBJPEG_ERROR_INVALID_PARAMETERS;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2Encode: jSource is NULL");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jSx <= 0)
	{
		ValToRet = GBJPEG_ERROR_INVALID_PARAMETERS;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2Encode: jSx <= 0");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jSy <= 0)
	{
		ValToRet = GBJPEG_ERROR_INVALID_PARAMETERS;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2Encode: jSy <= 0");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}
	if ((env)->GetArrayLength(jSource) < (jSx * jSy))
	{
		ValToRet = GBJPEG_ERROR_INVALID_PARAMETERS;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2Encode: jSource size too small");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jDestination == NULL)
	{
		ValToRet = GBJPEG_ERROR_INVALID_PARAMETERS;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2Encode: jDestination is NULL");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	jbyte *jbyteSource = (env)->GetByteArrayElements(jSource, NULL);
	unsigned char *Source = new unsigned char[jSx * jSy];
	for (int i = 0; i < jSx * jSy; i++)
	{
		Source[i] = jbyteSource[i];
	}
	(env)->ReleaseByteArrayElements(jSource, jbyteSource, 0);

	//////////////////////////////
	// call function from C
	//////////////////////////////
	unsigned char *Destination;
	unsigned long DestLen;
	ValToRet = Ptr_GBJPEG_JP2_Encode(
			&Destination, &DestLen,
			Source, (unsigned long) jSx, (unsigned long) jSy,
			(unsigned char) jBitsPerPixel, (unsigned short) jRate
	);
	if (ValToRet != GBJPEG_OK)
	{
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2Encode: Ptr_GBJPEG_JP2_Encode returned %d", ValToRet);
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// copy return values to
	// java
	//////////////////////////////
	if ((env)->GetArrayLength(jDestination) < (DestLen))
	{
		ValToRet = GBJPEG_ERROR_INVALID_PARAMETERS;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2Encode: jDestination size too small, minimum for this image is %d", DestLen);
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
	}
	else
	{
		env->SetByteArrayRegion(
				jDestination, 0, DestLen,
				reinterpret_cast<jbyte *>(Destination));
	}

	//////////////////////////////
	// release values
	//////////////////////////////
	delete[] Source;
	if (Ptr_GBJPEG_Free != NULL) Ptr_GBJPEG_Free(Destination);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_jpeg_GbjpegJavaWrapperLibrary_Jp2GetParamsOfEncodedBufferFP(
		JNIEnv *env,
		jobject /* this */,
		/***********
		 * input
		 */
		jbyteArray jSource,
		jint jSx,
		jint jSy,
		jint jBitsPerPixel,
		jdouble jRate,
		/**********
		 * output
		 */
		jobject jDestinationLen
)
{
	int ValToRet = GBJPEG_OK;

	//////////////////////////////
	// check lib
	//////////////////////////////
	if (GbjpegDllPtr == NULL)
	{
		ValToRet = GBJPEG_ERROR_INITIALIZATION;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2GetParamsOfEncodedBufferFP: GbjpegDllPtr is NULL");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBJPEG_JP2_Encode_FP == NULL)
	{
		ValToRet = GBJPEG_ERROR_INITIALIZATION;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2GetParamsOfEncodedBufferFP: Ptr_GBJPEG_JP2_EncodeFP is NULL");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (jSource == NULL)
	{
		ValToRet = GBJPEG_ERROR_INVALID_PARAMETERS;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2GetParamsOfEncodedBufferFP: jSource is NULL");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jSx <= 0)
	{
		ValToRet = GBJPEG_ERROR_INVALID_PARAMETERS;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2GetParamsOfEncodedBufferFP: jSx <= 0");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jSy <= 0)
	{
		ValToRet = GBJPEG_ERROR_INVALID_PARAMETERS;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2GetParamsOfEncodedBufferFP: jSy <= 0");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}
	if ((env)->GetArrayLength(jSource) < (jSx * jSy))
	{
		ValToRet = GBJPEG_ERROR_INVALID_PARAMETERS;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2GetParamsOfEncodedBufferFP: jSource size too small");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	jbyte *jbyteSource = (env)->GetByteArrayElements(jSource, NULL);
	unsigned char *Source = new unsigned char[jSx * jSy];
	for (int i = 0; i < jSx * jSy; i++)
	{
		Source[i] = jbyteSource[i];
	}
	(env)->ReleaseByteArrayElements(jSource, jbyteSource, 0);

	//////////////////////////////
	// call function from C
	//////////////////////////////
	unsigned char *Destination;
	unsigned long DestLen;
	ValToRet = Ptr_GBJPEG_JP2_Encode_FP(
			&Destination, &DestLen,
			Source, (unsigned long) jSx, (unsigned long) jSy,
			(unsigned char) jBitsPerPixel, (double) jRate
	);
	if (ValToRet != GBJPEG_OK)
	{
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2GetParamsOfEncodedBufferFP: Ptr_GBJPEG_JP2_Encode returned %d", ValToRet);
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// release values
	//////////////////////////////
	delete[] Source;
	if (Ptr_GBJPEG_Free != NULL) Ptr_GBJPEG_Free(Destination);

	//////////////////////////////
	// copy return values to
	// java
	//////////////////////////////
	// check if needed
	if ((jDestinationLen != NULL))
	{
		GBANUTIL_GetJIntExchangeFromCInt((int)DestLen,jDestinationLen,env);
	}

	return ValToRet;

}

JNIEXPORT jint JNICALL Java_com_greenbit_jpeg_GbjpegJavaWrapperLibrary_Jp2EncodeFP(
		JNIEnv *env,
		jobject /* this */,
		/***********
		 * input
		 */
		jbyteArray jSource,
		jint jSx,
		jint jSy,
		jint jBitsPerPixel,
		jdouble jRate,
		/**********
		 * output
		 */
		jbyteArray jDestination
)
{
	int ValToRet = GBJPEG_OK;

	//////////////////////////////
	// check lib
	//////////////////////////////
	if (GbjpegDllPtr == NULL)
	{
		ValToRet = GBJPEG_ERROR_INITIALIZATION;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2EncodeFP: GbjpegDllPtr is NULL");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBJPEG_JP2_Encode_FP == NULL)
	{
		ValToRet = GBJPEG_ERROR_INITIALIZATION;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2EncodeFP: Ptr_GBJPEG_JP2_Encode_FP is NULL");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (jSource == NULL)
	{
		ValToRet = GBJPEG_ERROR_INVALID_PARAMETERS;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2EncodeFP: jSource is NULL");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jSx <= 0)
	{
		ValToRet = GBJPEG_ERROR_INVALID_PARAMETERS;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2EncodeFP: jSx <= 0");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jSy <= 0)
	{
		ValToRet = GBJPEG_ERROR_INVALID_PARAMETERS;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2EncodeFP: jSy <= 0");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}
	if ((env)->GetArrayLength(jSource) < (jSx * jSy))
	{
		ValToRet = GBJPEG_ERROR_INVALID_PARAMETERS;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2EncodeFP: jSource size too small");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jDestination == NULL)
	{
		ValToRet = GBJPEG_ERROR_INVALID_PARAMETERS;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2EncodeFP: jDestination is NULL");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	jbyte *jbyteSource = (env)->GetByteArrayElements(jSource, NULL);
	unsigned char *Source = new unsigned char[jSx * jSy];
	for (int i = 0; i < jSx * jSy; i++)
	{
		Source[i] = jbyteSource[i];
	}
	(env)->ReleaseByteArrayElements(jSource, jbyteSource, 0);

	//////////////////////////////
	// call function from C
	//////////////////////////////
	unsigned char *Destination;
	unsigned long DestLen;
	ValToRet = Ptr_GBJPEG_JP2_Encode_FP(
			&Destination, &DestLen,
			Source, (unsigned long) jSx, (unsigned long) jSy,
			(unsigned char) jBitsPerPixel, (double) jRate
	);
	if (ValToRet != GBJPEG_OK)
	{
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2EncodeFP: Ptr_GBJPEG_JP2_Encode_FP returned %d", ValToRet);
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// copy return values to
	// java
	//////////////////////////////
	if ((env)->GetArrayLength(jDestination) < (DestLen))
	{
		ValToRet = GBJPEG_ERROR_INVALID_PARAMETERS;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2EncodeFP: jDestination size too small, minimum for this image is %d", DestLen);
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
	}
	else
	{
		env->SetByteArrayRegion(
				jDestination, 0, DestLen,
				reinterpret_cast<jbyte *>(Destination));
	}

	//////////////////////////////
	// release values
	//////////////////////////////
	delete[] Source;
	if (Ptr_GBJPEG_Free != NULL) Ptr_GBJPEG_Free(Destination);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_jpeg_GbjpegJavaWrapperLibrary_Jp2GetParametersOfDecodedBuffer(
		JNIEnv *env,
		jobject /* this */,
		/***********
		 * input
		 */
		jbyteArray jSource,
		/**********
		 * output
		 */
		jobject jSx,
		jobject jSy,
		jobject jBitsPerPixel
)
{
	int ValToRet = GBJPEG_OK;

	//////////////////////////////
	// check lib
	//////////////////////////////
	if (GbjpegDllPtr == NULL)
	{
		ValToRet = GBJPEG_ERROR_INITIALIZATION;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2GetParametersOfDecodedBuffer: GbjpegDllPtr is NULL");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBJPEG_JP2_Decode == NULL)
	{
		ValToRet = GBJPEG_ERROR_INITIALIZATION;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2GetParametersOfDecodedBuffer: Ptr_GBJPEG_JP2_Decode is NULL");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (jSource == NULL)
	{
		ValToRet = GBJPEG_ERROR_INVALID_PARAMETERS;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2GetParametersOfDecodedBuffer: jSource is NULL");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}
	if ((env)->GetArrayLength(jSource) <= (0))
	{
		ValToRet = GBJPEG_ERROR_INVALID_PARAMETERS;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2GetParametersOfDecodedBuffer: jSource size too small");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	jbyte *jbyteSource = (env)->GetByteArrayElements(jSource, NULL);
	unsigned char *Source = new unsigned char[(env)->GetArrayLength(jSource)];
	for (int i = 0; i < (env)->GetArrayLength(jSource); i++)
	{
		Source[i] = jbyteSource[i];
	}
	(env)->ReleaseByteArrayElements(jSource, jbyteSource, 0);

	//////////////////////////////
	// call function from C
	//////////////////////////////
	unsigned char *Destination;
	unsigned long Sx, Sy;
	unsigned char BitsPerPixel;
	/*
	 * DLL_EXPORT int __stdcall GBJPEG_JP2_Decode(unsigned char *ImgBuffer,
											unsigned long BuffLen,
                                            unsigned char **RawImage,
                                            unsigned long *ImageWidth,
                                            unsigned long *ImageHeight,
                                            unsigned char *BitsPerPixel);
	 */
	ValToRet = Ptr_GBJPEG_JP2_Decode(
			Source, (unsigned long) ((env)->GetArrayLength(jSource)),
			&Destination, &Sx, (unsigned long *) &Sy, &BitsPerPixel
	);
	if (ValToRet != GBJPEG_OK)
	{
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2GetParametersOfDecodedBuffer: Ptr_GBJPEG_JP2_Decode returned %d", ValToRet);
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}
	delete[] Source;

	//////////////////////////////
	// copy return values to
	// java
	//////////////////////////////
	// release values not needed anymore
	if (Ptr_GBJPEG_Free != NULL) Ptr_GBJPEG_Free(Destination);
	/*************************
	 * set int values
	 * **********************/
	if ((jBitsPerPixel != NULL)) GBANUTIL_GetJIntExchangeFromCInt((int)BitsPerPixel,jBitsPerPixel,env);
	if ((jSx != NULL)) GBANUTIL_GetJIntExchangeFromCInt((int)Sx,jSx,env);
	if ((jSy != NULL)) GBANUTIL_GetJIntExchangeFromCInt((int)Sy,jSy,env);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_jpeg_GbjpegJavaWrapperLibrary_Jp2Decode(
		JNIEnv *env,
		jobject /* this */,
		/***********
		 * input
		 */
		jbyteArray jSource,
		/**********
		 * output
		 */
		jbyteArray jDestination,
		jobject jSx,
		jobject jSy,
		jobject jBitsPerPixel
)
{
	int ValToRet = GBJPEG_OK;

	//////////////////////////////
	// check lib
	//////////////////////////////
	if (GbjpegDllPtr == NULL)
	{
		ValToRet = GBJPEG_ERROR_INITIALIZATION;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2Decode: GbjpegDllPtr is NULL");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBJPEG_JP2_Decode == NULL)
	{
		ValToRet = GBJPEG_ERROR_INITIALIZATION;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2Decode: Ptr_GBJPEG_JP2_Decode is NULL");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (jSource == NULL)
	{
		ValToRet = GBJPEG_ERROR_INVALID_PARAMETERS;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2Decode: jSource is NULL");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}
	if ((env)->GetArrayLength(jSource) <= (0))
	{
		ValToRet = GBJPEG_ERROR_INVALID_PARAMETERS;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2Decode: jSource size too small");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	jbyte *jbyteSource = (env)->GetByteArrayElements(jSource, NULL);
	unsigned char *Source = new unsigned char[(env)->GetArrayLength(jSource)];
	for (int i = 0; i < (env)->GetArrayLength(jSource); i++)
	{
		Source[i] = jbyteSource[i];
	}
	(env)->ReleaseByteArrayElements(jSource, jbyteSource, 0);

	//////////////////////////////
	// call function from C
	//////////////////////////////
	unsigned char *Destination;
	unsigned long Sx, Sy;
	unsigned char BitsPerPixel;
	/*
	 * DLL_EXPORT int __stdcall GBJPEG_JP2_Decode(unsigned char *ImgBuffer,
											unsigned long BuffLen,
                                            unsigned char **RawImage,
                                            unsigned long *ImageWidth,
                                            unsigned long *ImageHeight,
                                            unsigned char *BitsPerPixel);
	 */
	ValToRet = Ptr_GBJPEG_JP2_Decode(
			Source, (unsigned long) ((env)->GetArrayLength(jSource)),
			&Destination, &Sx, (unsigned long *) &Sy, &BitsPerPixel
	);
	if (ValToRet != GBJPEG_OK)
	{
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2Decode: Ptr_GBJPEG_JP2_Decode returned %d", ValToRet);
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}
	delete[] Source;

	//////////////////////////////
	// copy return values to
	// java
	//////////////////////////////
	/*************************
	 * set destination
	 * **********************/
	if (jDestination != NULL)
	{
		int destLen = Sx * Sy * (BitsPerPixel / 8);
		if ((env)->GetArrayLength(jDestination) < destLen)
		{
			ValToRet = GBJPEG_ERROR_INVALID_PARAMETERS;
			sprintf(Gbjpeg_JNI_LastErrorString,
			        "Jp2Decode: jDestination size too small, minimum for this image is %d",
			        destLen);
			a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
			if (Ptr_GBJPEG_Free != NULL) Ptr_GBJPEG_Free(Destination);
			return ValToRet;
		}
		env->SetByteArrayRegion(
				jDestination, 0, destLen,
				reinterpret_cast<jbyte *>(Destination));
	}
	// release values not needed anymore
	if (Ptr_GBJPEG_Free != NULL) Ptr_GBJPEG_Free(Destination);

	if ((jBitsPerPixel != NULL)) GBANUTIL_GetJIntExchangeFromCInt((int)BitsPerPixel,jBitsPerPixel,env);
	if ((jSx != NULL)) GBANUTIL_GetJIntExchangeFromCInt((int)Sx,jSx,env);
	if ((jSy != NULL)) GBANUTIL_GetJIntExchangeFromCInt((int)Sy,jSy,env);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_jpeg_GbjpegJavaWrapperLibrary_JpegGetParamsOfEncodedBuffer(
		JNIEnv *env,
		jobject /* this */,
		/***********
		 * input
		 */
		jbyteArray jSource,
		jint jSx,
		jint jSy,
		jint jBitsPerPixel,
		jint jDpi,
		jint jQuality,
		/**********
		 * output
		 */
		jobject jDestinationLen
)
{
	int ValToRet = GBJPEG_OK;

	//////////////////////////////
	// check lib
	//////////////////////////////
	if (GbjpegDllPtr == NULL)
	{
		ValToRet = GBJPEG_ERROR_INITIALIZATION;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "JpegGetParamsOfEncodedBuffer: GbjpegDllPtr is NULL");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBJPEG_JPG_Encode == NULL)
	{
		ValToRet = GBJPEG_ERROR_INITIALIZATION;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "JpegGetParamsOfEncodedBuffer: Ptr_GBJPEG_JPG_Encode is NULL");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (jSource == NULL)
	{
		ValToRet = GBJPEG_ERROR_INVALID_PARAMETERS;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "JpegGetParamsOfEncodedBuffer: jSource is NULL");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jSx <= 0)
	{
		ValToRet = GBJPEG_ERROR_INVALID_PARAMETERS;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "JpegGetParamsOfEncodedBuffer: jSx <= 0");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jSy <= 0)
	{
		ValToRet = GBJPEG_ERROR_INVALID_PARAMETERS;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "JpegGetParamsOfEncodedBuffer: jSy <= 0");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}
	if ((env)->GetArrayLength(jSource) < (jSx * jSy))
	{
		ValToRet = GBJPEG_ERROR_INVALID_PARAMETERS;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "JpegGetParamsOfEncodedBuffer: jSource size too small");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	jbyte *jbyteSource = (env)->GetByteArrayElements(jSource, NULL);
	unsigned char *Source = new unsigned char[jSx * jSy];
	for (int i = 0; i < jSx * jSy; i++)
	{
		Source[i] = jbyteSource[i];
	}
	(env)->ReleaseByteArrayElements(jSource, jbyteSource, 0);

	//////////////////////////////
	// call function from C
	//////////////////////////////
	unsigned char *Destination;
	unsigned long DestLen;
	ValToRet = Ptr_GBJPEG_JPG_Encode(
			&Destination, &DestLen,
			Source, (unsigned long) jSx, (unsigned long) jSy,
			(unsigned char) jBitsPerPixel, (unsigned short) jDpi, (unsigned short)jQuality
	);
	//////////////////////////////
	// release values
	//////////////////////////////
	delete[] Source;
	if (Ptr_GBJPEG_Free != NULL) Ptr_GBJPEG_Free(Destination);

	if (ValToRet != GBJPEG_OK)
	{
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "JpegGetParamsOfEncodedBuffer: Ptr_GBJPEG_JPG_Encode returned %d, jSx = %d, jSy = %d, jBitsPerPixel = %d,jDpi = %d,jQuality = %d",
		        ValToRet,jSx, jSy, jBitsPerPixel, jDpi, jQuality);
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// copy return values to
	// java
	//////////////////////////////
	// check if needed
	if ((jDestinationLen != NULL))
	{
		GBANUTIL_GetJIntExchangeFromCInt((int)DestLen,jDestinationLen,env);
	}

	return ValToRet;

}

JNIEXPORT jint JNICALL Java_com_greenbit_jpeg_GbjpegJavaWrapperLibrary_JpegEncode(
		JNIEnv *env,
		jobject /* this */,
		/***********
		 * input
		 */
		jbyteArray jSource,
		jint jSx,
		jint jSy,
		jint jBitsPerPixel,
		jint jDpi,
        jint jQuality,
		/**********
		 * output
		 */
		jbyteArray jDestination
)
{
	int ValToRet = GBJPEG_OK;

	//////////////////////////////
	// check lib
	//////////////////////////////
	if (GbjpegDllPtr == NULL)
	{
		ValToRet = GBJPEG_ERROR_INITIALIZATION;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "JpegEncode: GbjpegDllPtr is NULL");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBJPEG_JPG_Encode == NULL)
	{
		ValToRet = GBJPEG_ERROR_INITIALIZATION;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "JpegEncode: Ptr_GBJPEG_JPG_Encode is NULL");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (jSource == NULL)
	{
		ValToRet = GBJPEG_ERROR_INVALID_PARAMETERS;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "JpegEncode: jSource is NULL");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jSx <= 0)
	{
		ValToRet = GBJPEG_ERROR_INVALID_PARAMETERS;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "JpegEncode: jSx <= 0");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jSy <= 0)
	{
		ValToRet = GBJPEG_ERROR_INVALID_PARAMETERS;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "JpegEncode: jSy <= 0");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}
	if ((env)->GetArrayLength(jSource) < (jSx * jSy))
	{
		ValToRet = GBJPEG_ERROR_INVALID_PARAMETERS;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "JpegEncode: jSource size too small");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}
	if (jDestination == NULL)
	{
		ValToRet = GBJPEG_ERROR_INVALID_PARAMETERS;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "JpegEncode: jDestination is NULL");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	jbyte *jbyteSource = (env)->GetByteArrayElements(jSource, NULL);
	unsigned char *Source = new unsigned char[jSx * jSy];
	for (int i = 0; i < jSx * jSy; i++)
	{
		Source[i] = jbyteSource[i];
	}
	(env)->ReleaseByteArrayElements(jSource, jbyteSource, 0);

	//////////////////////////////
	// call function from C
	//////////////////////////////
	unsigned char *Destination;
	unsigned long DestLen;
	ValToRet = Ptr_GBJPEG_JPG_Encode(
			&Destination, &DestLen,
			Source, (unsigned long) jSx, (unsigned long) jSy,
			(unsigned char) jBitsPerPixel, (unsigned short) jDpi, (unsigned short)jQuality
	);
	if (ValToRet != GBJPEG_OK)
	{
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "JpegEncode: Ptr_GBJPEG_JPG_Encode returned %d, jSx = %d, jSy = %d, jBitsPerPixel = %d,jDpi = %d,jQuality = %d",
		        ValToRet,jSx, jSy, jBitsPerPixel, jDpi, jQuality);
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// copy return values to
	// java
	//////////////////////////////
	if ((env)->GetArrayLength(jDestination) < (DestLen))
	{
		ValToRet = GBJPEG_ERROR_INVALID_PARAMETERS;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "JpegEncode: jDestination size too small, minimum for this image is %d", DestLen);
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
	}
	else
	{
		env->SetByteArrayRegion(
				jDestination, 0, DestLen,
				reinterpret_cast<jbyte *>(Destination));
	}

	//////////////////////////////
	// release values
	//////////////////////////////
	delete[] Source;
	if (Ptr_GBJPEG_Free != NULL) Ptr_GBJPEG_Free(Destination);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_jpeg_GbjpegJavaWrapperLibrary_JpegGetParametersOfDecodedBuffer(
		JNIEnv *env,
		jobject /* this */,
		/***********
		 * input
		 */
		jbyteArray jSource,
		/**********
		 * output
		 */
		jobject jSx,
		jobject jSy,
		jobject jBitsPerPixel
)
{
	int ValToRet = GBJPEG_OK;

	//////////////////////////////
	// check lib
	//////////////////////////////
	if (GbjpegDllPtr == NULL)
	{
		ValToRet = GBJPEG_ERROR_INITIALIZATION;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "JpegGetParametersOfDecodedBuffer: GbjpegDllPtr is NULL");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBJPEG_JPG_Decode == NULL)
	{
		ValToRet = GBJPEG_ERROR_INITIALIZATION;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "JpegGetParametersOfDecodedBuffer: Ptr_GBJPEG_JPG_Decode is NULL");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (jSource == NULL)
	{
		ValToRet = GBJPEG_ERROR_INVALID_PARAMETERS;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "JpegGetParametersOfDecodedBuffer: jSource is NULL");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}
	if ((env)->GetArrayLength(jSource) <= (0))
	{
		ValToRet = GBJPEG_ERROR_INVALID_PARAMETERS;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "JpegGetParametersOfDecodedBuffer: jSource size too small");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	jbyte *jbyteSource = (env)->GetByteArrayElements(jSource, NULL);
	unsigned char *Source = new unsigned char[(env)->GetArrayLength(jSource)];
	for (int i = 0; i < (env)->GetArrayLength(jSource); i++)
	{
		Source[i] = jbyteSource[i];
	}
	(env)->ReleaseByteArrayElements(jSource, jbyteSource, 0);

	//////////////////////////////
	// call function from C
	//////////////////////////////
	unsigned char *Destination;
	unsigned long Sx, Sy;
	unsigned char BitsPerPixel;

	ValToRet = Ptr_GBJPEG_JPG_Decode(
			Source, (unsigned long) ((env)->GetArrayLength(jSource)),
			&Destination, &Sx, (unsigned long *) &Sy, &BitsPerPixel
	);
	if (ValToRet != GBJPEG_OK)
	{
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "JpegGetParametersOfDecodedBuffer: Ptr_GBJPEG_JPG_Decode returned %d", ValToRet);
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}
	delete[] Source;
	// release values not needed anymore
	if (Ptr_GBJPEG_Free != NULL) Ptr_GBJPEG_Free(Destination);

	//////////////////////////////
	// copy return values to
	// java
	//////////////////////////////

	if ((jBitsPerPixel != NULL)) GBANUTIL_GetJIntExchangeFromCInt((int)BitsPerPixel,jBitsPerPixel,env);
	if ((jSx != NULL)) GBANUTIL_GetJIntExchangeFromCInt((int)Sx,jSx,env);
	if ((jSy != NULL)) GBANUTIL_GetJIntExchangeFromCInt((int)Sy,jSy,env);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_jpeg_GbjpegJavaWrapperLibrary_JpegDecode(
		JNIEnv *env,
		jobject /* this */,
		/***********
		 * input
		 */
		jbyteArray jSource,
		/**********
		 * output
		 */
		jbyteArray jDestination,
		jobject jSx,
		jobject jSy,
		jobject jBitsPerPixel
)
{
	int ValToRet = GBJPEG_OK;

	//////////////////////////////
	// check lib
	//////////////////////////////
	if (GbjpegDllPtr == NULL)
	{
		ValToRet = GBJPEG_ERROR_INITIALIZATION;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "JpegDecode: GbjpegDllPtr is NULL");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBJPEG_JPG_Decode == NULL)
	{
		ValToRet = GBJPEG_ERROR_INITIALIZATION;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "JpegDecode: Ptr_GBJPEG_JPG_Decode is NULL");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// check parameters
	//////////////////////////////
	if (jSource == NULL)
	{
		ValToRet = GBJPEG_ERROR_INVALID_PARAMETERS;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "JpegDecode: jSource is NULL");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}
	if ((env)->GetArrayLength(jSource) <= (0))
	{
		ValToRet = GBJPEG_ERROR_INVALID_PARAMETERS;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "JpegDecode: jSource size too small");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}

	//////////////////////////////
	// get C parameters from
	// java ones
	//////////////////////////////
	jbyte *jbyteSource = (env)->GetByteArrayElements(jSource, NULL);
	unsigned char *Source = new unsigned char[(env)->GetArrayLength(jSource)];
	for (int i = 0; i < (env)->GetArrayLength(jSource); i++)
	{
		Source[i] = jbyteSource[i];
	}
	(env)->ReleaseByteArrayElements(jSource, jbyteSource, 0);

	//////////////////////////////
	// call function from C
	//////////////////////////////
	unsigned char *Destination;
	unsigned long Sx, Sy;
	unsigned char BitsPerPixel;

	ValToRet = Ptr_GBJPEG_JPG_Decode(
			Source, (unsigned long) ((env)->GetArrayLength(jSource)),
			&Destination, &Sx, (unsigned long *) &Sy, &BitsPerPixel
	);
	if (ValToRet != GBJPEG_OK)
	{
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "JpegDecode: Ptr_GBJPEG_JPG_Decode returned %d", ValToRet);
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}
	delete[] Source;

	//////////////////////////////
	// copy return values to
	// java
	//////////////////////////////
	/*************************
	 * set destination
	 * **********************/
	if (jDestination != NULL)
	{
		int destLen = Sx * Sy * (BitsPerPixel / 8);
		if ((env)->GetArrayLength(jDestination) < destLen)
		{
			ValToRet = GBJPEG_ERROR_INVALID_PARAMETERS;
			sprintf(Gbjpeg_JNI_LastErrorString,
			        "JpegDecode: jDestination size too small, minimum for this image is %d",
			        destLen);
			a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
			if (Ptr_GBJPEG_Free != NULL) Ptr_GBJPEG_Free(Destination);
			return ValToRet;
		}
		env->SetByteArrayRegion(
				jDestination, 0, destLen,
				reinterpret_cast<jbyte *>(Destination));
	}
	// release values not needed anymore
	if (Ptr_GBJPEG_Free != NULL) Ptr_GBJPEG_Free(Destination);

	if ((jBitsPerPixel != NULL)) GBANUTIL_GetJIntExchangeFromCInt((int)BitsPerPixel,jBitsPerPixel,env);
	if ((jSx != NULL)) GBANUTIL_GetJIntExchangeFromCInt((int)Sx,jSx,env);
	if ((jSy != NULL)) GBANUTIL_GetJIntExchangeFromCInt((int)Sy,jSy,env);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_jpeg_GbjpegJavaWrapperLibrary_Jp2SetDpi(
		JNIEnv *env,
		jobject /* this */,
		/***********
		 * input
		 */
		jint jDpi
)
{
	int ValToRet = GBJPEG_OK;

	//////////////////////////////
	// check lib
	//////////////////////////////
	if (GbjpegDllPtr == NULL)
	{
		ValToRet = GBJPEG_ERROR_INITIALIZATION;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2SetDpi: GbjpegDllPtr is NULL");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}

	if (Ptr_GBJPEG_JP2_Set_Dpi == NULL)
	{
		ValToRet = GBJPEG_ERROR_INITIALIZATION;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Jp2SetDpi: Ptr_GBJPEG_JP2_Set_Dpi is NULL");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}
	//////////////////////////////
	// call function from C
	//////////////////////////////
	Ptr_GBJPEG_JP2_Set_Dpi((int)jDpi);

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_jpeg_GbjpegJavaWrapperLibrary_Unload(
		JNIEnv *env,
		jobject /* this */)
{
	int ValToRet = GBJPEG_OK;

	if (GbjpegDllPtr != NULL)
	{
		FreeLibrary(GbjpegDllPtr);
	}
	else
	{
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Unload: WsqDllPtr is NULL");
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
	}

	Ptr_GBJPEG_JP2_Encode = NULL;
	Ptr_GBJPEG_JP2_Encode_FP = NULL;
	Ptr_GBJPEG_JP2_Decode = NULL;
	Ptr_GBJPEG_JPG_Encode = NULL;
	Ptr_GBJPEG_JPG_Decode = NULL;
	Ptr_GBJPEG_JP2_Set_Dpi = NULL;

	GbjpegDllPtr = NULL;

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_jpeg_GbjpegJavaWrapperLibrary_Load(
		JNIEnv *env,
		jobject /* this */)
{
	int ValToRet = GBJPEG_OK;

	Dl_info pathInfo;
	dladdr((void *) Java_com_greenbit_jpeg_GbjpegJavaWrapperLibrary_Load,
	       &pathInfo);
	a_printf("Load: pathName = %s", pathInfo.dli_fname);

	//////////////////////////////
	// free library
	//////////////////////////////
	if (GbjpegDllPtr != NULL)
		Java_com_greenbit_jpeg_GbjpegJavaWrapperLibrary_Unload(env, nullptr);

	//////////////////////////////
	// load library
	//////////////////////////////
	GbjpegDllPtr = LoadLibrary("libGBJPEG.so");
	if (GbjpegDllPtr == NULL)
	{
		ValToRet = GBJPEG_ERROR_INITIALIZATION;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Load: Unable to load GbJpeg: %s", dlerror());
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: GbJpeg loaded\n");

	//////////////////////////////
	// GBJPEG_JP2_Encode
	//////////////////////////////
	Ptr_GBJPEG_JP2_Encode = (int (*)(
			unsigned char **ImgBuffer,
			unsigned long *BuffLen,
			unsigned char *RawImage,
			unsigned long ImageWidth,
			unsigned long ImageHeight,
			unsigned char BitsPerPixel,
			unsigned short Rate
	)) GetProcAddress(GbjpegDllPtr, "GBJPEG_JP2_Encode");
	if (Ptr_GBJPEG_JP2_Encode == NULL)
	{
		ValToRet = GBJPEG_ERROR_INITIALIZATION;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Load: : Unable to load Ptr_GBJPEG_JP2_Encode: %s", dlerror());
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSFEJavaWrapperLibrary_Load: Ptr_GBJPEG_JP2_Encode loaded\n");

	//////////////////////////////
	// GBJPEG_JP2_Encode_FP
	//////////////////////////////
	Ptr_GBJPEG_JP2_Encode_FP = (int (*)(
			unsigned char **ImgBuffer,
			unsigned long *BuffLen,
			unsigned char *RawImage,
			unsigned long ImageWidth,
			unsigned long ImageHeight,
			unsigned char BitsPerPixel,
			double Rate
	)) GetProcAddress(GbjpegDllPtr, "GBJPEG_JP2_Encode_FP");
	if (Ptr_GBJPEG_JP2_Encode_FP == NULL)
	{
		ValToRet = GBJPEG_ERROR_INITIALIZATION;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Load: : Unable to load Ptr_GBJPEG_JP2_Encode_FP: %s", dlerror());
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSFEJavaWrapperLibrary_Load: Ptr_GBJPEG_JP2_Encode_FP loaded\n");

	//////////////////////////////
	// GBJPEG_JP2_Decode
	//////////////////////////////
	Ptr_GBJPEG_JP2_Decode = (int (*)(
			unsigned char *ImgBuffer,
			unsigned long BuffLen,
			unsigned char **RawImage,
			unsigned long *ImageWidth,
			unsigned long *ImageHeight,
			unsigned char *BitsPerPixel
	)) GetProcAddress(GbjpegDllPtr, "GBJPEG_JP2_Decode");
	if (Ptr_GBJPEG_JP2_Decode == NULL)
	{
		ValToRet = GBJPEG_ERROR_INITIALIZATION;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Load: : Unable to load Ptr_GBJPEG_JP2_Decode: %s", dlerror());
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSFEJavaWrapperLibrary_Load: Ptr_GBJPEG_JP2_Decode loaded\n");

	//////////////////////////////
	// GBJPEG_JPG_Encode
	//////////////////////////////
	Ptr_GBJPEG_JPG_Encode = (int (*)(
			unsigned char **ImgBuffer,
			unsigned long *BuffLen,
			unsigned char *RawImage,
			unsigned long ImageWidth,
			unsigned long ImageHeight,
			unsigned char BitsPerPixel,
			unsigned short DPI,
			unsigned short Q
	)) GetProcAddress(GbjpegDllPtr, "GBJPEG_JPG_Encode");
	if (Ptr_GBJPEG_JPG_Encode == NULL)
	{
		ValToRet = GBJPEG_ERROR_INITIALIZATION;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Load: : Unable to load Ptr_GBJPEG_JPG_Encode: %s", dlerror());
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSFEJavaWrapperLibrary_Load: Ptr_GBJPEG_JPG_Encode loaded\n");

	//////////////////////////////
	// GBJPEG_JPG_Decode
	//////////////////////////////
	Ptr_GBJPEG_JPG_Decode = (int (*)(
			unsigned char *ImgBuffer,
			unsigned long BuffLen,
			unsigned char **RawImage,
			unsigned long *ImageWidth,
			unsigned long *ImageHeight,
			unsigned char *BitsPerPixel
	)) GetProcAddress(GbjpegDllPtr, "GBJPEG_JPG_Decode");
	if (Ptr_GBJPEG_JPG_Decode == NULL)
	{
		ValToRet = GBJPEG_ERROR_INITIALIZATION;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Load: : Unable to load Ptr_GBJPEG_JPG_Decode: %s", dlerror());
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSFEJavaWrapperLibrary_Load: Ptr_GBJPEG_JPG_Decode loaded\n");

	//////////////////////////////
	// GBJPEG_JP2_Set_Dpi
	//////////////////////////////
	Ptr_GBJPEG_JP2_Set_Dpi = (void (*)(int)) GetProcAddress(GbjpegDllPtr, "GBJPEG_JP2_Set_Dpi");
	if (Ptr_GBJPEG_JP2_Set_Dpi == NULL)
	{
		ValToRet = GBJPEG_ERROR_INITIALIZATION;
		sprintf(Gbjpeg_JNI_LastErrorString,
		        "Load: : Unable to load Ptr_GBJPEG_JP2_Set_Dpi: %s", dlerror());
		a_printf ("%s\n", Gbjpeg_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("GBMSFEJavaWrapperLibrary_Load: Ptr_GBJPEG_JP2_Set_Dpi loaded\n");

	return ValToRet;
}

} // extern "C"

