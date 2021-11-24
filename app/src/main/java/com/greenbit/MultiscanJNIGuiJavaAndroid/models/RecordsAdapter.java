package com.greenbit.MultiscanJNIGuiJavaAndroid.models;

import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Typeface;
import android.util.Base64;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.cardview.widget.CardView;
import androidx.recyclerview.widget.RecyclerView;

import com.greenbit.MultiscanJNIGuiJavaAndroid.BioDetails;
import com.greenbit.MultiscanJNIGuiJavaAndroid.R;
import com.greenbit.MultiscanJNIGuiJavaAndroid.ViewRecords;

import java.util.Calendar;
import java.util.List;

/**
 * Created by android on 8/3/17.
 */

public class RecordsAdapter extends RecyclerView.Adapter<RecordsAdapter.Holder> {
    List<Enrollee> list;

    public RecordsAdapter(List<Enrollee> list) {
        this.list = list;
    }

    @Override
    public Holder onCreateViewHolder(ViewGroup parent, int viewType) {
        return new Holder(LayoutInflater.from(parent.getContext()).inflate(R.layout.registration_item, parent, false));
    }

    @Override
    public void onBindViewHolder(final Holder holder, int position) {


        holder.name.setText("Name: "+list.get(holder.getAdapterPosition()).name);
        holder.phone.setText("Phone: "+list.get(holder.getAdapterPosition()).phone);
        holder.synched.setText("Synched to Server?: "+list.get(holder.getAdapterPosition()).synched);
        holder.reg_date.setText("Registration Date: "+list.get(holder.getAdapterPosition()).registered_date);
        holder.reg_number.setText("Identification Number: "+list.get(holder.getAdapterPosition()).reg_number);
        String image=list.get(holder.getAdapterPosition()).image;
          if(image==null)
          {
              holder.image.setImageResource(R.drawable.placeholder);

          }
          else
          { byte[] decodedString = Base64.decode(image, Base64.DEFAULT);
              Bitmap decodedByte = BitmapFactory.decodeByteArray(decodedString, 0, decodedString.length);
              holder.image.setImageBitmap(decodedByte);}


        holder.itemView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ViewRecords.enrollee=list.get(holder.getAdapterPosition());
                Intent o=new Intent(v.getContext(), BioDetails.class);
                v.getContext().startActivity(o);
                        }
        });
        BookFont(holder, holder.phone);
        BookFont(holder, holder.synched);
        BookFont(holder, holder.reg_number);
        BookFont(holder, holder.reg_date);
        MediumFont(holder, holder.name);



    }

    @Override
    public int getItemCount() {
        return list.size();
    }

    public class Holder extends RecyclerView.ViewHolder {

        TextView name,phone,synched,reg_number,reg_date;
        ImageView image;
        CardView mCardView;
       // TextView f, t, dn, dt;

        public Holder(View itemView) {
            super(itemView);
            mCardView = (CardView) itemView.findViewById(R.id.cardView);
            name = (TextView) itemView.findViewById(R.id.name);
            phone = (TextView) itemView.findViewById(R.id.phone);
            synched = (TextView) itemView.findViewById(R.id.synched);
            reg_number = (TextView) itemView.findViewById(R.id.reg_number);
            reg_date = (TextView) itemView.findViewById(R.id.reg_date);
            image=(ImageView)itemView.findViewById(R.id.enrollee);

        }
    }

    public static String dateSuffix(final Calendar cal) {
        final int date = cal.get(Calendar.DATE);
        switch (date % 10) {
            case 1:
                if (date != 11) {
                    return "st";
                }
                break;

            case 2:
                if (date != 12) {
                    return "nd";
                }
                break;

            case 3:
                if (date != 13) {
                    return "rd";
                }
                break;
        }
        return "th";
    }

    public String getDateSuffix(int day) {
        switch (day) {
            case 1:
            case 21:
            case 31:
                return ("st");

            case 2:
            case 22:
                return ("nd");

            case 3:
            case 23:
                return ("rd");

            default:
                return ("th");
        }
    }


    public void BookFont(Holder holder, TextView view1) {
        Typeface font1 = Typeface.createFromAsset(holder.itemView.getContext().getAssets(), "font/AvenirLTStd_Book.otf");
        view1.setTypeface(font1);
    }

    public void MediumFont(Holder holder, TextView view) {
        Typeface font = Typeface.createFromAsset(holder.itemView.getContext().getAssets(), "font/AvenirLTStd_Medium.otf");
        view.setTypeface(font);
    }

}
