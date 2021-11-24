package com.greenbit.MultiscanJNIGuiJavaAndroid.models;

import com.google.gson.annotations.SerializedName;

public class LGApensionser {

    //    private String[] titles1 = {"FILE NUMBER", "NAIC NUMBER", "FULL NAME", "GENDER", "DATE OF BIRTH",
//            "PHONE NUMBER", "PENSIONER CATEGORY", "PAYROLL STATUS", "PERMANENT ADDRESS", "LOCAL GOVERNMENT OF ORIGIN"
//    };
//    private String[] titles2 = {"NEXT OF KIN'S NAME",
//            "NEXT OF KIN'S  PHONE NUMBER",
//            "RELATIONSHIP WITH NEXT OF KIN", "NEXT OF KIN'S ADDRESS"};
//    private String[] titles3 = {"DATE OF FIRST APPOINTMENT", "RETIREMENT DATE",
//            "NUMBER OF YEARS SERVED", "CLASSIFICATION", "RANK", "GRADE LEVEL/STEP", "DATE OF LAST PROMOTION",
//            "LOCAL GOVERNMENT OF RETIREMENT"};
//    private String[] titles4 = {"BANK", "ACCOUNT NUMBER", "BANK VERIFICATION NUMBER BVN"};


    public String getBippiis_no() {
        return bippiis_no;
    }

    public void setBippiis_no(String bippiis_no) {
        this.bippiis_no = bippiis_no;
    }

    public String getFull_name() {
        return full_name;
    }

    public void setFull_name(String full_name) {
        this.full_name = full_name;
    }

    public String getGender() {
        return gender;
    }

    public void setGender(String gender) {
        this.gender = gender;
    }

    public String getDob() {
        return dob;
    }

    public void setDob(String dob) {
        this.dob = dob;
    }

    public String getPhone_number() {
        return phone_number;
    }

    public void setPhone_number(String phone_number) {
        this.phone_number = phone_number;
    }

    public String getPensioners_category() {
        return pensioners_category;
    }

    public void setPensioners_category(String pensioners_category) {
        this.pensioners_category = pensioners_category;
    }

    public String getPayroll_status() {
        return payroll_status;
    }

    public void setPayroll_status(String payroll_status) {
        this.payroll_status = payroll_status;
    }

    public String getPermanent_address() {
        return permanent_address;
    }

    public void setPermanent_address(String permanent_address) {
        this.permanent_address = permanent_address;
    }

    public String getLga_origin() {
        return lga_origin;
    }

    public void setLga_origin(String lga_origin) {
        this.lga_origin = lga_origin;
    }

    public String getNext_of_kin_name() {
        return next_of_kin_name;
    }

    public void setNext_of_kin_name(String next_of_kin_name) {
        this.next_of_kin_name = next_of_kin_name;
    }

    public String getNext_of_kin_phone_number() {
        return next_of_kin_phone_number;
    }

    public void setNext_of_kin_phone_number(String next_of_kin_phone_number) {
        this.next_of_kin_phone_number = next_of_kin_phone_number;
    }

    public String getRelationship_nok() {
        return relationship_nok;
    }

    public void setRelationship_nok(String relationship_nok) {
        this.relationship_nok = relationship_nok;
    }

    public String getNext_of_kin_address() {
        return next_of_kin_address;
    }

    public void setNext_of_kin_address(String next_of_kin_address) {
        this.next_of_kin_address = next_of_kin_address;
    }

    public String getDate_first_appointment() {
        return date_first_appointment;
    }

    public void setDate_first_appointment(String date_first_appointment) {
        this.date_first_appointment = date_first_appointment;
    }

    public String getRetirement_date() {
        return retirement_date;
    }

    public void setRetirement_date(String retirement_date) {
        this.retirement_date = retirement_date;
    }

    public String getNo_years_served() {
        return no_years_served;
    }

    public void setNo_years_served(String no_years_served) {
        this.no_years_served = no_years_served;
    }

    public String getClassification() {
        return classification;
    }

    public void setClassification(String classification) {
        this.classification = classification;
    }

    public String getRank() {
        return rank;
    }

    public void setRank(String rank) {
        this.rank = rank;
    }

    public String getGrade_level() {
        return grade_level;
    }

    public void setGrade_level(String grade_level) {
        this.grade_level = grade_level;
    }

    public String getStep() {
        return step;
    }

    public void setStep(String step) {
        this.step = step;
    }

    public String getDate_last_promotion() {
        return date_last_promotion;
    }

    public void setDate_last_promotion(String date_last_promotion) {
        this.date_last_promotion = date_last_promotion;
    }

    public String getLga_retirement() {
        return lga_retirement;
    }

    public void setLga_retirement(String lga_retirement) {
        this.lga_retirement = lga_retirement;
    }

    public String getBank() {
        return bank;
    }

    public void setBank(String bank) {
        this.bank = bank;
    }

    public String getAccount_number() {
        return account_number;
    }

    public void setAccount_number(String account_number) {
        this.account_number = account_number;
    }

    public String getBvn() {
        return bvn;
    }

    public void setBvn(String bvn) {
        this.bvn = bvn;
    }

    //26 gets to make 25 sets
    @SerializedName("bippiis_number")
    private String file_no;

    @SerializedName("bippiis_no")
    private String bippiis_no;

    @SerializedName("fullname")
    private String full_name;

    @SerializedName("gender")
    private String gender;
    @SerializedName("date_of_birth")
    private String dob;
    @SerializedName("phone")
    private String phone_number;
    @SerializedName("category")
    private String pensioners_category;
    @SerializedName("status")
    private String payroll_status;
    @SerializedName("permanent_address")
    private String permanent_address;
    @SerializedName("lga_o")
    private String lga_origin;
    @SerializedName("nok_name")
    private String next_of_kin_name;
    @SerializedName("nok_phone")
    private String next_of_kin_phone_number;
    @SerializedName("nok_relationship")
    private String relationship_nok;
    @SerializedName("nok_address")
    private String next_of_kin_address;
    @SerializedName("f_a_d")
    private String date_first_appointment;
    @SerializedName("d_o_r")
    private String retirement_date;
    @SerializedName("years_served")
    private String no_years_served;
    @SerializedName("classification")
    private String classification;
    @SerializedName("rank")
    private String rank;
    @SerializedName("grade_level")
    private String grade_level;
    @SerializedName("step")
    private String step;
    @SerializedName("d_o_p")
    private String date_last_promotion;
    @SerializedName("lga_r")
    private String lga_retirement;
    @SerializedName("bank")
    private String bank;
    @SerializedName("account_number")
    private String account_number;
    @SerializedName("bvn")
    private String bvn;


    public String getFile_no() {
        return file_no;
    }

    public void setFile_no(String file_no) {
        this.file_no = file_no;
    }
}
