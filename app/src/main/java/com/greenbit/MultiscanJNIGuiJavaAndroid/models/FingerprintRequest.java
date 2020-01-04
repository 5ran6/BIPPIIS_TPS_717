package com.greenbit.MultiscanJNIGuiJavaAndroid.models;

import java.util.ArrayList;

public class FingerprintRequest {


    private ArrayList fingerprints;
    private String bippiis_number;

    public ArrayList getFingerprints() {
        return fingerprints;
    }

    public void setFingerprints(ArrayList fingerprints) {
        this.fingerprints = fingerprints;
    }

    public String getBippiis_number() {
        return bippiis_number;
    }

    public void setBippiis_number(String bippiis_number) {
        this.bippiis_number = bippiis_number;
    }
}
