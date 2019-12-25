package com.greenbit.ansinistitl;

import com.greenbit.utils.GBJavaWrapperUtilByteArrayForJavaToCExchange;
import com.greenbit.utils.GBJavaWrapperUtilDoubleForJavaToCExchange;
import com.greenbit.utils.GBJavaWrapperUtilIntForJavaToCExchange;
import com.greenbit.utils.GBJavaWrapperUtilStringArrayForJavaToCExchange;
import com.greenbit.utils.GBJavaWrapperUtilStringForJavaToCExchange;

public class Gban2000JavaWrapperLibrary
{
    // Used to load the 'native-lib' library on application startup.
    static
    {
        System.loadLibrary("AN2000_JNI");
    }

    public native String GetLastErrorString();

    public native int Load();

    public native int Unload();

    //--------------------------------------------------------------------------
    // ANSI NIST STRUCT AND TYPE 1
    //--------------------------------------------------------------------------
    public native int CreateAnsiNist(
            GBANJavaWrapperDefinesANStruct AnsiNist,
            int Version,
            String TransactionType, // MANDATORY
            int Priority,
            String DestinationAgency, // MANDATORY
            String OriginatingAgency, // MANDATORY
            String TransactionControlID, // MANDATORY
            String TransactionControlReference, // OPTIONAL
            double NominalScanningResolution,
            double NominalTransmitResolution,
            String DomainNameID // MANDATORY
    );

    public native int GetType1InfoFromAnsiNist(
            GBANJavaWrapperDefinesANStruct AnsiNist,
            GBJavaWrapperUtilIntForJavaToCExchange Version,                // version
            GBJavaWrapperUtilStringForJavaToCExchange TransactionType,        // transaction type
            GBJavaWrapperUtilIntForJavaToCExchange Priority,                // priority
            GBJavaWrapperUtilStringForJavaToCExchange DestinationAgency,        // destination agency
            GBJavaWrapperUtilStringForJavaToCExchange OriginatingAgency,        // originating agency
            GBJavaWrapperUtilStringForJavaToCExchange TransContrNum,            // transaction control number
            GBJavaWrapperUtilStringForJavaToCExchange TransContrRef,            // transaction control reference
            GBJavaWrapperUtilDoubleForJavaToCExchange NominalScanRes,            // Nominal Scanning Resolution
            GBJavaWrapperUtilDoubleForJavaToCExchange NominalTXRes,            // Nominal Transmitting Resolution
            GBJavaWrapperUtilStringForJavaToCExchange Domain                // Domain
    );

    public native int UpdateType1RecordInfoInAnsiNist(
            GBANJavaWrapperDefinesANStruct AnsiNist,            // transaction control reference
            double NominalScanningResolution,
            double NominalTransmitResolution
    );

    //--------------------------------------------------------------------------
    // TYPE 14 RECORD
    //--------------------------------------------------------------------------
    public native int ImageToType14Record(
            GBANJavaWrapperDefinesRecordStruct Record,
            byte [] FingerprintImage,
            int FPImageWidth,
            int FPImageHeight,
            double ScanResolutionPPMM,
            String CompressionType,
            int ImpressionType,
            String ImageSource,
            String Comment,
            String FingerPosition,
            GBANJavaWrapperDefinesAmputatedFingersDescription [] AmputationList,
            GBANJavaWrapperDefinesFingerSegmentsBoundsDescription [] boundaries,
            GBANJavaWrapperDefinesFingersNfiqDescription [] qualities,
            GBANJavaWrapperDefinesFingersAlternateQualityDescription [] segqual,
            GBANJavaWrapperDefinesFingersAlternateQualityDescription [] altqual
    );

    public native int UpdateType14Record(
            GBANJavaWrapperDefinesRecordStruct Record,
            byte [] FingerprintImage,
            int FPImageWidth,
            int FPImageHeight,
            double ScanResolutionPPMM,
            GBANJavaWrapperDefinesAmputatedFingersDescription [] AmputationList,
            GBANJavaWrapperDefinesFingerSegmentsBoundsDescription [] boundaries,
            GBANJavaWrapperDefinesFingersNfiqDescription [] qualities,
            GBANJavaWrapperDefinesFingersAlternateQualityDescription [] segqual,
            GBANJavaWrapperDefinesFingersAlternateQualityDescription [] altqual
    );

