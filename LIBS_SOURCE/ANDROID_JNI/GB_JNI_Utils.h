#ifndef DACTYMATCH_GUI_JAVAANDROID_GB_AN_UTILS_H
#define DACTYMATCH_GUI_JAVAANDROID_GB_AN_UTILS_H

//-------------------------------------------------------------//
//-------------------------------------------------------------//
// INCLUDE
//-------------------------------------------------------------//
//-------------------------------------------------------------//
#include <jni.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "GB_SystemConfiguration.h"
#include "an2k_dll.h"
#include "GB_AN2011.h"
#include <android/log.h>


//-------------------------------------------------------------//
//-------------------------------------------------------------//
// FUNCTIONS
//-------------------------------------------------------------//
//-------------------------------------------------------------//
void GBANUTIL_GetStringFromJString(jstring jSource, char **pDestination, JNIEnv *env);
void GBANUTIL_GetJStringExchangeFromCString(char * Source, jobject Destination, JNIEnv *env);
void GBANUTIL_GetJStringArrayExchangeFromCStringArray(char ** Source, int SourceLen, jobject Destination, JNIEnv *env);
void GBANUTIL_GetJBufferFromCBuffer(unsigned char *Source, int SourceLen, jobject Destination, JNIEnv *env);
void GBANUTIL_GetJIntExchangeFromCInt(int Source, jobject Destination, JNIEnv *env);
void GBANUTIL_GetJDoubleExchangeFromCDouble(double Source, jobject Destination, JNIEnv *env);

/**********************************
 * ANSI NIST, RECORD, SUBFIELD
 */
void GBANUTIL_GetCAnsiNistFromJAnsiNist(jobject jSource, ANSI_NIST** pDestination, JNIEnv *env);
void GBANUTIL_GetJAnsiNistFromCAnsiNist(ANSI_NIST *Source, jobject jDestination, JNIEnv *env);
void GBANUTIL_GetCRecordFromJRecord(jobject jSource, RECORD** pDestination, JNIEnv *env);
void GBANUTIL_GetJRecordFromCRecord(RECORD *Source, jobject jDestination, JNIEnv *env);
void GBANUTIL_GetCSubfieldFromJSubfield(jobject jSource, SUBFIELD** pDestination, JNIEnv *env);
void GBANUTIL_GetJSubfieldFromCSubfield(SUBFIELD *Source, jobject jDestination, JNIEnv *env);
void GBANUTIL_GetCFieldFromJField(jobject jSource, FIELD** pDestination, JNIEnv *env);
void GBANUTIL_GetJFieldFromCField(FIELD *Source, jobject jDestination, JNIEnv *env);

/**********************************
 * AN2000 FING_AMP_LIST
 *********************************/
int GBANUTIL_GetCAmpListFromJAmpStructArray(JNIEnv *env, jobjectArray Source,FING_AMP_LIST *Destination, char*logStr);

int GBANUTIL_GetJAmpStructArrayFromCAmpList(JNIEnv *env, jobjectArray Destination,FING_AMP_LIST *Source, char*logStr);

void GBANUTIL_FreeAmpListFields(FING_AMP_LIST *ampList);

/*********************************
 * AN2000 SEGMENTS BOUNDS
 ********************************/
int GBANUTIL_GetCSegmBoundsListFromJSegmBoundsStructArray(JNIEnv *env, jobjectArray Source,FOUR_FING_BOUND *Destination, char*logStr);

int GBANUTIL_GetJSegmBoundsStructArrayFromCSegmBoundsList(JNIEnv *env, jobjectArray Destination,FOUR_FING_BOUND *Source, char*logStr);

void GBANUTIL_FreeSegmBoundsFields(FOUR_FING_BOUND *fingSegmList);

/*********************************
 * AN2000 FINGERS NFIQ QUALITY
 ********************************/
int GBANUTIL_GetCFingerQualityListFromJFingerQualityStructArray(JNIEnv *env, jobjectArray Source,FOUR_FING_QUALITY *Destination, char*logStr);

int GBANUTIL_GetJFingerQualityStructArrayFromCFingerQualityList(JNIEnv *env, jobjectArray Destination,FOUR_FING_QUALITY *Source, char*logStr);

void GBANUTIL_FreeFingerQualityFields(FOUR_FING_QUALITY *fingQualityList);

/*********************************
 * AN2000 FINGERS ALTERNATE QUALITY
 ********************************/
int GBANUTIL_GetCAlternateFingerQualityListFromJFingerAlternateQualityStructArray(JNIEnv *env, jobjectArray Source,ALT_QUAL_LIST *Destination, char*logStr);

int GBANUTIL_GetJAlternateFingerQualityStructArrayFromCAlternateFingerQualityList(JNIEnv *env, jobjectArray Destination,ALT_QUAL_LIST *Source, char*logStr);

void GBANUTIL_FreeAlternateFingerQualityFields(ALT_QUAL_LIST *fingQualityList);

/*********************************
 * AN2000 PALM QUALITY ALGORITHMS
 ********************************/
int GBANUTIL_GetCPalmQualityListFromJPalmQualityStructArray(JNIEnv *env, jobjectArray Source,PALM_QUAL_LIST *Destination, char*logStr);

int GBANUTIL_GetJPalmQualityStructArrayFromCPalmQualityList(JNIEnv *env, jobjectArray Destination,PALM_QUAL_LIST *Source, char*logStr);

