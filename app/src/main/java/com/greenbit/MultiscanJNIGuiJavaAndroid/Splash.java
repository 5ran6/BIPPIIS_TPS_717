package com.greenbit.MultiscanJNIGuiJavaAndroid;

import android.os.Bundle;
import android.app.Activity;
import android.content.Intent;
import android.view.Gravity;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.animation.Animation;
import android.view.animation.Animation.AnimationListener;
import android.view.animation.AnimationUtils;
import android.view.animation.TranslateAnimation;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;

import androidx.appcompat.widget.AppCompatImageView;
import androidx.cardview.widget.CardView;

import com.greenbit.MultiscanJNIGuiJavaAndroid.utils.Tools;

public class Splash extends Activity {

    CardView enroll, verify;
    AppCompatImageView rl_footer;
    LinearLayout linearLayout;
    boolean isBottom = true;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_splash);
        linearLayout = findViewById(R.id.ll_main);
        rl_footer = findViewById(R.id.rl_footer);
        enroll = findViewById(R.id.enroll);
        verify = findViewById(R.id.verify);

enroll.setOnClickListener(new OnClickListener() {
    @Override
    public void onClick(View view) {
        Intent intent = new Intent(getApplicationContext(), Login.class);
        intent.putExtra("flag", "enroll");
        startActivity(intent);
    }
});
verify.setOnClickListener(new OnClickListener() {
    @Override
    public void onClick(View view) {
        Intent intent = new Intent(getApplicationContext(), Login.class);
        intent.putExtra("flag", "verify");
        startActivity(intent);
    }
});

    }

    public void SlideToAbove() {
        Animation slide = null;
        slide = new TranslateAnimation(Animation.RELATIVE_TO_SELF, 0.0f,
                Animation.RELATIVE_TO_SELF, 0.0f, Animation.RELATIVE_TO_SELF,
                0.0f, Animation.RELATIVE_TO_SELF, -5.0f);

        slide.setDuration(400);
        slide.setFillAfter(true);
        slide.setFillEnabled(true);
        rl_footer.startAnimation(slide);

        slide.setAnimationListener(new AnimationListener() {

            @Override
            public void onAnimationStart(Animation animation) {

            }

            @Override
            public void onAnimationRepeat(Animation animation) {
            }

            @Override
            public void onAnimationEnd(Animation animation) {

                rl_footer.clearAnimation();

                LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(
                        rl_footer.getWidth(), rl_footer.getHeight());
                LinearLayout.LayoutParams ll = new LinearLayout.LayoutParams(
                        linearLayout.getWidth(), linearLayout.getHeight());
                // lp.setMargins(0, 0, 0, 0);
                ll.gravity = Gravity.CENTER_HORIZONTAL;
                lp.gravity = Gravity.CENTER_HORIZONTAL;

                rl_footer.setLayoutParams(lp);
                linearLayout.setLayoutParams(ll);

            }

        });

    }

    public void SlideToDown() {
        Animation slide = null;
        slide = new TranslateAnimation(Animation.RELATIVE_TO_SELF, 0.0f,
                Animation.RELATIVE_TO_SELF, 0.0f, Animation.RELATIVE_TO_SELF,
                0.0f, Animation.RELATIVE_TO_SELF, 5.2f);

        slide.setDuration(400);
        slide.setFillAfter(true);
        slide.setFillEnabled(true);
        rl_footer.startAnimation(slide);

        slide.setAnimationListener(new AnimationListener() {

            @Override
            public void onAnimationStart(Animation animation) {

            }

            @Override
            public void onAnimationRepeat(Animation animation) {
            }

            @Override
            public void onAnimationEnd(Animation animation) {

                rl_footer.clearAnimation();

                LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(
                        rl_footer.getWidth(), rl_footer.getHeight());
                lp.setMargins(0, rl_footer.getWidth(), 0, 0);
                rl_footer.setLayoutParams(lp);

            }

        });

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

}
