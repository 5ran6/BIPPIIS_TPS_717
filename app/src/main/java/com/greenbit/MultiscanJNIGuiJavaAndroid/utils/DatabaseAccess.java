package com.greenbit.MultiscanJNIGuiJavaAndroid.utils;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

import com.greenbit.MultiscanJNIGuiJavaAndroid.EnrollDetailsString;
import com.greenbit.MultiscanJNIGuiJavaAndroid.models.Enrollee;
import com.greenbit.MultiscanJNIGuiJavaAndroid.models.FingerKey;
import com.greenbit.MultiscanJNIGuiJavaAndroid.models.FingerprintRequest;
import com.greenbit.MultiscanJNIGuiJavaAndroid.models.PassportRequest;
import com.greenbit.MultiscanJNIGuiJavaAndroid.models.UserRegisterRequest;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;

public class DatabaseAccess {
    private SQLiteOpenHelper openHelper;
    private SQLiteDatabase database;
    private static DatabaseAccess instance;

    private DatabaseAccess(Context context) {
        this.openHelper = new DatabaseOpenHelper(context);
    }

    /**
     * Return a singleton instance of DatabaseAccess.
     *
     * @param context the Context
     * @return the instance of DabaseAccess
     */
    public static DatabaseAccess getInstance(Context context) {
        if (instance == null) {
            instance = new DatabaseAccess(context);
        }
        return instance;
    }

    /**
     * Open the database connection.
     */
    public void open() {
        this.database = openHelper.getWritableDatabase();
    }

    /**
     * Close the database connection.
     */
    public void close() {
        if (database != null) {
            this.database.close();
        }
    }

    public ArrayList<Enrollee> getEnrollees() {
        ArrayList<Enrollee> enrollees = new ArrayList<>();

        Cursor c = database.rawQuery(
                "select fullname,bioreg.phone,bioreg.synched," +
                        "SubjectId,registration_date,image,coalesce(images.synched,'No'),left_finger_batcha_synched ," +
                        "left_finger_batchb_synched, " +
                        "left_finger_batchc_synched  ," +
                        "left_finger_batchd_synched ," +
                        "left_finger_batche_synched ,  " +
                        "right_finger_batcha_synched,right_finger_batchb_synched, " +
                        "right_finger_batchc_synched  ," +
                        "right_finger_batchd_synched , right_finger_batche_synched   " +
                        " from bioreg left outer join images on (bioreg.phone=images.phone) order by reg_id desc",
                null);
        c.moveToFirst();
        while (!c.isAfterLast()) {

            Enrollee e = new Enrollee();
            e.name = (c.getString(0));
            e.phone = (c.getString(1));
            e.synched = (c.getString(2));
            e.reg_number = (c.getString(3));
            e.registered_date = (c.getString(4));
            e.image = (c.getString(5));
            e.image_synched = (c.getString(6));
            ArrayList<String> left_hand_vals = new ArrayList<>();
            left_hand_vals.add("--Left Hand Synch Status--");
            left_hand_vals.add("Left Thumb Synched?:" + c.getString(7));
            left_hand_vals.add("Left Index Synched?:" + c.getString(8));
            left_hand_vals.add("Left Middle Synched?:" + c.getString(9));
            left_hand_vals.add("Left Ring Synched?:" + c.getString(10));
            left_hand_vals.add("Left Little Synched?:" + c.getString(11));
            e.left_hand = left_hand_vals;

            ArrayList<String> right_hand_vals = new ArrayList<>();
            right_hand_vals.add("--Right Hand Synch Status--");
            right_hand_vals.add("Right Thumb Synched?:" + c.getString(12));
            right_hand_vals.add("Right Index Synched?:" + c.getString(13));
            right_hand_vals.add("Right Middle Synched?:" + c.getString(14));
            right_hand_vals.add("Right Ring Synched?:" + c.getString(15));
            right_hand_vals.add("Right Little Synched?:" + c.getString(16));

            e.right_hand = right_hand_vals;
            // e.left_thumb=c.getString(17);
            enrollees.add(e);

            c.moveToNext();
        }
        return enrollees;

    }