void GBANUTIL_FreePalmQualityFields(PALM_QUAL_LIST *palmQualList);

/*********************************
 * AN2011 GMT AND DATE STRUCTS
 */
int GBAN2011UTIL_GetCGmtFromJGmt(JNIEnv *env, jobject Source,GB_AN2011_GmtStruct *Destination, char*logStr);
int GBAN2011UTIL_GetJGmtFromCGmt(JNIEnv *env, jobject Destination,GB_AN2011_GmtStruct *Source, char*logStr);
int GBAN2011UTIL_GetCDateFromJDate(JNIEnv *env, jobject Source,GB_AN2011_DateStruct *Destination, char*logStr);
int GBAN2011UTIL_GetJDateFromCDate(JNIEnv *env, jobject Destination,GB_AN2011_DateStruct *Source, char*logStr);

/*********************************
 * AN2011 APP PROFILE STRUCT
 */
int GBAN2011UTIL_GetCAppProfileArrayFromJAppProfileArray(JNIEnv *env, jobjectArray Source,GB_AN2011_ApplicationProfileStruct **pDestination, char*logStr);
void GBAN2011UTIL_FreeAppProfileArray(GB_AN2011_ApplicationProfileStruct *Destination);
int GBAN2011UTIL_GetJAppProfileArrayFromCAppProfileArray(JNIEnv *env, jobjectArray Destination,GB_AN2011_ApplicationProfileStruct *Source, int StrNum, char*logStr);

/*********************************
 * AN2011 PrintPositionDescriptorsStruct
 */
int GBAN2011UTIL_GetCPrintPositionDescFromJPrintPositionDesc(JNIEnv *env, jobject Source,GB_AN2011_PrintPositionDescriptorsStruct *Destination, char*logStr);
int GBAN2011UTIL_GetJPrintPositionDescFromCPrintPositionDesc(JNIEnv *env, jobject Destination,GB_AN2011_PrintPositionDescriptorsStruct *Source, char*logStr);

/*********************************
 * AN2011 GB_AN2011_PrintPositionCoordinatesStruct
 */
int GBAN2011UTIL_GetCPrintPositionCoordFromJPrintPositionCoord(JNIEnv *env, jobject Source,GB_AN2011_PrintPositionCoordinatesStruct *Destination, char*logStr);
int GBAN2011UTIL_GetJPrintPositionCoordFromCPrintPositionCoord(JNIEnv *env, jobject Destination,GB_AN2011_PrintPositionCoordinatesStruct *Source, char*logStr);

/*********************************
 * AN2011 AmputatedBandagedCode
 */
int GBAN2011UTIL_GetCAmputatedBandagedCodeFromJAmputatedBandagedCode(JNIEnv *env, jobject Source,GB_AN2011_AmputatedBandagedCodeStruct *Destination, char*logStr);
int GBAN2011UTIL_GetJAmputatedBandagedCodeFromCAmputatedBandagedCode(JNIEnv *env, jobject Destination,GB_AN2011_AmputatedBandagedCodeStruct *Source, char*logStr);

/*********************************
 * AN2011 Finger Segments
 */
int GBAN2011UTIL_GetCFingerSegmentPositionFromJFingerSegmentPosition(JNIEnv *env, jobject Source,GB_AN2011_FingerSegmentPositionStruct *Destination, char*logStr);
int GBAN2011UTIL_GetJFingerSegmentPositionFromCFingerSegmentPosition(JNIEnv *env, jobject Destination,GB_AN2011_FingerSegmentPositionStruct *Source, char*logStr);

/*********************************
 * AN2011 NFIQ struct
 */
int GBAN2011UTIL_GetCNistQualityMetricFromJNistQualityMetric(JNIEnv *env, jobject Source,GB_AN2011_NistQualityMetricStruct *Destination, char*logStr);
int GBAN2011UTIL_GetJNistQualityMetricFromCNistQualityMetric(JNIEnv *env, jobject Destination,GB_AN2011_NistQualityMetricStruct *Source, char*logStr);

/*********************************
 * AN2011 Alternate quality struct
 */
int GBAN2011UTIL_GetCFingOrSegQualityMetricFromJFingOrSegQualityMetric(JNIEnv *env, jobject Source,
                                                                       GB_AN2011_FingerOrSegmentQualityMetricStruct *Destination,
                                                                       char *logStr);
int GBAN2011UTIL_GetJFingOrSegQualityMetricFromCFingOrSegQualityMetric(JNIEnv *env,
                                                                       jobject Destination,
                                                                       GB_AN2011_FingerOrSegmentQualityMetricStruct *Source,
                                                                       char *logStr);

/*********************************
 * AN2011 Alternate segment position
 */
int GBAN2011UTIL_GetCAlternateQualityMetricFromJAlternateQualityMetric(JNIEnv *env, jobject Source,GB_AN2011_AlternateSegmentPositionStruct *Destination, char*logStr);
int GBAN2011UTIL_GetJAlternateQualityMetricFromCAlternateQualityMetric(JNIEnv *env, jobject Destination,GB_AN2011_AlternateSegmentPositionStruct *Source, char*logStr);


#endif //DACTYMATCH_GUI_JAVAANDROID_GB_AN_UTILS_H