    public native int GetType14Info(
            GBANJavaWrapperDefinesRecordStruct Record,
            GBJavaWrapperUtilByteArrayForJavaToCExchange FingerprintImage, // byte []
            GBJavaWrapperUtilIntForJavaToCExchange FPImageWidth, // int
            GBJavaWrapperUtilIntForJavaToCExchange FPImageHeight, // int
            GBJavaWrapperUtilIntForJavaToCExchange PixelDepth, // int
            GBJavaWrapperUtilDoubleForJavaToCExchange ScanResolutionPPMM, // double
            GBJavaWrapperUtilStringForJavaToCExchange CompressionType, // string
            GBJavaWrapperUtilIntForJavaToCExchange ImpressionType, // int
            GBJavaWrapperUtilStringForJavaToCExchange ImageSource, // string
            GBJavaWrapperUtilStringForJavaToCExchange Comment, // string
            GBJavaWrapperUtilStringForJavaToCExchange [] FingerPosition, // string [6],
            GBANJavaWrapperDefinesAmputatedFingersDescription [] AmputationList, GBJavaWrapperUtilIntForJavaToCExchange AmputatedFingerNumber, // valid items number in jAmputationList
            GBANJavaWrapperDefinesFingerSegmentsBoundsDescription [] boundaries, GBJavaWrapperUtilIntForJavaToCExchange SegmentedFingerNumber, // valid items number in jboundaries
            GBANJavaWrapperDefinesFingersNfiqDescription [] qualities, GBJavaWrapperUtilIntForJavaToCExchange NfiqEvaluatedFingerNumber, // valid items number in jqualities
            GBANJavaWrapperDefinesFingersAlternateQualityDescription [] segqual, GBJavaWrapperUtilIntForJavaToCExchange ProprietaryQualityEvaluatedFingerNumber, // valid items number in jsegqual
            GBANJavaWrapperDefinesFingersAlternateQualityDescription [] altqual, GBJavaWrapperUtilIntForJavaToCExchange AlternateQualityEvaluatedFingerNumber // valid items number in jaltqual
    );

    public native int UpdateType1415ScaleUnits(
            GBANJavaWrapperDefinesRecordStruct Record,
            int NewScale
    );

    //--------------------------------------------------------------------------
    // TYPE 4 RECORD
    //--------------------------------------------------------------------------
    /**
     *
     * @param Record
     * @param FingerprintImage
     * @param FPImageWidth
     * @param FPImageHeight
     * @param ScanResolutionId:
     * @see GBANJavaWrapperDefinesType4ScanResolutionValues
     * @param CompressionType:
     * @see GBANJavaWrapperDefinesCompressionAlgorithmsStrings
     * @param ImpressionType:
     * @see GBANJavaWrapperDefinesImpressionCodes
     * @param FingerPosition:
     * @see GBANJavaWrapperDefinesFingerPositions
     * @return
     */
    public native int ImageToType4Record(
            GBANJavaWrapperDefinesRecordStruct Record,
            byte [] FingerprintImage,
            int FPImageWidth,
            int FPImageHeight,
            int ScanResolutionId,
            String CompressionType,
            int ImpressionType,
            int FingerPosition
    );

    public native int UpdateType4Record(
            GBANJavaWrapperDefinesRecordStruct Record,
            byte [] FingerprintImage,
            int FPImageWidth,
            int FPImageHeight,
            int ScanResolutionId
    );

    public native int GetType4Info(
            GBANJavaWrapperDefinesRecordStruct Record,
            GBJavaWrapperUtilByteArrayForJavaToCExchange FingerprintImage, // byte []
            GBJavaWrapperUtilIntForJavaToCExchange FPImageWidth, // int
            GBJavaWrapperUtilIntForJavaToCExchange FPImageHeight, // int
            GBJavaWrapperUtilIntForJavaToCExchange ScanResolutionId, // int
            GBJavaWrapperUtilStringForJavaToCExchange CompressionType, // string
            GBJavaWrapperUtilIntForJavaToCExchange ImpressionType, // int
            int [] FingerPosition // int [6]
    );

