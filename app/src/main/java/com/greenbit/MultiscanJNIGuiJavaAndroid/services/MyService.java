package com.greenbit.MultiscanJNIGuiJavaAndroid.services;

import android.app.AlarmManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Handler;
import android.os.IBinder;
import android.os.Looper;
import android.preference.PreferenceManager;
import android.util.Log;
import android.widget.Button;
import android.widget.Toast;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.greenbit.MultiscanJNIGuiJavaAndroid.models.FingerprintRequest;
import com.greenbit.MultiscanJNIGuiJavaAndroid.models.PassportRequest;
import com.greenbit.MultiscanJNIGuiJavaAndroid.models.UserRegisterRequest;
import com.greenbit.MultiscanJNIGuiJavaAndroid.utils.DatabaseAccess;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import org.json.simple.parser.JSONParser;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.concurrent.TimeUnit;

import okhttp3.MediaType;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.RequestBody;

public class MyService extends Service {
    JSONArray items_list = null;
    OkHttpClient httpClient;
    RequestBody formBody;
    Request request;
    Button submit;
    String okresp;
    ArrayList<String> avls;
    String staffList;
    DatabaseAccess databaseAccess;
    JSONParser jParser = new JSONParser();
    ArrayList<HashMap<String, String>> itemsList;

    InputStream inputStream;
    SharedPreferences app_preferences;

    JSONArray datjsonArray = new JSONArray();
    JSONObject obj;
    JSONObject jsonArray = new JSONObject();
    JSONArray friendsarray;
    StringBuilder sb;
    String coy, year;
    byte[] data;
    private SharedPreferences sharedPref;
    private SharedPreferences.Editor editor;
    private String fromSharedPref = "";
    DatabaseAccess db;

