package com.greenbit.MultiscanJNIGuiJavaAndroid;

import android.content.Intent;
import android.os.Bundle;
import android.widget.GridLayout;

import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.GridLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.naic.nigerianarmy.services.MyService;
import com.naic.nigerianarmy.utils.Tools;

import java.util.ArrayList;
import java.util.List;

public class MenuVisitor extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        GridLayout mainGrid;
        com.naic.nigerianarmy.MenuData md;
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_menu);
        startService(new Intent(this, MyService.class));

        RecyclerView mRecyclerView = findViewById(R.id.recyclerview);
        GridLayoutManager mGridLayoutManager = new GridLayoutManager(MenuVisitor.this, 3);
        mRecyclerView.setLayoutManager(mGridLayoutManager);
        List<com.naic.nigerianarmy.MenuData> mFlowerList = new ArrayList<>();


        md = new com.naic.nigerianarmy.MenuData("Enroll Visitor",R.drawable.enroll);
        mFlowerList.add(md);

        md = new com.naic.nigerianarmy.MenuData("Verify",R.drawable.ver2);
        mFlowerList.add(md);
        md = new com.naic.nigerianarmy.MenuData("Profile",R.drawable.profile);
        mFlowerList.add(md);
        md = new com.naic.nigerianarmy.MenuData("About NAIC",R.drawable.globe);
        mFlowerList.add(md);
        md = new com.naic.nigerianarmy.MenuData("Home Menu",R.drawable.home_menu);
        mFlowerList.add(md);
        md = new com.naic.nigerianarmy.MenuData("Captured Data",R.drawable.view);
        mFlowerList.add(md);

        md = new com.naic.nigerianarmy.MenuData("Set Ip Address",R.drawable.settings);
        mFlowerList.add(md);
        md = new com.naic.nigerianarmy.MenuData("About GAMINT",R.drawable.gamint);
        mFlowerList.add(md);

         md = new com.naic.nigerianarmy.MenuData("Exit",R.drawable.logout);
         mFlowerList.add(md);


        com.naic.nigerianarmy.MenuAdapter myAdapter = new com.naic.nigerianarmy.MenuAdapter(MenuVisitor.this, mFlowerList);
        mRecyclerView.setAdapter(myAdapter);

    }


    @Override
    public void onBackPressed() {
        doExitApp();
    }

    private long exitTime = 0;

    public void doExitApp() {
        if ((System.currentTimeMillis() - exitTime) > 2000) {
            Tools.toast("Press again to exit app", MenuVisitor.this);
            exitTime = System.currentTimeMillis();
        } else {
            finishAffinity();
        }
    }
}