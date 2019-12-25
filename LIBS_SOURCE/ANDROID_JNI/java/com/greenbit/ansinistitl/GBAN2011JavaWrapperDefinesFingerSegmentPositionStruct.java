package com.greenbit.ansinistitl;

public class GBAN2011JavaWrapperDefinesFingerSegmentPositionStruct
{
	public void Build(int frictionRidgePosition, int leftHorizontalCoordinate, int rightHorizontalCoordinate, int topVerticalCoordinate, int bottomVerticalCoordinate)
	{
		FrictionRidgePosition = frictionRidgePosition;
		LeftHorizontalCoordinate = leftHorizontalCoordinate;
		RightHorizontalCoordinate = rightHorizontalCoordinate;
		BottomVerticalCoordinate = bottomVerticalCoordinate;
	}
	/*****************************************
	 ** Can assume values in the ranges:
	 ** [1-10], 16, 17
	 ** See Table 8
	 *****************************************/
	public int FrictionRidgePosition;
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
