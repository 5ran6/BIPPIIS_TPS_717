package com.greenbit.MultiscanJNIGuiJavaAndroid.models;

import com.google.gson.annotations.SerializedName;

public class LoginRequest {
    @SerializedName("")
    private String army_number;

    public String getBippiis_number() {
        return army_number;
    }

    public void setBippiis_number(String army_number) {
        this.army_number = army_number;
    }
}
