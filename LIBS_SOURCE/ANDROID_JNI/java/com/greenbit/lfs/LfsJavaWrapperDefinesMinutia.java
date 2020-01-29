package com.greenbit.lfs;

public class LfsJavaWrapperDefinesMinutia {
    int XCoord;
    int YCoord;
    int Direction;
    double Reliability;
    int Type;
    public void SetFields(int x, int y, int direct, double reliab, int type)
    {
        XCoord = x;
        YCoord = y;
        Direction = direct;
        Reliability =  reliab;
        Type = type;
    }
}