    public PassportRequest getPassport() {
        PassportRequest p = new PassportRequest();
        p.token = "No Token";

        Cursor c = database.rawQuery(
                "select SubjectId,image,token,images.phone from bioreg  join images on (bioreg.phone=images.phone) where bioreg.synched='Yes'" +
                        "and images.synched='No' limit 0,1",
                null);
        c.moveToFirst();
        while (!c.isAfterLast()) {


            p.setArmy_number(c.getString(0));
            p.setPassport("data:image/jpeg;base64," + c.getString(1));
            p.token = (c.getString(2));
            p.phone = (c.getString(3));


            c.moveToNext();
        }
        return p;

    }

    public void updatePassPortResponse(String phone) {

        String query = "update images set synched='Yes'  where phone='" + phone + "'";
        database.execSQL(query);

    }


    public String InsertEnrollee(UserRegisterRequest userRequest) {

        Calendar c = Calendar.getInstance();
        SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        String formattedDate = df.format(c.getTime());


        ContentValues cv = new ContentValues();


        cv.put("qualifications", userRequest.getQualification());
        cv.put("address", userRequest.getAddress());
        cv.put("registration_date", formattedDate);
        cv.put("fullname", userRequest.getName());
        cv.put("age", userRequest.getAge());
        cv.put("phone", userRequest.getPhone());
        cv.put("email", userRequest.getEmail());
        cv.put("height", userRequest.getHeight());
        cv.put("weight", userRequest.getWeight());
        cv.put("gender", userRequest.getSex());
        cv.put("tatoo", userRequest.getTattoo());
        cv.put("state", userRequest.getState_of_origin());
        cv.put("lga", userRequest.getLga());
        cv.put("eye_color", userRequest.getEye_color());
        cv.put("hair_color", userRequest.getHair_color());
        cv.put("marital_status", userRequest.getMarital_status());
        cv.put("role", userRequest.getRole());
        cv.put("blood_group", userRequest.getBlood_group());
        cv.put("genotype", userRequest.getGenotype());
        cv.put("date_of_birth", userRequest.getDate_of_birth());
        cv.put("nok", userRequest.getNok_name());
        cv.put("nok_phone", userRequest.getNok_phone());
        cv.put("hometown", userRequest.getHometown());
        cv.put("nin", userRequest.getNin());
        cv.put("school_in", userRequest.getSec_sch_year_in());
        cv.put("school_out", userRequest.getSec_sch_year_out());

        try {
            database.insertOrThrow("bioreg", null, cv);
            return "Enrolled Successfully";
        } catch (Exception e) {
            e.printStackTrace();
            return e.getMessage().toString();
        }


        //  if(rowInserted != -1)
        // {
        //   }
        // else
        //  {  return "Error Enrolling";}
        //  areaids.add(avls.get(i).AreaId);

    }


    public ArrayList<UserRegisterRequest> getBio() {
        ArrayList<UserRegisterRequest> requests = new ArrayList<>();

        try {

            UserRegisterRequest userRequest = new UserRegisterRequest();
            Cursor c = database.rawQuery(
                    "select fullname,age,phone,email,height,weight,eye_color,hair_color,tatoo," +
                            "gender, marital_status,role,blood_group,genotype,nok, nok_phone, state , lga, hometown,nin,school_in,school_out,qualifications,address from bioreg  where synched = 'No' limit 0,1",
                    null);
            c.moveToFirst();
            while (!c.isAfterLast()) {

                userRequest.setName(c.getString(0));
                userRequest.setAge(c.getString(1));
                userRequest.setPhone(c.getString(2));
                userRequest.setEmail(c.getString(3));
                userRequest.setHeight(c.getString(4));
                userRequest.setWeight(c.getString(5));
                userRequest.setEye_color(c.getString(6));
                userRequest.setHair_color(c.getString(7));
                userRequest.setTattoo(c.getString(8));
                userRequest.setSex(c.getString(9));
                userRequest.setMarital_status(c.getString(10));
                userRequest.setRole(c.getString(11));
                userRequest.setBlood_group(c.getString(12));
                userRequest.setGenotype(c.getString(13));
                userRequest.setNok_name(c.getString(14));
                userRequest.setNok_phone(c.getString(15));
                userRequest.setState_of_origin(c.getString(16));
                userRequest.setLga(c.getString(17));
                userRequest.setHometown(c.getString(18));
                userRequest.setNin(c.getString(19));
                userRequest.setSec_sch_year_in(c.getString(20));
                userRequest.setSec_sch_year_out(c.getString(21));
                userRequest.setQualification(c.getString(22));
                userRequest.setAddress(c.getString(23));
                requests.add(userRequest);

                c.moveToNext();
            }
            return requests;
        } catch (Exception e) {
            e.printStackTrace();
        }

        return requests;
    }

