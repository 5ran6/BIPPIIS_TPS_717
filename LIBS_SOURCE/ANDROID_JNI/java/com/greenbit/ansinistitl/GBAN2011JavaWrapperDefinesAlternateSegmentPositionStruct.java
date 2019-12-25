package com.greenbit.ansinistitl;

public class GBAN2011JavaWrapperDefinesAlternateSegmentPositionStruct
{
	/*****************************************
	 ** Can assume values in the ranges:
	 ** [1-10], 16, 17
	 ** See Table 8
	 *****************************************/
	public int FrictionRidgePosition;
	/*****************************************
	 ** Can assume values in the ranges:
	 ** [3-99]
	 *****************************************/
	public int NumberOfPoints;
	/*****************************************
	 ** Array of GB_AN2011_PointOffsetStruct
	 ** structures, representing a polygon.
	 ** The number of valid structures is
	 ** <NumberOfPoints>
	 *****************************************/
	public GBAN2011JavaWrapperDefinesPointOffsetStruct [] Points;

	public GBAN2011JavaWrapperDefinesAlternateSegmentPositionStruct()
	{
		Points = new GBAN2011JavaWrapperDefinesPointOffsetStruct[99];
		NumberOfPoints = 0;
		FrictionRidgePosition = GBAN2011JavaWrapperDefinesFingerPositions.GB_TABLE_AN2011_8_UNKNOWN;
	}
}
