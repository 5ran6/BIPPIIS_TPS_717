package com.greenbit.ansinistitl;

import com.greenbit.utils.GBJavaWrapperUtilIntForJavaToCExchange;

public class GBANJavaWrapperDefinesANStruct
{
    //----------------------------------------------------------
    // DO NOT USE THESE FIELDS OUTSIDE JNI!!!!!
    //----------------------------------------------------------

    /**************************************
     * this field stores a native pointer
     *************************************/
    public long Pointer;

    //----------------------------------------------------------
    // FIELDS ACCESSIBLE BY APP
    //----------------------------------------------------------
    public int GetVersion(Gban2000JavaWrapperLibrary lib)
    {
        GBJavaWrapperUtilIntForJavaToCExchange Version = new GBJavaWrapperUtilIntForJavaToCExchange();
        GBJavaWrapperUtilIntForJavaToCExchange NumRecords = new GBJavaWrapperUtilIntForJavaToCExchange();
        int RetVal = lib.AnEngineGetAnsiNistVersionAndNumOfRecords(this,Version, NumRecords);
        if (RetVal != GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_NO_ERROR)
        {
            return -1;
        }
        return Version.Get();
    }

    public int GetNumRecords(Gban2000JavaWrapperLibrary lib)
    {
        GBJavaWrapperUtilIntForJavaToCExchange Version = new GBJavaWrapperUtilIntForJavaToCExchange();
        GBJavaWrapperUtilIntForJavaToCExchange NumRecords = new GBJavaWrapperUtilIntForJavaToCExchange();
        int RetVal = lib.AnEngineGetAnsiNistVersionAndNumOfRecords(this,Version, NumRecords);
        if (RetVal != GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_NO_ERROR)
        {
            return -1;
        }
        return NumRecords.Get();
    }

    public GBANJavaWrapperDefinesRecordStruct [] GetRecordsList(Gban2000JavaWrapperLibrary lib)
	{
		int numRec = this.GetNumRecords(lib);
		if (numRec <= 0) return null;
		GBANJavaWrapperDefinesRecordStruct [] ValToRet = new GBANJavaWrapperDefinesRecordStruct [numRec];
		int RetVal = lib.AnEngineGetRecordsListFromAnsiNist(this, ValToRet);
		if (RetVal != GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_NO_ERROR)
		{
			return null;
		}
		return ValToRet;
	}
}
