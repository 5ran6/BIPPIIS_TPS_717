package com.greenbit.MultiscanJNIGuiJavaAndroid;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;

import androidx.appcompat.app.AppCompatActivity;

public class SummaryPage extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_printer);

        //timer
        Handler handler1 = new Handler();
        handler1.postDelayed(() -> {
            finish();
            startActivity(new Intent(this, com.naic.nigerianarmy.Login.class).putExtra("extra", "enroll"));
        }, 4000); // 4000 milliseconds delay
    }


    @Override
    protected void onPause() {
        super.onPause();
    }
}

