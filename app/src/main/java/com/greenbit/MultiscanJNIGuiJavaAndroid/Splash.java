package com.greenbit.MultiscanJNIGuiJavaAndroid;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.view.WindowManager;
import android.view.animation.AlphaAnimation;
import android.view.animation.Animation;
import android.view.animation.LinearInterpolator;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.AppCompatImageView;
import androidx.transition.Transition;
import androidx.transition.TransitionValues;

import com.naic.nigerianarmy.utils.LfsJavaWrapperDefinesMinutiaN;
import com.naic.nigerianarmy.utils.Tools;

import org.apache.commons.lang3.SerializationUtils;


public class Splash extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);


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

        AppCompatImageView img = findViewById(R.id.img);


        final Animation animation = new AlphaAnimation(1, 0); // Change alpha from fully visible to invisible
        new Transition() {
            @Override
            public void captureEndValues(@NonNull TransitionValues transitionValues) {

            }

            @Override
            public void captureStartValues(@NonNull TransitionValues transitionValues) {

            }
        };
        animation.setDuration(3000); // duration - 3 seconds
        animation.setInterpolator(new LinearInterpolator()); // do not alter animation rate
        animation.setRepeatCount(3); // Repeat animation infinitely
        animation.setRepeatMode(Animation.REVERSE); // Reverse animation at the end so the button will fade back in
        img.startAnimation(animation);
        new Handler().postDelayed(new Runnable() {
            @Override
            public void run() {
                startActivity(new Intent(getApplicationContext(), WelcomeMenuActivity.class));
            }
        }, 3000);

    }

    //DONE
    public void hideSoftKeyboard() {
        getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_HIDDEN);
    }

    private long exitTime = 0;

    public void doExitApp() {
        if ((System.currentTimeMillis() - exitTime) > 2000) {
            Tools.toast("Press again to exit app", Splash.this);
            exitTime = System.currentTimeMillis();
        } else {
            finishAffinity();
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
