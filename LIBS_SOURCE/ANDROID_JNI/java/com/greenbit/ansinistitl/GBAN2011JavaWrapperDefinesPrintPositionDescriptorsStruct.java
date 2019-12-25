package com.greenbit.ansinistitl;

public class GBAN2011JavaWrapperDefinesPrintPositionDescriptorsStruct
{
	/*****************************************
	 ** Can assume values in the ranges:
	 ** [1-10], 16, 17
	 ** See Table 8
	 *****************************************/
	public int DecimalFingerPosition;
	/*****************************************
	 ** Can assume the following values in
	 ** table 9:
	 ** EJI, TIP, FV1, ..., FV4, PRX, DST, MED
	 * max 9 ASCII chars
	 *****************************************/
	public String FingerImageCode;
}
