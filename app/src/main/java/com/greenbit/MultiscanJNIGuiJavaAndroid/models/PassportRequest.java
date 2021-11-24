package com.greenbit.MultiscanJNIGuiJavaAndroid.models;

import com.google.gson.annotations.SerializedName;

public class PassportRequest {
    public String phone;
    @SerializedName("passport")
    private String passport; //base64 string
  @SerializedName("army_number")
    private String army_number; //base64 string

    public String getPassport() {
        return passport;
    }

    public void setPassport(String passport) {
        this.passport = passport;
    }

    public String getArmy_number() {
        return army_number;
    }

    public void setArmy_number(String army_number) {
        this.army_number = army_number;
    }

    public String token;
}
