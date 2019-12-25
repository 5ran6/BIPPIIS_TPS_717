package com.greenbit.ansinistitl;

public class GBANJavaWrapperDefinesSubfieldStruct
{
	/**************************************
	 * this field stores a native pointer
	 *************************************/
	public long Pointer;

	/********************************
	 * DO NOT USE OUTSIDE JNI!!!!!
	 * @return
	 */
	public long GetPointer()
	{
		return Pointer;
	}

	/********************************
	 * DO NOT USE OUTSIDE JNI!!!!!
	 * @param Ptr
	 */
	public void SetPointer(long Ptr)
	{
		Pointer = Ptr;
	}
}
