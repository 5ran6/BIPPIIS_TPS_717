package com.greenbit.MultiscanJNIGuiJavaAndroid;

import static com.naic.nigerianarmy.GbExampleGrayScaleBitmapClass.GetBippiisDirectoryName;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.util.Base64;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.view.animation.AlphaAnimation;
import android.view.animation.Animation;
import android.view.animation.LinearInterpolator;
import android.view.inputmethod.EditorInfo;
import android.widget.ProgressBar;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.AppCompatEditText;
import androidx.appcompat.widget.AppCompatImageView;
import androidx.transition.Transition;
import androidx.transition.TransitionValues;

import com.google.gson.Gson;
import com.greenbit.MultiscanJNIGuiJavaAndroid.models.VerifiedUser;
import com.naic.nigerianarmy.interfaces.NAIC;
import com.naic.nigerianarmy.models.ArmyUserResponse;
import com.naic.nigerianarmy.models.LoginRequest;
import com.naic.nigerianarmy.models.VerifiedUser;
import com.naic.nigerianarmy.services.MyService;
import com.naic.nigerianarmy.utils.LfsJavaWrapperDefinesMinutiaN;
import com.naic.nigerianarmy.utils.Tools;

import org.apache.commons.lang3.SerializationUtils;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Objects;
import java.util.concurrent.TimeUnit;

import okhttp3.Interceptor;
import okhttp3.MediaType;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.RequestBody;
import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;


public class Login extends AppCompatActivity {
    private AppCompatEditText bip;
    private String token = "";
    private String bippiis, getBippiis, extra;
    private ProgressBar progressBar;
    private AppCompatImageView img;
    private String mode;

    private SharedPreferences sharedPref;
    private SharedPreferences.Editor editor;
    private String fromSharedPref = "";
   public static VerifiedUser verifiedUser;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login_with_number);
        sharedPref =  PreferenceManager.getDefaultSharedPreferences(getApplicationContext());
        editor = sharedPref.edit();
        fromSharedPref = sharedPref.getString("address", "nothing is here");

        extra = getIntent().getStringExtra("extra");
        bip = findViewById(R.id.bippiis_no);
        mode = getIntent().getStringExtra("extra");

