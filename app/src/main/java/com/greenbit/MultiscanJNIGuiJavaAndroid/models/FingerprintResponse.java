package com.greenbit.MultiscanJNIGuiJavaAndroid.models;

import com.google.gson.annotations.Expose;
import com.google.gson.annotations.SerializedName;

public class FingerprintResponse {

    @SerializedName("data")
    @Expose
    private Object data;

    @SerializedName("status")
    @Expose
    private String status;

    @SerializedName("status_code")
    @Expose
    private String status_code;

    @SerializedName("message")
    @Expose
    private String message;
    @SerializedName("profile")
    @Expose
    private String profile;


    @SerializedName("token")
    @Expose
    private String token;

    public String getToken() {
        return token;
    }

    public void setToken(String token) {
        this.token = token;
    }

    public String getStatus() {
        return status;
    }

    public void setStatus(String status) {
        this.status = status;
    }

    public String getStatus_code() {
        return status_code;
    }

    public void setStatus_code(String status_code) {
        this.status_code = status_code;
    }

    public Object getData() {

        return data;
    }

    public void setData(Object data) {
        this.data = data;
    }


    public String getMessage() {
        return message;
    }

    public void setMessage(String message) {
        this.message = message;
    }

    public String getProfile() {
        return profile;
    }

    public void setProfile(String profile) {
        this.profile = profile;
    }
}