    //--------------------------------------------------------------------------
    // TYPE 15 RECORD
    //--------------------------------------------------------------------------
    public native int ImageToType15Record(
            GBANJavaWrapperDefinesRecordStruct Record,
            byte [] PPImage,
            int PPImageWidth,
            int PPImageHeight,
            double ScanResolutionPPMM,
            String CompressionType,
            int ImpressionType,
            String ImageSource,
            String Comment,
            String PalmPosition,
            GBANJavaWrapperDefinesPalmQualityAlgorithms [] jPalmQualitiesList
    );

    public native int UpdateType15Record(
            GBANJavaWrapperDefinesRecordStruct Record,
            byte [] PPImage,
            int PPImageWidth,
            int PPImageHeight,
            double ScanResolutionPPMM,
            GBANJavaWrapperDefinesPalmQualityAlgorithms [] jPalmQualitiesList
    );

    public native int GetType15Info(
            GBANJavaWrapperDefinesRecordStruct Record,
            GBJavaWrapperUtilByteArrayForJavaToCExchange PPImage, // byte []
            GBJavaWrapperUtilIntForJavaToCExchange PPImageWidth, // int
            GBJavaWrapperUtilIntForJavaToCExchange PPImageHeight, // int
            GBJavaWrapperUtilDoubleForJavaToCExchange ScanResolutionPPMM, // double
            GBJavaWrapperUtilStringForJavaToCExchange CompressionType, // string
            GBJavaWrapperUtilIntForJavaToCExchange ImpressionType, // int
            GBJavaWrapperUtilStringForJavaToCExchange ImageSource, // string
            GBJavaWrapperUtilStringForJavaToCExchange Comment, // string
            GBJavaWrapperUtilStringForJavaToCExchange PalmPosition, // string,
            GBANJavaWrapperDefinesPalmQualityAlgorithms [] PalmQualitiesList, GBJavaWrapperUtilIntForJavaToCExchange PalmAlgorithmsNumber // valid items number in jPalmQualitiesList
    );

    //--------------------------------------------------------------------------
    // RECORDS STORAGE INTO AN ANSI NIST STRUCT ROUTINES
    //--------------------------------------------------------------------------
    public native int InsertRecordIntoAnsiNistStruct(
            GBANJavaWrapperDefinesANStruct AnsiNist,
            GBANJavaWrapperDefinesRecordStruct Record,
            int InsertionIndex
    );

    public native int DeleteRecordFromAnsiNistStruct(
            GBANJavaWrapperDefinesANStruct AnsiNist,
            int RecordToDeleteIndex
    );

    //--------------------------------------------------------------------------
    // READ/WRITE ANSINIST STRUCT FROM/TO BYTE BUFFER
    //--------------------------------------------------------------------------
    public native int ReadAnsiNistFromBuffer(
            //-------------------------
            // INPUT
            //-------------------------
            byte [] InputBuffer,
            //-------------------------
            // OUTPUT
            //-------------------------
            GBANJavaWrapperDefinesANStruct AnsiNist
    );

    public native int WriteAnsiNistToBuffer(
            //---------------------------
            // INPUT
            //---------------------------
            GBANJavaWrapperDefinesANStruct AnsiNist,
            //---------------------------
            // Output
            //---------------------------
            GBJavaWrapperUtilByteArrayForJavaToCExchange OutBuffer
    );

    //--------------------------------------------------------------------------
    // FREE ALLOCATED MEMORY
    //--------------------------------------------------------------------------
    public native int FreeAnsiNistAllocatedMemory(
            //-------------------------
            // INPUT/OUTPUT
            //-------------------------
            GBANJavaWrapperDefinesANStruct AnsiNist
    );

    public native int FreeRecordAllocatedMemory(
            //-------------------------
            // INPUT/OUTPUT
            //-------------------------
            GBANJavaWrapperDefinesRecordStruct Record
    );

	//--------------------------------------------------------------------------
	// AN2011 STRUCT
	//--------------------------------------------------------------------------

