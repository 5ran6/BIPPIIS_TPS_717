package com.greenbit.MultiscanJNIGuiJavaAndroid.models;

import com.google.gson.annotations.SerializedName;

public class LoginRequest {
    @SerializedName("bippiis_number")
    private String bippiis_number;

    public String getBippiis_number() {
        return bippiis_number;
    }

    public void setBippiis_number(String bippiis_number) {
        this.bippiis_number = bippiis_number;
    }
}
