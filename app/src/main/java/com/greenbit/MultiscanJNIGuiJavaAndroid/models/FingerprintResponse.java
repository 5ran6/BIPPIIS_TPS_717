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

    public String getToken() {
        return token;
    }

    public void setToken(String token) {
        this.token = token;
    }

    @SerializedName("token")
    @Expose
    private String token;


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


}