    public native int CreateAnsiNist2011(
            GBANJavaWrapperDefinesANStruct AnsiNist,
            String TransactionType, // MANDATORY
            GBAN2011JavaWrapperDefinesDateStruct Date, // MANDATORY
            int Priority,
            String DestinationAgency, // MANDATORY
            String OriginatingAgency, // MANDATORY
            String TransactionControlID, // MANDATORY
            String TransactionControlReference, // OPTIONAL, NULL to avoid
            double NativeScanResolution, // MANDATORY, 0 if no type-4 is present, else [1-99]
            double NominalTransmitResolution, // OPTIONAL, -1 to avoid, 0 if no type-4 is present, else [1-99]
            String DomainNameID, // OPTIONAL, NULL to avoid
            String DomainVersion, // OPTIONAL, NULL to avoid
            GBAN2011JavaWrapperDefinesGmtStruct Gmt, // OPTIONAL, NULL to avoid
            int CharEncIndex, // OPTIONAL, -1 to avoid, Values: 0, 2, 3, 4, [128-999]
            String CharEncName, // OPTIONAL, NULL to avoid, but MUST be present if jCharEncIndex is valid
            String CharEncVersion, // OPTIONAL, NULL to avoid
            GBAN2011JavaWrapperDefinesApplicationProfileStruct [] Aps, // OPTIONAL, NULL to avoid
            String DestAgName, // OPTIONAL, NULL to avoid
            String OrigAgName // OPTIONAL, NULL to avoid
    );

    public native int GetType1InfoFromAnsiNist2011(
			GBANJavaWrapperDefinesANStruct AnsiNist,
			GBJavaWrapperUtilIntForJavaToCExchange Version, // int
			GBJavaWrapperUtilStringForJavaToCExchange TypeOfTransaction, // string
			GBAN2011JavaWrapperDefinesDateStruct Date, // GB_AN2011_DateStruct
			GBJavaWrapperUtilIntForJavaToCExchange Priority, // int
			GBJavaWrapperUtilStringForJavaToCExchange DestAgency, // string
			GBJavaWrapperUtilStringForJavaToCExchange OrigAgency, // string
			GBJavaWrapperUtilStringForJavaToCExchange TransContrNum, // string
			GBJavaWrapperUtilStringForJavaToCExchange TransContrRef, // string
			GBJavaWrapperUtilDoubleForJavaToCExchange NativeScanRes, // float
			GBJavaWrapperUtilDoubleForJavaToCExchange NominalTxRes, // float
			GBJavaWrapperUtilStringForJavaToCExchange DomainName, // string
			GBJavaWrapperUtilStringForJavaToCExchange DomainVersion, // string
			GBAN2011JavaWrapperDefinesGmtStruct Gmt, // GB_AN2011_GmtStruct
			GBJavaWrapperUtilIntForJavaToCExchange CharEncIndex,
			GBJavaWrapperUtilStringForJavaToCExchange CharEncName, // string
			GBJavaWrapperUtilStringForJavaToCExchange CharEncVersion, // string
			GBAN2011JavaWrapperDefinesApplicationProfileStruct [] Aps, // pass a 99 items wide array
			GBJavaWrapperUtilIntForJavaToCExchange ApsNum, // int
			GBJavaWrapperUtilStringForJavaToCExchange DestAgName, // string
			GBJavaWrapperUtilStringForJavaToCExchange OrigAgName // string
	);

	//--------------------------------------------------------------------------
	// AN2011 TYPE 4
	//--------------------------------------------------------------------------

    public native int CreateType4Record2011(
			GBANJavaWrapperDefinesRecordStruct Record,
			int Idc,
			int ImpressionType,
			int FingerPosition,
			int ScanResolutionId,
			int FPImageWidth,
			int FPImageHeight,
			int CompressionType,
			byte [] FingerprintImage
	);

    public native int GetType4Info2011(
			GBANJavaWrapperDefinesRecordStruct Record,
			GBJavaWrapperUtilIntForJavaToCExchange Idc, // int
			GBJavaWrapperUtilIntForJavaToCExchange ImpressionType, // int
			GBJavaWrapperUtilIntForJavaToCExchange FingerPosition, // int
			GBJavaWrapperUtilIntForJavaToCExchange ScanResolutionId, // int
			GBJavaWrapperUtilIntForJavaToCExchange FPImageWidth, // int
			GBJavaWrapperUtilIntForJavaToCExchange FPImageHeight, // int
			GBJavaWrapperUtilIntForJavaToCExchange CompressionType, // int
			GBJavaWrapperUtilByteArrayForJavaToCExchange FingerprintImage // byte []
	);

	//--------------------------------------------------------------------------
	// AN2011 TYPE 14
	//--------------------------------------------------------------------------

