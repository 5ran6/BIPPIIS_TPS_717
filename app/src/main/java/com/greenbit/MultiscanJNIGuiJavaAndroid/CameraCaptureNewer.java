package com.greenbit.MultiscanJNIGuiJavaAndroid;

import android.content.Intent;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.provider.MediaStore;
import android.util.Base64;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import com.greenbit.MultiscanJNIGuiJavaAndroid.utils.DatabaseAccess;

import java.io.ByteArrayOutputStream;

public class CameraCaptureNewer extends AppCompatActivity implements View.OnClickListener  {
    Button expiry,next,pic,submit;
    private final int requestCode = 20;
    String img;
    ImageView picmg;
    DatabaseAccess databaseAccess;
    private String phone= "";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_camera_capture_newer);
        pic=(Button)findViewById(R.id.pic);
        pic.setOnClickListener(this);
        databaseAccess = DatabaseAccess.getInstance(getApplicationContext());
        databaseAccess.open();
        submit=(Button)findViewById(R.id.submit);
        submit.setOnClickListener(this);
        picmg=(ImageView)findViewById(R.id.ben);
        phone= getIntent().getStringExtra("phone");
       // phone="bbb";


    }

    @Override
    public void onClick(View view) {
        if(view==pic)
        {
            Intent photoCaptureIntent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
           // photoCaptureIntent.putExtra(MediaStore.EXTRA_SCREEN_ORIENTATION, ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
            startActivityForResult(photoCaptureIntent, requestCode);
        }
        if(view==submit)
        {


            String response=databaseAccess.insertImage(phone,img);

            Toast.makeText(CameraCaptureNewer.this,response,Toast.LENGTH_SHORT).show();
           if(response.contains("Successfully")) {
               Intent intent = new Intent(CameraCaptureNewer.this, MenuOfficers.class);
               intent.putExtra("extra", "enroll");
               intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TASK);
               startActivity(intent);
           }


        }
    }


    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if(this.requestCode == requestCode && resultCode == RESULT_OK){
            Bitmap bitmap = (Bitmap)data.getExtras().get("data");
            picmg.setImageBitmap(bitmap);
            img=convert(bitmap);
        }
    }
    public static String convert(Bitmap bitmap)
    {
        ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
        bitmap.compress(Bitmap.CompressFormat.JPEG, 100, outputStream);

        return Base64.encodeToString(outputStream.toByteArray(), Base64.DEFAULT);
    }

}