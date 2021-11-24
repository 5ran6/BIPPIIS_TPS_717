package com.greenbit.MultiscanJNIGuiJavaAndroid;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.util.Base64;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import com.naic.nigerianarmy.models.VerifiedUser;

public class VerifiedDetails extends AppCompatActivity {
    TextView name,phone,idnumber,email,age,height,weight,eye_colour,hair_colour,tatoo,gender,marital,
            blood,nok, nok_phone, hometown, nin, school_in, school_out,genotype,
            address,qualifications,dob,state,lga;
    ImageView image;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_verified_details);
        name = (TextView)findViewById(R.id.name);
        phone = (TextView)findViewById(R.id.phone);
        idnumber = (TextView)findViewById(R.id.idnumber);

        age = (TextView)findViewById(R.id.age);
        height = (TextView)findViewById(R.id.height);
        weight = (TextView)findViewById(R.id.weight);

        eye_colour = (TextView)findViewById(R.id.eye_colour);
        hair_colour = (TextView)findViewById(R.id.hair_colour);
        tatoo = (TextView)findViewById(R.id.tatoo);

        gender = (TextView)findViewById(R.id.gender);
        marital = (TextView)findViewById(R.id.marital);
        blood = (TextView)findViewById(R.id.blood);

        name = (TextView)findViewById(R.id.name);
        phone = (TextView)findViewById(R.id.phone);
        nok = (TextView)findViewById(R.id.nok);

        nok_phone = (TextView)findViewById(R.id.nok_phone);
        hometown = (TextView)findViewById(R.id.hometown);
        nin = (TextView)findViewById(R.id.nin);

        school_in= (TextView)findViewById(R.id.school_in);
        school_out = (TextView)findViewById(R.id.school_out);
        genotype = (TextView)findViewById(R.id.genotype);


        address= (TextView)findViewById(R.id.address);
        qualifications = (TextView)findViewById(R.id.qualifications);
        dob = (TextView)findViewById(R.id.dob);

        state = (TextView)findViewById(R.id.state);
        lga = (TextView)findViewById(R.id.lga);


        image=(ImageView)findViewById(R.id.enrollee);
        email=findViewById(R.id.email);
        VerifiedUser enrollee= com.naic.nigerianarmy.Login.verifiedUser;
        String imageBase64=enrollee.getPassport();
        imageBase64=imageBase64.replace("data:image/jpeg;base64,","");
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
        phone.setText("Phone: " + enrollee.getPhone());
        idnumber.setText("Id No: "+ enrollee.getIdentification_number());
        email.setText("Email: "+ enrollee.getEmail());

        name.setText("Name: " + enrollee.getName());
        age.setText("Age: "+ enrollee.getAge());
        weight.setText("Weight: "+ enrollee.getWeight());

        height.setText("Height: " + enrollee.getHeight());
        eye_colour.setText("Eye Colour: "+ enrollee.getEye_color());
        hair_colour.setText("Hair Colour: "+ enrollee.getHair_color());


        tatoo.setText("Tatoo ?: " + enrollee.getTattoo());
        gender.setText("Gender: "+ enrollee.getSex());
        marital.setText("Marital Status: "+ enrollee.getMarital_status());

        blood.setText("Blood Group: " + enrollee.getBlood_group());
        nok.setText("Nok: "+ enrollee.getNok_name());
        nok_phone.setText("Nok Phone: "+ enrollee.getNok_phone());


        hometown.setText("HomeTown: " + enrollee.getHometown());
        nin.setText("Nin: "+ enrollee.getNin());
        school_out.setText("School Out: "+ enrollee.getSec_sch_year_out());
        school_in.setText("School In: " + enrollee.getSec_sch_year_in());

        genotype.setText("Genotype: " + enrollee.getGenotype());
        address.setText("Address: "+ enrollee.getAddress());
        qualifications.setText("Qualifications: "+ enrollee.getQualification());

        state.setText("State: " + enrollee.getState_of_origin());
        lga.setText("Lga: "+ enrollee.getLga());
        dob.setText("Date Of Birth: "+ enrollee.getDate_of_birth());



    }
}