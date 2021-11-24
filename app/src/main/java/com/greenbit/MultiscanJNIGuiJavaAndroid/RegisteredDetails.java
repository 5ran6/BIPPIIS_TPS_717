package com.greenbit.MultiscanJNIGuiJavaAndroid;

import android.os.Bundle;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

public class RegisteredDetails extends AppCompatActivity {
    TextView name,phone,reg_number,reg_date;
    ImageView image;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_registered_details);
        name = (TextView)findViewById(R.id.name);
        phone = (TextView)findViewById(R.id.phone);
        reg_number = (TextView)findViewById(R.id.reg_number);
        reg_date = (TextView)findViewById(R.id.reg_date);
        image=(ImageView)findViewById(R.id.enrollee);

    }
}