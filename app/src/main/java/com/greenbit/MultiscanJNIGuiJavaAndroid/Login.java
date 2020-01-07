package com.greenbit.MultiscanJNIGuiJavaAndroid;

import android.content.Intent;
import android.os.Bundle;
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
import com.greenbit.MultiscanJNIGuiJavaAndroid.interfaces.BIPPIIS;
import com.greenbit.MultiscanJNIGuiJavaAndroid.models.LoginRequest;
import com.greenbit.MultiscanJNIGuiJavaAndroid.models.LoginResponse;
import com.greenbit.MultiscanJNIGuiJavaAndroid.utils.Tools;

import java.io.IOException;
import java.util.Objects;

import okhttp3.Interceptor;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

public class Login extends AppCompatActivity {
    private AppCompatEditText bip;
    private String token = "demo_token";
    private String bippiis, getBippiis;
    private ProgressBar progressBar;
    private AppCompatImageView img;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);
        bip = findViewById(R.id.bippiis_no);

        //to be removed
        bip.setText("O/S 2243");

        progressBar = findViewById(R.id.progress);
        img = findViewById(R.id.img);
        final Animation animation = new AlphaAnimation(1, 0); // Change alpha from fully visible to invisible
        final Transition transition = new Transition() {
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
                Toast.makeText(getApplicationContext(), "Logging in..", Toast.LENGTH_SHORT).show();
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
        bippiis = bip.getText().toString().trim();

        if (bippiis.isEmpty()) {
            Toast.makeText(getApplicationContext(), "BIPPIIS NO CANNOT BE EMPTY!", Toast.LENGTH_SHORT).show();
        } else {
            getBippiis = bip.getText().toString().trim();

            //retrofit_auth and get token
            progressBar.setVisibility(View.VISIBLE);

            OkHttpClient client = new OkHttpClient.Builder().addInterceptor(new Interceptor() {
                @Override
                public okhttp3.Response intercept(Chain chain) throws IOException {
                    Request newRequest = chain.request().newBuilder()
                            .addHeader("Authorization", "Bearer " + token)
                            .build();
                    return chain.proceed(newRequest);
                }
            }).build();

            Retrofit retrofit = new Retrofit.Builder().client(client)
                    .baseUrl(getString(R.string.base_url))
                    .addConverterFactory(GsonConverterFactory.create()).build();
            BIPPIIS service = retrofit.create(BIPPIIS.class);

            LoginRequest loginRequest = new LoginRequest();
            loginRequest.setBippiis_number(Objects.requireNonNull(bip.getText()).toString().trim());

            Call<LoginResponse> loginResponseCall = service.getLoginResponse(loginRequest);
            loginResponseCall.enqueue(new Callback<LoginResponse>() {
                @Override
                public void onResponse(Call<LoginResponse> call, Response<LoginResponse> response) {
                    progressBar.setVisibility(View.GONE);

                    LoginResponse loginResponse = response.body();


                    //validate response
                    try {
                        if (loginResponse.getStatus().equalsIgnoreCase("success")) {

                            Log.d("fingerprint", "Response: " + response.body().getData().toString());
                            String has_enrolled = "";

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                            String js = new Gson().toJson(loginResponse.getData()).trim();
//                            String json = js.substring(1, js.length() - 1);
//                            Log.d("fingerprint", "Response: JSON " + json);
//
//                            String[] values = json.split(",");
//                            String[] enroll_value = values[7].split(":");
//                            has_enrolled = enroll_value[1];
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                            String js = new Gson().toJson(loginResponse.getData()).trim();
                            String json = js.substring(1, js.length() - 1);
                            Log.d("fingerprint", "Response: JSON " + json);

                            String[] values = json.split(",");
                            String[] enroll_value = values[7].split(":");
                            has_enrolled = enroll_value[1];


                            Log.d("fingerprint", "has enroll: " + has_enrolled);

                            boolean enrolled = false;
                            if (has_enrolled.trim().equalsIgnoreCase("false")) {
                                enrolled = false;
                            } else if (has_enrolled.trim().equalsIgnoreCase("true")) {
                                enrolled = true;
                            }
                            if (!enrolled) {
                                bippiis = bippiis.replace("/", "_");
                                bippiis = bippiis.replace("-", "_");
                                bippiis = bippiis.replace(" ", "_").toUpperCase();
                                Log.d("fingerprint", "Coming from Original BIPPIIS : " + getBippiis + " edited BIPPIIS: " + bippiis + " token : " + token);

                                startActivity(new Intent(getApplicationContext(), EnrollFingerprints.class).putExtra("bippiis_number", getBippiis).putExtra("bippiis_number_edited", bippiis).putExtra("token", token));
                            } else {
                                // go to login then camera
                                Toast.makeText(getApplicationContext(), "Has been enrolled", Toast.LENGTH_LONG).show();
                            }
                        } else {
                            Log.d("fingerprint", "Response RAW: " + response.raw());

                            Toast.makeText(getApplicationContext(), "INVALID BIPPIIS NUMBER", Toast.LENGTH_SHORT).show();
                        }

                    } catch (NullPointerException n) {
                        n.printStackTrace();

                        Toast.makeText(getApplicationContext(), "INVALID BIPPIIS NUMBER", Toast.LENGTH_SHORT).show();

                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }

                @Override
                public void onFailure(Call<LoginResponse> call, Throwable t) {
                    progressBar.setVisibility(View.GONE);

                    Log.d("fingerprint", "Failed to Verify: " + t.getLocalizedMessage());

                }
            });


        }
    }

    private long exitTime = 0;

    public void doExitApp() {
        if ((System.currentTimeMillis() - exitTime) > 2000) {
            Tools.toast("Press again to exit app", Login.this);
            exitTime = System.currentTimeMillis();
        } else {
            finishAffinity();
        }
    }


    @Override
    public void onBackPressed() {
        doExitApp();
    }
}
