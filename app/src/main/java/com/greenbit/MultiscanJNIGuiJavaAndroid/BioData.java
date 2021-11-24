package com.greenbit.MultiscanJNIGuiJavaAndroid;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import com.google.android.material.textfield.TextInputEditText;
import com.greenbit.MultiscanJNIGuiJavaAndroid.models.UserRegisterRequest;
import com.greenbit.MultiscanJNIGuiJavaAndroid.utils.DatabaseAccess;
import com.greenbit.MultiscanJNIGuiJavaAndroid.utils.MyEditTextDatePicker;

import java.util.ArrayList;

import in.galaxyofandroid.spinerdialog.OnSpinerItemClick;
import in.galaxyofandroid.spinerdialog.SpinnerDialog;

public class BioData extends AppCompatActivity {

    private SharedPreferences sharedPref;
    private SharedPreferences.Editor editor;
    private String fromSharedPref = "";
    DatabaseAccess databaseAccess;
    ProgressBar progressBar;
    ImageView passport;
    TextView state_origin, lga;
    Button choose_state, choose_lga;
    String marital_status_val, role_val, genotype_val, blood_group_val, gender_val, tatoo_val, hair_color_val, eye_color_val = "";
    ArrayList<String> states, lgas, marital_status_vals, genotype_vals, role_vals, blood_group_vals, gender_vals, hair_vals, tatoo_vals, eye_color_vals;
    Spinner blood_group_spinner, role_spinner, marital_status_spinner, genotype_spinner, gender_spinner, hair_spinner, tatoo_spinner, eye_spinner;
    TextInputEditText fullname, email, age, phone, height, weight,
            blood_group,
            nok, nok_phone, hometown, nin, school_in, school_out,
            address, qualifications, date_of_birth;
    String extra = "";
    ArrayAdapter<String> bloodGroupAdapter, roleAdapter, maritalStatusAdapter, genotypeAdapter, genderAdapter, hairAdapter, tatooAdapter, eyeAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        extra = getIntent().getStringExtra("extra");
        if (extra.equalsIgnoreCase("visitor"))
            setContentView(R.layout.activity_bio_data_visitor);
        else
            setContentView(R.layout.activity_bio_data);
        initializer();

    }

    private void initializer() {

        //instantiate sqlite helper
        databaseAccess = DatabaseAccess.getInstance(getApplicationContext());
        databaseAccess.open();
        states = databaseAccess.getStates();
        lgas = new ArrayList<>();
        sharedPref = PreferenceManager.getDefaultSharedPreferences(getApplicationContext());
        editor = sharedPref.edit();
        fromSharedPref = sharedPref.getString("address", "nothing is here");
        Log.d("fingerprint", "FROM shared " + fromSharedPref);

        marital_status_vals = new ArrayList<>();
        marital_status_vals.add("--Choose Marital Status--");
        marital_status_vals.add("Single");
        marital_status_vals.add("Married");
        marital_status_vals.add("Divorced");

        hair_vals = new ArrayList<>();
        hair_vals.add("--Choose Hair Color--");
        hair_vals.add("Black");
        hair_vals.add("Brown");


        tatoo_vals = new ArrayList<>();
        tatoo_vals.add("--Are Tatoos Present?--");
        tatoo_vals.add("No");
        tatoo_vals.add("Yes");

        eye_color_vals = new ArrayList<>();
        eye_color_vals.add("--Eye Color--");
        eye_color_vals.add("Black");
        eye_color_vals.add("Brown");


        genotype_vals = new ArrayList<>();
        genotype_vals.add("--Choose Genotype--");
        genotype_vals.add("AA");
        genotype_vals.add("AS");
        genotype_vals.add("AC");
        genotype_vals.add("CC");
        genotype_vals.add("SC");
        genotype_vals.add("SS");

        role_vals = new ArrayList<>();
        role_vals.add("--Choose your Category--");
        role_vals.add("Army");
        role_vals.add("Navy");
        role_vals.add("Air Force");
        if (extra.equalsIgnoreCase("visitor")) role_vals.add("Visitor");


        blood_group_vals = new ArrayList<>();
        blood_group_vals.add("--Choose Bloodgroup--");
        blood_group_vals.add("A+");
        blood_group_vals.add("A-");
        blood_group_vals.add("AB+");
        blood_group_vals.add("AB-");
        blood_group_vals.add("B+");
        blood_group_vals.add("B-");
        blood_group_vals.add("O+");
        blood_group_vals.add("O-");

        gender_vals = new ArrayList<>();
        gender_vals.add("--Choose Gender--");
        gender_vals.add("Female");
        gender_vals.add("Male");


        address = findViewById(R.id.address);
        qualifications = findViewById(R.id.qualifications);


        date_of_birth = findViewById(R.id.dob);
        MyEditTextDatePicker ed2 = new MyEditTextDatePicker(BioData.this, R.id.dob);
        passport = findViewById(R.id.passport);
        progressBar = findViewById(R.id.progress);
        fullname = findViewById(R.id.fullname);
        email = findViewById(R.id.email);
        age = findViewById(R.id.age);
        phone = findViewById(R.id.phone);
        height = findViewById(R.id.height);
        weight = findViewById(R.id.weight);
        eye_spinner = findViewById(R.id.eye_colour);
        hair_spinner = findViewById(R.id.hair_colour);
        tatoo_spinner = findViewById(R.id.tatoo);
        genotype_spinner = findViewById(R.id.genotype);
        gender_spinner = findViewById(R.id.gender);
        marital_status_spinner = findViewById(R.id.marital);
        role_spinner = findViewById(R.id.role);
        blood_group_spinner = findViewById(R.id.blood);

        eyeAdapter = new ArrayAdapter<String>(this,
                android.R.layout.simple_spinner_item, android.R.id.text1);
        eyeAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        eye_spinner.setAdapter(eyeAdapter);
        eye_spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                eye_color_val = eye_color_vals.get(position);
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });
        eyeAdapter.addAll(eye_color_vals);
        eyeAdapter.notifyDataSetChanged();
        hairAdapter = new ArrayAdapter<String>(this,
                android.R.layout.simple_spinner_item, android.R.id.text1);
        hairAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        hair_spinner.setAdapter(hairAdapter);
        hair_spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                hair_color_val = hair_vals.get(position);
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });
        hairAdapter.addAll(hair_vals);
        hairAdapter.notifyDataSetChanged();

        //////////////////////
        tatooAdapter = new ArrayAdapter<String>(this,
                android.R.layout.simple_spinner_item, android.R.id.text1);
        tatooAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        tatoo_spinner.setAdapter(tatooAdapter);
        tatoo_spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                tatoo_val = tatoo_vals.get(position);
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });
        tatooAdapter.addAll(tatoo_vals);
        tatooAdapter.notifyDataSetChanged();

        bloodGroupAdapter = new ArrayAdapter<String>(this,
                android.R.layout.simple_spinner_item, android.R.id.text1);
        bloodGroupAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        blood_group_spinner.setAdapter(bloodGroupAdapter);
        blood_group_spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                blood_group_val = blood_group_vals.get(position);
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });
        bloodGroupAdapter.addAll(blood_group_vals);
        bloodGroupAdapter.notifyDataSetChanged();

        genderAdapter = new ArrayAdapter<String>(this,
                android.R.layout.simple_spinner_item, android.R.id.text1);
        genderAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        gender_spinner.setAdapter(genderAdapter);
        gender_spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                gender_val = gender_vals.get(position);
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });
        genderAdapter.addAll(gender_vals);
        genderAdapter.notifyDataSetChanged();

        genotypeAdapter = new ArrayAdapter<String>(this,
                android.R.layout.simple_spinner_item, android.R.id.text1);
        genotypeAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        genotype_spinner.setAdapter(genotypeAdapter);
        genotype_spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                genotype_val = genotype_vals.get(position);
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });
        genotypeAdapter.addAll(genotype_vals);
        genotypeAdapter.notifyDataSetChanged();


        roleAdapter = new ArrayAdapter<String>(this,
                android.R.layout.simple_spinner_item, android.R.id.text1);
        roleAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        role_spinner.setAdapter(roleAdapter);
        role_spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                role_val = role_vals.get(position);
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });
        roleAdapter.addAll(role_vals);
        roleAdapter.notifyDataSetChanged();


        maritalStatusAdapter = new ArrayAdapter<String>(this,
                android.R.layout.simple_spinner_item, android.R.id.text1);
        maritalStatusAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        marital_status_spinner.setAdapter(maritalStatusAdapter);
        marital_status_spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                marital_status_val = marital_status_vals.get(position);
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });
        maritalStatusAdapter.addAll(marital_status_vals);
        maritalStatusAdapter.notifyDataSetChanged();


        nok = findViewById(R.id.nok);
        nok_phone = findViewById(R.id.nok_phone);
        // state_origin = findViewById(R.id.state_of_origin);
        state_origin = findViewById(R.id.state_of_origin);
        lga = findViewById(R.id.lga);
        hometown = findViewById(R.id.hometown);
        nin = findViewById(R.id.nin);
        school_in = findViewById(R.id.school_in);
        school_out = findViewById(R.id.school_out);

        assert extra != null;
        if (extra.equalsIgnoreCase("profile")) {
            //getIntents and set to views
            fullname.setText(getIntent().getStringExtra("fullname"));
            fullname.setEnabled(false);

            age.setText(getIntent().getStringExtra("age"));
            age.setEnabled(false);

            height.setText(getIntent().getStringExtra("height"));
            height.setEnabled(false);

            weight.setText(getIntent().getStringExtra("weight"));
            weight.setEnabled(false);

            email.setText(getIntent().getStringExtra("email"));
            email.setEnabled(false);

            //  eye_color.setText(getIntent().getStringExtra("eye_color"));
            // eye_color.setEnabled(false);

            //   hair_color.setText(getIntent().getStringExtra("hair_color"));
            // hair_color.setEnabled(false);


            //  genotype.setText(getIntent().getStringExtra("genotype"));
            //  genotype.setEnabled(false);

            // gender.setText(getIntent().getStringExtra("sex"));
            // gender.setEnabled(false);

            phone.setText(getIntent().getStringExtra("phone"));
            phone.setEnabled(false);

            // marital_status.setText(getIntent().getStringExtra("marital_status"));
            //  marital_status.setEnabled(false);

            blood_group.setText(getIntent().getStringExtra("blood_group"));
            blood_group.setEnabled(false);

            state_origin.setText(getIntent().getStringExtra("state_of_origin"));
            state_origin.setEnabled(false);


            lga.setText(getIntent().getStringExtra("lga"));
            lga.setEnabled(false);


            hometown.setText(getIntent().getStringExtra("hometown"));
            hometown.setEnabled(false);


            nin.setText(getIntent().getStringExtra("nin"));
            nin.setEnabled(false);


            school_in.setText(getIntent().getStringExtra("sec_sch_year_in"));
            school_in.setEnabled(false);


            school_out.setText(getIntent().getStringExtra("sec_sch_year_out"));
            school_out.setEnabled(false);


            nok.setText(getIntent().getStringExtra("nok_name"));
            nok.setEnabled(false);


            nok_phone.setText(getIntent().getStringExtra("nok_phone"));
            nok_phone.setEnabled(false);

        }


        choose_state = findViewById(R.id.choose_state);
        //launch predictive drop down
        choose_state.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                cleanStateText();
                SpinnerDialog sd = new SpinnerDialog(BioData.this, states, "Get State");
                sd.bindOnSpinerListener(new OnSpinerItemClick() {
                                            @Override
                                            public void onClick(String s, int position) {

                                                state_origin.setText(s);
                                                getLga(s);
                                                return;
                                            }

                                            //  Toast.makeText(getApplicationContext(),"Selected: "+s,Toast.LENGTH_LONG).show();


                                        }
                );
                sd.showSpinerDialog();
            }
        });


        choose_lga = findViewById(R.id.choose_lga);
        //launch predictive drop down
        choose_lga.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                cleanLgaText();
                SpinnerDialog sd = new SpinnerDialog(BioData.this, lgas, "Get Lga");
                sd.bindOnSpinerListener(new OnSpinerItemClick() {
                                            @Override
                                            public void onClick(String s, int position) {

                                                lga.setText(s);
                                                return;
                                            }

                                            //  Toast.makeText(getApplicationContext(),"Selected: "+s,Toast.LENGTH_LONG).show();


                                        }
                );
                sd.showSpinerDialog();
            }
        });

    }

    void cleanStateText() {
        state_origin.setText("No State Chosen");
        lga.setText("No Lga Chosen");
    }

    void cleanLgaText() {
        lga.setText("No Lga Chosen");
    }

    void getLga(String state) {
        lgas = databaseAccess.getLgas(state);
    }

    ;

    public void proceed(View view) {
        if (extra.equalsIgnoreCase("profile")) {
            //just go back
            startActivity(new Intent(getApplicationContext(), MenuOfficers.class));
        } else if (extra.equalsIgnoreCase("visitor")) {
            if (fullname.getText().toString().isEmpty()) {
                fullname.setError("Please Enter Name");
                return;
            }
            if (age.getText().toString().isEmpty()) {
                age.setError("Please Enter Age");
                return;
            }
            if (phone.getText().toString().isEmpty() || phone.getText().toString().length() < 10) {
                age.setError("Please Enter a valid phone number");
                return;
            }
            if (email.getText().toString().isEmpty()) {
                email.setError("Please Enter Email");
                return;
            }
            String emailPattern = "[a-zA-Z0-9._-]+@[a-z]+\\.+[a-z]+";
            if (!email.getText().toString().matches(emailPattern)) {
                email.setError("Please Enter Valid Email");
                return;
            }
            if (gender_val.isEmpty() || gender_val.contains("--")) {
                Toast.makeText(BioData.this, "Please Choose Gender", Toast.LENGTH_SHORT).show();
                return;
            }
            if (marital_status_val.isEmpty() || marital_status_val.contains("--")) {
                Toast.makeText(BioData.this, "Please Choose Marital Status", Toast.LENGTH_SHORT).show();
                return;
            }
            if (role_val.isEmpty() || role_val.contains("--")) {
                Toast.makeText(BioData.this, "Please Role that best describes you", Toast.LENGTH_SHORT).show();
                return;
            }
            if (state_origin.getText().toString().contains("Chosen")) {
                state_origin.setError("Please Choose State of Origin");
                return;
            }
            if (lga.getText().toString().contains("Chosen")) {
                lga.setError("Please Choose Lga");
                return;
            }

            if (address.getText().toString().isEmpty()) {
                address.setError("Please Enter Address");
                return;
            }
            if (qualifications.getText().toString().isEmpty()) {
                qualifications.setError("Please Enter Qualifications");
                return;
            }
            if (nin.getText().toString().isEmpty()) {
                nin.setError("Please Enter Nin");
                return;
            }


            UserRegisterRequest userRequest = new UserRegisterRequest();
            userRequest.setName(fullname.getText().toString());
            userRequest.setAge(age.getText().toString());
            userRequest.setPhone(phone.getText().toString());
            userRequest.setEmail(email.getText().toString());
            userRequest.setSex(gender_val);
            userRequest.setRole(role_val);
            userRequest.setMarital_status(marital_status_val);
            userRequest.setState_of_origin(state_origin.getText().toString());
            userRequest.setLga(lga.getText().toString());
            userRequest.setAddress((address.getText().toString()));
            userRequest.setQualification((qualifications.getText().toString()));

            userRequest.setNin(nin.getText().toString());

            //save to sqlite
            String response = databaseAccess.InsertEnrollee(userRequest);

            Toast.makeText(getApplicationContext(), response, Toast.LENGTH_SHORT).show();
            if (response.contains("Successfully")) {
                //go to biometrics
                sendToServer();
            }

        } else {
            //after validation


            if (fullname.getText().toString().isEmpty()) {
                fullname.setError("Please Enter Name");
                return;
            }
            if (age.getText().toString().isEmpty()) {
                age.setError("Please Enter Age");
                return;
            }
            if (email.getText().toString().isEmpty()) {
                email.setError("Please Enter Email");
                return;
            }
            String emailPattern = "[a-zA-Z0-9._-]+@[a-z]+\\.+[a-z]+";
            if (!email.getText().toString().matches(emailPattern)) {
                email.setError("Please Enter Valid Email");
                return;
            }
            if (height.getText().toString().isEmpty()) {
                height.setError("Please Enter Height");
                return;
            }
            if (weight.getText().toString().isEmpty()) {
                weight.setError("Please Enter Weight");
                return;
            }

            if (eye_color_val.isEmpty() || eye_color_val.contains("--")) {
                Toast.makeText(BioData.this, "Please Choose Eye Color", Toast.LENGTH_SHORT).show();
                return;
            }
            if (hair_color_val.isEmpty() || hair_color_val.contains("--")) {
                Toast.makeText(BioData.this, "Please Choose Hair Color", Toast.LENGTH_SHORT).show();
                return;
            }


            if (tatoo_val.isEmpty() || tatoo_val.contains("--")) {
                Toast.makeText(BioData.this, "Please Choose Availability Of Tatoos", Toast.LENGTH_SHORT).show();
                return;
            }

            if (gender_val.isEmpty() || gender_val.contains("--")) {
                Toast.makeText(BioData.this, "Please Choose Gender", Toast.LENGTH_SHORT).show();
                return;
            }
            if (marital_status_val.isEmpty() || marital_status_val.contains("--")) {
                Toast.makeText(BioData.this, "Please Choose Marital Status", Toast.LENGTH_SHORT).show();
                return;
            }
            if (role_val.isEmpty() || role_val.contains("--")) {
                Toast.makeText(BioData.this, "Please Role that best describes you", Toast.LENGTH_SHORT).show();
                return;
            }

            if (blood_group_val.isEmpty() || blood_group_val.contains("--")) {
                Toast.makeText(BioData.this, "Please Choose Blood Group", Toast.LENGTH_SHORT).show();
                return;
            }
            if (genotype_val.isEmpty() || genotype_val.contains("--")) {
                Toast.makeText(BioData.this, "Please Choose Genotype", Toast.LENGTH_SHORT).show();
                return;
            }
            if (nok.getText().toString().isEmpty()) {
                nok.setError("Please Enter Nok Name");
                return;
            }
            if (nok_phone.getText().toString().isEmpty()) {
                nok_phone.setError("Please Enter Nok Phone");
                return;
            }
            if (state_origin.getText().toString().contains("Chosen")) {
                state_origin.setError("Please Choose State of Origin");
                return;
            }
            if (lga.getText().toString().contains("Chosen")) {
                lga.setError("Please Choose Lga");
                return;
            }
            if (hometown.getText().toString().isEmpty()) {
                hometown.setError("Please Enter Home Town");
                return;
            }
            if (address.getText().toString().isEmpty()) {
                address.setError("Please Enter Address");
                return;
            }
            if (qualifications.getText().toString().isEmpty()) {
                qualifications.setError("Please Enter Qualifications");
                return;
            }
            if (nin.getText().toString().isEmpty()) {
                nin.setError("Please Enter Nin");
                return;
            }
            if (date_of_birth.getText().toString().isEmpty()) {
                date_of_birth.setError("Please Enter Date of Birth");
                return;
            }
            if (school_in.getText().toString().isEmpty()) {
                school_in.setError("Please Enter Year In");
                return;
            }
            if (school_out.getText().toString().isEmpty()) {
                school_out.setError("Please Enter Year Out");
                return;
            }

            UserRegisterRequest userRequest = new UserRegisterRequest();
            userRequest.setName(fullname.getText().toString());
            userRequest.setAge(age.getText().toString());
            userRequest.setPhone(phone.getText().toString());
            userRequest.setEmail(email.getText().toString());
            userRequest.setHeight(height.getText().toString());
            userRequest.setWeight(weight.getText().toString());
            userRequest.setEye_color(eye_color_val);
            userRequest.setHair_color(hair_color_val);
            userRequest.setTattoo(tatoo_val);
            userRequest.setSex(gender_val);
            userRequest.setMarital_status(marital_status_val);
            userRequest.setBlood_group(blood_group_val);
            userRequest.setGenotype(genotype_val);
            userRequest.setNok_name(nok.getText().toString());
            userRequest.setNok_phone(nok_phone.getText().toString());
            userRequest.setState_of_origin(state_origin.getText().toString());
            userRequest.setLga(lga.getText().toString());
            userRequest.setHometown(hometown.getText().toString());
            userRequest.setAddress((address.getText().toString()));
            userRequest.setQualification((qualifications.getText().toString()));
            userRequest.setRole(role_val);

            userRequest.setNin(nin.getText().toString());
            userRequest.setSec_sch_year_in(school_in.getText().toString());
            userRequest.setSec_sch_year_out(school_out.getText().toString());
            userRequest.setDate_of_birth(date_of_birth.getText().toString());

            //save to sqlite
            String response = databaseAccess.InsertEnrollee(userRequest);

            Toast.makeText(getApplicationContext(), response, Toast.LENGTH_SHORT).show();
            if (response.contains("Successfully")) {
                //go to biometrics
                sendToServer();
            }
        }
    }

    private void sendToServer() {

        //Toast.makeText(getApplicationContext(), "Something", Toast.LENGTH_SHORT).show();

        startActivity(new Intent(getApplicationContext(), Enroll.class)
                .putExtra("extra", "extra")
                .putExtra("phone", phone.getText().toString())
                .putExtra("fullname", fullname.getText().toString()));
        //retrofit_auth and get token
        /**  progressBar.setVisibility(View.VISIBLE);

         OkHttpClient client = new OkHttpClient.Builder().addInterceptor(new Interceptor() {
        @Override public okhttp3.Response intercept(Chain chain) throws IOException {
        Request newRequest = chain.request().newBuilder()
        .build();
        return chain.proceed(newRequest);
        }
        }).build();

         Retrofit retrofit = new Retrofit.Builder().client(client)
         // .baseUrl(getString(R.string.base_url))
         .baseUrl(fromSharedPref)
         .addConverterFactory(GsonConverterFactory.create()).build();
         NAIC service = retrofit.create(NAIC.class);

         UserRegisterRequest userRequest = new UserRegisterRequest();
         userRequest.setName(fullname.getText().toString());
         userRequest.setAge(age.getText().toString());
         userRequest.setPhone(phone.getText().toString());
         userRequest.setEmail(email.getText().toString());
         userRequest.setHeight(height.getText().toString());
         userRequest.setWeight(weight.getText().toString());
         userRequest.setEye_color(eye_color.getText().toString());
         userRequest.setHair_color(hair_color.getText().toString());
         userRequest.setTattoo(tatoo.getText().toString());
         userRequest.setSex(gender.getText().toString());
         userRequest.setMarital_status(marital_status.getText().toString());
         userRequest.setBlood_group(blood_group.getText().toString());
         userRequest.setGenotype(genotype.getText().toString());
         userRequest.setNok_name(nok.getText().toString());
         userRequest.setNok_phone(nok_phone.getText().toString());
         userRequest.setState_of_origin(state_origin.getText().toString());
         userRequest.setLga(lga.getText().toString());
         userRequest.setHometown(hometown.getText().toString());
         userRequest.setNin(nin.getText().toString());
         userRequest.setSec_sch_year_in(school_in.getText().toString());
         userRequest.setSec_sch_year_out(school_out.getText().toString());

         Call<ResponseBody> responseBodyCall = service.getSignUpResponse(userRequest);

         responseBodyCall.enqueue(new Callback<ResponseBody>() {

        @Override public void onResponse(Call<ResponseBody> call, Response<ResponseBody> response) {
        progressBar.setVisibility(View.GONE);
        Log.d("fingerprint", "Response Code: " + response.code());
        String jsonResponse = "";

        try {
        InputStream inputStr = response.body().byteStream();


        jsonResponse = IOUtils.toString(inputStr, "UTF-8");
        Log.d("fingerprint", "" + jsonResponse);
        } catch (IOException e) {
        e.printStackTrace();
        }
        //validate response
        try {
        if (response.code() == 200 || response.code() == 201) {
        JSONParser parse = new JSONParser();
        JSONObject jobj;
        try {
        jobj = (JSONObject) parse.parse(jsonResponse);

        JSONObject jsonobj_1 = (JSONObject) jobj.get("data");

        String army_number = (String) jsonobj_1.get("army_number");
        String token = (String) jobj.get("token");

        finish();
        startActivity(new Intent(getApplicationContext(), Enroll.class)
        .putExtra("extra", extra)
        .putExtra("army_number", army_number)
        .putExtra("token", token)
        .putExtra("fullname", fullname.getText().toString())); //put all string extras
        } catch (ParseException | NullPointerException e) {
        e.printStackTrace();
        }

        } else {
        Log.d("fingerprint", "Response RAW: " + response.raw());
        //something went wrong. Try again.
        progressBar.setVisibility(View.GONE);
        Toast.makeText(getApplicationContext(), "Something went wrong try again", Toast.LENGTH_SHORT).show();
        }
        } catch (NullPointerException n) {
        n.printStackTrace();


        //Toast.makeText(getApplicationContext(), "Something went wrong. Fill field appropriately and try again", Toast.LENGTH_SHORT).show();
        //                    Toast.makeText(getApplicationContext(), response.body().getMessage(), Toast.LENGTH_SHORT).show();
        progressBar.setVisibility(View.GONE);

        } catch (Exception e) {
        e.printStackTrace();
        }
        }

        @Override public void onFailure(Call<ResponseBody> call, Throwable t) {
        progressBar.setVisibility(View.GONE);
        Toast.makeText(getApplicationContext(), "Network Error", Toast.LENGTH_SHORT).show();

        Log.d("fingerprint", "Failed to upload: " + t.toString());

        }
        });
         **/
    }
}