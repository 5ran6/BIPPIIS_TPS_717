#include "GB_JNI_Utils.h"
//-------------------------------------------------------------//
//-------------------------------------------------------------//
// INCLUDE
//-------------------------------------------------------------//
//-------------------------------------------------------------//

//-------------------------------------------------------------//
//-------------------------------------------------------------//
// DEFINES
//-------------------------------------------------------------//
//-------------------------------------------------------------//
#define a_printf(...) __android_log_print(ANDROID_LOG_DEBUG, "AN2000_LOG", __VA_ARGS__);

//-------------------------------------------------------------//
//-------------------------------------------------------------//
// FUNCTIONS
//-------------------------------------------------------------//
//-------------------------------------------------------------//
void GBANUTIL_GetStringFromJString(jstring jSource, char **pDestination, JNIEnv *env)
{
	if (jSource == NULL)
	{
		*pDestination = NULL;
		return;
	}

	const char *constDestination;
	char *Destination = *pDestination;
	constDestination = (env)->GetStringUTFChars(jSource, 0);
	*pDestination = new char[(env)->GetStringUTFLength(jSource) + 1];
	memcpy((*pDestination), constDestination, (env)->GetStringUTFLength(jSource));
	(*pDestination)[(env)->GetStringUTFLength(jSource)] = 0;
	env->ReleaseStringUTFChars(jSource, constDestination);

	return;
}