    @Override
    public int onStartCommand(Intent in, int flags, int startId) {
        // Toast.makeText(MyService.this,"Service Running From Service", Toast.LENGTH_LONG).show();

        sharedPref = PreferenceManager.getDefaultSharedPreferences(getApplicationContext());
        editor = sharedPref.edit();
        fromSharedPref = sharedPref.getString("address", "nothing is here");
        db = DatabaseAccess.getInstance(getApplicationContext());
        db.open();
        start();


        stopSelf();
        return START_NOT_STICKY;
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onDestroy() {
        AlarmManager al = (AlarmManager) getSystemService(ALARM_SERVICE);
        al.set(
                al.RTC_WAKEUP, System.currentTimeMillis() + (1000 * 10 * 1),
                PendingIntent.getService(this, 0, new Intent(this, MyService.class), 0)

        );

    }

    public static boolean isConnected(Context context) {
        ConnectivityManager connectivity = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
        if (connectivity != null) {
            NetworkInfo[] info = connectivity.getAllNetworkInfo();
            if (info != null) {
                for (int i = 0; i < info.length; i++) {
                    if (info[i].getState() == NetworkInfo.State.CONNECTED)
                        return true;
                }
            }
        }
        return false;
    }

    public void start() {
        if (fromSharedPref.equalsIgnoreCase("nothing is here")) {
            return;
        }

        if (isConnected(getApplicationContext())) {


            synchEnrollee();
            synchLeftFingersA();
            synchLeftFingersB();
            synchLeftFingersC();
            synchLeftFingersD();
            synchLeftFingersE();
            synchRightFingersA();
            synchRightFingersB();
            synchRightFingersC();
            synchRightFingersD();
            synchRightFingersE();
            synchPassport();

        }


    }


    public void synchEnrollee() {


        ArrayList<UserRegisterRequest> user = db.getBio();
        //Toast.makeText(getApplicationContext(),"new data", Toast.LENGTH_SHORT).show();
        if (!user.isEmpty()) {
            // Toast.makeText(getApplicationContext(),"data here", Toast.LENGTH_SHORT).show();
            UserRegisterRequest us = user.get(0);


            String enroll_url = fromSharedPref + "signup";

            OkHttpClient client = new OkHttpClient.Builder()
                    .connectTimeout(1, TimeUnit.MINUTES)
                    .readTimeout(60, TimeUnit.SECONDS)
                    .writeTimeout(60, TimeUnit.SECONDS)
                    .build();
            MediaType JSON = MediaType.parse("application/json; charset=utf-8");
            // put your json here
            Gson gson = new GsonBuilder().create();
            String postdata = gson.toJson(us);
            RequestBody body = RequestBody.create(JSON, postdata);
            Request request = new Request.Builder()
                    .url(enroll_url)
                    .post(body)
                    .build();

            okhttp3.Call call = client.newCall(request);
            call.enqueue(new okhttp3.Callback() {
                @Override
                public void onFailure(okhttp3.Call call, IOException e) {
                    // waitingDialog.dismiss();
                    Log.d("OKHTTP RESPONSE", "failed in callback");
                    // toastj(getApplicationContext(), e.getLocalizedMessage().toString());
                    //  Intent i=new Intent(getApplicationContext(),Login.class);
                    //  startActivity(i);
                }

                @Override
                public void onResponse(okhttp3.Call call, final okhttp3.Response response) throws IOException {


                    // Stuff that updates the UI
                    try {
                        //Response response = client.newCall(request).execute();
                        okresp = response.body().string();
                        Log.d("fingerprint", "Response: " + okresp);
                        //  toastj(getApplicationContext(),okresp);

                        JSONObject jb = null;
                        try {
                            jb = new JSONObject(okresp);
                            String status = jb.getString("status");

                            if (status.equalsIgnoreCase("success")) {
                                JSONObject data = jb.getJSONObject("data");

                                String army_number = data.getString("army_number");
                                String token = jb.getString("token");
                                db.UpdateBioEnrollResponse(us.getPhone(), army_number, token);
                            }


                        } catch (JSONException e) {
                            e.printStackTrace();
                        }


                    } catch (Exception e) {
                        e.printStackTrace();

                    }

                }
            });

        } else { //Toast.makeText(getApplicationContext(),"no new data", Toast.LENGTH_SHORT).show();
        }
    }


    public void synchLeftFingersA() {
        FingerprintRequest fr = db.getFingersLeftHandA();
        ArrayList<String> images = fr.getFingerprintsImages();
        ArrayList<String> templates = fr.getFingerprints();
        //  Toast.makeText(getApplicationContext(),"a", Toast.LENGTH_SHORT).show();

        if (images != null && !images.isEmpty() && templates != null && !templates.isEmpty()) {


            String synch_url = fromSharedPref + "enrollFingerPrint";
            //String synch_url="http://epayment.com.ng/epayment/api/raw";
            //  Toast.makeText(getApplicationContext(),synch_url, Toast.LENGTH_SHORT).show();
            OkHttpClient client = new OkHttpClient.Builder()
                    .connectTimeout(1, TimeUnit.MINUTES)
                    .readTimeout(60, TimeUnit.SECONDS)
                    .writeTimeout(60, TimeUnit.SECONDS)
                    .build();
            MediaType JSON = MediaType.parse("application/json; charset=utf-8");
            // put your json here
            Gson gson = new GsonBuilder().create();
            String postdata = gson.toJson(fr);
            // Log.d("JSON request", postdata);
            RequestBody body = RequestBody.create(JSON, postdata);
            Request request = new Request.Builder()
                    .url(synch_url)
                    .addHeader("Authorization", "Bearer " + fr.token)
                    .post(body)
                    .build();

            okhttp3.Call call = client.newCall(request);
            call.enqueue(new okhttp3.Callback() {
                @Override
                public void onFailure(okhttp3.Call call, IOException e) {
                    // waitingDialog.dismiss();
                    Log.d("OKHTTP RESPONSE", "failed in callback");
                    toastj(getApplicationContext(), e.getLocalizedMessage().toString());
                    //  Intent i=new Intent(getApplicationContext(),Login.class);
                    //  startActivity(i);
                }

                @Override
                public void onResponse(okhttp3.Call call, final okhttp3.Response response) throws IOException {


                    // Stuff that updates the UI
                    try {
                        //Response response = client.newCall(request).execute();
                        okresp = response.body().string();

                        Log.d("token", "Token: " + fr.token);
                        Log.d("fingerprint", "Response: " + okresp);
                        // toastj(getApplicationContext(),okresp);

                        JSONObject jb = null;
                        try {
                            jb = new JSONObject(okresp);
                            String status = jb.getString("status");

                            if (status.equalsIgnoreCase("success")) {

                                db.UpdateLeftFingerAResponse(fr.phone);
                            }


                        } catch (JSONException e) {
                            // Log.d("fingerprint token", fr.token);
                            e.printStackTrace();
                        }


                    } catch (Exception e) {
                        // Log.d("fingerprint token", fr.token);
                        e.printStackTrace();

                    }

                }
            });


        } else {
            // Toast.makeText(MyService.this,"No finger  a", Toast.LENGTH_LONG).show();
        }


    }

    public void synchLeftFingersB() {
        FingerprintRequest fr = db.getFingersLeftHandB();
        ArrayList<String> images = fr.getFingerprintsImages();
        ArrayList<String> templates = fr.getFingerprints();
        //  Toast.makeText(getApplicationContext(),"b", Toast.LENGTH_SHORT).show();
        if (images != null && !images.isEmpty() && templates != null && !templates.isEmpty()) {


            String synch_url = fromSharedPref + "enrollFingerPrint";
            // Toast.makeText(getApplicationContext(),synch_url, Toast.LENGTH_SHORT).show();
            OkHttpClient client = new OkHttpClient.Builder()
                    .connectTimeout(1, TimeUnit.MINUTES)
                    .readTimeout(60, TimeUnit.SECONDS)
                    .writeTimeout(60, TimeUnit.SECONDS)
                    .build();
            MediaType JSON = MediaType.parse("application/json; charset=utf-8");
            // put your json here
            Gson gson = new GsonBuilder().create();
            String postdata = gson.toJson(fr);
            RequestBody body = RequestBody.create(JSON, postdata);
            Request request = new Request.Builder()
                    .url(synch_url)
                    .addHeader("Authorization", "Bearer " + fr.token)
                    .post(body)
                    .build();

            okhttp3.Call call = client.newCall(request);
            call.enqueue(new okhttp3.Callback() {
                @Override
                public void onFailure(okhttp3.Call call, IOException e) {
                    // waitingDialog.dismiss();
                    Log.d("OKHTTP RESPONSE", "failed in callback");
                    toastj(getApplicationContext(), e.getLocalizedMessage().toString());
                    //  Intent i=new Intent(getApplicationContext(),Login.class);
                    //  startActivity(i);
                }

                @Override
                public void onResponse(okhttp3.Call call, final okhttp3.Response response) throws IOException {


                    // Stuff that updates the UI
                    try {
                        //Response response = client.newCall(request).execute();
                        okresp = response.body().string();
                        Log.d("fingerprint", "Response: " + okresp);
                        //   toastj(getApplicationContext(),okresp);

                        JSONObject jb = null;
                        try {
                            jb = new JSONObject(okresp);
                            String status = jb.getString("status");

                            if (status.equalsIgnoreCase("success")) {

                                db.UpdateLeftFingerBResponse(fr.phone);
                            }


                        } catch (JSONException e) {
                            e.printStackTrace();
                        }


                    } catch (Exception e) {
                        e.printStackTrace();

                    }

                }
            });


        } else {
            // Toast.makeText(MyService.this,"No finger b", Toast.LENGTH_LONG).show();
        }


    }

    public void synchLeftFingersC() {
        FingerprintRequest fr = db.getFingersLeftHandC();
        ArrayList<String> images = fr.getFingerprintsImages();
        ArrayList<String> templates = fr.getFingerprints();
        // Toast.makeText(getApplicationContext(),"c", Toast.LENGTH_SHORT).show();
        if (images != null && !images.isEmpty() && templates != null && !templates.isEmpty()) {


            String synch_url = fromSharedPref + "enrollFingerPrint";
            // Toast.makeText(getApplicationContext(),synch_url, Toast.LENGTH_SHORT).show();
            OkHttpClient client = new OkHttpClient.Builder()
                    .connectTimeout(1, TimeUnit.MINUTES)
                    .readTimeout(60, TimeUnit.SECONDS)
                    .writeTimeout(60, TimeUnit.SECONDS)
                    .build();
            MediaType JSON = MediaType.parse("application/json; charset=utf-8");
            // put your json here
            Gson gson = new GsonBuilder().create();
            String postdata = gson.toJson(fr);
            RequestBody body = RequestBody.create(JSON, postdata);
            Request request = new Request.Builder()
                    .url(synch_url)
                    .addHeader("Authorization", "Bearer " + fr.token)
                    .post(body)
                    .build();

            okhttp3.Call call = client.newCall(request);
            call.enqueue(new okhttp3.Callback() {
                @Override
                public void onFailure(okhttp3.Call call, IOException e) {
                    // waitingDialog.dismiss();
                    Log.d("OKHTTP RESPONSE", "failed in callback");
                    toastj(getApplicationContext(), e.getLocalizedMessage().toString());
                    //  Intent i=new Intent(getApplicationContext(),Login.class);
                    //  startActivity(i);
                }

                @Override
                public void onResponse(okhttp3.Call call, final okhttp3.Response response) throws IOException {


                    // Stuff that updates the UI
                    try {
                        //Response response = client.newCall(request).execute();
                        okresp = response.body().string();
                        Log.d("fingerprint", "Response: " + okresp);
                        //toastj(getApplicationContext(),okresp);

                        JSONObject jb = null;
                        try {
                            jb = new JSONObject(okresp);
                            String status = jb.getString("status");

                            if (status.equalsIgnoreCase("success")) {

                                db.UpdateLeftFingerCResponse(fr.phone);
                            }


                        } catch (JSONException e) {
                            e.printStackTrace();
                        }


                    } catch (Exception e) {
                        e.printStackTrace();

                    }

                }
            });


        } else {
            // Toast.makeText(MyService.this,"No da", Toast.LENGTH_LONG).show();
        }


    }

    public void synchLeftFingersD() {
        FingerprintRequest fr = db.getFingersLeftHandD();
        ArrayList<String> images = fr.getFingerprintsImages();
        ArrayList<String> templates = fr.getFingerprints();
        //Toast.makeText(getApplicationContext(), user.get(0).getName(), Toast.LENGTH_SHORT).show();
        if (images != null && !images.isEmpty() && templates != null && !templates.isEmpty()) {


            String synch_url = fromSharedPref + "enrollFingerPrint";
            // Toast.makeText(getApplicationContext(),synch_url, Toast.LENGTH_SHORT).show();
            OkHttpClient client = new OkHttpClient.Builder()
                    .connectTimeout(1, TimeUnit.MINUTES)
                    .readTimeout(60, TimeUnit.SECONDS)
                    .writeTimeout(60, TimeUnit.SECONDS)
                    .build();
            MediaType JSON = MediaType.parse("application/json; charset=utf-8");
            // put your json here
            Gson gson = new GsonBuilder().create();
            String postdata = gson.toJson(fr);
            RequestBody body = RequestBody.create(JSON, postdata);
            Request request = new Request.Builder()
                    .url(synch_url)
                    .addHeader("Authorization", "Bearer " + fr.token)
                    .post(body)
                    .build();

            okhttp3.Call call = client.newCall(request);
            call.enqueue(new okhttp3.Callback() {
                @Override
                public void onFailure(okhttp3.Call call, IOException e) {
                    // waitingDialog.dismiss();
                    Log.d("OKHTTP RESPONSE", "failed in callback");
                    toastj(getApplicationContext(), e.getLocalizedMessage().toString());
                    //  Intent i=new Intent(getApplicationContext(),Login.class);
                    //  startActivity(i);
                }

                @Override
                public void onResponse(okhttp3.Call call, final okhttp3.Response response) throws IOException {


                    // Stuff that updates the UI
                    try {
                        //Response response = client.newCall(request).execute();
                        okresp = response.body().string();
                        Log.d("fingerprint", "Response: " + okresp);
                        //toastj(getApplicationContext(),okresp);

                        JSONObject jb = null;
                        try {
                            jb = new JSONObject(okresp);
                            String status = jb.getString("status");

                            if (status.equalsIgnoreCase("success")) {

                                db.UpdateLeftFingerDResponse(fr.phone);
                            }


                        } catch (JSONException e) {
                            e.printStackTrace();
                        }


                    } catch (Exception e) {
                        e.printStackTrace();

                    }

                }
            });


        } else {
            // Toast.makeText(MyService.this,"No da", Toast.LENGTH_LONG).show();
        }


    }

    public void synchLeftFingersE() {
        FingerprintRequest fr = db.getFingersLeftHandE();
        ArrayList<String> images = fr.getFingerprintsImages();
        ArrayList<String> templates = fr.getFingerprints();
        //Toast.makeText(getApplicationContext(), user.get(0).getName(), Toast.LENGTH_SHORT).show();
        if (images != null && !images.isEmpty() && templates != null && !templates.isEmpty()) {


            String synch_url = fromSharedPref + "enrollFingerPrint";
            // Toast.makeText(getApplicationContext(),synch_url, Toast.LENGTH_SHORT).show();
            OkHttpClient client = new OkHttpClient.Builder()
                    .connectTimeout(1, TimeUnit.MINUTES)
                    .readTimeout(60, TimeUnit.SECONDS)
                    .writeTimeout(60, TimeUnit.SECONDS)
                    .build();
            MediaType JSON = MediaType.parse("application/json; charset=utf-8");
            // put your json here
            Gson gson = new GsonBuilder().create();
            String postdata = gson.toJson(fr);
            RequestBody body = RequestBody.create(JSON, postdata);
            Request request = new Request.Builder()
                    .url(synch_url)
                    .addHeader("Authorization", "Bearer " + fr.token)
                    .post(body)
                    .build();

            okhttp3.Call call = client.newCall(request);
            call.enqueue(new okhttp3.Callback() {
                @Override
                public void onFailure(okhttp3.Call call, IOException e) {
                    // waitingDialog.dismiss();
                    Log.d("OKHTTP RESPONSE", "failed in callback");
                    toastj(getApplicationContext(), e.getLocalizedMessage().toString());
                    //  Intent i=new Intent(getApplicationContext(),Login.class);
                    //  startActivity(i);
                }

                @Override
                public void onResponse(okhttp3.Call call, final okhttp3.Response response) throws IOException {


                    // Stuff that updates the UI
                    try {
                        //Response response = client.newCall(request).execute();
                        okresp = response.body().string();
                        Log.d("fingerprint", "Response: " + okresp);
                        //toastj(getApplicationContext(),okresp);

                        JSONObject jb = null;
                        try {
                            jb = new JSONObject(okresp);
                            String status = jb.getString("status");

                            if (status.equalsIgnoreCase("success")) {

                                db.UpdateLeftFingerEResponse(fr.phone);
                            }


                        } catch (JSONException e) {
                            e.printStackTrace();
                        }


                    } catch (Exception e) {
                        e.printStackTrace();

                    }

                }
            });


        } else {
            // Toast.makeText(MyService.this,"No da", Toast.LENGTH_LONG).show();
        }


    }


    public void synchRightFingers() {
        FingerprintRequest fr = db.getFingersLeftHandA();
        ArrayList<String> images = fr.getFingerprintsImages();
        ArrayList<String> templates = fr.getFingerprints();
        //Toast.makeText(getApplicationContext(), user.get(0).getName(), Toast.LENGTH_SHORT).show();
        if (images != null && !images.isEmpty() && templates != null && !templates.isEmpty()) {


            String synch_url = fromSharedPref + "enrollFingerPrint";
            // Toast.makeText(getApplicationContext(),synch_url, Toast.LENGTH_SHORT).show();
            OkHttpClient client = new OkHttpClient.Builder()
                    .connectTimeout(1, TimeUnit.MINUTES)
                    .readTimeout(60, TimeUnit.SECONDS)
                    .writeTimeout(60, TimeUnit.SECONDS)
                    .build();
            MediaType JSON = MediaType.parse("application/json; charset=utf-8");
            // put your json here
            Gson gson = new GsonBuilder().create();
            String postdata = gson.toJson(fr);
            RequestBody body = RequestBody.create(JSON, postdata);
            Request request = new Request.Builder()
                    .url(synch_url)
                    .addHeader("Authorization", "Bearer " + fr.token)
                    .post(body)
                    .build();

            okhttp3.Call call = client.newCall(request);
            call.enqueue(new okhttp3.Callback() {
                @Override
                public void onFailure(okhttp3.Call call, IOException e) {
                    // waitingDialog.dismiss();
                    Log.d("OKHTTP RESPONSE", "failed in callback");
                    toastj(getApplicationContext(), e.getLocalizedMessage().toString());
                    //  Intent i=new Intent(getApplicationContext(),Login.class);
                    //  startActivity(i);
                }

                @Override
                public void onResponse(okhttp3.Call call, final okhttp3.Response response) throws IOException {


                    // Stuff that updates the UI
                    try {
                        //Response response = client.newCall(request).execute();
                        okresp = response.body().string();
                        Log.d("fingerprint", "Response: " + okresp);
                        //toastj(getApplicationContext(),okresp);

                        JSONObject jb = null;
                        try {
                            jb = new JSONObject(okresp);
                            String status = jb.getString("status");

                            if (status.equalsIgnoreCase("success")) {

                                db.UpdateRightFingerResponse(fr.phone);
                            }


                        } catch (JSONException e) {
                            e.printStackTrace();
                        }


                    } catch (Exception e) {
                        e.printStackTrace();

                    }

                }
            });


        } else {
            // Toast.makeText(MyService.this,"No da", Toast.LENGTH_LONG).show();
        }


    }

    public static void toastj(final Context context,
                              final String msg) {
        if (context != null && msg != null) {
            new Handler(Looper.getMainLooper()).post(new Runnable() {

                @Override
                public void run() {
                    Toast.makeText(context, msg, Toast.LENGTH_SHORT).show();
                }
            });
        }
    }

    public void synchRightFingersA() {
        FingerprintRequest fr = db.getFingersRightHandA();
        ArrayList<String> images = fr.getFingerprintsImages();
        ArrayList<String> templates = fr.getFingerprints();
        //Toast.makeText(getApplicationContext(), user.get(0).getName(), Toast.LENGTH_SHORT).show();
        if (images != null && !images.isEmpty() && templates != null && !templates.isEmpty()) {


            String synch_url = fromSharedPref + "enrollFingerPrint";
            //  Toast.makeText(getApplicationContext(),synch_url, Toast.LENGTH_SHORT).show();
            OkHttpClient client = new OkHttpClient.Builder()
                    .connectTimeout(1, TimeUnit.MINUTES)
                    .readTimeout(60, TimeUnit.SECONDS)
                    .writeTimeout(60, TimeUnit.SECONDS)
                    .build();
            MediaType JSON = MediaType.parse("application/json; charset=utf-8");
            // put your json here
            Gson gson = new GsonBuilder().create();
            String postdata = gson.toJson(fr);
            RequestBody body = RequestBody.create(JSON, postdata);
            Request request = new Request.Builder()
                    .url(synch_url)
                    .addHeader("Authorization", "Bearer " + fr.token)
                    .post(body)
                    .build();

            okhttp3.Call call = client.newCall(request);
            call.enqueue(new okhttp3.Callback() {
                @Override
                public void onFailure(okhttp3.Call call, IOException e) {
                    // waitingDialog.dismiss();
                    Log.d("OKHTTP RESPONSE", "failed in callback");
                    toastj(getApplicationContext(), e.getLocalizedMessage().toString());
                    //  Intent i=new Intent(getApplicationContext(),Login.class);
                    //  startActivity(i);
                }

                @Override
                public void onResponse(okhttp3.Call call, final okhttp3.Response response) throws IOException {


                    // Stuff that updates the UI
                    try {
                        //Response response = client.newCall(request).execute();
                        okresp = response.body().string();
                        Log.d("fingerprint", "Response: " + okresp);
                        //  toastj(getApplicationContext(),okresp);

                        JSONObject jb = null;
                        try {
                            jb = new JSONObject(okresp);
                            String status = jb.getString("status");

                            if (status.equalsIgnoreCase("success")) {

                                db.UpdateRightFingerAResponse(fr.phone);
                            }


                        } catch (JSONException e) {
                            e.printStackTrace();
                        }


                    } catch (Exception e) {
                        e.printStackTrace();

                    }

                }
            });


        } else {
            // Toast.makeText(MyService.this,"No finger  a", Toast.LENGTH_LONG).show();
        }


    }

    public void synchRightFingersB() {
        FingerprintRequest fr = db.getFingersRightHandB();
        ArrayList<String> images = fr.getFingerprintsImages();
        ArrayList<String> templates = fr.getFingerprints();
        //Toast.makeText(getApplicationContext(), user.get(0).getName(), Toast.LENGTH_SHORT).show();
        if (images != null && !images.isEmpty() && templates != null && !templates.isEmpty()) {


            String synch_url = fromSharedPref + "enrollFingerPrint";
            // Toast.makeText(getApplicationContext(),synch_url, Toast.LENGTH_SHORT).show();
            OkHttpClient client = new OkHttpClient.Builder()
                    .connectTimeout(1, TimeUnit.MINUTES)
                    .readTimeout(60, TimeUnit.SECONDS)
                    .writeTimeout(60, TimeUnit.SECONDS)
                    .build();
            MediaType JSON = MediaType.parse("application/json; charset=utf-8");
            // put your json here
            Gson gson = new GsonBuilder().create();
            String postdata = gson.toJson(fr);
            RequestBody body = RequestBody.create(JSON, postdata);
            Request request = new Request.Builder()
                    .url(synch_url)
                    .addHeader("Authorization", "Bearer " + fr.token)
                    .post(body)
                    .build();

            okhttp3.Call call = client.newCall(request);
            call.enqueue(new okhttp3.Callback() {
                @Override
                public void onFailure(okhttp3.Call call, IOException e) {
                    // waitingDialog.dismiss();
                    Log.d("OKHTTP RESPONSE", "failed in callback");
                    toastj(getApplicationContext(), e.getLocalizedMessage().toString());
                    //  Intent i=new Intent(getApplicationContext(),Login.class);
                    //  startActivity(i);
                }

                @Override
                public void onResponse(okhttp3.Call call, final okhttp3.Response response) throws IOException {


                    // Stuff that updates the UI
                    try {
                        //Response response = client.newCall(request).execute();
                        okresp = response.body().string();
                        Log.d("fingerprint", "Response: " + okresp);
                        // toastj(getApplicationContext(),okresp);

                        JSONObject jb = null;
                        try {
                            jb = new JSONObject(okresp);
                            String status = jb.getString("status");

                            if (status.equalsIgnoreCase("success")) {

                                db.UpdateRightFingerBResponse(fr.phone);
                            }


                        } catch (JSONException e) {
                            e.printStackTrace();
                        }


                    } catch (Exception e) {
                        e.printStackTrace();

                    }

                }
            });


        } else {
            // Toast.makeText(MyService.this,"No finger b", Toast.LENGTH_LONG).show();
        }


    }

    public void synchRightFingersC() {
        FingerprintRequest fr = db.getFingersRightHandC();
        ArrayList<String> images = fr.getFingerprintsImages();
        ArrayList<String> templates = fr.getFingerprints();
        //Toast.makeText(getApplicationContext(), user.get(0).getName(), Toast.LENGTH_SHORT).show();
        if (images != null && !images.isEmpty() && templates != null && !templates.isEmpty()) {


            String synch_url = fromSharedPref + "enrollFingerPrint";
            // Toast.makeText(getApplicationContext(),synch_url, Toast.LENGTH_SHORT).show();
            OkHttpClient client = new OkHttpClient.Builder()
                    .connectTimeout(1, TimeUnit.MINUTES)
                    .readTimeout(60, TimeUnit.SECONDS)
                    .writeTimeout(60, TimeUnit.SECONDS)
                    .build();
            MediaType JSON = MediaType.parse("application/json; charset=utf-8");
            // put your json here
            Gson gson = new GsonBuilder().create();
            String postdata = gson.toJson(fr);
            RequestBody body = RequestBody.create(JSON, postdata);
            Request request = new Request.Builder()
                    .url(synch_url)
                    .addHeader("Authorization", "Bearer " + fr.token)
                    .post(body)
                    .build();

            okhttp3.Call call = client.newCall(request);
            call.enqueue(new okhttp3.Callback() {
                @Override
                public void onFailure(okhttp3.Call call, IOException e) {
                    // waitingDialog.dismiss();
                    Log.d("OKHTTP RESPONSE", "failed in callback");
                    toastj(getApplicationContext(), e.getLocalizedMessage().toString());
                    //  Intent i=new Intent(getApplicationContext(),Login.class);
                    //  startActivity(i);
                }

                @Override
                public void onResponse(okhttp3.Call call, final okhttp3.Response response) throws IOException {


                    // Stuff that updates the UI
                    try {
                        //Response response = client.newCall(request).execute();
                        okresp = response.body().string();
                        Log.d("fingerprint", "Response: " + okresp);
                        //toastj(getApplicationContext(),okresp);

                        JSONObject jb = null;
                        try {
                            jb = new JSONObject(okresp);
                            String status = jb.getString("status");

                            if (status.equalsIgnoreCase("success")) {

                                db.UpdateRightFingerCResponse(fr.phone);
                            }


                        } catch (JSONException e) {
                            e.printStackTrace();
                        }


                    } catch (Exception e) {
                        e.printStackTrace();

                    }

                }
            });


        } else {
            // Toast.makeText(MyService.this,"No da", Toast.LENGTH_LONG).show();
        }


    }

    public void synchRightFingersD() {
        FingerprintRequest fr = db.getFingersRightHandD();
        ArrayList<String> images = fr.getFingerprintsImages();
        ArrayList<String> templates = fr.getFingerprints();
        //Toast.makeText(getApplicationContext(), user.get(0).getName(), Toast.LENGTH_SHORT).show();
        if (images != null && !images.isEmpty() && templates != null && !templates.isEmpty()) {


            String synch_url = fromSharedPref + "enrollFingerPrint";
            //  Toast.makeText(getApplicationContext(),synch_url, Toast.LENGTH_SHORT).show();
            OkHttpClient client = new OkHttpClient.Builder()
                    .connectTimeout(1, TimeUnit.MINUTES)
                    .readTimeout(60, TimeUnit.SECONDS)
                    .writeTimeout(60, TimeUnit.SECONDS)
                    .build();
            MediaType JSON = MediaType.parse("application/json; charset=utf-8");
            // put your json here
            Gson gson = new GsonBuilder().create();
            String postdata = gson.toJson(fr);
            RequestBody body = RequestBody.create(JSON, postdata);
            Request request = new Request.Builder()
                    .url(synch_url)
                    .addHeader("Authorization", "Bearer " + fr.token)
                    .post(body)
                    .build();

            okhttp3.Call call = client.newCall(request);
            call.enqueue(new okhttp3.Callback() {
                @Override
                public void onFailure(okhttp3.Call call, IOException e) {
                    // waitingDialog.dismiss();
                    Log.d("OKHTTP RESPONSE", "failed in callback");
                    toastj(getApplicationContext(), e.getLocalizedMessage().toString());
                    //  Intent i=new Intent(getApplicationContext(),Login.class);
                    //  startActivity(i);
                }

                @Override
                public void onResponse(okhttp3.Call call, final okhttp3.Response response) throws IOException {


                    // Stuff that updates the UI
                    try {
                        //Response response = client.newCall(request).execute();
                        okresp = response.body().string();
                        Log.d("fingerprint", "Response: " + okresp);
                        //toastj(getApplicationContext(),okresp);

                        JSONObject jb = null;
                        try {
                            jb = new JSONObject(okresp);
                            String status = jb.getString("status");

                            if (status.equalsIgnoreCase("success")) {

                                db.UpdateRightFingerDResponse(fr.phone);
                            }


                        } catch (JSONException e) {
                            e.printStackTrace();
                        }


                    } catch (Exception e) {
                        e.printStackTrace();

                    }

                }
            });


        } else {
            //  Toast.makeText(MyService.this,"No da", Toast.LENGTH_LONG).show();
        }


    }

    public void synchRightFingersE() {
        FingerprintRequest fr = db.getFingersRightHandE();
        ArrayList<String> images = fr.getFingerprintsImages();
        ArrayList<String> templates = fr.getFingerprints();
        //Toast.makeText(getApplicationContext(), user.get(0).getName(), Toast.LENGTH_SHORT).show();
        if (images != null && !images.isEmpty() && templates != null && !templates.isEmpty()) {


            String synch_url = fromSharedPref + "enrollFingerPrint";
            //  Toast.makeText(getApplicationContext(),synch_url, Toast.LENGTH_SHORT).show();
            OkHttpClient client = new OkHttpClient.Builder()
                    .connectTimeout(1, TimeUnit.MINUTES)
                    .readTimeout(60, TimeUnit.SECONDS)
                    .writeTimeout(60, TimeUnit.SECONDS)
                    .build();
            MediaType JSON = MediaType.parse("application/json; charset=utf-8");
            // put your json here
            Gson gson = new GsonBuilder().create();
            String postdata = gson.toJson(fr);
            RequestBody body = RequestBody.create(JSON, postdata);
            Request request = new Request.Builder()
                    .url(synch_url)
                    .addHeader("Authorization", "Bearer " + fr.token)
                    .post(body)
                    .build();

            okhttp3.Call call = client.newCall(request);
            call.enqueue(new okhttp3.Callback() {
                @Override
                public void onFailure(okhttp3.Call call, IOException e) {
                    // waitingDialog.dismiss();
                    Log.d("OKHTTP RESPONSE", "failed in callback");
                    toastj(getApplicationContext(), e.getLocalizedMessage().toString());
                    //  Intent i=new Intent(getApplicationContext(),Login.class);
                    //  startActivity(i);
                }

                @Override
                public void onResponse(okhttp3.Call call, final okhttp3.Response response) throws IOException {


                    // Stuff that updates the UI
                    try {
                        //Response response = client.newCall(request).execute();
                        okresp = response.body().string();
                        Log.d("fingerprint", "Response: " + okresp);
                        //toastj(getApplicationContext(),okresp);

                        JSONObject jb = null;
                        try {
                            jb = new JSONObject(okresp);
                            String status = jb.getString("status");

                            if (status.equalsIgnoreCase("success")) {

                                db.UpdateRightFingerEResponse(fr.phone);
                            }


                        } catch (JSONException e) {
                            e.printStackTrace();
                        }


                    } catch (Exception e) {
                        e.printStackTrace();

                    }

                }
            });


        } else {
            // Toast.makeText(MyService.this,"No da", Toast.LENGTH_LONG).show();
        }


    }

    public void synchPassport() {


        PassportRequest p = db.getPassport();
        // Toast.makeText(getApplicationContext(),"new data", Toast.LENGTH_SHORT).show();
        if (!p.token.contains("Token")) {
            //  Toast.makeText(getApplicationContext(),p.token, Toast.LENGTH_SHORT).show();


            String passport_url = fromSharedPref + "uploadPassport";

            OkHttpClient client = new OkHttpClient.Builder()
                    .connectTimeout(1, TimeUnit.MINUTES)
                    .readTimeout(60, TimeUnit.SECONDS)
                    .writeTimeout(60, TimeUnit.SECONDS)
                    .build();
            MediaType JSON = MediaType.parse("application/json; charset=utf-8");
            // put your json here
            Gson gson = new GsonBuilder().create();
            String postdata = gson.toJson(p);
            RequestBody body = RequestBody.create(JSON, postdata);
            Request request = new Request.Builder()
                    .url(passport_url)
                    .addHeader("Authorization", "Bearer " + p.token)
                    .post(body)
                    .build();

            okhttp3.Call call = client.newCall(request);
            call.enqueue(new okhttp3.Callback() {
                @Override
                public void onFailure(okhttp3.Call call, IOException e) {
                    // waitingDialog.dismiss();
                    Log.d("OKHTTP RESPONSE", "failed in callback");
                    // toastj(getApplicationContext(), e.getLocalizedMessage().toString());
                    //  Intent i=new Intent(getApplicationContext(),Login.class);
                    //  startActivity(i);
                }

                @Override
                public void onResponse(okhttp3.Call call, final okhttp3.Response response) throws IOException {


                    // Stuff that updates the UI
                    try {
                        //Response response = client.newCall(request).execute();
                        okresp = response.body().string();
                        Log.d("Passport", "Response: " + okresp);
                        // toastj(getApplicationContext(),okresp);

                        JSONObject jb = null;
                        try {
                            jb = new JSONObject(okresp);
                            String status = jb.getString("status");

                            if (status.equalsIgnoreCase("success")) {
                                JSONObject data = jb.getJSONObject("data");


                                db.updatePassPortResponse(p.phone);
                            }


                        } catch (JSONException e) {
                            e.printStackTrace();
                        }


                    } catch (Exception e) {
                        e.printStackTrace();

                    }

                }
            });

        } else {
            //Toast.makeText(getApplicationContext(),"no new data", Toast.LENGTH_SHORT).show();

        }
    }


}
