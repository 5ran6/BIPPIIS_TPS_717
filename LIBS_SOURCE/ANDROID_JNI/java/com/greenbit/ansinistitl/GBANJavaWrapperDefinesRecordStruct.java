package com.greenbit.ansinistitl;

import com.greenbit.utils.GBJavaWrapperUtilIntForJavaToCExchange;
import com.greenbit.ansinistitl.Gban2000JavaWrapperLibrary;

public class GBANJavaWrapperDefinesRecordStruct
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
	public static final int IdcFieldId = 2;
    public int GetType(Gban2000JavaWrapperLibrary lib)
    {
        GBJavaWrapperUtilIntForJavaToCExchange RecType = new GBJavaWrapperUtilIntForJavaToCExchange();
        GBJavaWrapperUtilIntForJavaToCExchange NumFields = new GBJavaWrapperUtilIntForJavaToCExchange();
        int RetVal = lib.AnEngineGetRecordTypeAndNumFields(this,RecType,NumFields);
        if (RetVal != GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_NO_ERROR)
        {
            return -1;
        }
        return RecType.Get();
    }

    public int GetNumFields(Gban2000JavaWrapperLibrary lib)
    {
        GBJavaWrapperUtilIntForJavaToCExchange RecType = new GBJavaWrapperUtilIntForJavaToCExchange();
        GBJavaWrapperUtilIntForJavaToCExchange NumFields = new GBJavaWrapperUtilIntForJavaToCExchange();
        int RetVal = lib.AnEngineGetRecordTypeAndNumFields(this,RecType,NumFields);
        if (RetVal != GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_NO_ERROR)
        {
            return -1;
        }
        return NumFields.Get();
    }

    public GBANJavaWrapperDefinesFieldStruct [] GetFieldsList(Gban2000JavaWrapperLibrary lib)
    {
        int numFields = this.GetNumFields(lib);
        if (numFields <= 0) return null;
        GBANJavaWrapperDefinesFieldStruct [] ValToRet = new GBANJavaWrapperDefinesFieldStruct [numFields];
        int RetVal = lib.AnEngineGetFieldsListFromRecord(this, ValToRet);
        if (RetVal != GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_NO_ERROR)
        {
            return null;
        }
        return ValToRet;
    }
}
