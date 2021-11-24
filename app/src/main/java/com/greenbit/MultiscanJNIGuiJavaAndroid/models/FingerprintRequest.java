package com.greenbit.MultiscanJNIGuiJavaAndroid.models;

import com.google.gson.annotations.SerializedName;

import java.util.ArrayList;

public class FingerprintRequest {

    @SerializedName("type")
    private String type;
    @SerializedName("fingerprints")
    private ArrayList<String> fingerprints;
    @SerializedName("fingerprint_images")
    private ArrayList<String> fingerprintsImages;

   public  String phone;
    public  String token;

    public ArrayList<String> getFingerprints() {
        return fingerprints;
    }

    public void setFingerprints(ArrayList<String> fingerprints) {
        this.fingerprints = fingerprints;
    }

    public ArrayList<String> getFingerprintsImages() {
        return fingerprintsImages;
    }

    public void setFingerprintsImages(ArrayList<String> fingerprintsImages) {
        this.fingerprintsImages = fingerprintsImages;
    }

    public void setType(String type) {
        this.type = type;
    }

    public String getType() {
        return type;
    }

}