	public native int CreateType14Record2011(
			GBANJavaWrapperDefinesRecordStruct Record,
			int Idc, // MANDATORY
			int ImpressionType, // MANDATORY
			String SourceAgency, // MANDATORY
			GBAN2011JavaWrapperDefinesDateStruct FingerprintCaptureDate, // GB_AN2011_DateStruct, // MANDATORY
			int HorizontalNumberOfPixels, // MANDATORY
			int VerticalNumberOfPixels, // MANDATORY
			int ScaleUnits, // MANDATORY
			int TransmittedHorizontalPixelScale, // MANDATORY
			int TransmittedVerticalPixelScale, // MANDATORY
			int CompressionAlgorithm, // MANDATORY
			int BitsPerPixel, // MANDATORY
			int FingerprintPosition, // MANDATORY
			GBAN2011JavaWrapperDefinesPrintPositionDescriptorsStruct Ppd, // GB_AN2011_PrintPositionDescriptorsStruct // MANDATORY if FingerprintPosition == 19
			GBAN2011JavaWrapperDefinesPrintPositionCoordinatesStruct [] Ppc, // GB_AN2011_PrintPositionCoordinatesStruct[] // OPTIONAL
			int ScannedHorizontalPixelScale, // OPTIONAL
			int ScannedVerticalPixelScale, // OPTIONAL
			GBAN2011JavaWrapperDefinesAmputatedBandagedCodeStruct [] Amp, // GB_AN2011_AmputatedBandagedCodeStruct[] // OPTIONAL
			String Comment, // OPTIONAL
			GBAN2011JavaWrapperDefinesFingerSegmentPositionStruct [] Seg, // GB_AN2011_FingerSegmentPositionStruct[] // MANDATORY if FingerprintPosition in [13-15] or [40-50]
			GBAN2011JavaWrapperDefinesNistQualityMetricStruct [] Nqm, // GB_AN2011_NistQualityMetricStruct[] // OPTIONAL
			GBAN2011JavaWrapperDefinesFingerOrSegmentQualityMetricStruct [] Sqm, // GB_AN2011_FingerOrSegmentQualityMetricStruct[] // OPTIONAL
			GBAN2011JavaWrapperDefinesFingerOrSegmentQualityMetricStruct [] Fqm, // GB_AN2011_FingerOrSegmentQualityMetricStruct[] // OPTIONAL
			GBAN2011JavaWrapperDefinesAlternateSegmentPositionStruct [] Aseg, // GB_AN2011_AlternateSegmentPositionStruct[] // OPTIONAL
			int SimultaneousCaptureFingerprint, // OPTIONAL
			int StitchedImageFlag, // boolean, // OPTIONAL
			String DeviceMonitoringMode, // OPTIONAL
			int FingerprintAcquisitionProfile, // OPTIONAL
			byte [] FingerprintImage // MANDATORY
	);

