package com.greenbit.ansinistitl;

public class GBAN2011JavaWrapperDefinesNistQualityMetricStruct
{
	/*****************************************
	 ** Can assume values in the ranges:
	 ** [1-10], 16, 17
	 ** See Table 8
	 *****************************************/
	public int FrictionRidgePosition;
	/*****************************************
	 ** Can assume values in the ranges:
	 ** [1-5], 254 (No Score), 255 (Failed
	 ** attempt)
	 *****************************************/
	public int Quality;

	public void Build(int frictionRidgePosition, int quality)
	{
		FrictionRidgePosition = frictionRidgePosition;
		Quality = quality;
	}
}
