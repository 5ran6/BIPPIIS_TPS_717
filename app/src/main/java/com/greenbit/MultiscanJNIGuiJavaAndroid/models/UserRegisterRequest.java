package com.greenbit.MultiscanJNIGuiJavaAndroid.models;

import com.google.gson.annotations.Expose;
import com.google.gson.annotations.SerializedName;


public class UserRegisterRequest {



    @SerializedName("date_of_birth")
    @Expose
    private String date_of_birth;
    @SerializedName("qualification")
    @Expose
    private String qualification;
    @SerializedName("address")
    @Expose
    private String address;
    @SerializedName("name")
    @Expose
    private String name;

    @SerializedName("age")
    @Expose
    private String age;

    @SerializedName("height")
    @Expose
    private String height;

    @SerializedName("eye_color")
    @Expose
    private String eye_color;

    @SerializedName("hair_color")
    @Expose
    private String hair_color;

    @SerializedName("tattoo")
    @Expose
    private String tattoo;

    @SerializedName("sex")
    @Expose
    private String sex;

    @SerializedName("phone")
    @Expose
    private String phone;

    @SerializedName("weight")
    @Expose
    private String weight;

    @SerializedName("marital_status")
    @Expose
    private String marital_status;

    @SerializedName("email")
    @Expose
    private String email;

    @SerializedName("blood_group")
    @Expose
    private String blood_group;

    @SerializedName("genotype")
    @Expose
    private String genotype;

    @SerializedName("state_of_origin")
    @Expose
    private String state_of_origin;

    @SerializedName("lga")
    @Expose
    private String lga;

    @SerializedName("role")
    @Expose
    private String role;


    public String getRole() {
        return role;
    }

    public void setRole(String role) {
        this.role = role;
    }


    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getAge() {
        return age;
    }

    public void setAge(String age) {
        this.age = age;
    }

    public String getHeight() {
        return height;
    }

    public void setHeight(String height) {
        this.height = height;
    }

    public String getEye_color() {
        return eye_color;
    }

    public void setEye_color(String eye_color) {
        this.eye_color = eye_color;
    }

    public String getHair_color() {
        return hair_color;
    }

    public void setHair_color(String hair_color) {
        this.hair_color = hair_color;
    }

    public String getTattoo() {
        return tattoo;
    }

    public void setTattoo(String tattoo) {
        this.tattoo = tattoo;
    }

    public String getSex() {
        return sex;
    }

    public void setSex(String sex) {
        this.sex = sex;
    }

    public String getPhone() {
        return phone;
    }

    public void setPhone(String phone) {
        this.phone = phone;
    }

    public String getWeight() {
        return weight;
    }

    public void setWeight(String weight) {
        this.weight = weight;
    }

    public String getMarital_status() {
        return marital_status;
    }

    public void setMarital_status(String marital_status) {
        this.marital_status = marital_status;
    }

    public String getBlood_group() {
        return blood_group;
    }

    public void setBlood_group(String blood_group) {
        this.blood_group = blood_group;
    }

    public String getState_of_origin() {
        return state_of_origin;
    }

    public void setState_of_origin(String state_of_origin) {
        this.state_of_origin = state_of_origin;
    }

    public String getLga() {
        return lga;
    }

    public void setLga(String lga) {
        this.lga = lga;
    }

    public String getHometown() {
        return hometown;
    }

    public void setHometown(String hometown) {
        this.hometown = hometown;
    }

    public String getNin() {
        return nin;
    }

    public void setNin(String nin) {
        this.nin = nin;
    }

    public String getSec_sch_year_in() {
        return sec_sch_year_in;
    }

    public void setSec_sch_year_in(String sec_sch_year_in) {
        this.sec_sch_year_in = sec_sch_year_in;
    }

    public String getSec_sch_year_out() {
        return sec_sch_year_out;
    }

    public void setSec_sch_year_out(String sec_sch_year_out) {
        this.sec_sch_year_out = sec_sch_year_out;
    }

    public String getNok_name() {
        return nok_name;
    }

    public void setNok_name(String nok_name) {
        this.nok_name = nok_name;
    }

    public String getNok_phone() {
        return nok_phone;
    }

    public void setNok_phone(String nok_phone) {
        this.nok_phone = nok_phone;
    }

    @SerializedName("hometown")
    @Expose
    private String hometown;

    @SerializedName("nin")
    @Expose
    private String nin;

    @SerializedName("sec_sch_year_in")
    @Expose
    private String sec_sch_year_in;


    @SerializedName("sec_sch_year_out")
    @Expose
    private String sec_sch_year_out;


    @SerializedName("nok_name")
    @Expose
    private String nok_name;


    @SerializedName("nok_phone")
    @Expose
    private String nok_phone;


    public String getGenotype() {
        return genotype;
    }

    public void setGenotype(String genotype) {
        this.genotype = genotype;
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }


    public String getQualification() {
        return qualification;
    }

    public void setQualification(String qualification) {
        this.qualification = qualification;
    }

    public String getAddress() {
        return address;
    }

    public void setAddress(String address) {
        this.address = address;
    }

    public String getDate_of_birth() {
        return date_of_birth;
    }

    public void setDate_of_birth(String date_of_birth) {
        this.date_of_birth = date_of_birth;
    }
}
