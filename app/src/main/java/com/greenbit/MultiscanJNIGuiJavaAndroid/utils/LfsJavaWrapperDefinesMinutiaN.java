package com.greenbit.MultiscanJNIGuiJavaAndroid.utils;

import java.io.Serializable;

public class LfsJavaWrapperDefinesMinutiaN implements Serializable {
    int XCoord;
    int YCoord;
    int Direction;
    double Reliability;
    int Type;

    public LfsJavaWrapperDefinesMinutiaN() {
    }

    void SetFields(int x, int y, int direct, double reliab, int type) {
        this.XCoord = x;
        this.YCoord = y;
        this.Direction = direct;
        this.Reliability = reliab;
        this.Type = type;
    }
}
