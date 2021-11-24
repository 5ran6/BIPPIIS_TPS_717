package com.greenbit.MultiscanJNIGuiJavaAndroid;

import android.content.Intent;
import android.os.Bundle;
import android.widget.GridLayout;

import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.GridLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.greenbit.MultiscanJNIGuiJavaAndroid.services.MyService;
import com.greenbit.MultiscanJNIGuiJavaAndroid.utils.Tools;

import java.util.ArrayList;
import java.util.List;

public class MenuOfficers extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        GridLayout mainGrid;
        MenuData md;
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_menu);
        startService(new Intent(this, MyService.class));

        RecyclerView mRecyclerView = findViewById(R.id.recyclerview);
        GridLayoutManager mGridLayoutManager = new GridLayoutManager(MenuOfficers.this, 3);
        mRecyclerView.setLayoutManager(mGridLayoutManager);
        List<MenuData> mFlowerList = new ArrayList<>();


        md = new MenuData("Enroll Personnel",R.drawable.add_army);
        mFlowerList.add(md);

        md = new MenuData("Verify",R.drawable.ver2);
        mFlowerList.add(md);
        md = new MenuData("Profile",R.drawable.profile);
        mFlowerList.add(md);
        md = new MenuData("About NAIC",R.drawable.globe);
        mFlowerList.add(md);
        md = new MenuData("Home Menu",R.drawable.home_menu);
        mFlowerList.add(md);
        md = new MenuData("Captured Data",R.drawable.view);
        mFlowerList.add(md);

        md = new MenuData("Set Ip Address",R.drawable.settings);
        mFlowerList.add(md);
        md = new MenuData("About GAMINT",R.drawable.gamint);
        mFlowerList.add(md);

         md = new MenuData("Exit",R.drawable.logout);
         mFlowerList.add(md);


        com.naic.nigerianarmy.MenuAdapter myAdapter = new com.naic.nigerianarmy.MenuAdapter(MenuOfficers.this, mFlowerList);
        mRecyclerView.setAdapter(myAdapter);

    }


    @Override
    public void onBackPressed() {
        doExitApp();
    }

    private long exitTime = 0;

    public void doExitApp() {
        if ((System.currentTimeMillis() - exitTime) > 2000) {
            Tools.toast("Press again to exit app", MenuOfficers.this);
            exitTime = System.currentTimeMillis();
        } else {
            finishAffinity();
        }
    }
}