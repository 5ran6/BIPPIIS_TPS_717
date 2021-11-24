package com.greenbit.MultiscanJNIGuiJavaAndroid;

import android.app.DatePickerDialog;
import android.os.Bundle;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.greenbit.MultiscanJNIGuiJavaAndroid.models.Enrollee;
import com.greenbit.MultiscanJNIGuiJavaAndroid.models.RecordsAdapter;
import com.greenbit.MultiscanJNIGuiJavaAndroid.utils.DatabaseAccess;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.List;

public class ViewRecords extends AppCompatActivity {
    private List<Enrollee> enrolleeList = new ArrayList<Enrollee>();
    private ListView listView;
    Button date1,sheets,reset;
    TextView dateval;
    public static String ball,tsum,tcount,salesv,agname,psp;
    TextView txt_error,sales,paysynched;
    RecyclerView recyclerView;
    private DatePickerDialog fromDatePickerDialog;
    private SimpleDateFormat dateFormatter;
    private RecordsAdapter adapter;
    public static Enrollee enrollee;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
         enrollee=new Enrollee();
        setContentView(R.layout.activity_view_records);
        recyclerView = (RecyclerView)findViewById(R.id.recyclerview);
        LinearLayoutManager linearLayoutManager = new LinearLayoutManager(getApplicationContext(), LinearLayoutManager.VERTICAL, false);
        recyclerView.setLayoutManager(linearLayoutManager);

        final DatabaseAccess db = DatabaseAccess.getInstance(getApplicationContext());
        db.open();
        enrolleeList=db.getEnrollees();
        if (!enrolleeList.isEmpty()) {

            //String image="";
           // image=enrolleeList.get(1).image;
            //Toast.makeText(ViewRecords.this,image+enrolleeList.get(0).name,Toast.LENGTH_SHORT).show();
            adapter = new RecordsAdapter(enrolleeList);
            recyclerView.setAdapter(adapter);
            adapter.notifyDataSetChanged();
        }
        else
        {
            Toast.makeText(ViewRecords.this,"No records found", Toast.LENGTH_SHORT).show();
        }


    }
}