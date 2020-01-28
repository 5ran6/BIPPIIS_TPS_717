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
#include "Bozorth_dll.h"
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
#define a_printf(...) __android_log_print(ANDROID_LOG_DEBUG, "BOZORTH_LOG", __VA_ARGS__);

//-------------------------------------------------------------//
//-------------------------------------------------------------//
// GLOBALS
//-------------------------------------------------------------//
//-------------------------------------------------------------//
#define BOZORTH_NO_ERROR 0
#define BOZORTH_ERROR 1
#define BOZORTH_MAX_MINUTIAE 1000
HMODULE BozorthDllPtr = NULL;
char Bozorth_JNI_LastErrorString[1000];

int (__stdcall *Ptr_bozDirectCall)(int maxminutiae,
											MINUTIAE *probe,
											MINUTIAE *gallery);										


//-------------------------------------------------------------//
//-------------------------------------------------------------//
// FUNCTIONS
//-------------------------------------------------------------//
//-------------------------------------------------------------//
extern "C"
{

JNIEXPORT jstring JNICALL Java_com_greenbit_bozorth_BozorthJavaWrapperLibrary_GetLastErrorString(
		JNIEnv *env,
		jobject /* this */) {
	return env->NewStringUTF(Bozorth_JNI_LastErrorString);
}

JNIEXPORT jint JNICALL Java_com_greenbit_bozorth_BozorthJavaWrapperLibrary_Unload(
		JNIEnv *env,
		jobject /* this */) {
	int ValToRet = BOZORTH_NO_ERROR;

	if (BozorthDllPtr != NULL) {
		FreeLibrary(BozorthDllPtr);
	} else {
		sprintf(Bozorth_JNI_LastErrorString,
				"Unload: GbBozorthDllPtr is NULL");
		a_printf ("%s\n", Bozorth_JNI_LastErrorString);
        ValToRet = BOZORTH_ERROR;
	}

	//Ptr_GBBOZORTH_GetImageQuality = NULL;

	BozorthDllPtr = NULL;

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_bozorth_BozorthJavaWrapperLibrary_Load(
		JNIEnv *env,
		jobject /* this */) {
	int ValToRet = BOZORTH_NO_ERROR;

	Dl_info pathInfo;
	dladdr((void *) Java_com_greenbit_bozorth_BozorthJavaWrapperLibrary_Load, &pathInfo);
	a_printf("Load: pathName = %s", pathInfo.dli_fname);

	//////////////////////////////
	// free library
	//////////////////////////////
	if (BozorthDllPtr != NULL)
		Java_com_greenbit_bozorth_BozorthJavaWrapperLibrary_Unload(env, nullptr);

	//////////////////////////////
	// load library
	//////////////////////////////
	BozorthDllPtr = LoadLibrary("libBozorth.so");
	if (BozorthDllPtr == NULL) {
		ValToRet = BOZORTH_ERROR;
		sprintf(Bozorth_JNI_LastErrorString,
				"Load: Unable to load BOZORTH: %s", dlerror());
		a_printf ("%s\n", Bozorth_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: BOZORTH loaded\n");

	
	//////////////////////////////
	// GBBOZORTH_direct_call
	//////////////////////////////
    Ptr_bozDirectCall = (int (*)( int maxminutiae,
										  MINUTIAE *probe,
										  MINUTIAE *gallery
	)) GetProcAddress(BozorthDllPtr, "boz_direct_call");
	if (Ptr_bozDirectCall == NULL) {
		ValToRet = BOZORTH_ERROR;
		sprintf(Bozorth_JNI_LastErrorString,
				"Load: Unable to load Ptr_bozDirectCall: %s", dlerror());
		a_printf ("%s\n", Bozorth_JNI_LastErrorString);
		return ValToRet;
	}
	a_printf ("Load: bozDirectCall loaded\n");

	return ValToRet;
}

JNIEXPORT jint JNICALL Java_com_greenbit_bozorth_BozorthJavaWrapperLibrary_bozDirectCall(
        JNIEnv *env,
        jobject thisObj,
        // INPUT
        jint jMaxminutiae,
		jobjectArray jMinutiaeProbe,
        jint jProbeLength,
        jobjectArray jMinutiaeGallery,
        jint jGalleryLength,
        // OUTPUT
        jobject jScore)
{
    int ValToRet = BOZORTH_NO_ERROR;
    int score;
    //////////////////////////////////
    // check functions
    //////////////////////////////////
    if (BozorthDllPtr == NULL)
    {
        ValToRet = BOZORTH_ERROR;
        sprintf(Bozorth_JNI_LastErrorString,
                "bozDirectCall: BozorthDllPtr is NULL");
        a_printf ("%s\n", Bozorth_JNI_LastErrorString);
        return ValToRet;
    }
/*******************************************************/
    if (Ptr_bozDirectCall == NULL)
    {
        ValToRet = BOZORTH_ERROR;
        sprintf(Bozorth_JNI_LastErrorString,
                "bozDirectCall: Ptr_BOZORTH_direct_call is NULL");
        a_printf ("%s\n", Bozorth_JNI_LastErrorString);
        return ValToRet;
    }

    //////////////////////////////
    // get C parameters from
    // java ones
    //////////////////////////////

    //////////////////////////////////
    // call function
    //////////////////////////////////
    int	ImageQuality;
    MINUTIAE *probe = NULL;
    MINUTIAE *gallery = NULL;


    if (jMinutiaeProbe != nullptr)
    {
        probe = new MINUTIAE;//1000
        probe->list = reinterpret_cast<MINUTIA **>(new MINUTIA[(int)jProbeLength]);
        probe->alloc = (int)jProbeLength;
        probe->num = (int)jProbeLength;

        //int ListSize = (env)->GetArrayLength(jMinutiaeList);
        for (int i=0; i<jProbeLength; i++)
        {
            probe->list[i] = new MINUTIA;

            jobject obj = (jobject) env->GetObjectArrayElement(jMinutiaeProbe, i);
            jclass cls = env->GetObjectClass(obj);

            //jfieldID appearingFieldId = env->GetFieldID(cls, "appearing", "I");
            //probe->list[i]->appearing = env->GetIntField(obj, appearingFieldId);

            jfieldID xFieldId = env->GetFieldID(cls, "XCoord", "I");
            probe->list[i]->x = env->GetIntField(obj, xFieldId);

            jfieldID yFieldId = env->GetFieldID(cls, "YCoord", "I");
            probe->list[i]->y = env->GetIntField(obj, yFieldId);

            jfieldID directionFieldId = env->GetFieldID(cls, "Direction", "I");
            probe->list[i]->direction = env->GetIntField(obj, directionFieldId);

            jfieldID reliabilityFieldId = env->GetFieldID(cls, "Reliability", "D");
            probe->list[i]->reliability = env->GetDoubleField(obj, reliabilityFieldId);

            jfieldID typeFieldId = env->GetFieldID(cls, "Type", "I");
            probe->list[i]->type = env->GetIntField(obj, typeFieldId);
        }


    }

    if (jMinutiaeGallery != nullptr)
    {
        gallery = new MINUTIAE;//1000
        gallery->list = reinterpret_cast<MINUTIA **>(new MINUTIA[(int)jGalleryLength]);
        gallery->alloc = (int)jGalleryLength;
        gallery->num = (int)jGalleryLength;

        //int ListSize = (env)->GetArrayLength(jMinutiaeList);
        for (int i=0; i<jGalleryLength; i++) {
            gallery->list[i] = new MINUTIA;

            jobject obj = (jobject) env->GetObjectArrayElement(jMinutiaeGallery, i);
            jclass cls = env->GetObjectClass(obj);

            //jfieldID appearingFieldId = env->GetFieldID(cls, "appearing", "I");
            //probe->list[i]->appearing = env->GetIntField(obj, appearingFieldId);

            jfieldID xFieldId = env->GetFieldID(cls, "XCoord", "I");
            gallery->list[i]->x = env->GetIntField(obj, xFieldId);

            jfieldID yFieldId = env->GetFieldID(cls, "YCoord", "I");
            gallery->list[i]->y = env->GetIntField(obj, yFieldId);

            jfieldID directionFieldId = env->GetFieldID(cls, "Direction", "I");
            gallery->list[i]->direction = env->GetIntField(obj, directionFieldId);

            jfieldID reliabilityFieldId = env->GetFieldID(cls, "Reliability", "D");
            gallery->list[i]->reliability = env->GetDoubleField(obj, reliabilityFieldId);

            jfieldID typeFieldId = env->GetFieldID(cls, "Type", "I");
            gallery->list[i]->type = env->GetIntField(obj, typeFieldId);
        }
    }

    score = Ptr_bozDirectCall((int)jMaxminutiae,
                                probe,
                                gallery);

    GBANUTIL_GetJIntExchangeFromCInt((int) (score),jScore,env);

    for (int i=0; i<jProbeLength; i++)
    {
        delete probe->list[i];
    }

    for (int i=0; i<jGalleryLength; i++)
    {
        delete gallery->list[i];
    }

    delete probe;
    delete gallery;

    return ValToRet;
}

} // extern "C"
												