package com.greenbit.MultiscanJNIGuiJavaAndroid.models;

import com.google.gson.annotations.SerializedName;

public class UsersDetails {
    @SerializedName("lga_pensioners")
    private Object lga_pensionsers;

    public Object getLga_pensionsers() {
        return lga_pensionsers;
    }

    public void setLga_pensionsers(Object lga_pensionsers) {
        this.lga_pensionsers = lga_pensionsers;
    }
}
