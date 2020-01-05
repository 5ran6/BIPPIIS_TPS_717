package com.greenbit.MultiscanJNIGuiJavaAndroid.models;

import com.google.gson.annotations.SerializedName;

import java.util.ArrayList;


public class FingerprintRequest {

    @SerializedName("fingerprints")
    private ArrayList<String> fingerprints;
    @SerializedName("bippiis_number")
    private String bippiis_number;

    public ArrayList<String> getFingerprints() {
        return fingerprints;
    }

    public void setFingerprints(ArrayList<String> fingerprints) {
        this.fingerprints = fingerprints;
    }

    public String getBippiis_number() {
        return bippiis_number;
    }

    public void setBippiis_number(String bippiis_number) {
        this.bippiis_number = bippiis_number;
    }
}
