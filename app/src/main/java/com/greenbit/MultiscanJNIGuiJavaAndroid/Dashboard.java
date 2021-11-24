package com.greenbit.MultiscanJNIGuiJavaAndroid;

import android.app.Dialog;
import android.content.Intent;
import android.content.SharedPreferences;
import android.net.Uri;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;
import androidx.multidex.BuildConfig;

import com.google.android.material.bottomsheet.BottomSheetDialog;
import com.google.android.material.textfield.TextInputEditText;
import com.greenbit.MultiscanJNIGuiJavaAndroid.utils.Tools;

import java.util.Objects;

public class Dashboard extends AppCompatActivity {
    private SharedPreferences sharedPref;
    private SharedPreferences.Editor editor;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        sharedPref =  PreferenceManager.getDefaultSharedPreferences(getApplicationContext());
        editor = sharedPref.edit();

        String fromSharedPref = sharedPref.getString("address", "nothing is here");
        Log.d("fingerprint", "FROM SHAREDPREF: " + fromSharedPref);
    }

    public void verify(View view) {
        startActivity(new Intent(this, Login.class).putExtra("extra", "verify"));
    }

    public void enroll(View view) {
        startActivity(new Intent(this, BioData.class).putExtra("extra", "enroll"));
    }

    public void dashboard(View view) {
        startActivity(new Intent(this, Login.class).putExtra("extra", "profile"));
    }

    public void bippiis_web(View view) {
        Intent i = new Intent(Intent.ACTION_VIEW);
        i.setData(Uri.parse("https://naic.gov.ng"));
        startActivity(i);
    }

    public void about(View view) {
        showDialogAbout();
    }

    public void exitApp(View view) {
        doExitApp();
    }

    @Override
    public void onBackPressed() {
        doExitApp();
    }

    private long exitTime = 0;

    public void doExitApp() {
        if ((System.currentTimeMillis() - exitTime) > 2000) {
            Tools.toast("Press again to exit app", Dashboard.this);
            exitTime = System.currentTimeMillis();
        } else {
            finishAffinity();
        }
    }

    private void showDialogAbout() {
        final Dialog dialog = new Dialog(this);
        dialog.requestWindowFeature(Window.FEATURE_NO_TITLE); // before
        dialog.setContentView(R.layout.dialog_about);
        dialog.setCancelable(true);

        WindowManager.LayoutParams lp = new WindowManager.LayoutParams();
        lp.copyFrom(dialog.getWindow().getAttributes());
        lp.width = WindowManager.LayoutParams.WRAP_CONTENT;
        lp.height = WindowManager.LayoutParams.WRAP_CONTENT;

        ((TextView) dialog.findViewById(R.id.tv_version)).setText("Version " + BuildConfig.VERSION_NAME);

        dialog.findViewById(R.id.bt_getcode).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent i = new Intent(Intent.ACTION_VIEW);
                i.setData(Uri.parse("https://army.mil.ng"));
                startActivity(i);
            }
        });

        dialog.findViewById(R.id.bt_close).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                dialog.dismiss();
            }
        });

        dialog.show();
        dialog.getWindow().setAttributes(lp);
    }

    public void setIpAddress(View view) {
        //create a bottom sheet
        /**
         * showing bottom sheet dialog fragment
         * same layout is used in both dialog and dialog fragment
         */

        BottomSheetDialog bottomSheetDialog = new BottomSheetDialog(Dashboard.this);
        View bottomSheet = LayoutInflater.from(Dashboard.this).inflate(R.layout.bottom_sheet, findViewById(R.id.parent));

        TextInputEditText ip = bottomSheet.findViewById(R.id.ip);

        bottomSheet.findViewById(R.id.okay).setOnClickListener(view1 -> {
            if (!Objects.requireNonNull(ip.getText()).toString().isEmpty()) {
                String toSharedPref = Objects.requireNonNull(ip.getText()).toString().trim();
                //<string name="base_url">http://192.168.0.106:8000/api/</string>

                if (toSharedPref.contains(":") || toSharedPref.contains("..")) {
                    Toast.makeText(Dashboard.this, "Please remove the PORT number", Toast.LENGTH_SHORT).show();
                } else {
                    String value = "http://" + toSharedPref + ":8000/api/";
                    editor.putString("address", value);
                    editor.apply();
                    Toast.makeText(Dashboard.this, "Done!", Toast.LENGTH_SHORT).show();
                    bottomSheetDialog.dismiss();
                }


            } else {
                Toast.makeText(Dashboard.this, "If left empty, the previous address will be used", Toast.LENGTH_SHORT).show();

            }

        });

        bottomSheetDialog.setContentView(bottomSheet);
        bottomSheetDialog.show();

        //refactor pages to listen from shared pref
    }
}