    public void UpdateLeftFingerAResponse(String phone) {

        String query = "update bioreg set left_finger_batcha_synched='Yes'  where phone='" + phone + "'";
        database.execSQL(query);

    }

    public void UpdateLeftFingerBResponse(String phone) {

        String query = "update bioreg set left_finger_batchb_synched='Yes'  where phone='" + phone + "'";
        database.execSQL(query);

    }

    public void UpdateLeftFingerCResponse(String phone) {

        String query = "update bioreg set left_finger_batchc_synched='Yes'  where phone='" + phone + "'";
        database.execSQL(query);

    }

    public void UpdateLeftFingerDResponse(String phone) {

        String query = "update bioreg set left_finger_batchd_synched='Yes'  where phone='" + phone + "'";
        database.execSQL(query);

    }

    public void UpdateLeftFingerEResponse(String phone) {

        String query = "update bioreg set left_finger_batche_synched='Yes'  where phone='" + phone + "'";
        database.execSQL(query);

    }

    public void UpdateRightFingerResponse(String phone) {

        String query = "update bioreg set right_fingers_synched='Yes'  where phone='" + phone + "'";
        database.execSQL(query);

    }

    public void UpdateBioEnrollResponse(String phone_val, String army_number, String token) {

        String query = "update bioreg set SubjectId='" + army_number + "' , token='" + token + "' , synched='Yes'  where phone='" + phone_val + "'";
        database.execSQL(query);

    }


    public String UpdateFingerPrintEnroll(String phone, FingerKey fk, EnrollDetailsString eds) {

        String column_label = fk.column_label;
        String pic = eds.FingerPrintImage;
        String template = eds.FingerPrintTemplate;
        String template_column = column_label + "_temp";
        String pic_column = column_label + "_pic";

        ContentValues cv = new ContentValues();
        cv.put(template_column, template);
        cv.put(pic_column, pic);

        long rowInserted = database.update("bioreg", cv, "phone = ?", new String[]{phone});

        if (rowInserted != -1) {
            return "Biometric Saved Successfully";
        } else {
            return "Error Saving Biometric";
        }

    }


    public FingerprintRequest getFingersLeftHandA() {
        FingerprintRequest requests = new FingerprintRequest();
        ArrayList<String> fingerImages = new ArrayList<>();
        ArrayList<String> fingerTemplates = new ArrayList<>();
        Cursor c = database.rawQuery(
                "select left_thumb_pic,phone,token," +
                        "left_thumb_temp from bioreg  where left_finger_batcha_synched = 'No'  and synched='Yes' and left_thumb_pic is not null limit 0,1",
                null);
        c.moveToFirst();
        while (!c.isAfterLast()) {
            fingerImages.add(c.getString(0));
            fingerTemplates.add(c.getString(3));
            requests.phone = c.getString(1);
            requests.token = c.getString(2);
            requests.setFingerprints(fingerTemplates);
            requests.setFingerprintsImages(fingerImages);
            requests.setType("left_thumb");


            c.moveToNext();
        }
        return requests;
    }

