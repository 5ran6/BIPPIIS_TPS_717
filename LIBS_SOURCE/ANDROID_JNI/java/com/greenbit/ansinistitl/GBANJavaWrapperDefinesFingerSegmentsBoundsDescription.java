package com.greenbit.ansinistitl;

public class GBANJavaWrapperDefinesFingerSegmentsBoundsDescription
{
    public String FingID;
    public String Left;
    public String Right;
    public String Top;
    public String Bottom;

    public void Build (String fingID,String left, String right, String top, String bottom)
    {
        FingID = fingID;
        Left = left;
        Right = right;
        Top = top;
        Bottom = bottom;
    }
}
