package com.greenbit.MultiscanJNIGuiJavaAndroid;

import com.greenbit.ansinistitl.GBAN2011JavaWrapperDefinesDateStruct;
import com.greenbit.ansinistitl.GBANJavaWrapperDefinesFieldStruct;
import com.greenbit.ansinistitl.GBANJavaWrapperDefinesRecordStruct;
import com.greenbit.ansinistitl.GBANJavaWrapperDefinesReturnCodes;
import com.greenbit.ansinistitl.Gban2000JavaWrapperLibrary;
import com.greenbit.utils.GBJavaWrapperUtilIntForJavaToCExchange;
import com.greenbit.utils.GBJavaWrapperUtilStringArrayForJavaToCExchange;
import com.greenbit.utils.GBJavaWrapperUtilStringForJavaToCExchange;

public class GbAndroidExampleRecord2Class extends GBANJavaWrapperDefinesRecordStruct
{
	public static final int Rec2Type = 2;
	/**
	 * simple string that "certifies" that this record is not from another standard
	 */
	public static final int Rec2GreenbitCertificateFieldId = 3;
	public static final String GreenbitCertificate = "GREENBIT_REC_2";
	public static final int Rec2NameFieldId = 4;
	public static final int Rec2DobFieldId = 5;

	public int GetInfo(Gban2000JavaWrapperLibrary lib, GBJavaWrapperUtilIntForJavaToCExchange idc,
					   GBJavaWrapperUtilStringForJavaToCExchange Name, GBJavaWrapperUtilStringForJavaToCExchange Surname,
					   GBAN2011JavaWrapperDefinesDateStruct DateOfBirth)
	{
		GBJavaWrapperUtilStringArrayForJavaToCExchange ValuesArray = new GBJavaWrapperUtilStringArrayForJavaToCExchange();
		GBANJavaWrapperDefinesFieldStruct FoundField = new GBANJavaWrapperDefinesFieldStruct();

		/////////////////////////////////////////
		// IDC
		/////////////////////////////////////////
		int RetVal = lib.AnEngineSearchFieldInTaggedRecord(this,IdcFieldId,FoundField);
		if (RetVal != GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_NO_ERROR)
		{
			return RetVal;
		}
		RetVal = lib.AnEngineGetValuesFromFieldForTaggedRecords(FoundField,ValuesArray);
		if (RetVal != GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_NO_ERROR)
		{
			return RetVal;
		}
		if (ValuesArray.Value == null || ValuesArray.Value.length < 1)
		{
			return GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_BAD_PARAMETER;
		}
		try
		{
			idc.Value = Integer.parseInt(ValuesArray.Value[0]);
		}
		catch (NumberFormatException ex)
		{
			return GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_BAD_PARAMETER;
		}

		/////////////////////////////////////////
		// GREENBIT "CERTIFICATE"
		/////////////////////////////////////////
		RetVal = lib.AnEngineSearchFieldInTaggedRecord(this,Rec2GreenbitCertificateFieldId,FoundField);
		if (RetVal != GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_NO_ERROR)
		{
			return RetVal;
		}
		RetVal = lib.AnEngineGetValuesFromFieldForTaggedRecords(FoundField,ValuesArray);
		if (RetVal != GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_NO_ERROR)
		{
			return RetVal;
		}
		if (ValuesArray.Value == null || ValuesArray.Value.length < 1)
		{
			return GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_BAD_PARAMETER;
		}
		if (!(ValuesArray.Value[0].equals(GreenbitCertificate)))
		{
			//  record 2 comes from another standard
			return GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_BAD_PARAMETER;
		}

		/////////////////////////////////////////
		// NAME
		/////////////////////////////////////////
		RetVal = lib.AnEngineSearchFieldInTaggedRecord(this,Rec2NameFieldId,FoundField);
		if (RetVal != GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_NO_ERROR)
		{
			return RetVal;
		}
		RetVal = lib.AnEngineGetValuesFromFieldForTaggedRecords(FoundField,ValuesArray);
		if (RetVal != GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_NO_ERROR)
		{
			return RetVal;
		}
		if (ValuesArray.Value == null || ValuesArray.Value.length < 2)
		{
			return GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_BAD_PARAMETER;
		}
		Name.Value = ValuesArray.Value[0];
		Surname.Value = ValuesArray.Value[1];

		/////////////////////////////////////////
		// DATE OF BIRTH
		/////////////////////////////////////////
		RetVal = lib.AnEngineSearchFieldInTaggedRecord(this,Rec2DobFieldId,FoundField);
		if (RetVal != GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_NO_ERROR)
		{
			return RetVal;
		}
		RetVal = lib.AnEngineGetValuesFromFieldForTaggedRecords(FoundField,ValuesArray);
		if (RetVal != GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_NO_ERROR)
		{
			return RetVal;
		}
		if (ValuesArray.Value == null || ValuesArray.Value.length < 3)
		{
			return GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_BAD_PARAMETER;
		}
		try
		{
			DateOfBirth.Year = Integer.parseInt(ValuesArray.Value[0]);
			DateOfBirth.Month = Integer.parseInt(ValuesArray.Value[1]);
			DateOfBirth.Day = Integer.parseInt(ValuesArray.Value[2]);
		}
		catch (NumberFormatException ex)
		{
			return GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_BAD_PARAMETER;
		}

		return GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_NO_ERROR;
	}

