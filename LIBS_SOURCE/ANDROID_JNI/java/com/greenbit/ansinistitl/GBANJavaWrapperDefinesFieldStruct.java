package com.greenbit.ansinistitl;

import com.greenbit.utils.GBJavaWrapperUtilIntForJavaToCExchange;

public class GBANJavaWrapperDefinesFieldStruct
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

	public int GetFieldId(Gban2000JavaWrapperLibrary lib)
	{
		GBJavaWrapperUtilIntForJavaToCExchange ContainingRecordType = new GBJavaWrapperUtilIntForJavaToCExchange();
		GBJavaWrapperUtilIntForJavaToCExchange FieldId = new GBJavaWrapperUtilIntForJavaToCExchange();
		GBJavaWrapperUtilIntForJavaToCExchange NumSubfields = new GBJavaWrapperUtilIntForJavaToCExchange();
		int RetVal = lib.AnEngineGetFieldGeneralInfo(this,ContainingRecordType,FieldId,NumSubfields);
		if (RetVal != GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_NO_ERROR)
		{
			return -1;
		}
		return FieldId.Get();
	}

	public int GetContainingRecordType(Gban2000JavaWrapperLibrary lib)
	{
		GBJavaWrapperUtilIntForJavaToCExchange ContainingRecordType = new GBJavaWrapperUtilIntForJavaToCExchange();
		GBJavaWrapperUtilIntForJavaToCExchange FieldId = new GBJavaWrapperUtilIntForJavaToCExchange();
		GBJavaWrapperUtilIntForJavaToCExchange NumSubfields = new GBJavaWrapperUtilIntForJavaToCExchange();
		int RetVal = lib.AnEngineGetFieldGeneralInfo(this,ContainingRecordType,FieldId,NumSubfields);
		if (RetVal != GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_NO_ERROR)
		{
			return -1;
		}
		return ContainingRecordType.Get();
	}

	public int GetNumSubfields(Gban2000JavaWrapperLibrary lib)
	{
		GBJavaWrapperUtilIntForJavaToCExchange ContainingRecordType = new GBJavaWrapperUtilIntForJavaToCExchange();
		GBJavaWrapperUtilIntForJavaToCExchange FieldId = new GBJavaWrapperUtilIntForJavaToCExchange();
		GBJavaWrapperUtilIntForJavaToCExchange NumSubfields = new GBJavaWrapperUtilIntForJavaToCExchange();
		int RetVal = lib.AnEngineGetFieldGeneralInfo(this,ContainingRecordType,FieldId,NumSubfields);
		if (RetVal != GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_NO_ERROR)
		{
			return -1;
		}
		return NumSubfields.Get();
	}

	public GBANJavaWrapperDefinesSubfieldStruct [] GetFieldsList(Gban2000JavaWrapperLibrary lib)
	{
		int numSubfields = this.GetNumSubfields(lib);
		if (numSubfields <= 0) return null;
		GBANJavaWrapperDefinesSubfieldStruct [] ValToRet = new GBANJavaWrapperDefinesSubfieldStruct [numSubfields];
		int RetVal = lib.AnEngineGetSubfieldsListFromField(this, ValToRet);
		if (RetVal != GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_NO_ERROR)
		{
			return null;
		}
		return ValToRet;
	}
}
