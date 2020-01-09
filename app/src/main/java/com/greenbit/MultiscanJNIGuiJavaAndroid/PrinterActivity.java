package com.greenbit.MultiscanJNIGuiJavaAndroid;

import android.Manifest;
import android.content.Context;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.os.AsyncTask;
import android.os.Bundle;
import android.print.PrintAttributes;
import android.print.PrintDocumentAdapter;
import android.print.PrintManager;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ProgressBar;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import com.greenbit.MultiscanJNIGuiJavaAndroid.interfaces.BIPPIIS;
import com.itextpdf.text.BaseColor;
import com.itextpdf.text.Chunk;
import com.itextpdf.text.Document;
import com.itextpdf.text.DocumentException;
import com.itextpdf.text.Element;
import com.itextpdf.text.Font;
import com.itextpdf.text.Image;
import com.itextpdf.text.PageSize;
import com.itextpdf.text.Paragraph;
import com.itextpdf.text.pdf.BaseFont;
import com.itextpdf.text.pdf.PdfContentByte;
import com.itextpdf.text.pdf.PdfImportedPage;
import com.itextpdf.text.pdf.PdfReader;
import com.itextpdf.text.pdf.PdfWriter;
import com.itextpdf.text.pdf.draw.LineSeparator;

import org.apache.commons.io.IOUtils;
import org.json.JSONException;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;
import java.util.Calendar;

import okhttp3.Interceptor;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.ResponseBody;
import pl.droidsonroids.gif.GifImageView;
import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

public class PrinterActivity extends AppCompatActivity {
    private Button print;
    private GifImageView gifImageView;
    //font
    BaseColor colorAccent = new BaseColor(0, 153, 204, 255);
    float sectionHeaderSize = 10.0f;
    float titleSize = 5.0f;
    float valueFontSize = 8.0f;
    //    float sectionHeaderSize = 30.0f;
//    float titleSize = 20.0f;
//    float valueFontSize = 26.0f;
    String jsonResponse = "";


    private BaseFont fontName;
    private Document document;

    private byte[] imageBytes;
    private String[] sections = {"Personal Information", "Next of Kin's Information", "Employment Information", "Payment Information"};

    private ArrayList values1 = new ArrayList();
    private ArrayList values2 = new ArrayList();
    private ArrayList values3 = new ArrayList();
    private ArrayList values4 = new ArrayList();

//    private String[] values1 = {};
//    private String[] values2 = {};
//    private String[] values3 = {};
//    private String[] values4 = {};

    private String[] titles1 = {"FILE NUMBER", "BIPPIIS NUMBER", "FULL NAME", "GENDER", "DATE OF BIRTH",
            "PHONE NUMBER", "PENSIONER CATEGORY", "PAYROLL STATUS", "PERMANENT ADDRESS", "LOCAL GOVERNMENT OF ORIGIN"
    };
    private String[] titles2 = {"NEXT OF KIN'S NAME",
            "NEXT OF KIN'S  PHONE NUMBER",
            "RELATIONSHIP WITH NEXT OF KIN", "NEXT OF KIN'S ADDRESS"};
    private String[] titles3 = {"DATE OF FIRST APPOINTMENT", "RETIREMENT DATE",
            "NUMBER OF YEARS SERVED", "CLASSIFICATION", "RANK", "GRADE LEVEL/STEP", "DATE OF LAST PROMOTION",
            "LOCAL GOVERNMENT OF RETIREMENT"};
    private String[] titles4 = {"BANK", "ACCOUNT NUMBER", "BANK VERIFICATION NUMBER BVN"};

