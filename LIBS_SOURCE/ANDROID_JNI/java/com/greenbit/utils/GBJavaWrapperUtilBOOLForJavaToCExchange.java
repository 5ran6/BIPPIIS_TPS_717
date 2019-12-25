package com.greenbit.utils;

public class GBJavaWrapperUtilBOOLForJavaToCExchange {
    public boolean Value;
    public void SetFromInt(int val)
    {
        Value = (val != 0)?true:false;
    }
    public int GetAsInt()
    {
        if (Value) return 1;
        else return 0;
    }
    public void SetFromBoolean(boolean val){Value = val;}
    public boolean GetAsBoolean(){return Value;}
}