//        //to be removed
//        bip.setText("O/S 2243");
//        FirebaseInstanceId.getInstance().getInstanceId().addOnSuccessListener(Login.this, new OnSuccessListener<InstanceIdResult>() {
//            @Override
//            public void onSuccess(InstanceIdResult instanceIdResult) {
//                mToken = instanceIdResult.getToken();
//                Log.e("Token", mToken);
//                SharedPreferences pref = getApplicationContext().getSharedPreferences("bippiis", MODE_PRIVATE);
//                SharedPreferences.Editor editor = pref.edit();
//                editor.putString("firebaseToken", mToken);
//                editor.apply(); //commit changes
//            }
//        });

        progressBar = findViewById(R.id.progress);
        img = findViewById(R.id.img);


        final Animation animation = new AlphaAnimation(1, 0); // Change alpha from fully visible to invisible
        new Transition() {
            @Override
            public void captureEndValues(@NonNull TransitionValues transitionValues) {

            }

            @Override
            public void captureStartValues(@NonNull TransitionValues transitionValues) {

            }
        };
        animation.setDuration(2000); // duration - 2 seconds
        animation.setInterpolator(new LinearInterpolator()); // do not alter animation rate
        animation.setRepeatCount(3); // Repeat animation infinitely
        animation.setRepeatMode(Animation.REVERSE); // Reverse animation at the end so the button will fade back in
        img.startAnimation(animation);

        bip.setOnEditorActionListener((v, actionId, event) -> {
            boolean handled = false;
            if (actionId == EditorInfo.IME_ACTION_DONE) {
                hideSoftKeyboard();
                Toast.makeText(getApplicationContext(), "Verifying ID..", Toast.LENGTH_SHORT).show();
                login();
                handled = true;
            }
            return handled;

        });

    }

    //DONE
    public void hideSoftKeyboard() {
        getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_HIDDEN);
    }

    public void proceed(View view) {
        login();
    }



    private void login() {
        bippiis = Objects.requireNonNull(bip.getText()).toString().trim();

        if (bippiis.isEmpty()) {
            Toast.makeText(getApplicationContext(), "PHONE NUMBER CANNOT BE EMPTY!", Toast.LENGTH_SHORT).show();
        } else {
            getBippiis = bip.getText().toString().trim();

            //retrofit_auth and get token
            progressBar.setVisibility(View.VISIBLE);

            String verify_url = fromSharedPref + "verifyUser";

            OkHttpClient client = new OkHttpClient.Builder()
                    .connectTimeout(1, TimeUnit.MINUTES)
                    .readTimeout(60, TimeUnit.SECONDS)
                    .writeTimeout(60, TimeUnit.SECONDS)
                    .build();
            MediaType JSON = MediaType.parse("application/json; charset=utf-8");
            // put your json here
           org.json.JSONObject jsonObject = new org.json.JSONObject();
            try {
                jsonObject.put("phone",getBippiis);


            } catch (JSONException e) {
                e.printStackTrace();
            }

            RequestBody body = RequestBody.create(JSON,jsonObject.toString());
            Request request = new Request.Builder()
                    .url(verify_url)
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
                    runOnUiThread(new Runnable() {

                        @Override
                        public void run() {

                            // Stuff that updates the UI
                            progressBar.setVisibility(View.GONE);

                        }
                    });


                    try {
                        //Response response = client.newCall(request).execute();
                     String   okresp = response.body().string();


                        org.json.JSONObject jb = null;
                        try {
                            jb = new org.json.JSONObject(okresp);
                            String status = jb.getString("status");

                            if (status.equalsIgnoreCase("success")) {
                                verifiedUser = new VerifiedUser();
                                org.json.JSONObject data = jb.getJSONObject("data");
                                org.json.JSONObject profile = data.getJSONObject("profile");
                                org.json.JSONObject user = profile.getJSONObject("user");
                                org.json.JSONObject passport = data.getJSONObject("passport");


                                verifiedUser.setPassport(passport.getString("passport"));
                                verifiedUser.setPhone(profile.getString("phone"));
                                verifiedUser.setIdentification_number(profile.getString("army_number"));
                                verifiedUser.setName(profile.getString("name"));
                                verifiedUser.setAge(profile.getString("age"));
                                verifiedUser.setEmail(user.getString("email"));
                                verifiedUser.setHeight(profile.getString("height"));
                                verifiedUser.setEye_color(profile.getString("eye_color"));
                                verifiedUser.setHair_color(profile.getString("hair_color"));
                                verifiedUser.setTattoo(profile.getString("tattoo"));
                                verifiedUser.setSex(profile.getString("sex"));
                                verifiedUser.setWeight(profile.getString("weight"));
                                verifiedUser.setMarital_status(profile.getString("marital_status"));
                                verifiedUser.setBlood_group(profile.getString("blood_group"));
                                verifiedUser.setState_of_origin(profile.getString("state_of_origin"));
                                verifiedUser.setLga(profile.getString("lga"));
                                verifiedUser.setHometown(profile.getString("hometown"));
                                verifiedUser.setNin(profile.getString("nin"));
                                verifiedUser.setGenotype(profile.getString("genotype"));
                                verifiedUser.setSec_sch_year_in(profile.getString("sec_sch_year_in"));
                                verifiedUser.setSec_sch_year_out(profile.getString("sec_sch_year_out"));
                                verifiedUser.setNok_name(profile.getString("nok_name"));
                                verifiedUser.setNok_phone(profile.getString("nok_phone"));
                                verifiedUser.setDate_of_birth(profile.getString("date_of_birth"));
                                verifiedUser.setQualification(profile.getString("qualification"));
                                if(user.has("address")) {
                                    verifiedUser.setAddress(user.getString("address"));
                                }

                                if(profile.has("address")) {
                                    verifiedUser.setAddress(profile.getString("address"));
                                }
                                ArrayList<String> fingeprints = new ArrayList<String>();
                                JSONArray biometrics = data.getJSONArray("biometrics");

                                assert biometrics != null;
                                int len = biometrics.length();

                                //fingeprints=new ArrayList<>();
                                ArrayList<String> templates = new ArrayList<>();
                                for (int j = 0; j < len; j++) {
                                    org.json.JSONObject json = biometrics.getJSONObject(j);
                                    String finger = json.getString("fingerprint");
                                    //    Log.d("myProbe", fingeprints.get(j));
                                    byte[] bytes = Base64.decode(finger, Base64.DEFAULT);
                                    LfsJavaWrapperDefinesMinutiaN[] Probe = deSerialize(bytes);
                                    try {
                                        // Serialize Java object into JSON file.
                                        Gson gson = new Gson();
                                        String json1 = gson.toJson(Probe);
                                        templates.add(json1);

                                        //           mapper.writeValue(file, json);


                                    } catch (Exception e) {
                                        e.printStackTrace();
                                    }

                                }


                                verifiedUser.setTemplates(templates);
                              //  MyService.toastj(Login.this,String.valueOf(verifiedUser.getTemplates().size()));






                                    if(verifiedUser.getTemplates().size()>0)
                                    {
                                        startActivity(new Intent(getApplicationContext(), Verify.class).putExtra("fullname", verifiedUser.getName()).putExtra("extra", extra));
                                    }
                                    else
                                        {
                                            MyService.toastj(Login.this,"No Fingerprints Found");
                                        }


                                }


                            else
                            {
                                MyService.toastj(Login.this,"Invalid Phone");}
                        }catch (JSONException e) {
                            e.printStackTrace();
                        }


                    } catch (Exception e) {
                        e.printStackTrace();

                    }

                }
            });

        }

        }



    private void login_old() {
        bippiis = Objects.requireNonNull(bip.getText()).toString().trim();

        if (bippiis.isEmpty()) {
            Toast.makeText(getApplicationContext(), "PHONE NUMBER CANNOT BE EMPTY!", Toast.LENGTH_SHORT).show();
        } else {
            getBippiis = bip.getText().toString().trim();

            //retrofit_auth and get token
            progressBar.setVisibility(View.VISIBLE);

            OkHttpClient client = new OkHttpClient.Builder().addInterceptor(new Interceptor() {
                @Override
                public okhttp3.Response intercept(Chain chain) throws IOException {
                    Request newRequest = chain.request().newBuilder()
                            .build();
                    return chain.proceed(newRequest);
                }
            }).build();

            Retrofit retrofit = new Retrofit.Builder().client(client)
                    // .baseUrl(getString(R.string.base_url))
                    .baseUrl(fromSharedPref)
                    .addConverterFactory(GsonConverterFactory.create()).build();
            NAIC service = retrofit.create(NAIC.class);

            LoginRequest loginRequest = new LoginRequest();
            loginRequest.setBippiis_number(Objects.requireNonNull(bip.getText()).toString().trim());
            //  loginRequest.setFirebaseToken(mToken);

            Call<ArmyUserResponse> ResponseBodyCall = service.getLoginResponse(loginRequest);


            ResponseBodyCall.enqueue(new Callback<ArmyUserResponse>() {
                @Override
                public void onResponse(Call<ArmyUserResponse> call, Response<ArmyUserResponse> response) {
                    progressBar.setVisibility(View.GONE);
                    Log.d("fingerprint", "Response Code: " + response.code());
//                    Log.d("fingerprint",   getBippiis + " edited STAFF ID: " + bippiis + " token : " + token + " firebase token : " + mToken);
                    // Log.d("fingerprint",   getBippiis + " edited STAFF ID: " + bippiis + " token : " + token);

                    //validate response
                    try {
                        if (response.code() >= 200) {
//                            InputStream inputStr = response.body() != null ? response.body().byteStream() : null;
//
//                            String jsonResponse = "";
//
//
//                            try {
//                                jsonResponse = IOUtils.toString(inputStr, "UTF-8");
//                                Log.d("fingerprint", "" + jsonResponse);
//                            } catch (IOException e) {
//                                e.printStackTrace();
//                            }

                            if (!response.body().getData().toString().isEmpty()) {

                                JSONParser parse = new JSONParser();
                                JSONObject jsonobj_1;
                                try {
                                    jsonobj_1 = (JSONObject) response.body().getData();

                                    Log.d("fingerprint", jsonobj_1.toString());

                                    org.json.JSONObject jsonObject = new org.json.JSONObject(jsonobj_1);

//                                    JSONObject jobj = (JSONObject) jsonobj_1.get("profile");
                                    org.json.JSONObject jobj = jsonObject.getJSONObject("profile");

                                    String army_number = (String) jobj.getString("army_number");
                                    String name = (String) jobj.getString("name");
                                    String age = (String) jobj.getString("age");

                                    String email = jobj.getJSONObject("user").getString("email");
                                    //   String email = name+"@gmail.com";
                                    String height = (String) jobj.getString("height");
                                    String eye_color = (String) jobj.getString("eye_color");
                                    String hair_color = (String) jobj.getString("hair_color");
                                    String tattoo = (String) jobj.getString("tattoo");
                                    String sex = (String) jobj.getString("sex");
                                    String phone = (String) jobj.getString("phone");
                                    String weight = (String) jobj.getString("weight");
                                    String marital_status = (String) jobj.getString("marital_status");
                                    String blood_group = (String) jobj.getString("blood_group");
                                    String state_of_origin = (String) jobj.getString("state_of_origin");
                                    String lga = (String) jobj.getString("lga");
                                    String hometown = (String) jobj.getString("hometown");
                                    String nin = (String) jobj.getString("nin");
                                    String genotype = (String) jobj.getString("genotype");
                                    String sec_sch_year_in = (String) jobj.getString("sec_sch_year_in");
                                    String sec_sch_year_out = (String) jobj.getString("sec_sch_year_out");
                                    String nok_name = (String) jobj.getString("nok_name");
                                    String nok_phone = (String) jobj.getString("nok_phone");

                                    Log.d("fingerprint", name + " " + email);
                                    // store fingerprint in arrayList

                                    ArrayList<String> fingeprints = new ArrayList<String>();
                                    JSONArray biometrics = jsonObject.getJSONArray("biometrics");

                                    assert biometrics != null;
                                    int len = biometrics.length();

                                    for (int j = 0; j < len; j++) {
                                        org.json.JSONObject json = biometrics.getJSONObject(j);
                                        fingeprints.add((String) json.getString("fingerprint"));
                                        //    Log.d("myProbe", fingeprints.get(j));
                                        byte[] bytes = Base64.decode(fingeprints.get(j), Base64.DEFAULT);
                                        LfsJavaWrapperDefinesMinutiaN[] Probe = deSerialize(bytes);
                                        File file = new File(GetBippiisDirectoryName(),
                                                "temp_" + j + ".json");
                                        try {
                                            // Serialize Java object into JSON file.
                                            Gson gson = new Gson();
                                            String json1 = gson.toJson(Probe);

                                            //           mapper.writeValue(file, json);
                                            FileWriter fw = new FileWriter(file.getAbsolutePath());
                                            fw.write(json1);
                                            fw.close();

                                        } catch (IOException e) {
                                            e.printStackTrace();
                                        }

                                        Log.d("Fingerprint", "Closed " + j + " successfully");

                                    }
                                    if (extra.equalsIgnoreCase("profile"))
                                        startActivity(new Intent(getApplicationContext(), BioData.class)
                                                .putExtra("extra", extra)
                                                .putExtra("army_number", army_number)
                                                .putExtra("fullname", name)
                                                .putExtra("age", age)
                                                .putExtra("email", email)
                                                .putExtra("height", height)
                                                .putExtra("eye_color", eye_color)
                                                .putExtra("hair_color", hair_color)
                                                .putExtra("tattoo", tattoo)
                                                .putExtra("sex", sex)
                                                .putExtra("phone", phone)
                                                .putExtra("weight", weight)
                                                .putExtra("marital_status", marital_status)
                                                .putExtra("blood_group", blood_group)
                                                .putExtra("state_of_origin", state_of_origin)
                                                .putExtra("lga", lga)
                                                .putExtra("hometown", hometown)
                                                .putExtra("nin", nin)
                                                .putExtra("sec_sch_year_in", sec_sch_year_in)
                                                .putExtra("sec_sch_year_out", sec_sch_year_out)
                                                .putExtra("nok_name", nok_name)
                                                .putExtra("nok_phone", nok_phone)

                                        ); //put all string extras
                                    else if (extra.equalsIgnoreCase("verify"))
                                        startActivity(new Intent(getApplicationContext(), Verify.class).putExtra("fullname", name).putExtra("extra", extra));
                                } catch (NullPointerException e) {
                                    e.printStackTrace();
                                }
                            } else {
                                Log.d("fingerprint", "Response RAW: " + response.raw());

                                Toast.makeText(getApplicationContext(), "INVALID PHONE NUMBER", Toast.LENGTH_SHORT).show();
                            }
                        } else {
                            //something went wrong. Try again.
                            Toast.makeText(getApplicationContext(), "INVALID PHONE NUMBER. Try again", Toast.LENGTH_SHORT).show();
                        }
                    } catch (NullPointerException n) {
                        n.printStackTrace();

                        Toast.makeText(getApplicationContext(), "INVALID PHONE NUMBER", Toast.LENGTH_SHORT).show();

                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }

                @Override
                public void onFailure(Call<ArmyUserResponse> call, Throwable t) {
                    progressBar.setVisibility(View.GONE);
                    Toast.makeText(getApplicationContext(), "Network Error", Toast.LENGTH_SHORT).show();

                    Log.d("fingerprint", "Failed to Verify: " + t.getLocalizedMessage());

                }
            });
        }
    }

    private long exitTime = 0;

    public void doExitApp() {
        if ((System.currentTimeMillis() - exitTime) > 2000) {
            Tools.toast("Press again to go back", Login.this);
            exitTime = System.currentTimeMillis();
        } else {
            finish();
        }
    }


    @Override
    public void onBackPressed() {
        doExitApp();
    }

    private LfsJavaWrapperDefinesMinutiaN[] deSerialize(byte[] templateCode) {
        return SerializationUtils.deserialize(templateCode);
    }


}