    private String[] values = {};
    // TODO: remove token value
    private String token = "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpc3MiOiJodHRwczpcL1wvYmlwcGlpcy5jb21cL2FwaVwvdjFcL2Vucm9sbCIsImlhdCI6MTU3ODMxNDQzOSwiZXhwIjoxNTc5NjEwNDM5LCJuYmYiOjE1NzgzMTQ0MzksImp0aSI6ImFISGpMYjRadENjcERDT04iLCJzdWIiOjksInBydiI6IjczOWJhZTk4ZmZlZWNjNGE0ZmNkNTkxNDRmMDVlY2Q3MzA0ODg5ZGUifQ.9n5051vwV4PZf1B2bigW9c_vgbRUnkaf1lTv6cOEP1E";
    private boolean isPrintoutCreated = false;
    private String year = "";
    private String headerString;
    private ProgressBar progressBar;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_printer);
        if (ContextCompat.checkSelfPermission(this,
                Manifest.permission.READ_EXTERNAL_STORAGE)
                != PackageManager.PERMISSION_GRANTED) {
            // No explanation needed, we can request the permission.
            String[] permissions = {
                    Manifest.permission.READ_EXTERNAL_STORAGE,
                    Manifest.permission.WRITE_EXTERNAL_STORAGE
            };
            ActivityCompat.requestPermissions(this,
                    permissions,
                    1);
        }
        print = findViewById(R.id.print);
        gifImageView = findViewById(R.id.gif);
        progressBar = findViewById(R.id.progress);


        try {
            year = String.valueOf(Calendar.getInstance().get(Calendar.YEAR));
            headerString = "BENUE STATE LOCAL GOVERNMENT PENSIONS BOARD \n PENSIONER BIOMETRICS CAPTURE " + year;
            imageBytes = getIntent().getByteArrayExtra("image");
            // TODO:
            //  token = getIntent().getStringExtra("token");
            print.setVisibility(View.GONE);
            getDetails();

            //custom font
            fontName = BaseFont.createFont("assets/fonts/HVD Fonts - BrandonText-Regular.otf", "UTF-8", BaseFont.EMBEDDED);

        } catch (DocumentException e) {
            e.printStackTrace();
            Log.d("fingerprint", e.getMessage());
        } catch (IOException e) {
            e.printStackTrace();
        }

        print.setOnClickListener(v -> {
            //Toast.makeText(PrinterActivity.this, "createPDFFile almost called", Toast.LENGTH_SHORT).show();
            if (isPrintoutCreated)
                createPDFFile(Common.getAppPath(PrinterActivity.this) + "test_pdf.pdf");
            else {
                try {
                    getDetails();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });

    }


    private void getPrintDetails() throws IOException, ParseException, JSONException {
        progressBar.setVisibility(View.VISIBLE);

//        new MyAsyncTask().execute();
        //  new initialLoad().execute(token);
//        jsonResponse = getResponseFromJsonURL(getResources().getString(R.string.base_url), token);


        //            JSONArray jsonarr_2 = (JSONArray) (String) jsonobj_1.get("lga_pensioners");
//                System.out.println("Elements under results array");
//                System.out.println("\nPlace id: " + (String) jsonobj_1.get("place_id"));
//                System.out.println("Types: " + (String) jsonobj_1.get("types"));
//                //Get data for the Address Components array
//                System.out.println("Elements under address_components array");
//                System.out.println("The long names, short names and types are:");


//                for (int j = 0; j < jsonarr_2.size(); j++) {
        //Same just store the JSON objects in an array
        //Get the index of the JSON objects and print the values as per the index
//  //          JSONObject jsonobj_2 = (JSONObject) jsonarr_2.get(j);
//            //Store the data as String objects
//
//            String str_data1 = (String) jsonobj_2.get("long_name");
//            System.out.println(str_data1);
//            String str_data2 = (String) jsonobzj_2.get("short_name");
//            System.out.println(str_data2);
//            System.out.println(jsonobj_2.get("types"));
//            System.out.println("\n");
//            //              }
        //        }


    }


    private void getDetails() throws IOException {
        progressBar.setVisibility(View.VISIBLE);
        OkHttpClient client = new OkHttpClient.Builder().addInterceptor(new Interceptor() {
            @Override
            public okhttp3.Response intercept(Chain chain) throws IOException {
                Request newRequest = chain.request().newBuilder()
                        .addHeader("Authorization", "Bearer " + token)
                        .build();
                return chain.proceed(newRequest);
            }
        }).build();

        Retrofit retrofit = new Retrofit.Builder().client(client)
                .baseUrl(getString(R.string.base_url))
                .addConverterFactory(GsonConverterFactory.create()).build();
        BIPPIIS service = retrofit.create(BIPPIIS.class);

        Call<ResponseBody> getUserResponseCall = service.getUserResponse();

        getUserResponseCall.enqueue(new Callback<ResponseBody>() {
            @Override
            public void onResponse(Call<ResponseBody> call, Response<ResponseBody> response) {
                if (response.code() == 200) {
                    InputStream inputStr = response.body().byteStream();
                    try {
                        jsonResponse = IOUtils.toString(inputStr, "UTF-8");
                        Log.d("fingerprint", jsonResponse);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    if (!jsonResponse.isEmpty()) {
                        isPrintoutCreated = true;
                        print.setVisibility(View.VISIBLE);
                        print.setEnabled(true);

                        JSONParser parse = new JSONParser();
                        JSONObject jobj = null;
                        try {
                            jobj = (JSONObject) parse.parse(jsonResponse);

                            JSONObject jsonarr_1 = (JSONObject) jobj.get("data");
                            JSONObject jsonobj_1 = (JSONObject) jsonarr_1.get("lga_pensioners");


                            values1.add(jsonobj_1.get("bippiis_number"));
                            values1.add(jsonobj_1.get("bippiis_no"));
                            values1.add(jsonobj_1.get("fullname"));
                            values1.add(jsonobj_1.get("gender"));
                            values1.add(jsonobj_1.get("date_of_birth"));
                            values1.add(jsonobj_1.get("phone"));
                            values1.add(jsonobj_1.get("category"));


                            Log.d("fingerprint JSON: ", jsonobj_1.toString());


                            if (jsonobj_1.get("payroll_status").equals(1))
                                values1.add("Active");
                            else
                                values1.add("Not Active");
                            //        values1[7] = (String) jsonobj_1.get("payroll_status").toString();

                            values1.add(jsonobj_1.get("permanent_address"));
                            values1.add(jsonobj_1.get("lga_o"));

                            values2.add(jsonobj_1.get("nok_name"));
                            values2.add(jsonobj_1.get("nok_phone"));
                            values2.add(jsonobj_1.get("nok_relationship"));
                            values2.add(jsonobj_1.get("nok_address"));

                            values3.add(jsonobj_1.get("f_a_d"));
                            values3.add("" + jsonobj_1.get("d_o_r"));
                            values3.add("" + jsonobj_1.get("years_served"));
                            values3.add(jsonobj_1.get("classification"));
                            values3.add(jsonobj_1.get("rank"));

                            values3.add("Grade Level " + jsonobj_1.get("grade_level") + "/ Step " + jsonobj_1.get("step"));

                            values3.add(jsonobj_1.get("d_o_p"));
                            values3.add(jsonobj_1.get("lga_r"));


                            values4.add(jsonobj_1.get("bank"));
                            values4.add(jsonobj_1.get("account_number"));
                            values4.add(jsonobj_1.get("bvn"));


                            // send gotten data to appropriate array
                        } catch (ParseException e) {
                            e.printStackTrace();
                        }

                    }

                    /************** For getting response from HTTP URL end ***************/
                } else {
                    //retry
                    Log.d("fingerprint", "Error response/not 200/: " + response.code());
                    progressBar.setVisibility(View.GONE);
                    Log.d("fingerprint", "Response RAW: " + response.raw());
                    print.setVisibility(View.VISIBLE);
                    print.setText("Click to Create Printout".toUpperCase());
                    isPrintoutCreated = false;
                }

            }

            @Override
            public void onFailure(Call<ResponseBody> call, Throwable t) {
                //retry
                Log.d("fingerprint", "Error response/not 200/: " + t.getLocalizedMessage());
                progressBar.setVisibility(View.GONE);
                print.setVisibility(View.VISIBLE);
                print.setText("Click to Create Printout".toUpperCase());
                isPrintoutCreated = false;

            }
        });

    }

    private void createPDFFile(String path) {
        //   Toast.makeText(this, "createPDFFile called", Toast.LENGTH_SHORT).show();
        if (new File(path).exists())
            new File(path).delete();

        try {
            document = new Document();
            //save
            PdfWriter writer = PdfWriter.getInstance(document, new FileOutputStream(path));
            //open
            document.open();
            document.setPageSize(PageSize.A4);
            document.addCreationDate();
            document.addAuthor("BIPPIIS");
            document.addCreator("Benue State Pensions Board");


            //section HEADER
            Font headerFont = new Font(fontName, sectionHeaderSize + 2.0f, Font.BOLD, BaseColor.BLACK);
            addNewItem(document, headerString, Element.ALIGN_CENTER, headerFont);

            /// PASSPORT

//            Drawable d = getResources().getDrawable(R.drawable.two_thumbs);
//            Bitmap bitmap = ((BitmapDrawable) d).getBitmap();
//            ByteArrayOutputStream stream = new ByteArrayOutputStream();
//            bitmap.compress(Bitmap.CompressFormat.JPEG, 100, stream);
//            byte[] bitmapdata = stream.toByteArray();

            Image image = Image.getInstance(imageBytes);
            //Image image = Image.getInstance(bitmapdata);
            image.scalePercent(15f);
            image.scaleAbsoluteWidth(80f);
            image.scaleAbsoluteHeight(60f);
            image.setAlignment(Element.ALIGN_CENTER);
            document.add(image);

            //section PERSONAL INFO title
            Font titleFont = new Font(fontName, sectionHeaderSize, Font.BOLD, BaseColor.BLACK);
            addNewItem(document, sections[0], Element.ALIGN_CENTER, titleFont);
            for (int i = 0, titles1Length = titles1.length; i < titles1Length; i++) {
                String title = titles1[i];
                String value = values1.get(i).toString();
                addSection(title, value);
            }

            addLineSeparator(document);
            //section PERSONAL NOK title
            addNewItem(document, sections[1], Element.ALIGN_CENTER, titleFont);
            for (int i = 0, titles1Length = titles2.length; i < titles1Length; i++) {
                String title = titles2[i];
                String value = values2.get(i).toString();
                addSection(title, value);
            }


            addLineSeparator(document);
            //section PERSONAL EMPLOYMENT title
            addNewItem(document, sections[2], Element.ALIGN_CENTER, titleFont);
            for (int i = 0, titles1Length = titles3.length; i < titles1Length; i++) {
                String title = titles3[i];
                String value = values3.get(i).toString();
                addSection(title, value);
            }

            addLineSeparator(document);
            //section PERSONAL PAYMENT title
            addNewItem(document, sections[3], Element.ALIGN_CENTER, titleFont);

            for (int i = 0, titles1Length = titles4.length; i < titles1Length; i++) {
                String title = titles4[i];
                String value = values4.get(i).toString();
                addSection(title, value);
            }

            //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// adding watermark
            PdfReader reader = new PdfReader(getResources().getAssets().open("registration_form.pdf"));
            PdfContentByte canvas = writer.getDirectContent();
            PdfImportedPage page;
            for (int i = 1; i <= reader.getNumberOfPages(); i++) {
                page = writer.getImportedPage(reader, i);
                canvas.addTemplate(page, 1f, 0, 0, 1, 0, 0);
            }
            document.close();

            Toast.makeText(this, "Select a Printer", Toast.LENGTH_SHORT).show();
            printPDF();

        } catch (FileNotFoundException e) {
            e.printStackTrace();
            Log.d("fingerprint", "File not found " + e.getMessage());
        } catch (DocumentException e) {
            e.printStackTrace();
            Log.d("fingerprint", "DocumentException " + e.getMessage());

        } catch (MalformedURLException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void printPDF() {
        PrintManager printManager = (PrintManager) getSystemService(Context.PRINT_SERVICE);


        try {
            PrintDocumentAdapter printDocumentAdapter = new PdfDocumentAdapter(PrinterActivity.this, Common.getAppPath(PrinterActivity.this) + "test_pdf.pdf");
            printManager.print("Document", printDocumentAdapter, new PrintAttributes.Builder().build());
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }

    private void addSection(String title, String value) {

        Font orderNumberFont = new Font(fontName, titleSize, Font.NORMAL, colorAccent);
        try {
            //add heading/title
            addNewItem(document, title, Element.ALIGN_LEFT, orderNumberFont);

            //add value
            Font orderNumberValue = new Font(fontName, valueFontSize, Font.NORMAL, BaseColor.BLACK);
            addNewItem(document, value, Element.ALIGN_LEFT, orderNumberValue);


        } catch (DocumentException e) {
            e.printStackTrace();
        }


    }

    private void addLineSeparator(Document document) throws DocumentException {
        LineSeparator lineSeparator = new LineSeparator();
        lineSeparator.setLineColor(new BaseColor(0, 0, 0, 68));
        addLineSpace(document);
        document.add(new Chunk(lineSeparator));
        addLineSpace(document);

    }

    private void addLineSpace(Document document) throws DocumentException {
        document.add(new Paragraph(""));
    }


    private void addNewItem(Document document, String section_title, int align, Font font) throws DocumentException {
        Chunk chunk = new Chunk(section_title, font);
        Paragraph paragraph = new Paragraph(chunk);
        paragraph.setAlignment(align);
        document.add(paragraph);
    }

    public static Bitmap drawableToBitmap(Drawable drawable) {
        if (drawable instanceof BitmapDrawable) {
            return ((BitmapDrawable) drawable).getBitmap();
        }

        final int width = !drawable.getBounds().isEmpty() ? drawable
                .getBounds().width() : drawable.getIntrinsicWidth();

        final int height = !drawable.getBounds().isEmpty() ? drawable
                .getBounds().height() : drawable.getIntrinsicHeight();

        final Bitmap bitmap = Bitmap.createBitmap(width <= 0 ? 1 : width,
                height <= 0 ? 1 : height, Bitmap.Config.ARGB_8888);

        Log.v("Bitmap width - Height :", width + " : " + height);
        Canvas canvas = new Canvas(bitmap);
        drawable.setBounds(0, 0, canvas.getWidth(), canvas.getHeight());
        drawable.draw(canvas);

        return bitmap;
    }

    @Override
    protected void onPause() {
        super.onPause();
        //     startActivity(new Intent(getApplicationContext(), Login.class));
    }

    private class initialLoad extends AsyncTask<String, Integer, String> {

        @Override
        protected String doInBackground(String... strings) {
            try {
                //         jsonResponse = getResponseFromJsonURL(getResources().getString(R.string.base_url), strings[0]);
            } catch (Exception ex) {
                ex.printStackTrace();
                return "network error";
            }

            return jsonResponse;
        }

        @Override
        protected void onPreExecute() {
            super.onPreExecute();
        }

        @Override
        protected void onPostExecute(String text) {
            super.onPostExecute(text);
            System.out.println(text);
            Log.d("fingerprint", "Json: " + jsonResponse);
        }

    }


    public String getResponseFromJsonURL(String url, String token) {
        String jsonResponse = "";
        if (!url.isEmpty()) {
            try {
                /************** For getting response from HTTP URL start ***************/
//                URL object = new URL(url);
//
//                HttpURLConnection connection = (HttpURLConnection) object
//                        .openConnection();
//                // int timeOut = connection.getReadTimeout();
//                connection.setRequestMethod("GET");
//                connection.setReadTimeout(60 * 1000);
//                connection.setConnectTimeout(60 * 1000);
////                String authorization="xyz:xyz$123";
//                String encodedAuth = "Bearer " + token;
//                connection.setRequestProperty("Authorization", encodedAuth);
//
//
                URL myURL = new URL(url);
                HttpURLConnection conn = (HttpURLConnection) myURL.openConnection();
                conn.setRequestMethod("GET");
                String basicAuth = "Bearer " + token;
                conn.setRequestProperty("Authorization", basicAuth);
                conn.setRequestProperty("Content-Type", "application/x-www-form-urlencoded");
                conn.setRequestProperty("Content-Language", "en-US");
                conn.setUseCaches(false);
                conn.setDoInput(true);
                conn.setDoOutput(true);
                conn.connect();


                int responseCode = conn.getResponseCode();
                //String responseMsg = connection.getResponseMessage();


                if (responseCode == 200) {
                    InputStream inputStr = conn.getInputStream();
                    String encoding = conn.getContentEncoding() == null ? "UTF-8"
                            : conn.getContentEncoding();
                    jsonResponse = IOUtils.toString(inputStr, encoding);
                    /************** For getting response from HTTP URL end ***************/
                } else {
                    //retry
                    Log.d("fingerprint", "Error response/not 200/: " + responseCode);
                }
            } catch (Exception e) {
                e.printStackTrace();

            }
        }
        return jsonResponse;
    }

}

//class AsyncTaskRunner extends AsyncTask<String, Integer, String> {
//
//    @Override
//    protected String doInBackground(String... token) {
//        // Do something asynchronously
////        String basicAuth = "Basic " + token;
////        String jsonResponse = "";
//        int response = 0;
////        try {
////
////            URL url = new URL(getResources().getString(R.string.base_url));
////            HttpURLConnection conn = (HttpURLConnection) url.openConnection();
////
////            conn.setRequestProperty("Authorization", basicAuth);
////            conn.setRequestMethod("GET");
////            conn.setRequestProperty("Content-Type", "application/x-www-form-urlencoded");
//////        conn.setRequestProperty("Content-Length", "" + postData.getBytes().length);
////            conn.setRequestProperty("Content-Language", "en-US");
////            conn.setUseCaches(false);
////            conn.setDoInput(true);
////            conn.setDoOutput(true);
////            conn.connect();
////
////
//////                return conn.getResponseCode();
////            response = conn.getResponseCode();
////        } catch (IOException e) {
////            e.printStackTrace();
////        }
//
//
////        getResponseFromJsonURL();
//
//        return String.valueOf(response);
//    }
//
//    /*
//     * (non-Javadoc)
//     *
//     * @see android.os.AsyncTask#onPostExecute(java.lang.Object)
//     */
//    @Override
//    protected void onPostExecute(String responseCode) {
//        int responsecode = Integer.valueOf(responseCode);
//        if (responsecode != 200)
//            throw new RuntimeException("HttpResponseCode: " + responsecode);
//        else {
//
//
//            JSONParser parse = new JSONParser();
////            JSONObject jobj = (JSONObject) parse.parse(inline);
//
//
//        }
//
//        // execution of result of Long time consuming operation
//
//    }
//
//    @Override
//    protected void onPreExecute() {
//        // Things to be done before execution of long running operation. For
//        // example showing ProgessDialog
//    }
//
//    /*
//     * (non-Javadoc)
//     *
//     * @see android.os.AsyncTask#onProgressUpdate(Progress[])
//     */
//
//
//}

