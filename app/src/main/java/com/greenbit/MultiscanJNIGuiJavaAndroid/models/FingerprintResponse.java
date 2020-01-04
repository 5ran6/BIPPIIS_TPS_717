package com.greenbit.MultiscanJNIGuiJavaAndroid.models;

import com.google.gson.annotations.Expose;
import com.google.gson.annotations.SerializedName;

public class FingerprintResponse {
    @SerializedName("data")
    @Expose
    private String data;

    @SerializedName("status")
    @Expose
    private String status;

    @SerializedName("status_code")
    @Expose
    private String status_code;

    @SerializedName("code")
    @Expose
    private String code;

    @SerializedName("message")
    @Expose
    private String message;

    public String getData() {
        return data;
    }

    public void setData(String data) {
        this.data = data;
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

    public String getCode() {
        return code;
    }

    public void setCode(String code) {
        this.code = code;
    }

    public String getMessage() {
        return message;
    }

    public void setMessage(String message) {
        this.message = message;
    }


}
