package com.greenbit.MultiscanJNIGuiJavaAndroid;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.util.Base64;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.Spinner;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import com.greenbit.MultiscanJNIGuiJavaAndroid.models.Enrollee;

import java.util.ArrayList;

public class BioDetails extends AppCompatActivity {
ImageView image;
TextView name,synched,passport_synched,phone;
Spinner left_hand_spinner,right_hand_spinner;
ArrayList<String> left_hand_vals,right_hand_vals;
ArrayAdapter<String> leftHandAdapter,rightHandAdapter;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_bio_details);
        Enrollee enrollee= ViewRecords.enrollee;
        name=findViewById(R.id.name);
        name.setText("Name: "+enrollee.name);
        synched=findViewById(R.id.synched);
        synched.setText("Biodata Synched?:" +enrollee.synched);
        phone=findViewById(R.id.phone);
        phone.setText("Phone: "+enrollee.phone);

        passport_synched=findViewById(R.id.passport_synched);
        passport_synched.setText("Passport Synched: "+enrollee.image_synched);

         String imageBase64=enrollee.image;
      // Toast.makeText(BioDetails.this,enrollee.left_thumb,Toast.LENGTH_SHORT).show();
        image=(ImageView)findViewById(R.id.enrollee);
        if(imageBase64==null)
        {
            image.setImageResource(R.drawable.placeholder);

        }
        else
        { byte[] decodedString = Base64.decode(imageBase64, Base64.DEFAULT);
            Bitmap decodedByte = BitmapFactory.decodeByteArray(decodedString, 0, decodedString.length);
            image.setImageBitmap(decodedByte);
            }

        left_hand_spinner=findViewById(R.id.left_hand);
        right_hand_spinner=findViewById(R.id.right_hand);
        left_hand_vals=enrollee.left_hand;
        right_hand_vals=enrollee.right_hand;
        leftHandAdapter = new ArrayAdapter<String>(this,
                android.R.layout.simple_spinner_item, android.R.id.text1);
        leftHandAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        left_hand_spinner.setAdapter(leftHandAdapter);
        left_hand_spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {

            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });
        leftHandAdapter.addAll(left_hand_vals);
        leftHandAdapter.notifyDataSetChanged();

        rightHandAdapter = new ArrayAdapter<String>(this,
                android.R.layout.simple_spinner_item, android.R.id.text1);
        rightHandAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        right_hand_spinner.setAdapter(rightHandAdapter);
        right_hand_spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {

            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });
        rightHandAdapter.addAll(right_hand_vals);
        rightHandAdapter.notifyDataSetChanged();




    }

}