    public FingerprintRequest getFingersLeftHandB() {
        FingerprintRequest requests = new FingerprintRequest();
        ArrayList<String> fingerImages = new ArrayList<>();
        ArrayList<String> fingerTemplates = new ArrayList<>();
        Cursor c = database.rawQuery(
                "select phone,token,left_index_pic,left_index_temp from bioreg  where left_finger_batchb_synched = 'No'  and synched='Yes'  and left_index_pic is not null limit 0,1",
                null);
        c.moveToFirst();
        while (!c.isAfterLast()) {
            fingerImages.add(c.getString(2));
            fingerTemplates.add(c.getString(3));
            requests.phone = c.getString(0);
            requests.token = c.getString(1);
            requests.setFingerprints(fingerTemplates);
            requests.setFingerprintsImages(fingerImages);
            requests.setType("left_index");


            c.moveToNext();
        }
        return requests;
    }

    public FingerprintRequest getFingersLeftHandC() {
        FingerprintRequest requests = new FingerprintRequest();
        ArrayList<String> fingerImages = new ArrayList<>();
        ArrayList<String> fingerTemplates = new ArrayList<>();
        Cursor c = database.rawQuery(
                "select phone,token,left_middle_pic,left_middle_temp from bioreg  where left_finger_batchc_synched = 'No'  and synched='Yes' and left_middle_pic is not null limit 0,1",
                null);
        c.moveToFirst();
        while (!c.isAfterLast()) {
            fingerImages.add(c.getString(2));
            fingerTemplates.add(c.getString(3));
            requests.phone = c.getString(0);
            requests.token = c.getString(1);

            requests.setFingerprints(fingerTemplates);
            requests.setFingerprintsImages(fingerImages);
            requests.setType("left_middle");


            c.moveToNext();
        }
        return requests;
    }

    public FingerprintRequest getFingersLeftHandD() {
        FingerprintRequest requests = new FingerprintRequest();
        ArrayList<String> fingerImages = new ArrayList<>();
        ArrayList<String> fingerTemplates = new ArrayList<>();
        Cursor c = database.rawQuery(
                "select phone,token,left_ring_pic,left_ring_temp from bioreg  where left_finger_batchd_synched = 'No'  and synched='Yes' and left_ring_pic is not null limit 0,1",
                null);
        c.moveToFirst();
        while (!c.isAfterLast()) {
            fingerImages.add(c.getString(2));
            fingerTemplates.add(c.getString(3));
            requests.phone = c.getString(0);
            requests.token = c.getString(1);

            requests.setFingerprints(fingerTemplates);
            requests.setFingerprintsImages(fingerImages);
            requests.setType("left_ring");


            c.moveToNext();
        }
        return requests;
    }

    public FingerprintRequest getFingersLeftHandE() {
        FingerprintRequest requests = new FingerprintRequest();
        ArrayList<String> fingerImages = new ArrayList<>();
        ArrayList<String> fingerTemplates = new ArrayList<>();
        Cursor c = database.rawQuery(
                "select phone,token,left_little_pic,left_little_temp from bioreg  where left_finger_batche_synched = 'No'  and synched='Yes' and left_little_pic is not null limit 0,1",
                null);
        c.moveToFirst();
        while (!c.isAfterLast()) {
            fingerImages.add(c.getString(2));
            fingerTemplates.add(c.getString(3));
            requests.phone = c.getString(0);
            requests.token = c.getString(1);

            requests.setFingerprints(fingerTemplates);
            requests.setFingerprintsImages(fingerImages);
            requests.setType("left_little");


            c.moveToNext();
        }
        return requests;
    }


