package com.greenbit.MultiscanJNIGuiJavaAndroid;

import android.content.Intent;
import android.os.Bundle;
import android.view.Gravity;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.TranslateAnimation;
import android.widget.LinearLayout;

import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.AppCompatImageView;
import androidx.cardview.widget.CardView;

import com.greenbit.MultiscanJNIGuiJavaAndroid.utils.Tools;

public class WelcomeMenuActivity extends AppCompatActivity {

    CardView officer, visitor;
    AppCompatImageView rl_footer;
    LinearLayout linearLayout;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_welcome_menu);
        linearLayout = findViewById(R.id.ll_main);
        rl_footer = findViewById(R.id.rl_footer);
        officer = findViewById(R.id.officer);
        visitor = findViewById(R.id.visitor);

        officer.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(getApplicationContext(), MenuOfficers.class);
                intent.putExtra("flag", "officers");
                startActivity(intent);
            }
        });
        visitor.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(getApplicationContext(), MenuVisitor.class);
                intent.putExtra("flag", "visitors");
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

        slide.setAnimationListener(new Animation.AnimationListener() {

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

        slide.setAnimationListener(new Animation.AnimationListener() {

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
            Tools.toast("Press again to exit app", WelcomeMenuActivity.this);
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