	public int Create(Gban2000JavaWrapperLibrary lib, int idc,
			String Name, String Surname, GBAN2011JavaWrapperDefinesDateStruct DateOfBirth)
	{
		String [] ValuesArray;
		///////////////////////////
		// check parameters
		///////////////////////////
		if (
				(Name == null) || (Surname == null) ||
						(DateOfBirth == null)
				)
		{
			return GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_BAD_PARAMETER;
		}
		///////////////////////////
		// create
		///////////////////////////
		int RetVal = lib.AnEngineCreateEmptyTaggedRecord(this,Rec2Type,idc);
		if (RetVal != GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_NO_ERROR)
		{
			return RetVal;
		}

		/////////////////////////////////////////
		// INSERT GREENBIT "CERTIFICATE"
		/////////////////////////////////////////
		ValuesArray = new String[1];
		ValuesArray[0] = GreenbitCertificate;
		GBANJavaWrapperDefinesFieldStruct GbCertificateField = new GBANJavaWrapperDefinesFieldStruct();
		RetVal = lib.AnEngineCreateFieldFromValuesForTaggedRecords(
				GbCertificateField,2, Rec2GreenbitCertificateFieldId,
				ValuesArray
		);
		RetVal = lib.AnEngineInsertFieldIntoTaggedRecord(this,GbCertificateField);
		if (RetVal != GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_NO_ERROR)
		{
			return RetVal;
		}

		/////////////////////////////////////////
		// INSERT NAME FIELD
		/////////////////////////////////////////
		ValuesArray = new String[2];
		ValuesArray[0] = Name; ValuesArray[1] = Surname;
		GBANJavaWrapperDefinesFieldStruct NameField = new GBANJavaWrapperDefinesFieldStruct();
		RetVal = lib.AnEngineCreateFieldFromValuesForTaggedRecords(
				NameField,2,Rec2NameFieldId,
				ValuesArray
		);
		RetVal = lib.AnEngineInsertFieldIntoTaggedRecord(this,NameField);
		if (RetVal != GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_NO_ERROR)
		{
			return RetVal;
		}

		/////////////////////////////////////////
		// INSERT DATE OF BIRTH FIELD
		/////////////////////////////////////////
		ValuesArray = new String[3];
		ValuesArray[0] = "" + DateOfBirth.Year; ValuesArray[1] = "" + DateOfBirth.Month;
		ValuesArray[2] = "" + DateOfBirth.Day;
		GBANJavaWrapperDefinesFieldStruct DobField = new GBANJavaWrapperDefinesFieldStruct();
		RetVal = lib.AnEngineCreateFieldFromValuesForTaggedRecords(
				DobField,2,Rec2DobFieldId,
				ValuesArray
		);
		RetVal = lib.AnEngineInsertFieldIntoTaggedRecord(this,DobField);
		if (RetVal != GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_NO_ERROR)
		{
			return RetVal;
		}

		return GBANJavaWrapperDefinesReturnCodes.AN2K_DLL_NO_ERROR;
	}
}