    public FingerprintRequest getFingersRighttHand() {
        FingerprintRequest requests = new FingerprintRequest();
        ArrayList<String> fingerImages = new ArrayList<>();
        ArrayList<String> fingerTemplates = new ArrayList<>();
        Cursor c = database.rawQuery(
                "select right_thumb_pic,right_little_temp,phone,token,right_middle_pic,right_index_pic,right_ring_pic," +
                        "right_thumb_temp,right_middle_temp,right_index_temp,right_little_pic,right_ring_temp from bioreg  where  right_fingers_synched = 'No' " +
                        " and left_fingers_synched = 'Yes' and synched='Yes' limit 0,1",
                null);
        c.moveToFirst();
        while (!c.isAfterLast()) {
            fingerImages.add(c.getString(0));
            fingerTemplates.add(c.getString(1));
            requests.phone = c.getString(2);
            requests.token = c.getString(3);
            fingerImages.add(c.getString(4));
            fingerImages.add(c.getString(5));
            fingerImages.add(c.getString(6));
            fingerTemplates.add(c.getString(7));
            fingerTemplates.add(c.getString(8));
            fingerTemplates.add(c.getString(9));

            fingerImages.add(c.getString(10));
            fingerTemplates.add(c.getString(11));

            /** fingerImages.add (c.getString(12));
             fingerImages.add (c.getString(13));
             fingerImages.add (c.getString(14));
             fingerImages.add (c.getString(15));
             fingerImages.add (c.getString(16));
             fingerTemplates.add (c.getString(17));
             fingerTemplates.add (c.getString(18));
             fingerTemplates.add (c.getString(19));
             fingerTemplates.add (c.getString(20));
             fingerTemplates.add (c.getString(21));
             **/
            requests.setFingerprints(fingerTemplates);
            requests.setFingerprintsImages(fingerImages);


            c.moveToNext();
        }
        return requests;
    }


    public FingerprintRequest getFingersRightHandA() {
        FingerprintRequest requests = new FingerprintRequest();
        ArrayList<String> fingerImages = new ArrayList<>();
        ArrayList<String> fingerTemplates = new ArrayList<>();
        Cursor c = database.rawQuery(
                "select right_thumb_pic,phone,token," +
                        "right_thumb_temp from bioreg  where right_finger_batcha_synched = 'No'  and synched='Yes' and right_thumb_pic is not null limit 0,1",
                null);
        c.moveToFirst();
        while (!c.isAfterLast()) {
            fingerImages.add(c.getString(0));
            fingerTemplates.add(c.getString(3));
            requests.phone = c.getString(1);
            requests.token = c.getString(2);
            requests.setFingerprints(fingerTemplates);
            requests.setFingerprintsImages(fingerImages);
            requests.setType("right_thumb");


            c.moveToNext();
        }
        return requests;
    }

    public FingerprintRequest getFingersRightHandB() {
        FingerprintRequest requests = new FingerprintRequest();
        ArrayList<String> fingerImages = new ArrayList<>();
        ArrayList<String> fingerTemplates = new ArrayList<>();
        Cursor c = database.rawQuery(
                "select phone,token,right_index_pic,right_index_temp from bioreg  where right_finger_batchb_synched = 'No'  and synched='Yes'  and right_index_pic is not null limit 0,1",
                null);
        c.moveToFirst();
        while (!c.isAfterLast()) {
            fingerImages.add(c.getString(2));
            fingerTemplates.add(c.getString(3));
            requests.phone = c.getString(0);
            requests.token = c.getString(1);
            requests.setFingerprints(fingerTemplates);
            requests.setFingerprintsImages(fingerImages);
            requests.setType("right_index");


            c.moveToNext();
        }
        return requests;
    }

    public FingerprintRequest getFingersRightHandC() {
        FingerprintRequest requests = new FingerprintRequest();
        ArrayList<String> fingerImages = new ArrayList<>();
        ArrayList<String> fingerTemplates = new ArrayList<>();
        Cursor c = database.rawQuery(
                "select phone,token,right_middle_pic,right_middle_temp from bioreg  where right_finger_batchc_synched = 'No'  and synched='Yes'  and right_middle_pic is not null limit 0,1",
                null);
        c.moveToFirst();
        while (!c.isAfterLast()) {
            fingerImages.add(c.getString(2));
            fingerTemplates.add(c.getString(3));
            requests.phone = c.getString(0);
            requests.token = c.getString(1);

            requests.setFingerprints(fingerTemplates);
            requests.setFingerprintsImages(fingerImages);
            requests.setType("right_middle");


            c.moveToNext();
        }
        return requests;
    }