	public native int GetType14Info2011(
			GBANJavaWrapperDefinesRecordStruct Record,
			GBJavaWrapperUtilIntForJavaToCExchange Idc, // int
			GBJavaWrapperUtilIntForJavaToCExchange ImpressionType, // int
			GBJavaWrapperUtilStringForJavaToCExchange SourceAgency, // string
			GBAN2011JavaWrapperDefinesDateStruct FingerprintCaptureDate, // GB_AN2011_DateStruct
			GBJavaWrapperUtilIntForJavaToCExchange HorizontalNumberOfPixels, // int
			GBJavaWrapperUtilIntForJavaToCExchange VerticalNumberOfPixels, // int
			GBJavaWrapperUtilIntForJavaToCExchange ScaleUnits, // int
			GBJavaWrapperUtilIntForJavaToCExchange TransmittedHorizontalPixelScale, // int
			GBJavaWrapperUtilIntForJavaToCExchange TransmittedVerticalPixelScale, // int
			GBJavaWrapperUtilIntForJavaToCExchange CompressionAlgorithm, // int
			GBJavaWrapperUtilIntForJavaToCExchange BitsPerPixel, // int
			GBJavaWrapperUtilIntForJavaToCExchange FingerprintPosition, // int
			GBAN2011JavaWrapperDefinesPrintPositionDescriptorsStruct Ppd, // GB_AN2011_PrintPositionDescriptorsStruct
			GBAN2011JavaWrapperDefinesPrintPositionCoordinatesStruct [] Ppc, // GB_AN2011_PrintPositionCoordinatesStruct[12]
			GBJavaWrapperUtilIntForJavaToCExchange PpcNum, // int
			GBJavaWrapperUtilIntForJavaToCExchange ScannedHorizontalPixelScale, // int
			GBJavaWrapperUtilIntForJavaToCExchange ScannedVerticalPixelScale, // int
			GBAN2011JavaWrapperDefinesAmputatedBandagedCodeStruct [] Amp, // GB_AN2011_AmputatedBandagedCodeStruct[5]
			GBJavaWrapperUtilIntForJavaToCExchange AmpNum, // int
			GBJavaWrapperUtilStringForJavaToCExchange Comment, // string
			GBAN2011JavaWrapperDefinesFingerSegmentPositionStruct [] Seg, // GB_AN2011_FingerSegmentPositionStruct[5]
			GBJavaWrapperUtilIntForJavaToCExchange SegNum, // int
			GBAN2011JavaWrapperDefinesNistQualityMetricStruct [] Nqm, // GB_AN2011_NistQualityMetricStruct[5]
			GBJavaWrapperUtilIntForJavaToCExchange NqmNum, // int
			GBAN2011JavaWrapperDefinesFingerOrSegmentQualityMetricStruct [] Sqm, // GB_AN2011_FingerOrSegmentQualityMetricStruct[5]
			GBJavaWrapperUtilIntForJavaToCExchange SqmNum, // int
			GBAN2011JavaWrapperDefinesFingerOrSegmentQualityMetricStruct [] Fqm, // GB_AN2011_FingerOrSegmentQualityMetricStruct[5]
			GBJavaWrapperUtilIntForJavaToCExchange FqmNum, // int
			GBAN2011JavaWrapperDefinesAlternateSegmentPositionStruct [] Aseg, // GB_AN2011_AlternateSegmentPositionStruct[5]
			GBJavaWrapperUtilIntForJavaToCExchange AsegNum, // int
			GBJavaWrapperUtilIntForJavaToCExchange SimultaneousCaptureFingerprint, // int
			GBJavaWrapperUtilIntForJavaToCExchange StitchedImageFlag, // boolean as int
			GBJavaWrapperUtilStringForJavaToCExchange DeviceMonitoringMode, // string
			GBJavaWrapperUtilIntForJavaToCExchange FingerprintAcquisitionProfile, // int
			GBJavaWrapperUtilByteArrayForJavaToCExchange ImageBuffer // byte []
	);

	//--------------------------------------------------------------------------
	// AN2011 TYPE 15
	//--------------------------------------------------------------------------

	public native int CreateType15Record2011(
			GBANJavaWrapperDefinesRecordStruct Record,
			int Idc, // MANDATORY
			int ImpressionType, // MANDATORY
			String SourceAgency, // MANDATORY
			GBAN2011JavaWrapperDefinesDateStruct PalmprintCaptureDate, // GB_AN2011_DateStruct, // MANDATORY
			int HorizontalNumberOfPixels, // MANDATORY
			int VerticalNumberOfPixels, // MANDATORY
			int ScaleUnits, // MANDATORY
			int TransmittedHorizontalPixelScale, // MANDATORY
			int TransmittedVerticalPixelScale, // MANDATORY
			int CompressionAlgorithm, // MANDATORY
			int BitsPerPixel, // MANDATORY
			int FingerprintPosition, // MANDATORY
			int ScannedHorizontalPixelScale, // OPTIONAL
			int ScannedVerticalPixelScale, // OPTIONAL
			GBAN2011JavaWrapperDefinesAmputatedBandagedCodeStruct [] Amp, // GB_AN2011_AmputatedBandagedCodeStruct[] // OPTIONAL
			String Comment, // OPTIONAL
			GBAN2011JavaWrapperDefinesFingerOrSegmentQualityMetricStruct [] Pqm, // GB_AN2011_FingerOrSegmentQualityMetricStruct[] // OPTIONAL
			String DeviceMonitoringMode, // OPTIONAL
			byte [] FingerprintImage // MANDATORY
	);