int GBANUTIL_GetJAmpStructArrayFromCAmpList(JNIEnv *env, jobjectArray Destination,
                                            FING_AMP_LIST *Source, char *logStr)
{
	if (Source == NULL)
	{
		sprintf(logStr, "GBANUTIL_GetJAmpStructArrayFromCAmpList: Source is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if (Destination == NULL)
	{
		sprintf(logStr, "GBANUTIL_GetJAmpStructArrayFromCAmpList: Destination is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if ((env)->GetArrayLength(Destination) < 4)
	{
		sprintf(logStr, "GBANUTIL_GetJAmpStructArrayFromCAmpList: Destination len is < 4");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	jclass cls = env->GetObjectClass((jobject) env->GetObjectArrayElement(Destination, 0));
	jfieldID fingIdFieldId = env->GetFieldID(cls, "FingID", "Ljava/lang/String;");
	jfieldID ampCodeFieldId = env->GetFieldID(cls, "AmputationCode", "Ljava/lang/String;");

	// init elements
	for (int i = 0; i < (env)->GetArrayLength(Destination); i++)
	{
		jobject obj = (jobject) env->GetObjectArrayElement(Destination, i);
		env->SetObjectField(obj, fingIdFieldId, (jobject) (env->NewStringUTF("")));
		env->SetObjectField(obj, ampCodeFieldId, (jobject) (env->NewStringUTF("")));
	}

	// set elements
	for (int i = 0; i < Source->num_fingers; i++)
	{
		Source->fingers[i]->fing_id[2] = 0;
		Source->fingers[i]->amp_code[2] = 0;
		jobject obj = (jobject) env->GetObjectArrayElement(Destination, i);
		env->SetObjectField(obj, fingIdFieldId,
		                    (jobject) (env->NewStringUTF(Source->fingers[i]->fing_id)));
		env->SetObjectField(obj, ampCodeFieldId,
		                    (jobject) (env->NewStringUTF(Source->fingers[i]->amp_code)));
	}
	return AN2K_DLL_NO_ERROR;
}

int GBANUTIL_GetCAmpListFromJAmpStructArray(JNIEnv *env, jobjectArray Source,
                                            FING_AMP_LIST *Destination, char *logStr)
{
	if (Source == NULL)
	{
		sprintf(logStr, "GBANUTIL_GetCAmpListFromJAmpStructArray: Source is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if (Destination == NULL)
	{
		sprintf(logStr, "GBANUTIL_GetCAmpListFromJAmpStructArray: Destination is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if ((env)->GetArrayLength(Source) <= 0)
	{
		sprintf(logStr, "GBANUTIL_GetCAmpListFromJAmpStructArray: Source len is <= 0");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}

	Destination->fingers = (FING_AMP **) malloc((env)->GetArrayLength(Source) * sizeof(FING_AMP *));
	if (Destination->fingers == NULL)
	{
		sprintf(logStr,
		        "GBANUTIL_GetCAmpListFromJAmpStructArray: Memory error in creating Destination->fingers");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_MEMORY;
	}
	// copy
	for (int i = 0; i < (env)->GetArrayLength(Source); i++)
	{
		Destination->fingers[i] = (FING_AMP *) malloc(sizeof(FING_AMP));
		if (Destination->fingers[i] == NULL)
		{
			sprintf(logStr,
			        "GBANUTIL_GetCAmpListFromJAmpStructArray: Memory error in creating Destination->fingers at %d",
			        i);
			a_printf ("%s\n", logStr);
			return AN2K_DLL_MEMORY;
		}
		jobject obj = (jobject) env->GetObjectArrayElement(Source, i);
		jclass cls = env->GetObjectClass(obj);
		jfieldID fingIdFieldId = env->GetFieldID(cls, "FingID", "Ljava/lang/String;");
		jfieldID ampCodeFieldId = env->GetFieldID(cls, "AmputationCode", "Ljava/lang/String;");
		jstring jFingId = (jstring) env->GetObjectField(obj, fingIdFieldId);
		jstring jAmpCode = (jstring) env->GetObjectField(obj, ampCodeFieldId);
		char *fingId, *ampCode;
		GBANUTIL_GetStringFromJString(jFingId, &fingId, env);
		GBANUTIL_GetStringFromJString(jAmpCode, &ampCode, env);
		fingId[2] = 0;
		ampCode[2] = 0;
		strcpy(Destination->fingers[i]->fing_id, fingId);
		strcpy(Destination->fingers[i]->amp_code, ampCode);
		free(fingId);
		free(ampCode);
	}
	Destination->num_fingers = (env)->GetArrayLength(Source);
	return AN2K_DLL_NO_ERROR;
}

void GBANUTIL_FreeAmpListFields(FING_AMP_LIST *ampList)
{
	if (ampList == NULL) return;
	for (int i = 0; i < ampList->num_fingers; i++)
	{
		free(ampList->fingers[i]);
	}
	free(ampList->fingers);
}

void GBANUTIL_FreeSegmBoundsFields(FOUR_FING_BOUND *fingSegmList)
{
	if (fingSegmList == NULL) return;
	for (int i = 0; i < fingSegmList->num_fingers; i++)
	{
		free(fingSegmList->fingers[i]);
	}
	free(fingSegmList->fingers);
}

int GBANUTIL_GetCSegmBoundsListFromJSegmBoundsStructArray(JNIEnv *env, jobjectArray Source,
                                                          FOUR_FING_BOUND *Destination,
                                                          char *logStr)
{
	if (Source == NULL)
	{
		sprintf(logStr, "GBANUTIL_GetCSegmBoundsListFromJSegmBoundsStructArray: Source is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if (Destination == NULL)
	{
		sprintf(logStr,
		        "GBANUTIL_GetCSegmBoundsListFromJSegmBoundsStructArray: Destination is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if ((env)->GetArrayLength(Source) <= 0)
	{
		sprintf(logStr,
		        "GBANUTIL_GetCSegmBoundsListFromJSegmBoundsStructArray: Source len is <= 0");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}

	Destination->fingers = (FING_SEGMENT **) malloc(
			(env)->GetArrayLength(Source) * sizeof(FING_SEGMENT *));
	if (Destination->fingers == NULL)
	{
		sprintf(logStr,
		        "GBANUTIL_GetCSegmBoundsListFromJSegmBoundsStructArray: Memory error in creating Destination->fingers");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_MEMORY;
	}
	// copy
	for (int i = 0; i < (env)->GetArrayLength(Source); i++)
	{
		Destination->fingers[i] = (FING_SEGMENT *) malloc(sizeof(FING_SEGMENT));
		if (Destination->fingers[i] == NULL)
		{
			sprintf(logStr,
			        "GBANUTIL_GetCSegmBoundsListFromJSegmBoundsStructArray: Memory error in creating Destination->fingers at %d",
			        i);
			a_printf ("%s\n", logStr);
			return AN2K_DLL_MEMORY;
		}
		jobject obj = (jobject) env->GetObjectArrayElement(Source, i);
		jclass cls = env->GetObjectClass(obj);
		jfieldID fingIdFieldId = env->GetFieldID(cls, "FingID", "Ljava/lang/String;");
		jfieldID leftFieldId = env->GetFieldID(cls, "Left", "Ljava/lang/String;");
		jfieldID rightFieldId = env->GetFieldID(cls, "Right", "Ljava/lang/String;");
		jfieldID topFieldId = env->GetFieldID(cls, "Top", "Ljava/lang/String;");
		jfieldID bottomFieldId = env->GetFieldID(cls, "Bottom", "Ljava/lang/String;");
		jstring jFingId = (jstring) env->GetObjectField(obj, fingIdFieldId);
		jstring jLeft = (jstring) env->GetObjectField(obj, leftFieldId);
		jstring jRight = (jstring) env->GetObjectField(obj, rightFieldId);
		jstring jTop = (jstring) env->GetObjectField(obj, topFieldId);
		jstring jBottom = (jstring) env->GetObjectField(obj, bottomFieldId);
		char *fingId, *leftCode, *rightCode, *topCode, *bottomCode;
		GBANUTIL_GetStringFromJString(jFingId, &fingId, env);
		GBANUTIL_GetStringFromJString(jLeft, &leftCode, env);
		GBANUTIL_GetStringFromJString(jRight, &rightCode, env);
		GBANUTIL_GetStringFromJString(jTop, &topCode, env);
		GBANUTIL_GetStringFromJString(jBottom, &bottomCode, env);
		fingId[2] = 0;
		leftCode[4] = 0;
		rightCode[4] = 0;
		topCode[4] = 0;
		bottomCode[4] = 0;
		strcpy(Destination->fingers[i]->fing_id, fingId);
		strcpy(Destination->fingers[i]->left_bound, leftCode);
		strcpy(Destination->fingers[i]->right_bound, rightCode);
		strcpy(Destination->fingers[i]->top_bound, topCode);
		strcpy(Destination->fingers[i]->bottom_bound, bottomCode);
		free(fingId);
		free(leftCode);
		free(rightCode);
		free(topCode);
		free(bottomCode);
	}
	Destination->num_fingers = (env)->GetArrayLength(Source);
	return AN2K_DLL_NO_ERROR;
}

int GBANUTIL_GetJSegmBoundsStructArrayFromCSegmBoundsList(JNIEnv *env, jobjectArray Destination,
                                                          FOUR_FING_BOUND *Source, char *logStr)
{
	if (Source == NULL)
	{
		sprintf(logStr, "GBANUTIL_GetJSegmBoundsStructArrayFromCSegmBoundsList: Source is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if (Destination == NULL)
	{
		sprintf(logStr,
		        "GBANUTIL_GetJSegmBoundsStructArrayFromCSegmBoundsList: Destination is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if ((env)->GetArrayLength(Destination) < 4)
	{
		sprintf(logStr,
		        "GBANUTIL_GetJSegmBoundsStructArrayFromCSegmBoundsList: Destination len is < 4");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	jclass cls = env->GetObjectClass((jobject) env->GetObjectArrayElement(Destination, 0));
	jfieldID fingIdFieldId = env->GetFieldID(cls, "FingID", "Ljava/lang/String;");
	jfieldID leftCodeFieldId = env->GetFieldID(cls, "Left", "Ljava/lang/String;");
	jfieldID rightCodeFieldId = env->GetFieldID(cls, "Right", "Ljava/lang/String;");
	jfieldID topCodeFieldId = env->GetFieldID(cls, "Top", "Ljava/lang/String;");
	jfieldID bottomCodeFieldId = env->GetFieldID(cls, "Bottom", "Ljava/lang/String;");

	// init elements
	for (int i = 0; i < (env)->GetArrayLength(Destination); i++)
	{
		jobject obj = (jobject) env->GetObjectArrayElement(Destination, i);
		env->SetObjectField(obj, fingIdFieldId, (jobject) (env->NewStringUTF("")));
		env->SetObjectField(obj, leftCodeFieldId, (jobject) (env->NewStringUTF("")));
		env->SetObjectField(obj, rightCodeFieldId, (jobject) (env->NewStringUTF("")));
		env->SetObjectField(obj, topCodeFieldId, (jobject) (env->NewStringUTF("")));
		env->SetObjectField(obj, bottomCodeFieldId, (jobject) (env->NewStringUTF("")));
	}

	// set elements
	for (int i = 0; i < Source->num_fingers; i++)
	{
		Source->fingers[i]->fing_id[2] = 0;
		Source->fingers[i]->left_bound[4] = 0;
		Source->fingers[i]->right_bound[4] = 0;
		Source->fingers[i]->top_bound[4] = 0;
		Source->fingers[i]->bottom_bound[4] = 0;
		jobject obj = (jobject) env->GetObjectArrayElement(Destination, i);
		env->SetObjectField(obj, fingIdFieldId,
		                    (jobject) (env->NewStringUTF(Source->fingers[i]->fing_id)));
		env->SetObjectField(obj, leftCodeFieldId,
		                    (jobject) (env->NewStringUTF(Source->fingers[i]->left_bound)));
		env->SetObjectField(obj, rightCodeFieldId,
		                    (jobject) (env->NewStringUTF(Source->fingers[i]->right_bound)));
		env->SetObjectField(obj, topCodeFieldId,
		                    (jobject) (env->NewStringUTF(Source->fingers[i]->top_bound)));
		env->SetObjectField(obj, bottomCodeFieldId,
		                    (jobject) (env->NewStringUTF(Source->fingers[i]->bottom_bound)));
	}
	return AN2K_DLL_NO_ERROR;
}

int GBANUTIL_GetCFingerQualityListFromJFingerQualityStructArray(JNIEnv *env, jobjectArray Source,
                                                                FOUR_FING_QUALITY *Destination,
                                                                char *logStr)
{
	if (Source == NULL)
	{
		sprintf(logStr,
		        "GBANUTIL_GetCFingerQualityListFromJFingerQualityStructArray: Source is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if (Destination == NULL)
	{
		sprintf(logStr,
		        "GBANUTIL_GetCFingerQualityListFromJFingerQualityStructArray: Destination is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if ((env)->GetArrayLength(Source) <= 0)
	{
		sprintf(logStr,
		        "GBANUTIL_GetCFingerQualityListFromJFingerQualityStructArray: Source len is <= 0");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}

	Destination->fingers = (FING_QUALITY **) malloc(
			(env)->GetArrayLength(Source) * sizeof(FING_QUALITY *));
	if (Destination->fingers == NULL)
	{
		sprintf(logStr,
		        "GBANUTIL_GetCFingerQualityListFromJFingerQualityStructArray: Memory error in creating Destination->fingers");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_MEMORY;
	}
	// copy
	for (int i = 0; i < (env)->GetArrayLength(Source); i++)
	{
		Destination->fingers[i] = (FING_QUALITY *) malloc(sizeof(FING_QUALITY));
		if (Destination->fingers[i] == NULL)
		{
			sprintf(logStr,
			        "GBANUTIL_GetCFingerQualityListFromJFingerQualityStructArray: Memory error in creating Destination->fingers at %d",
			        i);
			a_printf ("%s\n", logStr);
			return AN2K_DLL_MEMORY;
		}
		jobject obj = (jobject) env->GetObjectArrayElement(Source, i);
		jclass cls = env->GetObjectClass(obj);
		jfieldID fingIdFieldId = env->GetFieldID(cls, "FingID", "Ljava/lang/String;");
		jfieldID qualityFieldId = env->GetFieldID(cls, "Quality", "Ljava/lang/String;");
		jstring jFingId = (jstring) env->GetObjectField(obj, fingIdFieldId);
		jstring jQuality = (jstring) env->GetObjectField(obj, qualityFieldId);
		char *fingId, *quality;
		GBANUTIL_GetStringFromJString(jFingId, &fingId, env);
		GBANUTIL_GetStringFromJString(jQuality, &quality, env);
		fingId[2] = 0;
		quality[1] = 0;
		strcpy(Destination->fingers[i]->fing_id, fingId);
		strcpy(Destination->fingers[i]->quality, quality);
		free(fingId);
		free(quality);
	}
	Destination->num_fingers = (env)->GetArrayLength(Source);
	return AN2K_DLL_NO_ERROR;
}

int
GBANUTIL_GetJFingerQualityStructArrayFromCFingerQualityList(JNIEnv *env, jobjectArray Destination,
                                                            FOUR_FING_QUALITY *Source, char *logStr)
{
	if (Source == NULL)
	{
		sprintf(logStr,
		        "GBANUTIL_GetJFingerQualityStructArrayFromCFingerQualityList: Source is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if (Destination == NULL)
	{
		sprintf(logStr,
		        "GBANUTIL_GetJFingerQualityStructArrayFromCFingerQualityList: Destination is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if ((env)->GetArrayLength(Destination) < 4)
	{
		sprintf(logStr,
		        "GBANUTIL_GetJFingerQualityStructArrayFromCFingerQualityList: Destination len is < 4");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	jclass cls = env->GetObjectClass((jobject) env->GetObjectArrayElement(Destination, 0));
	jfieldID fingIdFieldId = env->GetFieldID(cls, "FingID", "Ljava/lang/String;");
	jfieldID qualityFieldId = env->GetFieldID(cls, "Quality", "Ljava/lang/String;");

	// init elements
	for (int i = 0; i < (env)->GetArrayLength(Destination); i++)
	{
		jobject obj = (jobject) env->GetObjectArrayElement(Destination, i);
		env->SetObjectField(obj, fingIdFieldId, (jobject) (env->NewStringUTF("")));
		env->SetObjectField(obj, qualityFieldId, (jobject) (env->NewStringUTF("")));
	}

	// set elements
	for (int i = 0; i < Source->num_fingers; i++)
	{
		Source->fingers[i]->fing_id[2] = 0;
		Source->fingers[i]->quality[1] = 0;
		jobject obj = (jobject) env->GetObjectArrayElement(Destination, i);
		env->SetObjectField(obj, fingIdFieldId,
		                    (jobject) (env->NewStringUTF(Source->fingers[i]->fing_id)));
		env->SetObjectField(obj, qualityFieldId,
		                    (jobject) (env->NewStringUTF(Source->fingers[i]->quality)));
	}
	return AN2K_DLL_NO_ERROR;
}

void GBANUTIL_FreeFingerQualityFields(FOUR_FING_QUALITY *fingQualityList)
{
	if (fingQualityList == NULL) return;
	for (int i = 0; i < fingQualityList->num_fingers; i++)
	{
		free(fingQualityList->fingers[i]);
	}
	free(fingQualityList->fingers);
}

int GBANUTIL_GetCAlternateFingerQualityListFromJFingerAlternateQualityStructArray(JNIEnv *env,
                                                                                  jobjectArray Source,
                                                                                  ALT_QUAL_LIST *Destination,
                                                                                  char *logStr)
{
	if (Source == NULL)
	{
		sprintf(logStr,
		        "GBANUTIL_GetCAlternateFingerQualityListFromJFingerAlternateQualityStructArray: Source is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if (Destination == NULL)
	{
		sprintf(logStr,
		        "GBANUTIL_GetCAlternateFingerQualityListFromJFingerAlternateQualityStructArray: Destination is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if ((env)->GetArrayLength(Source) <= 0)
	{
		sprintf(logStr,
		        "GBANUTIL_GetCAlternateFingerQualityListFromJFingerAlternateQualityStructArray: Source len is <= 0");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}

	Destination->fingers = (ALT_QUAL **) malloc((env)->GetArrayLength(Source) * sizeof(ALT_QUAL *));
	if (Destination->fingers == NULL)
	{
		sprintf(logStr,
		        "GBANUTIL_GetCAlternateFingerQualityListFromJFingerAlternateQualityStructArray: Memory error in creating Destination->fingers");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_MEMORY;
	}
	// copy
	for (int i = 0; i < (env)->GetArrayLength(Source); i++)
	{
		Destination->fingers[i] = (ALT_QUAL *) malloc(sizeof(ALT_QUAL));
		if (Destination->fingers[i] == NULL)
		{
			sprintf(logStr,
			        "GBANUTIL_GetCAlternateFingerQualityListFromJFingerAlternateQualityStructArray: Memory error in creating Destination->fingers at %d",
			        i);
			a_printf ("%s\n", logStr);
			return AN2K_DLL_MEMORY;
		}
		jobject obj = (jobject) env->GetObjectArrayElement(Source, i);
		jclass cls = env->GetObjectClass(obj);
		jfieldID fingIdFieldId = env->GetFieldID(cls, "FingID", "Ljava/lang/String;");
		jfieldID qualityFieldId = env->GetFieldID(cls, "Quality", "Ljava/lang/String;");
		jfieldID vendorFieldId = env->GetFieldID(cls, "VendorId", "Ljava/lang/String;");
		jfieldID algorithmFieldId = env->GetFieldID(cls, "AlgorithmId", "Ljava/lang/String;");
		jstring jFingId = (jstring) env->GetObjectField(obj, fingIdFieldId);
		jstring jQuality = (jstring) env->GetObjectField(obj, qualityFieldId);
		jstring jVendor = (jstring) env->GetObjectField(obj, vendorFieldId);
		jstring jAlgorithm = (jstring) env->GetObjectField(obj, algorithmFieldId);
		char *fingId, *quality, *vendor, *algorithm;
		GBANUTIL_GetStringFromJString(jFingId, &fingId, env);
		GBANUTIL_GetStringFromJString(jQuality, &quality, env);
		GBANUTIL_GetStringFromJString(jVendor, &vendor, env);
		GBANUTIL_GetStringFromJString(jAlgorithm, &algorithm, env);
		fingId[2] = 0;
		quality[3] = 0;
		vendor[5] = 0;
		algorithm[5] = 0;
		strcpy(Destination->fingers[i]->fing_id, fingId);
		strcpy(Destination->fingers[i]->quality, quality);
		strcpy(Destination->fingers[i]->Vendor_ID, vendor);
		strcpy(Destination->fingers[i]->Algorithm_ID, algorithm);
		free(fingId);
		free(quality);
		free(vendor);
		free(algorithm);
	}
	Destination->num_fingers = (env)->GetArrayLength(Source);
	return AN2K_DLL_NO_ERROR;
}

int GBANUTIL_GetJAlternateFingerQualityStructArrayFromCAlternateFingerQualityList(JNIEnv *env,
                                                                                  jobjectArray Destination,
                                                                                  ALT_QUAL_LIST *Source,
                                                                                  char *logStr)
{
	if (Source == NULL)
	{
		sprintf(logStr,
		        "GBANUTIL_GetJAlternateFingerQualityStructArrayFromCAlternateFingerQualityList: Source is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if (Destination == NULL)
	{
		sprintf(logStr,
		        "GBANUTIL_GetJAlternateFingerQualityStructArrayFromCAlternateFingerQualityList: Destination is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if ((env)->GetArrayLength(Destination) < 4)
	{
		sprintf(logStr,
		        "GBANUTIL_GetJAlternateFingerQualityStructArrayFromCAlternateFingerQualityList: Destination len is < 4");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	jclass cls = env->GetObjectClass((jobject) env->GetObjectArrayElement(Destination, 0));
	jfieldID fingIdFieldId = env->GetFieldID(cls, "FingID", "Ljava/lang/String;");
	jfieldID qualityFieldId = env->GetFieldID(cls, "Quality", "Ljava/lang/String;");
	jfieldID vendorFieldId = env->GetFieldID(cls, "VendorId", "Ljava/lang/String;");
	jfieldID algorithmFieldId = env->GetFieldID(cls, "AlgorithmId", "Ljava/lang/String;");

	// init elements
	for (int i = 0; i < (env)->GetArrayLength(Destination); i++)
	{
		jobject obj = (jobject) env->GetObjectArrayElement(Destination, i);
		env->SetObjectField(obj, fingIdFieldId, (jobject) (env->NewStringUTF("")));
		env->SetObjectField(obj, qualityFieldId, (jobject) (env->NewStringUTF("")));
		env->SetObjectField(obj, vendorFieldId, (jobject) (env->NewStringUTF("")));
		env->SetObjectField(obj, algorithmFieldId, (jobject) (env->NewStringUTF("")));
	}

	// set elements
	for (int i = 0; i < Source->num_fingers; i++)
	{
		Source->fingers[i]->fing_id[2] = 0;
		Source->fingers[i]->quality[3] = 0;
		Source->fingers[i]->Vendor_ID[5] = 0;
		Source->fingers[i]->Algorithm_ID[5] = 0;
		jobject obj = (jobject) env->GetObjectArrayElement(Destination, i);
		env->SetObjectField(obj, fingIdFieldId,
		                    (jobject) (env->NewStringUTF(Source->fingers[i]->fing_id)));
		env->SetObjectField(obj, qualityFieldId,
		                    (jobject) (env->NewStringUTF(Source->fingers[i]->quality)));
		env->SetObjectField(obj, vendorFieldId, (jobject) (Source->fingers[i]->Vendor_ID));
		env->SetObjectField(obj, algorithmFieldId,
		                    (jobject) (env->NewStringUTF(Source->fingers[i]->Algorithm_ID)));
	}
	return AN2K_DLL_NO_ERROR;
}

void GBANUTIL_FreeAlternateFingerQualityFields(ALT_QUAL_LIST *fingQualityList)
{
	if (fingQualityList == NULL) return;
	for (int i = 0; i < fingQualityList->num_fingers; i++)
	{
		free(fingQualityList->fingers[i]);
	}
	free(fingQualityList->fingers);
}


int GBANUTIL_GetCPalmQualityListFromJPalmQualityStructArray(JNIEnv *env, jobjectArray Source,
                                                            PALM_QUAL_LIST *Destination,
                                                            char *logStr)
{
	if (Source == NULL)
	{
		sprintf(logStr,
		        "GBANUTIL_GetCPalmQualityListFromJPalmQualityStructArray: Source is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if (Destination == NULL)
	{
		sprintf(logStr,
		        "GBANUTIL_GetCPalmQualityListFromJPalmQualityStructArray: Destination is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if ((env)->GetArrayLength(Source) <= 0)
	{
		sprintf(logStr,
		        "GBANUTIL_GetCPalmQualityListFromJPalmQualityStructArray: Source len is <= 0");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}

	Destination->Algorhythms = (PALM_QUAL **) malloc((env)->GetArrayLength(Source) * sizeof(PALM_QUAL *));
	if (Destination->Algorhythms == NULL)
	{
		sprintf(logStr,
		        "GBANUTIL_GetCPalmQualityListFromJPalmQualityStructArray: Memory error in creating Destination->Algorhythms");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_MEMORY;
	}
	// copy
	for (int i = 0; i < (env)->GetArrayLength(Source); i++)
	{
		Destination->Algorhythms[i] = (PALM_QUAL *) malloc(sizeof(PALM_QUAL));
		if (Destination->Algorhythms[i] == NULL)
		{
			sprintf(logStr,
			        "GBANUTIL_GetCPalmQualityListFromJPalmQualityStructArray: Memory error in creating Destination->Algorhythms at %d",
			        i);
			a_printf ("%s\n", logStr);
			return AN2K_DLL_MEMORY;
		}
		jobject obj = (jobject) env->GetObjectArrayElement(Source, i);
		if (obj == NULL)
		{
			sprintf(logStr,
			        "GBANUTIL_GetCPalmQualityListFromJPalmQualityStructArray: Source at %d is null",
			        i);
			a_printf ("%s\n", logStr);
			return AN2K_DLL_BAD_PARAMETER;
		}
		jclass cls = env->GetObjectClass(obj);
		jfieldID palmIdFieldId = env->GetFieldID(cls, "PalmID", "Ljava/lang/String;");
		jfieldID qualityFieldId = env->GetFieldID(cls, "Quality", "Ljava/lang/String;");
		jfieldID vendorFieldId = env->GetFieldID(cls, "VendorId", "Ljava/lang/String;");
		jfieldID algorithmFieldId = env->GetFieldID(cls, "AlgorithmId", "Ljava/lang/String;");
		jstring jPalmId = (jstring) env->GetObjectField(obj, palmIdFieldId);
		jstring jQuality = (jstring) env->GetObjectField(obj, qualityFieldId);
		jstring jVendor = (jstring) env->GetObjectField(obj, vendorFieldId);
		jstring jAlgorithm = (jstring) env->GetObjectField(obj, algorithmFieldId);
		char *palmId, *quality, *vendor, *algorithm;
		GBANUTIL_GetStringFromJString(jPalmId, &palmId, env);
		GBANUTIL_GetStringFromJString(jQuality, &quality, env);
		GBANUTIL_GetStringFromJString(jVendor, &vendor, env);
		GBANUTIL_GetStringFromJString(jAlgorithm, &algorithm, env);
		palmId[2] = 0;
		quality[3] = 0;
		vendor[5] = 0;
		algorithm[5] = 0;
		strcpy(Destination->Algorhythms[i]->PalmID, palmId);
		strcpy(Destination->Algorhythms[i]->Quality, quality);
		strcpy(Destination->Algorhythms[i]->Vendor_ID, vendor);
		strcpy(Destination->Algorhythms[i]->Algorithm_ID, algorithm);
		free(palmId);
		free(quality);
		free(vendor);
		free(algorithm);
	}
	Destination->AlgorhythmsNumber = (env)->GetArrayLength(Source);
	return AN2K_DLL_NO_ERROR;
}

int GBANUTIL_GetJPalmQualityStructArrayFromCPalmQualityList(JNIEnv *env, jobjectArray Destination,
                                                            PALM_QUAL_LIST *Source, char *logStr)
{
	if (Source == NULL)
	{
		sprintf(logStr,
		        "GBANUTIL_GetJPalmQualityStructArrayFromCPalmQualityList: Source is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if (Destination == NULL)
	{
		sprintf(logStr,
		        "GBANUTIL_GetJPalmQualityStructArrayFromCPalmQualityList: Destination is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if ((env)->GetArrayLength(Destination) < 10)
	{
		sprintf(logStr,
		        "GBANUTIL_GetJPalmQualityStructArrayFromCPalmQualityList: Destination len is < 10");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	jclass cls = env->GetObjectClass((jobject) env->GetObjectArrayElement(Destination, 0));
	jfieldID palmIdFieldId = env->GetFieldID(cls, "PalmID", "Ljava/lang/String;");
	jfieldID qualityFieldId = env->GetFieldID(cls, "Quality", "Ljava/lang/String;");
	jfieldID vendorFieldId = env->GetFieldID(cls, "VendorId", "Ljava/lang/String;");
	jfieldID algorithmFieldId = env->GetFieldID(cls, "AlgorithmId", "Ljava/lang/String;");

	// init elements
	for (int i = 0; i < (env)->GetArrayLength(Destination); i++)
	{
		jobject obj = (jobject) env->GetObjectArrayElement(Destination, i);
		env->SetObjectField(obj, palmIdFieldId, (jobject) (env->NewStringUTF("")));
		env->SetObjectField(obj, qualityFieldId, (jobject) (env->NewStringUTF("")));
		env->SetObjectField(obj, vendorFieldId, (jobject) (env->NewStringUTF("")));
		env->SetObjectField(obj, algorithmFieldId, (jobject) (env->NewStringUTF("")));
	}

	// set elements
	for (int i = 0; i < Source->AlgorhythmsNumber; i++)
	{
		Source->Algorhythms[i]->PalmID[2] = 0;
		Source->Algorhythms[i]->Quality[3] = 0;
		Source->Algorhythms[i]->Vendor_ID[5] = 0;
		Source->Algorhythms[i]->Algorithm_ID[5] = 0;
		jobject obj = (jobject) env->GetObjectArrayElement(Destination, i);
		env->SetObjectField(obj, palmIdFieldId,
		                    (jobject) (env->NewStringUTF(Source->Algorhythms[i]->PalmID)));
		env->SetObjectField(obj, qualityFieldId,
		                    (jobject) (env->NewStringUTF(Source->Algorhythms[i]->Quality)));
		env->SetObjectField(obj, vendorFieldId, (jobject) (Source->Algorhythms[i]->Vendor_ID));
		env->SetObjectField(obj, algorithmFieldId,
		                    (jobject) (env->NewStringUTF(Source->Algorhythms[i]->Algorithm_ID)));
	}
	return AN2K_DLL_NO_ERROR;
}

void GBANUTIL_FreePalmQualityFields(PALM_QUAL_LIST *palmQualList)
{
	if (palmQualList == NULL) return;
	for (int i = 0; i < palmQualList->AlgorhythmsNumber; i++)
	{
		free(palmQualList->Algorhythms[i]);
	}
	free(palmQualList->Algorhythms);
}

void GBANUTIL_GetJBufferFromCBuffer(unsigned char *Source, int SourceLen, jobject Destination,
                                    JNIEnv *env)
{
	jclass cls = env->GetObjectClass((jobject) (Destination));
	jfieldID ValueFieldId = env->GetFieldID(cls, "Value", "[B");
	// init to null
	env->SetObjectField(Destination, ValueFieldId,NULL);
	jbyteArray jframe = (env)->NewByteArray(SourceLen);
	if (jframe == NULL)
	{
		return;
	}
	if (Source != NULL) (env)->SetByteArrayRegion(jframe, 0, SourceLen, (jbyte *) Source);

	env->SetObjectField(Destination, ValueFieldId,jframe);
	return;
}

void GBANUTIL_GetJIntExchangeFromCInt(int Source, jobject Destination, JNIEnv *env)
{
	jclass cls = env->GetObjectClass((jobject) (Destination));
	jfieldID ValueFieldId = env->GetFieldID(cls, "Value", "I");
	env->SetIntField(Destination, ValueFieldId,Source);
}

void GBANUTIL_GetJDoubleExchangeFromCDouble(double Source, jobject Destination, JNIEnv *env)
{
	jclass cls = env->GetObjectClass((jobject)(Destination));
	jfieldID ValueFieldId = env->GetFieldID(cls, "Value", "D");
	env->SetDoubleField(Destination, ValueFieldId,Source);
}

void GBANUTIL_GetJStringExchangeFromCString(char *Source, jobject Destination, JNIEnv *env)
{
	jclass cls = env->GetObjectClass((jobject) (Destination));
	jfieldID ValueFieldId = env->GetFieldID(cls, "Value", "Ljava/lang/String;");
	env->SetObjectField(Destination, ValueFieldId,(jobject) (env->NewStringUTF(Source)));
}



void
GBANUTIL_GetJStringArrayExchangeFromCStringArray(char **Source, int SourceLen, jobject Destination,
                                                 JNIEnv *env)
{
	jclass cls = env->GetObjectClass((jobject) (Destination));
	jfieldID ValueFieldId = env->GetFieldID(cls, "Value", "[Ljava/lang/String;");
	if (Source == NULL)
	{
		env->SetObjectField(Destination, ValueFieldId,(jobject)NULL);
		return;
	}
	jobjectArray Value = (jobjectArray)env->NewObjectArray(SourceLen,env->FindClass("java/lang/String"),NULL);
	for (int i = 0; i < SourceLen; i++)
	{
		env->SetObjectArrayElement(Value,i,env->NewStringUTF(Source[i]));
	}
	env->SetObjectField(Destination, ValueFieldId,(jobject)Value);
}

void GBANUTIL_GetCAnsiNistFromJAnsiNist(jobject jSource, ANSI_NIST **pDestination, JNIEnv *env)
{
	jclass cls = env->GetObjectClass((jobject)(jSource));
	jfieldID ValueFieldId = env->GetFieldID(cls, "Pointer", "J");
	*pDestination = (ANSI_NIST *)env->GetLongField(jSource,ValueFieldId);
}

void GBANUTIL_GetJAnsiNistFromCAnsiNist(ANSI_NIST *Source, jobject jDestination, JNIEnv *env)
{
	jclass cls = env->GetObjectClass((jobject)(jDestination));
	jfieldID ValueFieldId = env->GetFieldID(cls, "Pointer", "J");
	env->SetLongField(jDestination, ValueFieldId,(jlong)(Source));
}

void GBANUTIL_GetCRecordFromJRecord(jobject jSource, RECORD **pDestination, JNIEnv *env)
{
	jclass cls = env->GetObjectClass((jobject)(jSource));
	jfieldID ValueFieldId = env->GetFieldID(cls, "Pointer", "J");
	*pDestination = (RECORD *)env->GetLongField(jSource,ValueFieldId);
}

void GBANUTIL_GetJRecordFromCRecord(RECORD *Source, jobject jDestination, JNIEnv *env)
{
	jclass cls = env->GetObjectClass((jobject)(jDestination));
	jfieldID ValueFieldId = env->GetFieldID(cls, "Pointer", "J");
	env->SetLongField(jDestination, ValueFieldId,(jlong)(Source));
}

void GBANUTIL_GetCSubfieldFromJSubfield(jobject jSource, SUBFIELD **pDestination, JNIEnv *env)
{
	jclass cls = env->GetObjectClass((jobject)(jSource));
	jfieldID ValueFieldId = env->GetFieldID(cls, "Pointer", "J");
	*pDestination = (SUBFIELD *)env->GetLongField(jSource,ValueFieldId);
}

void GBANUTIL_GetJSubfieldFromCSubfield(SUBFIELD *Source, jobject jDestination, JNIEnv *env)
{
	jclass cls = env->GetObjectClass((jobject)(jDestination));
	jfieldID ValueFieldId = env->GetFieldID(cls, "Pointer", "J");
	env->SetLongField(jDestination, ValueFieldId,(jlong)(Source));
}

void GBANUTIL_GetCFieldFromJField(jobject jSource, FIELD** pDestination, JNIEnv *env)
{
	jclass cls = env->GetObjectClass((jobject)(jSource));
	jfieldID ValueFieldId = env->GetFieldID(cls, "Pointer", "J");
	*pDestination = (FIELD *)env->GetLongField(jSource,ValueFieldId);
}

void GBANUTIL_GetJFieldFromCField(FIELD *Source, jobject jDestination, JNIEnv *env)
{
	jclass cls = env->GetObjectClass((jobject)(jDestination));
	jfieldID ValueFieldId = env->GetFieldID(cls, "Pointer", "J");
	env->SetLongField(jDestination, ValueFieldId,(jlong)(Source));
}

int GBAN2011UTIL_GetCGmtFromJGmt(JNIEnv *env, jobject Source, GB_AN2011_GmtStruct *Destination,
                                 char *logStr)
{
	/*************************************
	 * Check parameters
	 */
	if (Source == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetCGmtFromJGmt: Source is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if (Destination == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetCGmtFromJGmt: Destination is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	/*************************************
	 * Copy
	 */
	jclass cls = env->GetObjectClass(Source);
	jfieldID YearFieldId = env->GetFieldID(cls, "Year", "I");
	jfieldID MonthFieldId = env->GetFieldID(cls, "Month", "I");
	jfieldID DayFieldId = env->GetFieldID(cls, "Day", "I");
	jfieldID hourFieldId = env->GetFieldID(cls, "hour", "I");
	jfieldID minuteFieldId = env->GetFieldID(cls, "minute", "I");
	jfieldID secondFieldId = env->GetFieldID(cls, "second", "I");
	Destination->Year = env->GetIntField(Source,YearFieldId);
	Destination->Month = env->GetIntField(Source,MonthFieldId);
	Destination->Day = env->GetIntField(Source,DayFieldId);
	Destination->hour = env->GetIntField(Source,hourFieldId);
	Destination->minute = env->GetIntField(Source,minuteFieldId);
	Destination->second = env->GetIntField(Source,secondFieldId);
	return AN2K_DLL_NO_ERROR;
}

int GBAN2011UTIL_GetCDateFromJDate(JNIEnv *env, jobject Source, GB_AN2011_DateStruct *Destination,
                                   char *logStr)
{
	/*************************************
	 * Check parameters
	 */
	if (Source == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetCDateFromJDate: Source is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if (Destination == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetCDateFromJDate: Destination is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	/*************************************
	 * Copy
	 */
	jclass cls = env->GetObjectClass(Source);
	jfieldID YearFieldId = env->GetFieldID(cls, "Year", "I");
	jfieldID MonthFieldId = env->GetFieldID(cls, "Month", "I");
	jfieldID DayFieldId = env->GetFieldID(cls, "Day", "I");
	Destination->Year = env->GetIntField(Source,YearFieldId);
	Destination->Month = env->GetIntField(Source,MonthFieldId);
	Destination->Day = env->GetIntField(Source,DayFieldId);
	return AN2K_DLL_NO_ERROR;
}

int GBAN2011UTIL_GetJGmtFromCGmt(JNIEnv *env, jobject Destination, GB_AN2011_GmtStruct *Source,
                                 char *logStr)
{
	/*************************************
	 * Check parameters
	 */
	if (Source == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetJGmtFromCGmt: Source is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if (Destination == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetJGmtFromCGmt: Destination is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	/*************************************
	 * Copy
	 */
	jclass cls = env->GetObjectClass(Destination);
	jfieldID YearFieldId = env->GetFieldID(cls, "Year", "I");
	jfieldID MonthFieldId = env->GetFieldID(cls, "Month", "I");
	jfieldID DayFieldId = env->GetFieldID(cls, "Day", "I");
	jfieldID hourFieldId = env->GetFieldID(cls, "hour", "I");
	jfieldID minuteFieldId = env->GetFieldID(cls, "minute", "I");
	jfieldID secondFieldId = env->GetFieldID(cls, "second", "I");
	env->SetIntField(Destination,YearFieldId,(jint)Source->Year);
	env->SetIntField(Destination,MonthFieldId,(jint)Source->Month);
	env->SetIntField(Destination,DayFieldId,(jint)Source->Day);
	env->SetIntField(Destination,hourFieldId,(jint)Source->hour);
	env->SetIntField(Destination,minuteFieldId,(jint)Source->minute);
	env->SetIntField(Destination,secondFieldId,(jint)Source->second);

	return AN2K_DLL_NO_ERROR;
}

int GBAN2011UTIL_GetJDateFromCDate(JNIEnv *env, jobject Destination, GB_AN2011_DateStruct *Source,
                                   char *logStr)
{
	/*************************************
	 * Check parameters
	 */
	if (Source == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetJDateFromCDate: Source is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if (Destination == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetJDateFromCDate: Destination is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	/*************************************
	 * Copy
	 */
	jclass cls = env->GetObjectClass(Destination);
	jfieldID YearFieldId = env->GetFieldID(cls, "Year", "I");
	jfieldID MonthFieldId = env->GetFieldID(cls, "Month", "I");
	jfieldID DayFieldId = env->GetFieldID(cls, "Day", "I");
	env->SetIntField(Destination,YearFieldId,(jint)Source->Year);
	env->SetIntField(Destination,MonthFieldId,(jint)Source->Month);
	env->SetIntField(Destination,DayFieldId,(jint)Source->Day);

	return AN2K_DLL_NO_ERROR;
}

int GBAN2011UTIL_GetCAppProfileFromJAppProfile(JNIEnv *env, jobject Source,
                                               GB_AN2011_ApplicationProfileStruct *Destination,
                                               char *logStr)
{
	if (Source == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetCAppProfileFromJAppProfile: Source is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if (Destination == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetCAppProfileFromJAppProfile: Destination is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}

	jclass cls = env->GetObjectClass(Source);
	jfieldID AppProfOrgFieldId = env->GetFieldID(cls, "AppProfOrg", "Ljava/lang/String;");
	jfieldID AppProfNameFieldId = env->GetFieldID(cls, "AppProfName", "Ljava/lang/String;");
	jfieldID AppProfVerFieldId = env->GetFieldID(cls, "AppProfVer", "Ljava/lang/String;");
	jstring jAppProfOrg = (jstring) env->GetObjectField(Source, AppProfOrgFieldId);
	jstring jAppProfName = (jstring) env->GetObjectField(Source, AppProfNameFieldId);
	jstring jAppProfVer = (jstring) env->GetObjectField(Source, AppProfVerFieldId);
	GBANUTIL_GetStringFromJString(jAppProfOrg, &(Destination->AppProfOrg), env);
	GBANUTIL_GetStringFromJString(jAppProfName, &(Destination->AppProfName), env);
	GBANUTIL_GetStringFromJString(jAppProfVer, &(Destination->AppProfVer), env);

	return AN2K_DLL_NO_ERROR;
}

int GBAN2011UTIL_GetJAppProfileFromCAppProfile(JNIEnv *env, jobject Destination,
                                               GB_AN2011_ApplicationProfileStruct *Source,
                                               char *logStr)
{
	if (Source == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetJAppProfileFromCAppProfile: Source is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if (Destination == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetJAppProfileFromCAppProfile: Destination is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	jclass cls = env->GetObjectClass(Destination);
	jfieldID AppProfOrgFieldId = env->GetFieldID(cls, "AppProfOrg", "Ljava/lang/String;");
	jfieldID AppProfNameFieldId = env->GetFieldID(cls, "AppProfName", "Ljava/lang/String;");
	jfieldID AppProfVerFieldId = env->GetFieldID(cls, "AppProfVer", "Ljava/lang/String;");

	env->SetObjectField(Destination, AppProfOrgFieldId,
	                    (jobject) (env->NewStringUTF(Source->AppProfOrg)));
	env->SetObjectField(Destination, AppProfNameFieldId,
	                    (jobject) (env->NewStringUTF(Source->AppProfName)));
	env->SetObjectField(Destination, AppProfVerFieldId,
	                    (jobject) (env->NewStringUTF(Source->AppProfVer)));
	return AN2K_DLL_NO_ERROR;
}

int GBAN2011UTIL_GetCPrintPositionDescFromJPrintPositionDesc(JNIEnv *env, jobject Source,
                                                             GB_AN2011_PrintPositionDescriptorsStruct *Destination,
                                                             char *logStr)
{
	if (Source == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetCPrintPositionDescFromJPrintPositionDesc: Source is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if (Destination == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetCPrintPositionDescFromJPrintPositionDesc: Destination is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}

	jclass cls = env->GetObjectClass(Source);
	// FingerImageCode
	jfieldID FingerImageCodeFieldId = env->GetFieldID(cls, "FingerImageCode", "Ljava/lang/String;");
	jstring jFingerImageCode = (jstring) env->GetObjectField(Source, FingerImageCodeFieldId);
	char *FingerImageCode;
	GBANUTIL_GetStringFromJString(jFingerImageCode, &(FingerImageCode), env);
	if (strlen(FingerImageCode) >= 10) FingerImageCode[9] = 0;
	strcpy(Destination->FingerImageCode,FingerImageCode);
	free(FingerImageCode);
	// DecimalFingerPosition
	jfieldID DecimalFingerPositionFieldId = env->GetFieldID(cls, "DecimalFingerPosition", "I");
	Destination->DecimalFingerPosition = env->GetIntField(Source,DecimalFingerPositionFieldId);

	return AN2K_DLL_NO_ERROR;
}

int GBAN2011UTIL_GetJPrintPositionDescFromCPrintPositionDesc(JNIEnv *env, jobject Destination,
                                                             GB_AN2011_PrintPositionDescriptorsStruct *Source,
                                                             char *logStr)
{
	if (Source == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetJPrintPositionDescFromCPrintPositionDesc: Source is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if (Destination == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetJPrintPositionDescFromCPrintPositionDesc: Destination is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}

	jclass cls = env->GetObjectClass(Destination);
	// FingerImageCode
	jfieldID FingerImageCodeFieldId = env->GetFieldID(cls, "FingerImageCode", "Ljava/lang/String;");
	Source->FingerImageCode[9] = 0;
	env->SetObjectField(Destination, FingerImageCodeFieldId,
	                    (jobject) (env->NewStringUTF(Source->FingerImageCode)));
	// DecimalFingerPosition
	jfieldID DecimalFingerPositionFieldId = env->GetFieldID(cls, "DecimalFingerPosition", "I");
	env->SetIntField(Destination,DecimalFingerPositionFieldId,Source->DecimalFingerPosition);

	return AN2K_DLL_NO_ERROR;
}

int GBAN2011UTIL_GetCPrintPositionCoordFromJPrintPositionCoord(JNIEnv *env, jobject Source,
                                                               GB_AN2011_PrintPositionCoordinatesStruct *Destination,
                                                               char *logStr)
{
	if (Source == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetCPrintPositionCoordFromJPrintPositionCoord: Source is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if (Destination == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetCPrintPositionCoordFromJPrintPositionCoord: Destination is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}

	jclass cls = env->GetObjectClass(Source);
	// FullFingerView
	jfieldID FullFingerViewFieldId = env->GetFieldID(cls, "FullFingerView", "Ljava/lang/String;");
	jstring jFullFingerView = (jstring) env->GetObjectField(Source, FullFingerViewFieldId);
	char *FullFingerView;
	GBANUTIL_GetStringFromJString(jFullFingerView, &(FullFingerView), env);
	if (strlen(FullFingerView) >= 10) FullFingerView[9] = 0;
	strcpy(Destination->FullFingerView,FullFingerView);
	free(FullFingerView);
	// LocationOfASegment
	jfieldID LocationOfASegmentFieldId = env->GetFieldID(cls, "LocationOfASegment", "Ljava/lang/String;");
	jstring jLocationOfASegment = (jstring) env->GetObjectField(Source, LocationOfASegmentFieldId);
	char *LocationOfASegment;
	GBANUTIL_GetStringFromJString(jLocationOfASegment, &(LocationOfASegment), env);
	if (strlen(LocationOfASegment) >= 10) LocationOfASegment[9] = 0;
	strcpy(Destination->LocationOfASegment,LocationOfASegment);
	free(LocationOfASegment);
	// LeftHorizontalCoordinate
	jfieldID LeftHorizontalCoordinateFieldId = env->GetFieldID(cls, "LeftHorizontalCoordinate", "I");
	Destination->LeftHorizontalCoordinate = env->GetIntField(Source,LeftHorizontalCoordinateFieldId);
	// RightHorizontalCoordinate
	jfieldID RightHorizontalCoordinateFieldId = env->GetFieldID(cls, "RightHorizontalCoordinate", "I");
	Destination->RightHorizontalCoordinate = env->GetIntField(Source,RightHorizontalCoordinateFieldId);
	// DecimalFingerPosition
	jfieldID TopVerticalCoordinateFieldId = env->GetFieldID(cls, "TopVerticalCoordinate", "I");
	Destination->TopVerticalCoordinate = env->GetIntField(Source,TopVerticalCoordinateFieldId);
	// DecimalFingerPosition
	jfieldID BottomVerticalCoordinateFieldId = env->GetFieldID(cls, "BottomVerticalCoordinate", "I");
	Destination->BottomVerticalCoordinate = env->GetIntField(Source,BottomVerticalCoordinateFieldId);

	return AN2K_DLL_NO_ERROR;
}

int GBAN2011UTIL_GetJPrintPositionCoordFromCPrintPositionCoord(JNIEnv *env, jobject Destination,
                                                               GB_AN2011_PrintPositionCoordinatesStruct *Source,
                                                               char *logStr)
{
	if (Source == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetJPrintPositionCoordFromCPrintPositionCoord: Source is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if (Destination == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetJPrintPositionCoordFromCPrintPositionCoord: Destination is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}

	jclass cls = env->GetObjectClass(Destination);
	// FullFingerView
	jfieldID FullFingerViewFieldId = env->GetFieldID(cls, "FullFingerView", "Ljava/lang/String;");
	Source->FullFingerView[9] = 0;
	env->SetObjectField(Destination, FullFingerViewFieldId,
	                    (jobject) (env->NewStringUTF(Source->FullFingerView)));
	// FullFingerView
	jfieldID LocationOfASegmentFieldId = env->GetFieldID(cls, "LocationOfASegment", "Ljava/lang/String;");
	Source->LocationOfASegment[9] = 0;
	env->SetObjectField(Destination, LocationOfASegmentFieldId,
	                    (jobject) (env->NewStringUTF(Source->LocationOfASegment)));
	// LeftHorizontalCoordinate
	jfieldID LeftHorizontalCoordinateFieldId = env->GetFieldID(cls, "LeftHorizontalCoordinate", "I");
	env->SetIntField(Destination,LeftHorizontalCoordinateFieldId,Source->LeftHorizontalCoordinate);
	// RightHorizontalCoordinate
	jfieldID RightHorizontalCoordinateFieldId = env->GetFieldID(cls, "RightHorizontalCoordinate", "I");
	env->SetIntField(Destination,RightHorizontalCoordinateFieldId,Source->RightHorizontalCoordinate);
	// TopVerticalCoordinate
	jfieldID TopVerticalCoordinateFieldId = env->GetFieldID(cls, "TopVerticalCoordinate", "I");
	env->SetIntField(Destination,TopVerticalCoordinateFieldId,Source->TopVerticalCoordinate);
	// BottomVerticalCoordinate
	jfieldID BottomVerticalCoordinateFieldId = env->GetFieldID(cls, "BottomVerticalCoordinate", "I");
	env->SetIntField(Destination,BottomVerticalCoordinateFieldId,Source->BottomVerticalCoordinate);

	return AN2K_DLL_NO_ERROR;
}

int GBAN2011UTIL_GetCAmputatedBandagedCodeFromJAmputatedBandagedCode(JNIEnv *env, jobject Source,
                                                                     GB_AN2011_AmputatedBandagedCodeStruct *Destination,
                                                                     char *logStr)
{
	if (Source == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetCAmputatedBandagedCodeFromJAmputatedBandagedCode: Source is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if (Destination == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetCAmputatedBandagedCodeFromJAmputatedBandagedCode: Destination is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}

	jclass cls = env->GetObjectClass(Source);
	// FingerImageCode
	jfieldID AmpBandCodeFieldId = env->GetFieldID(cls, "AmpBandCode", "Ljava/lang/String;");
	jstring jAmpBandCode = (jstring) env->GetObjectField(Source, AmpBandCodeFieldId);
	char *AmpBandCode;
	GBANUTIL_GetStringFromJString(jAmpBandCode, &(AmpBandCode), env);
	if (strlen(AmpBandCode) >= 10) AmpBandCode[9] = 0;
	strcpy(Destination->AmpBandCode,AmpBandCode);
	free(AmpBandCode);
	// DecimalFingerPosition
	jfieldID FrictionRidgePositionFieldId = env->GetFieldID(cls, "FrictionRidgePosition", "I");
	Destination->FrictionRidgePosition = env->GetIntField(Source,FrictionRidgePositionFieldId);

	return AN2K_DLL_NO_ERROR;
}

int
GBAN2011UTIL_GetJAmputatedBandagedCodeFromCAmputatedBandagedCode(JNIEnv *env, jobject Destination,
                                                                 GB_AN2011_AmputatedBandagedCodeStruct *Source,
                                                                 char *logStr)
{
	if (Source == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetJAmputatedBandagedCodeFromCAmputatedBandagedCode: Source is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if (Destination == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetJAmputatedBandagedCodeFromCAmputatedBandagedCode: Destination is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}

	jclass cls = env->GetObjectClass(Destination);
	// FingerImageCode
	jfieldID AmpBandCodeFieldId = env->GetFieldID(cls, "AmpBandCode", "Ljava/lang/String;");
	Source->AmpBandCode[9] = 0;
	env->SetObjectField(Destination, AmpBandCodeFieldId,
	                    (jobject) (env->NewStringUTF(Source->AmpBandCode)));
	// DecimalFingerPosition
	jfieldID FrictionRidgePositionFieldId = env->GetFieldID(cls, "FrictionRidgePosition", "I");
	env->SetIntField(Destination,FrictionRidgePositionFieldId,Source->FrictionRidgePosition);

	return AN2K_DLL_NO_ERROR;
}

int GBAN2011UTIL_GetCFingerSegmentPositionFromJFingerSegmentPosition(JNIEnv *env, jobject Source,
                                                                     GB_AN2011_FingerSegmentPositionStruct *Destination,
                                                                     char *logStr)
{
	if (Source == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetCFingerSegmentPositionFromJFingerSegmentPosition: Source is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if (Destination == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetCFingerSegmentPositionFromJFingerSegmentPosition: Destination is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}

	jclass cls = env->GetObjectClass(Source);
	// FrictionRidgePosition
	jfieldID FrictionRidgePositionFieldId = env->GetFieldID(cls, "FrictionRidgePosition", "I");
	Destination->FrictionRidgePosition = env->GetIntField(Source,FrictionRidgePositionFieldId);
	// FrictionRidgePosition
	jfieldID LeftHorizontalCoordinateFieldId = env->GetFieldID(cls, "LeftHorizontalCoordinate", "I");
	Destination->LeftHorizontalCoordinate = env->GetIntField(Source,LeftHorizontalCoordinateFieldId);
	// FrictionRidgePosition
	jfieldID RightHorizontalCoordinateFieldId = env->GetFieldID(cls, "RightHorizontalCoordinate", "I");
	Destination->RightHorizontalCoordinate = env->GetIntField(Source,RightHorizontalCoordinateFieldId);
	// FrictionRidgePosition
	jfieldID TopVerticalCoordinateFieldId = env->GetFieldID(cls, "TopVerticalCoordinate", "I");
	Destination->TopVerticalCoordinate = env->GetIntField(Source,TopVerticalCoordinateFieldId);
	// FrictionRidgePosition
	jfieldID BottomVerticalCoordinateFieldId = env->GetFieldID(cls, "BottomVerticalCoordinate", "I");
	Destination->BottomVerticalCoordinate = env->GetIntField(Source,BottomVerticalCoordinateFieldId);

	return AN2K_DLL_NO_ERROR;
}

int
GBAN2011UTIL_GetJFingerSegmentPositionFromCFingerSegmentPosition(JNIEnv *env, jobject Destination,
                                                                 GB_AN2011_FingerSegmentPositionStruct *Source,
                                                                 char *logStr)
{
	if (Source == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetJFingerSegmentPositionFromCFingerSegmentPosition: Source is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if (Destination == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetJFingerSegmentPositionFromCFingerSegmentPosition: Destination is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}

	jclass cls = env->GetObjectClass(Destination);
	// FrictionRidgePosition
	jfieldID FrictionRidgePositionFieldId = env->GetFieldID(cls, "FrictionRidgePosition", "I");
	env->SetIntField(Destination,FrictionRidgePositionFieldId,Source->FrictionRidgePosition);
	// TopVerticalCoordinate
	jfieldID TopVerticalCoordinateFieldId = env->GetFieldID(cls, "TopVerticalCoordinate", "I");
	env->SetIntField(Destination,TopVerticalCoordinateFieldId,Source->TopVerticalCoordinate);
	// LeftHorizontalCoordinate
	jfieldID LeftHorizontalCoordinateFieldId = env->GetFieldID(cls, "LeftHorizontalCoordinate", "I");
	env->SetIntField(Destination,LeftHorizontalCoordinateFieldId,Source->LeftHorizontalCoordinate);
	// RightHorizontalCoordinate
	jfieldID RightHorizontalCoordinateFieldId = env->GetFieldID(cls, "RightHorizontalCoordinate", "I");
	env->SetIntField(Destination,RightHorizontalCoordinateFieldId,Source->RightHorizontalCoordinate);
	// BottomVerticalCoordinate
	jfieldID BottomVerticalCoordinateFieldId = env->GetFieldID(cls, "BottomVerticalCoordinate", "I");
	env->SetIntField(Destination,BottomVerticalCoordinateFieldId,Source->BottomVerticalCoordinate);

	return AN2K_DLL_NO_ERROR;
}

int GBAN2011UTIL_GetCNistQualityMetricFromJNistQualityMetric(JNIEnv *env, jobject Source,
                                                             GB_AN2011_NistQualityMetricStruct *Destination,
                                                             char *logStr)
{
	if (Source == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetCNistQualityMetricFromJNistQualityMetric: Source is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if (Destination == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetCNistQualityMetricFromJNistQualityMetric: Destination is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}

	jclass cls = env->GetObjectClass(Source);
	// FrictionRidgePosition
	jfieldID FrictionRidgePositionFieldId = env->GetFieldID(cls, "FrictionRidgePosition", "I");
	Destination->FrictionRidgePosition = env->GetIntField(Source,FrictionRidgePositionFieldId);
	// Quality
	jfieldID QualityFieldId = env->GetFieldID(cls, "Quality", "I");
	Destination->Quality = env->GetIntField(Source,QualityFieldId);

	return AN2K_DLL_NO_ERROR;
}

int GBAN2011UTIL_GetJNistQualityMetricFromCNistQualityMetric(JNIEnv *env, jobject Destination,
                                                             GB_AN2011_NistQualityMetricStruct *Source,
                                                             char *logStr)
{
	if (Source == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetJNistQualityMetricFromCNistQualityMetric: Source is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if (Destination == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetJNistQualityMetricFromCNistQualityMetric: Destination is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}

	jclass cls = env->GetObjectClass(Destination);
	// FrictionRidgePosition
	jfieldID FrictionRidgePositionFieldId = env->GetFieldID(cls, "FrictionRidgePosition", "I");
	env->SetIntField(Destination,FrictionRidgePositionFieldId,Source->FrictionRidgePosition);
	// Quality
	jfieldID QualityFieldId = env->GetFieldID(cls, "Quality", "I");
	env->SetIntField(Destination,QualityFieldId,Source->Quality);

	return AN2K_DLL_NO_ERROR;
}

int GBAN2011UTIL_GetCFingOrSegQualityMetricFromJFingOrSegQualityMetric(JNIEnv *env, jobject Source,
                                                                       GB_AN2011_FingerOrSegmentQualityMetricStruct *Destination,
                                                                       char *logStr)
{
	if (Source == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetCFingOrSegQualityMetricFromJFingOrSegQualityMetric: Source is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if (Destination == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetCFingOrSegQualityMetricFromJFingOrSegQualityMetric: Destination is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}

	jclass cls = env->GetObjectClass(Source);
	// FrictionRidgePosition
	jfieldID FrictionRidgePositionFieldId = env->GetFieldID(cls, "FrictionRidgePosition", "I");
	Destination->FrictionRidgePosition = env->GetIntField(Source,FrictionRidgePositionFieldId);
	// QualityValue
	jfieldID QualityValueFieldId = env->GetFieldID(cls, "QualityValue", "I");
	Destination->QualityValue = env->GetIntField(Source,QualityValueFieldId);
	// AlgorithmVendorId
	jfieldID AlgorithmVendorIdFieldId = env->GetFieldID(cls, "AlgorithmVendorId", "I");
	Destination->AlgorithmVendorId = env->GetIntField(Source,AlgorithmVendorIdFieldId);
	// AlgorithmProductId
	jfieldID AlgorithmProductIdFieldId = env->GetFieldID(cls, "AlgorithmProductId", "I");
	Destination->AlgorithmProductId = env->GetIntField(Source,AlgorithmProductIdFieldId);

	return AN2K_DLL_NO_ERROR;
}

int
GBAN2011UTIL_GetJFingOrSegQualityMetricFromCFingOrSegQualityMetric(JNIEnv *env, jobject Destination,
                                                                   GB_AN2011_FingerOrSegmentQualityMetricStruct *Source,
                                                                   char *logStr)
{
	if (Source == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetJFingOrSegQualityMetricFromCFingOrSegQualityMetric: Source is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if (Destination == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetJFingOrSegQualityMetricFromCFingOrSegQualityMetric: Destination is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}

	jclass cls = env->GetObjectClass(Destination);
	// FrictionRidgePosition
	jfieldID FrictionRidgePositionFieldId = env->GetFieldID(cls, "FrictionRidgePosition", "I");
	env->SetIntField(Destination,FrictionRidgePositionFieldId,Source->FrictionRidgePosition);
	// QualityValue
	jfieldID QualityValueFieldId = env->GetFieldID(cls, "QualityValue", "I");
	env->SetIntField(Destination,QualityValueFieldId,Source->QualityValue);
	// AlgorithmVendorId
	jfieldID AlgorithmVendorIdFieldId = env->GetFieldID(cls, "AlgorithmVendorId", "I");
	env->SetIntField(Destination,AlgorithmVendorIdFieldId,Source->AlgorithmVendorId);
	// AlgorithmProductId
	jfieldID AlgorithmProductIdFieldId = env->GetFieldID(cls, "AlgorithmProductId", "I");
	env->SetIntField(Destination,AlgorithmProductIdFieldId,Source->AlgorithmProductId);

	return AN2K_DLL_NO_ERROR;
}

int GBAN2011UTIL_GetCAlternateQualityMetricFromJAlternateQualityMetric(JNIEnv *env, jobject Source,
                                                                       GB_AN2011_AlternateSegmentPositionStruct *Destination,
                                                                       char *logStr)
{
	if (Source == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetCFingOrSegQualityMetricFromJFingOrSegQualityMetric: Source is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if (Destination == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetCFingOrSegQualityMetricFromJFingOrSegQualityMetric: Destination is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}

	jclass cls = env->GetObjectClass(Source);
	// FrictionRidgePosition
	jfieldID FrictionRidgePositionFieldId = env->GetFieldID(cls, "FrictionRidgePosition", "I");
	Destination->FrictionRidgePosition = env->GetIntField(Source,FrictionRidgePositionFieldId);
	// NumberOfPoints
	jfieldID NumberOfPointsFieldId = env->GetFieldID(cls, "NumberOfPoints", "I");
	Destination->NumberOfPoints = env->GetIntField(Source,NumberOfPointsFieldId);
	if (Destination->NumberOfPoints < 3 || Destination->NumberOfPoints > 99)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetCFingOrSegQualityMetricFromJFingOrSegQualityMetric: NumberOfPoints, %d, is out of range", Destination->NumberOfPoints);
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	// Points
	jfieldID PointsFieldId = env->GetFieldID(cls, "Points", "[Ljava/lang/Object");
	jobjectArray jPoints =  reinterpret_cast<jobjectArray>(env->GetObjectField(Source,PointsFieldId));
	if (jPoints == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetCFingOrSegQualityMetricFromJFingOrSegQualityMetric: Source.Points is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if (env->GetArrayLength(jPoints) < Destination->NumberOfPoints)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetCFingOrSegQualityMetricFromJFingOrSegQualityMetric: Points array lenght, %d, is less thanNumberOfPoints, %d",
		        env->GetArrayLength(jPoints),
		        Destination->NumberOfPoints);
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	GB_AN2011_PointOffsetStruct *CPoints = new GB_AN2011_PointOffsetStruct[Destination->NumberOfPoints];
	for (int i = 0; i < Destination->NumberOfPoints; i ++)
	{
		jobject jPointItem = env->GetObjectArrayElement(jPoints, i);
		if (jPointItem == NULL)
		{
			delete [] CPoints;
			sprintf(logStr,
			        "GBAN2011UTIL_GetCFingOrSegQualityMetricFromJFingOrSegQualityMetric: Point item at %d position is NULL", i);
			a_printf ("%s\n", logStr);
			return AN2K_DLL_BAD_PARAMETER;
		}
		jclass pointCls = env->GetObjectClass(jPointItem);
		jfieldID HorizontalPointOffsetFieldId = env->GetFieldID(pointCls, "HorizontalPointOffset", "I");
		CPoints[i].HorizontalPointOffset = env->GetIntField(jPointItem,HorizontalPointOffsetFieldId);
		jfieldID VerticalPointOffsetFieldId = env->GetFieldID(pointCls, "VerticalPointOffset", "I");
		CPoints[i].VerticalPointOffset = env->GetIntField(jPointItem,VerticalPointOffsetFieldId);
	}

	Destination->Points = CPoints;

	return AN2K_DLL_NO_ERROR;
}

int
GBAN2011UTIL_GetJAlternateQualityMetricFromCAlternateQualityMetric(JNIEnv *env, jobject Destination,
                                                                   GB_AN2011_AlternateSegmentPositionStruct *Source,
                                                                   char *logStr)
{
	if (Source == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetJFingOrSegQualityMetricFromCFingOrSegQualityMetric: Source is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if (Destination == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetJFingOrSegQualityMetricFromCFingOrSegQualityMetric: Destination is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}

	jclass cls = env->GetObjectClass(Destination);
	// FrictionRidgePosition
	jfieldID FrictionRidgePositionFieldId = env->GetFieldID(cls, "FrictionRidgePosition", "I");
	env->SetIntField(Destination,FrictionRidgePositionFieldId,Source->FrictionRidgePosition);
	// NumberOfPoints
	jfieldID NumberOfPointsFieldId = env->GetFieldID(cls, "NumberOfPoints", "I");
	env->SetIntField(Destination,NumberOfPointsFieldId,Source->NumberOfPoints);
	// Points
	jfieldID PointsFieldId = env->GetFieldID(cls, "Points", "[Ljava/lang/Object");
	if (Source->Points == NULL)
	{
		env->SetObjectField(Destination,PointsFieldId,NULL);
		env->SetIntField(Destination,NumberOfPointsFieldId,0);
		return AN2K_DLL_NO_ERROR;
	}
	jobjectArray jPoints =  reinterpret_cast<jobjectArray>(env->GetObjectField(Destination,PointsFieldId));
	if (jPoints == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetJFingOrSegQualityMetricFromCFingOrSegQualityMetric: Source.Points is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if (env->GetArrayLength(jPoints) < 99)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetJFingOrSegQualityMetricFromCFingOrSegQualityMetric: Points array lenght, %d, is less 99",
		        env->GetArrayLength(jPoints));
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	for (int i = 0; i < Source->NumberOfPoints; i ++)
	{
		jobject jPointItem = env->GetObjectArrayElement(jPoints, i);
		if (jPointItem == NULL)
		{
			env->SetObjectField(Destination,PointsFieldId,NULL);
			env->SetIntField(Destination,NumberOfPointsFieldId,0);
			sprintf(logStr,
			        "GBAN2011UTIL_GetJFingOrSegQualityMetricFromCFingOrSegQualityMetric: Point item at %d position is NULL", i);
			a_printf ("%s\n", logStr);
			return AN2K_DLL_BAD_PARAMETER;
		}
		jclass pointCls = env->GetObjectClass(jPointItem);
		jfieldID HorizontalPointOffsetFieldId = env->GetFieldID(pointCls, "HorizontalPointOffset", "I");
		env->SetIntField(jPointItem,HorizontalPointOffsetFieldId,Source->Points[i].HorizontalPointOffset);
		jfieldID VerticalPointOffsetFieldId = env->GetFieldID(pointCls, "VerticalPointOffset", "I");
		env->SetIntField(jPointItem,VerticalPointOffsetFieldId,Source->Points[i].VerticalPointOffset);
	}

	return AN2K_DLL_NO_ERROR;
}

int GBAN2011UTIL_GetCAppProfileArrayFromJAppProfileArray(JNIEnv *env, jobjectArray Source,
                                                         GB_AN2011_ApplicationProfileStruct **pDestination,
                                                         char *logStr)
{
	if (pDestination == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetJFingOrSegQualityMetricFromCFingOrSegQualityMetric: Destination is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if ((Source == NULL) || (env->GetArrayLength(Source) <= 0))
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetCAppProfileArrayFromJAppProfileArray: Source is NULL or length <= 0");
		a_printf ("%s\n", logStr);
		*pDestination = NULL;
		return AN2K_DLL_NO_ERROR;
	}
	*pDestination = (GB_AN2011_ApplicationProfileStruct *)malloc(env->GetArrayLength(Source) * sizeof(GB_AN2011_ApplicationProfileStruct));
	for (int i = 0; i < env->GetArrayLength(Source); i ++)
	{
		jobject item = env->GetObjectArrayElement(Source, i);
		jclass cls = env->GetObjectClass(item);
		jfieldID AppProfOrgFieldId = env->GetFieldID(cls, "AppProfOrg", "Ljava/lang/String;");
		jfieldID AppProfNameFieldId = env->GetFieldID(cls, "AppProfName", "Ljava/lang/String;");
		jfieldID AppProfVerFieldId = env->GetFieldID(cls, "AppProfVer", "Ljava/lang/String;");
		jstring jAppProfOrg = (jstring) env->GetObjectField(item, AppProfOrgFieldId);
		jstring jAppProfName = (jstring) env->GetObjectField(item, AppProfNameFieldId);
		jstring jAppProfVer = (jstring) env->GetObjectField(item, AppProfVerFieldId);
		GBANUTIL_GetStringFromJString(jAppProfOrg, &((*pDestination)[i].AppProfOrg), env);
		GBANUTIL_GetStringFromJString(jAppProfName, &((*pDestination)[i].AppProfName), env);
		GBANUTIL_GetStringFromJString(jAppProfVer, &((*pDestination)[i].AppProfVer), env);
	}
	return AN2K_DLL_NO_ERROR;
}

void GBAN2011UTIL_FreeAppProfileArray(GB_AN2011_ApplicationProfileStruct *Destination)
{
	if (Destination != NULL)
	{
		free(Destination);
	}
}

int GBAN2011UTIL_GetJAppProfileArrayFromCAppProfileArray(JNIEnv *env, jobjectArray Destination,
                                                         GB_AN2011_ApplicationProfileStruct *Source, int StrNum,
                                                         char *logStr)
{
	if (Destination == NULL)
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetJAppProfileArrayFromCAppProfileArray: Destination is NULL");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	if ((Source == NULL) || (StrNum <= 0))
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetCAppProfileArrayFromJAppProfileArray: Source is NULL or length <= 0");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_NO_ERROR;
	}
	if (StrNum > 99) StrNum = 99;
	if ((env->GetArrayLength(Destination) < StrNum))
	{
		sprintf(logStr,
		        "GBAN2011UTIL_GetJAppProfileArrayFromCAppProfileArray: Destination length too small");
		a_printf ("%s\n", logStr);
		return AN2K_DLL_BAD_PARAMETER;
	}
	for (int i = 0; i < StrNum; i ++)
	{
		jobject item = env->GetObjectArrayElement(Destination, i);
		jclass cls = env->GetObjectClass(item);
		jfieldID AppProfOrgFieldId = env->GetFieldID(cls, "AppProfOrg", "Ljava/lang/String;");
		jfieldID AppProfNameFieldId = env->GetFieldID(cls, "AppProfName", "Ljava/lang/String;");
		jfieldID AppProfVerFieldId = env->GetFieldID(cls, "AppProfVer", "Ljava/lang/String;");
		env->SetObjectField(item,AppProfOrgFieldId,(jobject)(env->NewStringUTF(Source[i].AppProfOrg)));
		env->SetObjectField(item,AppProfNameFieldId,(jobject)(env->NewStringUTF(Source[i].AppProfName)));
		env->SetObjectField(item,AppProfVerFieldId,(jobject)(env->NewStringUTF(Source[i].AppProfVer)));
	}
	return AN2K_DLL_NO_ERROR;
}


