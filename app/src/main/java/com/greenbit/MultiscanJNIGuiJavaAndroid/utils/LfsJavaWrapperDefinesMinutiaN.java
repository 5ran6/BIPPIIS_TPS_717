package com.greenbit.MultiscanJNIGuiJavaAndroid.utils;

import java.io.Serializable;

public class LfsJavaWrapperDefinesMinutiaN implements Serializable {
    private int XCoord;
    private int YCoord;
    private int Direction;
    private double Reliability;
    private int Type;

    public LfsJavaWrapperDefinesMinutiaN() {
    }

    public void SetFields(int x, int y, int direct, double reliab, int type) {
        this.setXCoord(x);
        this.setYCoord(y);
        this.setDirection(direct);
        this.setReliability(reliab);
        this.setType(type);
    }

    public int getXCoord() {
        return XCoord;
    }

    public void setXCoord(int XCoord) {
        this.XCoord = XCoord;
    }

    public int getYCoord() {
        return YCoord;
    }

    public void setYCoord(int YCoord) {
        this.YCoord = YCoord;
    }

    public int getDirection() {
        return Direction;
    }

    public void setDirection(int direction) {
        Direction = direction;
    }

    public double getReliability() {
        return Reliability;
    }

    public void setReliability(double reliability) {
        Reliability = reliability;
    }

    public int getType() {
        return Type;
    }

    public void setType(int type) {
        Type = type;
    }
}
