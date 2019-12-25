package com.greenbit.ansinistitl;

public class GBAN2011JavaWrapperDefinesPrintPositionCoordinatesStruct
{
	/*****************************************
	 ** Can assume the following values in
	 ** table 9:
	 ** NA (if LocationOfASegment != NA),
	 ** TIP, FV1, ..., FV4
	 * max 9 ASCII chars
	 *****************************************/
	public String FullFingerView;
	/*****************************************
	 ** Can assume the following values in
	 ** table 9:
	 ** NA (if FullFingerView != NA),
	 ** PRX, DST, MED
	 * max 9 ASCII chars
	 *****************************************/
	public String LocationOfASegment;
	/*****************************************
	 ** Can assume values in the ranges:
	 ** [0-HLL]
	 *****************************************/
	public int LeftHorizontalCoordinate;
	/*****************************************
	 ** Can assume values in the ranges:
	 ** [LeftHorizontalCoordinate-HLL]
	 *****************************************/
	public int RightHorizontalCoordinate;
	/*****************************************
	 ** Can assume values in the ranges:
	 ** [0-VLL]
	 *****************************************/
	public int TopVerticalCoordinate;
	/*****************************************
	 ** Can assume values in the ranges:
	 ** [TopVerticalCoordinate-VLL]
	 *****************************************/
	public int BottomVerticalCoordinate;
}