    public FingerprintRequest getFingersRightHandD() {
        FingerprintRequest requests = new FingerprintRequest();
        ArrayList<String> fingerImages = new ArrayList<>();
        ArrayList<String> fingerTemplates = new ArrayList<>();
        Cursor c = database.rawQuery(
                "select phone,token,right_ring_pic,right_ring_temp from bioreg  where right_finger_batchd_synched = 'No'  and synched='Yes' and right_ring_pic is not null  limit 0,1",
                null);
        c.moveToFirst();
        while (!c.isAfterLast()) {
            fingerImages.add(c.getString(2));
            fingerTemplates.add(c.getString(3));
            requests.phone = c.getString(0);
            requests.token = c.getString(1);

            requests.setFingerprints(fingerTemplates);
            requests.setFingerprintsImages(fingerImages);
            requests.setType("right_ring");


            c.moveToNext();
        }
        return requests;
    }

    public FingerprintRequest getFingersRightHandE() {
        FingerprintRequest requests = new FingerprintRequest();
        ArrayList<String> fingerImages = new ArrayList<>();
        ArrayList<String> fingerTemplates = new ArrayList<>();
        Cursor c = database.rawQuery(
                "select phone,token,right_little_pic,right_little_temp from bioreg  where right_finger_batche_synched = 'No'  and synched='Yes' and right_little_pic is not null limit 0,1",
                null);
        c.moveToFirst();
        while (!c.isAfterLast()) {
            fingerImages.add(c.getString(2));
            fingerTemplates.add(c.getString(3));
            requests.phone = c.getString(0);
            requests.token = c.getString(1);

            requests.setFingerprints(fingerTemplates);
            requests.setFingerprintsImages(fingerImages);
            requests.setType("right_little");


            c.moveToNext();
        }
        return requests;
    }


    public void UpdateRightFingerAResponse(String phone) {

        String query = "update bioreg set right_finger_batcha_synched='Yes'  where phone='" + phone + "'";
        database.execSQL(query);

    }

    public void UpdateRightFingerBResponse(String phone) {

        String query = "update bioreg set right_finger_batchb_synched='Yes'  where phone='" + phone + "'";
        database.execSQL(query);

    }

    public void UpdateRightFingerCResponse(String phone) {

        String query = "update bioreg set right_finger_batchc_synched='Yes'  where phone='" + phone + "'";
        database.execSQL(query);

    }

    public void UpdateRightFingerDResponse(String phone) {

        String query = "update bioreg set right_finger_batchd_synched='Yes'  where phone='" + phone + "'";
        database.execSQL(query);

    }

    public void UpdateRightFingerEResponse(String phone) {

        String query = "update bioreg set right_finger_batche_synched='Yes'  where phone='" + phone + "'";
        database.execSQL(query);

    }

    public String insertImage(String phone, String image) {
        ContentValues cv = new ContentValues();


        cv.put("phone", phone);
        cv.put("image", image);


        long rowInserted = database.insert("images", null, cv);
        if (rowInserted != -1) {
            return "Image Saved Successfully";

        } else {
            return "Error Saving Image";
        }
    }

    public ArrayList<String> getStates() {
        ArrayList<String> states = new ArrayList<>();

        Cursor c = database.rawQuery(
                "select StateName from states order by StateName",
                null);
        c.moveToFirst();
        while (!c.isAfterLast()) {


            states.add(c.getString(0));


            c.moveToNext();
        }
        return states;

    }


    public ArrayList<String> getLgas(String state) {
        ArrayList<String> lgas = new ArrayList<>();

        Cursor c = database.rawQuery(
                "select LGAName from lga where StateName='" + state + "' order by LGAName",
                null);
        c.moveToFirst();
        while (!c.isAfterLast()) {


            lgas.add(c.getString(0));


            c.moveToNext();
        }
        return lgas;

    }

}