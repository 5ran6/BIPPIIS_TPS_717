package com.greenbit.MultiscanJNIGuiJavaAndroid;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.net.Uri;
import android.preference.PreferenceManager;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import androidx.cardview.widget.CardView;
import androidx.recyclerview.widget.RecyclerView;

import com.google.android.material.bottomsheet.BottomSheetDialog;
import com.google.android.material.textfield.TextInputEditText;

import java.util.List;
import java.util.Objects;

public class MenuAdapter extends RecyclerView.Adapter<MenuHolder> {

    private Context mContext;
    private List<MenuData> mFlowerList;

    MenuAdapter(Context mContext, List<MenuData> mFlowerList) {
        this.mContext = mContext;
        this.mFlowerList = mFlowerList;
    }

    @Override
    public MenuHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View mView = LayoutInflater.from(parent.getContext()).inflate(R.layout.recyclerview_item_row, parent, false);
        return new MenuHolder(mView);
    }

    @Override
    public void onBindViewHolder(MenuHolder holder, @SuppressLint("RecyclerView") final int position) {
        holder.mImage.setImageResource(mFlowerList.get(position).menuPic);
        holder.mTitle.setText(mFlowerList.get(position).menuName);
        holder.mCardView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String name = mFlowerList.get(position).menuName;
                if (name.equalsIgnoreCase("Enroll Visitor")) {
                    Intent mIntent = new Intent(mContext, BioData.class);
                    mIntent.putExtra("extra", "visitor");
                    mContext.startActivity(mIntent);
                }

                if (name.equalsIgnoreCase("Enroll Personnel")) {
                    Intent mIntent = new Intent(mContext, BioData.class);
                    mIntent.putExtra("extra", "personnel");
                    mContext.startActivity(mIntent);
                }

                if (name.equalsIgnoreCase("Verify")) {
                    Intent mIntent = new Intent(mContext, Login.class);
                    mIntent.putExtra("extra", "verify");
                    mContext.startActivity(mIntent);
                }

                if (name.equalsIgnoreCase("Home Menu")) {
                    Intent mIntent = new Intent(mContext, WelcomeMenuActivity.class);
                    mIntent.putExtra("extra", "menu");
                    mContext.startActivity(mIntent);
                }

                if (name.equalsIgnoreCase("About NAIC")) {
                    mContext.startActivity(new Intent(Intent.ACTION_VIEW,
                            Uri.parse("https://army.mil.ng/")));
                }

                if (name.equalsIgnoreCase("About GAMINT")) {
                    mContext.startActivity(new Intent(Intent.ACTION_VIEW,
                            Uri.parse("https://www.gamintcorporateltd.com/")));
                }

                if (name.equalsIgnoreCase("Set Ip Address")) {
                    setIpAddress();
                }

                if (name.equalsIgnoreCase("Captured Data")) {
                    Intent mIntent = new Intent(mContext, ViewRecords.class);
                    mContext.startActivity(mIntent);

                }


            }
        });

    }

    @Override
    public int getItemCount() {
        return mFlowerList.size();
    }


    public void setIpAddress() {
        SharedPreferences sharedPref;
        SharedPreferences.Editor editor;
        sharedPref = PreferenceManager.getDefaultSharedPreferences(mContext);
        editor = sharedPref.edit();

        String fromSharedPref = sharedPref.getString("address", "Nothing is configured yet");
        //create a bottom sheet
        /**
         * showing bottom sheet dialog fragment
         * same layout is used in both dialog and dialog fragment
         */

        BottomSheetDialog bottomSheetDialog = new BottomSheetDialog(mContext);
        View bottomSheet = LayoutInflater.from(mContext).inflate(R.layout.bottom_sheet, null);

        TextInputEditText ip = bottomSheet.findViewById(R.id.ip);
        // ip.setText(fromSharedPref);
        ip.setHint("Addr: " + fromSharedPref);

        bottomSheet.findViewById(R.id.okay).setOnClickListener(view1 -> {
            if (!Objects.requireNonNull(ip.getText()).toString().isEmpty()) {
                String toSharedPref = Objects.requireNonNull(ip.getText()).toString().trim();
                //<string name="base_url">http://192.168.0.106:8000/api/</string>

                //   if (toSharedPref.contains(":") || toSharedPref.contains("..")) {
                // Toast.makeText(mContext, "Please remove the PORT number", Toast.LENGTH_SHORT).show();
                //  }
                // else {
                String value = toSharedPref + "/api/";
                editor.putString("address", value);
                editor.apply();
                Toast.makeText(mContext, "Done!", Toast.LENGTH_SHORT).show();
                bottomSheetDialog.dismiss();
                // }


            } else {
                Toast.makeText(mContext, "If left empty, the previous address will be used", Toast.LENGTH_SHORT).show();

            }

        });

        bottomSheetDialog.setContentView(bottomSheet);
        bottomSheetDialog.show();

        //refactor pages to listen from shared pref
    }

}

class MenuHolder extends RecyclerView.ViewHolder {


    ImageView mImage;
    TextView mTitle;
    CardView mCardView;

    MenuHolder(View itemView) {
        super(itemView);
        mImage = itemView.findViewById(R.id.ivImage);
        mTitle = itemView.findViewById(R.id.tvTitle);
        mCardView = itemView.findViewById(R.id.cardview);
    }


}