	public native int GetType15Info2011(
			GBANJavaWrapperDefinesRecordStruct Record,
			GBJavaWrapperUtilIntForJavaToCExchange Idc, // int
			GBJavaWrapperUtilIntForJavaToCExchange ImpressionType, // int
			GBJavaWrapperUtilStringForJavaToCExchange SourceAgency, // string
			GBAN2011JavaWrapperDefinesDateStruct FingerprintCaptureDate, // GB_AN2011_DateStruct
			GBJavaWrapperUtilIntForJavaToCExchange HorizontalNumberOfPixels, // int
			GBJavaWrapperUtilIntForJavaToCExchange VerticalNumberOfPixels, // int
			GBJavaWrapperUtilIntForJavaToCExchange ScaleUnits, // int
			GBJavaWrapperUtilIntForJavaToCExchange TransmittedHorizontalPixelScale, // int
			GBJavaWrapperUtilIntForJavaToCExchange TransmittedVerticalPixelScale, // int
			GBJavaWrapperUtilIntForJavaToCExchange CompressionAlgorithm, // int
			GBJavaWrapperUtilIntForJavaToCExchange BitsPerPixel, // int
			GBJavaWrapperUtilIntForJavaToCExchange FingerprintPosition, // int
			GBJavaWrapperUtilIntForJavaToCExchange ScannedHorizontalPixelScale, // int
			GBJavaWrapperUtilIntForJavaToCExchange ScannedVerticalPixelScale, // int
			GBAN2011JavaWrapperDefinesAmputatedBandagedCodeStruct [] Amp, // GB_AN2011_AmputatedBandagedCodeStruct[5]
			GBJavaWrapperUtilIntForJavaToCExchange AmpNum, // int
			GBJavaWrapperUtilStringForJavaToCExchange Comment, // string
			GBAN2011JavaWrapperDefinesFingerOrSegmentQualityMetricStruct [] Sqm, // GB_AN2011_FingerOrSegmentQualityMetricStruct[5]
			GBJavaWrapperUtilIntForJavaToCExchange SqmNum, // int
			GBJavaWrapperUtilStringForJavaToCExchange DeviceMonitoringMode, // string
			GBJavaWrapperUtilByteArrayForJavaToCExchange ImageBuffer // byte []
	);

	//--------------------------------------------------------------------------
	// AN ENGINE
	//--------------------------------------------------------------------------

	public native int AnEngineCreateSubFieldFromValuesForTaggedRecords(
			GBANJavaWrapperDefinesSubfieldStruct Subfield,
			String [] ValuesArray // String []
	);

	public native int AnEngineCreateEmptyFieldForTaggedRecords(
			GBANJavaWrapperDefinesFieldStruct Field,
			int RecordType,
			int FieldId
	);

	public native int AnEngineInsertSubfieldIntoFieldForTaggedRecords(
			GBANJavaWrapperDefinesFieldStruct FieldToUpdate,
			GBANJavaWrapperDefinesSubfieldStruct SubfieldToInsert
	);

	public native int AnEngineCreateFieldFromValuesForTaggedRecords(
			GBANJavaWrapperDefinesFieldStruct Field,
			int RecordType,
			int FieldId,
			String [] ValuesArray // String []
	);

	public native int AnEngineCreateImageFieldForTaggedRecord(
			GBANJavaWrapperDefinesFieldStruct Field,
			int RecordType,
			byte [] ImageBuffer // MANDATORY
	);

	public native int AnEngineCreateEmptyTaggedRecord(
			GBANJavaWrapperDefinesRecordStruct Record,
			int RecordType,
			int Idc
	);

	public native int AnEngineInsertFieldIntoTaggedRecord(
			GBANJavaWrapperDefinesRecordStruct RecordToUpdate,
			GBANJavaWrapperDefinesFieldStruct FieldToInsert
	);

	public native int AnEngineCreateBinaryRecord(
			GBANJavaWrapperDefinesRecordStruct Record,
			int RecordType,
			int Idc,
			int ImpressionType,
			byte [] FingerPositionArray,
			int ImageScanResolution,
			int HorizontalNumberOfPixels,
			int VerticalNumberOfPixels,
			int CompressionAlgorithm,
			byte [] FingerprintImage
	);

	public native int AnEngineCreateEmptyAnsiNist(
			GBANJavaWrapperDefinesANStruct AnsiNist
	);

