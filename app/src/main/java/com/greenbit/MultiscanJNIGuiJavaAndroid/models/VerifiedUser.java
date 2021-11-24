package com.greenbit.MultiscanJNIGuiJavaAndroid.models;

import com.google.gson.annotations.SerializedName;

import java.util.ArrayList;

public class VerifiedUser extends UserRegisterRequest
{
    @SerializedName("type")
    private String type;
    @SerializedName("fingerprints")
    private ArrayList<String> fingerprints;
    @SerializedName("fingerprint_images")
    private ArrayList<String> fingerprintsImages;

    public String getPassport() {
        return passport;
    }

    public void setPassport(String passport) {
        this.passport = passport;
    }

    private String passport;

    public ArrayList<String> getTemplates() {
        return templates;
    }

    public void setTemplates(ArrayList<String> templates) {
        this.templates = templates;
    }

    private  ArrayList<String> templates;
    public String getIdentification_number() {
        return identification_number;
    }

    public void setIdentification_number(String identification_number) {
        this.identification_number = identification_number;
    }

    public  String identification_number;


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
