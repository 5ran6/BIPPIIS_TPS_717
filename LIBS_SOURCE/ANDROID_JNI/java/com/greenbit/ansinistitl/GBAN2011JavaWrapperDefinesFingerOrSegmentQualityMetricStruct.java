package com.greenbit.ansinistitl;

public class GBAN2011JavaWrapperDefinesFingerOrSegmentQualityMetricStruct
{
	/*****************************************
	 ** Can assume values in the ranges:
	 ** [1-10], 16, 17 (for type-14 record)
	 ** [21-38], [81-84] (for type-15 record)
	 ** See Table 8
	 *****************************************/
	public int FrictionRidgePosition;
	/*****************************************
	 ** Can assume values in the ranges:
	 ** [0-100], 254 (No Score), 255 (Failed
	 ** attempt)
	 *****************************************/
	public int QualityValue;
	/*****************************************
	 ** Can assume values in the ranges:
	 ** [0x0000-0xFFFF]
	 *****************************************/
	public int AlgorithmVendorId;
	/*****************************************
	 ** Can assume values in the ranges:
	 ** [0x0000-0xFFFF]
	 *****************************************/
	public int AlgorithmProductId;
}