	public native int AnEngineInsertRecordIntoAnsiNistStruct(
			GBANJavaWrapperDefinesANStruct AnsiNistToUpdate,
			GBANJavaWrapperDefinesRecordStruct RecordToInsert,
			int RecordIndex,
			int AutomaticIdc // boolean as int
	);

	public native int AnEngineSearchFieldInTaggedRecord(
			GBANJavaWrapperDefinesRecordStruct Record,
			int FieldId,
			GBANJavaWrapperDefinesFieldStruct FoundField
	);

	public native int AnEngineGetValuesFromFieldForTaggedRecords(
			GBANJavaWrapperDefinesFieldStruct Field,
			GBJavaWrapperUtilStringArrayForJavaToCExchange Values
	);

	public native int AnEngineGetValuesFromSubfieldForTaggedRecords(
			GBANJavaWrapperDefinesSubfieldStruct Subfield,
			GBJavaWrapperUtilStringArrayForJavaToCExchange Values
	);

	public native int AnEngineGetImageFieldFromTaggedRecord(
			GBANJavaWrapperDefinesRecordStruct Record,
			GBJavaWrapperUtilByteArrayForJavaToCExchange ImageBuffer // GBJavaWrapperUtilByteArrayForJavaToCExchange
	);

	public native int AnEngineGetBinaryRecordInfo(
			GBANJavaWrapperDefinesRecordStruct Record,
			GBJavaWrapperUtilIntForJavaToCExchange RecordType, // int
			GBJavaWrapperUtilIntForJavaToCExchange Idc, // int
			GBJavaWrapperUtilIntForJavaToCExchange ImpressionType, // int
			GBJavaWrapperUtilByteArrayForJavaToCExchange Fgp, // GBJavaWrapperUtilByteArrayForJavaToCExchange
			GBJavaWrapperUtilIntForJavaToCExchange ImageScanResolution, // int
			GBJavaWrapperUtilIntForJavaToCExchange HorizontalNumberOfPixels, // int
			GBJavaWrapperUtilIntForJavaToCExchange VerticalNumberOfPixels, // int
			GBJavaWrapperUtilIntForJavaToCExchange CompressionAlgorithm, // int
			GBJavaWrapperUtilByteArrayForJavaToCExchange ImageBuffer // GBJavaWrapperUtilByteArrayForJavaToCExchange
	);

	public native int AnEngineDeleteFieldFromTaggedRecord(
			GBANJavaWrapperDefinesRecordStruct Record,
			int FieldId
	);

	public native int AnEngineDeleteFieldFromTaggedRecordInAnsiNist(
			GBANJavaWrapperDefinesANStruct AnsiNistToUpdate,
			int RecordIndex,
			int FieldId
	);

	public native int AnEngineGetAnsiNistVersionAndNumOfRecords(
			GBANJavaWrapperDefinesANStruct AnsiNist,
			GBJavaWrapperUtilIntForJavaToCExchange Version, // int
			GBJavaWrapperUtilIntForJavaToCExchange NumRecords // int
	);

	public native int AnEngineGetRecordsListFromAnsiNist(
			GBANJavaWrapperDefinesANStruct AnsiNist,
			GBANJavaWrapperDefinesRecordStruct [] ListOfRecords // int
	);

	public native int AnEngineGetFieldsListFromRecord(
			GBANJavaWrapperDefinesRecordStruct Record,
			GBANJavaWrapperDefinesFieldStruct [] ListOfFields // int
	);

	public native int AnEngineGetFieldGeneralInfo(
			GBANJavaWrapperDefinesFieldStruct Field,
			GBJavaWrapperUtilIntForJavaToCExchange ContainingRecordType, // int
			GBJavaWrapperUtilIntForJavaToCExchange FieldId, // int
			GBJavaWrapperUtilIntForJavaToCExchange NumSubfields // int
	);

	public native int AnEngineGetRecordTypeAndNumFields(
			GBANJavaWrapperDefinesRecordStruct Record,
			GBJavaWrapperUtilIntForJavaToCExchange Type, // int
			GBJavaWrapperUtilIntForJavaToCExchange NumFields // int
	);

	public native int AnEngineGetSubfieldsListFromField(
			GBANJavaWrapperDefinesFieldStruct Field,
			GBANJavaWrapperDefinesSubfieldStruct [] ListOfSubields // int
	);

}
