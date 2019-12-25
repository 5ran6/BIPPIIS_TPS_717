package com.greenbit.gbfinimg;

public class GbfinimgJavaWrapperDefineSegmentImageDescriptor {
    public byte [] SegmentImage;
    public int FingerCenterX;
    public int FingerCenterY;
    public int Quality;
    /**
     * @see GbfinimgJavaWrapperDefinesHandNames
     */
    public int Hand;
    /**
     * @see GbfinimgJavaWrapperDefinesFingerNames
     */
    public int Finger;
    public int BoundingBoxL, BoundingBoxR, BoundingBoxT, BoundingBoxB;
    public int PatternValidity;
    public int PatternCompleteness;
    public int Size;
    public int Inclination;

    public void SetFields(
            byte [] Segment,
            int FCX, int FCY,
            int Q,
            int H, int F,
            int BBL, int BBR, int BBT, int BBB,
            int PV, int PC,
            int Area, int Inc
    )
    {
        SegmentImage = Segment;
        FingerCenterX = FCX;
        FingerCenterY = FCY;
        Quality = Q;
        Hand = H;
        Finger = F;
        BoundingBoxL = BBL;
        BoundingBoxR = BBR;
        BoundingBoxT = BBT;
        BoundingBoxB = BBB;
        PatternValidity = PV;
        PatternCompleteness = PC;
        Size = Area;
        Inclination = Inc;
    }
}
