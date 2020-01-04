//package com.greenbit.MultiscanJNIGuiJavaAndroid.models;
//
//import com.google.gson.annotations.Expose;
//import com.google.gson.annotations.SerializedName;
//
//import java.io.Serializable;
//
//public class LoginBack implements Serializable {
//    /*
//    *  // Creating toString
//        @Override
//        public String toString()
//        {
//            return "Organisation [organisation_name="
//                + organisation_name
//                + ", description="
//                + description
//                + ", Employees="
//                + Employees + "]";
//        }
//        *
//        *  "id": 4,
//            "bippiis_number": "A/S 504",
//            "category": 2,
//            "created_at": "2019-10-16 05:06:06",
//            "updated_at": "2019-10-24 18:46:24",
//            "verification_type": 0,
//            "has_password": true,
//            "has_enrolled": false,
//            "biometrics": []
//    * */
//    @SerializedName("id")
//    @Expose
//    private String id;
//
//    @SerializedName("bippiis_number")
//    @Expose
//    private String bippiis_number;
//
//    @SerializedName("category")
//    @Expose
//    private String category;
//
//    @SerializedName("created_at")
//    @Expose
//    private String created_at;
//
//    @SerializedName("updated_at")
//    @Expose
//    private String updated_at;
//
//    @SerializedName("verification_type")
//    @Expose
//    private String verification_type;
//
//
//    @SerializedName("has_password")
//    @Expose
//    private String has_password;
//
//    @SerializedName("has_enrolled")
//    @Expose
//    private String has_enrolled;
//
//    public String getId() {
//        return id;
//    }
//
//    public void setId(String id) {
//        this.id = id;
//    }
//
//    public String getBippiis_number() {
//        return bippiis_number;
//    }
//
//    public void setBippiis_number(String bippiis_number) {
//        this.bippiis_number = bippiis_number;
//    }
//
//    public String getCategory() {
//        return category;
//    }
//
//    public void setCategory(String category) {
//        this.category = category;
//    }
//
//    public String getCreated_at() {
//        return created_at;
//    }
//
//    public void setCreated_at(String created_at) {
//        this.created_at = created_at;
//    }
//
//    public String getUpdated_at() {
//        return updated_at;
//    }
//
//    public void setUpdated_at(String updated_at) {
//        this.updated_at = updated_at;
//    }
//
//    public String getVerification_type() {
//        return verification_type;
//    }
//
//    public void setVerification_type(String verification_type) {
//        this.verification_type = verification_type;
//    }
//
//    public String getHas_password() {
//        return has_password;
//    }
//
//    public void setHas_password(String has_password) {
//        this.has_password = has_password;
//    }
//
//    public String getHas_enrolled() {
//        return has_enrolled;
//    }
//
//    public void setHas_enrolled(String has_enrolled) {
//        this.has_enrolled = has_enrolled;
//    }
//
//    public String getBiometrics() {
//        return biometrics;
//    }
//
//    public void setBiometrics(String biometrics) {
//        this.biometrics = biometrics;
//    }
//
//    @SerializedName("biometrics")
//    @Expose
//    private String biometrics;
//
//
//    public String getbippiis_number() {
//        return bippiis_number;
//    }
//
//    public void setbippiis_number(String bippiis_number) {
//        this.bippiis_number = bippiis_number;
//    }
//
//    public String getcategory() {
//        return category;
//    }
//
//    public void setcategory(String category) {
//        this.category = category;
//    }
//
//    public String getid() {
//        return id;
//    }
//
//    public void setid(String id) {
//        this.id = id;
//    }
//
//    @Override
//    public String toString()
//    {
//        return "data [organisation_name="
//                + organisation_name
//                + ", description="
//                + description
//                + ", Employees="
//                + Employees + "]";
//    }
//
//}
