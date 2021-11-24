package com.greenbit.MultiscanJNIGuiJavaAndroid;

import static com.naic.nigerianarmy.GbExampleGrayScaleBitmapClass.GetBippiisDirectoryName;

import android.annotation.SuppressLint;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbManager;
import android.os.Build;
import android.os.Bundle;
import android.os.SystemClock;
import android.preference.PreferenceManager;
import android.util.Log;
import android.view.View;
import android.view.animation.AlphaAnimation;
import android.view.animation.Animation;
import android.view.animation.LinearInterpolator;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.transition.Transition;
import androidx.transition.TransitionValues;
import androidx.viewpager.widget.ViewPager;

import com.google.android.material.floatingactionbutton.FloatingActionButton;
import com.greenbit.ansinistitl.Gban2000JavaWrapperLibrary;
import com.greenbit.bozorth.BozorthJavaWrapperLibrary;
import com.greenbit.gbfinimg.GbfinimgJavaWrapperLibrary;
import com.greenbit.gbfir.GbfirJavaWrapperLibrary;
import com.greenbit.gbfrsw.GbfrswJavaWrapperLibrary;
import com.greenbit.gbmsapi.GBMSAPIJavaWrapperLibrary;
import com.greenbit.gbnfiq.GbNfiqJavaWrapperLibrary;
import com.greenbit.gbnfiq2.GbNfiq2JavaWrapperLibrary;
import com.greenbit.jpeg.GbjpegJavaWrapperLibrary;
import com.greenbit.lfs.LfsJavaWrapperLibrary;
import com.greenbit.usbPermission.IGreenbitLogger;
import com.greenbit.wsq.WsqJavaWrapperLibrary;
import com.naic.nigerianarmy.interfaces.NAIC;
import com.naic.nigerianarmy.models.FingerprintRequest;
import com.naic.nigerianarmy.models.FingerprintResponse;
import com.naic.nigerianarmy.models.storageFile;
import com.naic.nigerianarmy.models.storageFileImages;
import com.naic.nigerianarmy.utils.Tools;
import com.suprema.BioMiniFactory;
import com.suprema.CaptureResponder;
import com.suprema.IBioMiniDevice;
import com.suprema.IUsbEventHandler;
import com.telpo.tps550.api.fingerprint.FingerPrint;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Locale;

import okhttp3.OkHttpClient;
import okhttp3.Request;
import pl.droidsonroids.gif.GifImageView;
import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;


public class Verify extends AppCompatActivity
        implements IGreenbitLogger {
    private int i = 0;
    //Flag.
    public static final boolean mbUsbExternalUSBManager = false;
    private static final String ACTION_USB_PERMISSION = "com.android.example.USB_PERMISSION";
    private UsbManager mUsbManager = null;
    private PendingIntent mPermissionIntent = null;
    //

    private SharedPreferences sharedPref;
    private SharedPreferences.Editor editor;
    private String fromSharedPref = "";

    private static BioMiniFactory mBioMiniFactory = null;
    public static final int REQUEST_WRITE_PERMISSION = 786;
    public IBioMiniDevice mCurrentDevice = null;
    private Verify mainContext;

    public final static String TAG = "fingerprint";
    private EditText mLogView;
    private ScrollView mScrollLog = null;
    private GifImageView img;
    private TextView report, name;
    private ViewPager mPager;
    private String token = "", fullname = "";
    private FloatingActionButton upload;
    private String bippiis_number = "";

    private int[] mNaviPicks = {R.id.accelerate, R.id.accelerate, R.id.accelerate, R.id.accelerate};

    private boolean uploaded = false;
    public ArrayList fingerprints_array = new ArrayList();
    public ArrayList fingerprints_images_array = new ArrayList();

    @Override
    public void LogOnScreen(String s) {

    }

    @Override
    public void LogAsDialog(String s) {
        Tools.toast(s, Verify.this);
    }

    class UserData {
        String name;
        byte[] template;

        public UserData(String name, byte[] data, int len) {
            this.name = name;
            this.template = Arrays.copyOf(data, len);
        }
    }

    private ArrayList<UserData> mUsers = new ArrayList<>();

    private IBioMiniDevice.CaptureOption mCaptureOptionDefault = new IBioMiniDevice.CaptureOption();
    private CaptureResponder mCaptureResponseDefault = new CaptureResponder() {
        @Override
        public boolean onCaptureEx(final Object context, final Bitmap capturedImage,
                                   final IBioMiniDevice.TemplateData capturedTemplate,
                                   final IBioMiniDevice.FingerState fingerState) {
            log("onCapture : Capture successful!");
            printState(getResources().getText(R.string.capture_single_ok));
            log(((IBioMiniDevice) context).popPerformanceLog());
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    report.setText("Verifying...");

                    if (capturedImage != null) {

                        ImageView iv = findViewById(R.id.imagePreview);
                        if (iv != null) {
                            iv.setImageBitmap(capturedImage);


                            byte[] bmp = ARGB2Gray(capturedImage);

                            // convert bitmap to byte []
                            ByteArrayOutputStream stream = new ByteArrayOutputStream();
                            capturedImage.compress(Bitmap.CompressFormat.PNG, 100, stream);
                            byte[] byteArray = stream.toByteArray();


                            //                            byte[] bmp = mCurrentDevice.getCaptureImageAsBmp();

                            if (bmp == null) {
                                log("<<ERROR>> Cannot get BMP buffer");
                                printState(getResources().getText(R.string.export_bmp_fail));
                                return;
                            }
                            try {
                                File file = new File(GetBippiisDirectoryName(),
                                        "temp");
                                FileOutputStream fos = new FileOutputStream(file);
                                fos.write(byteArray);
                                fos.close();

                                printState(getResources().getText(R.string.export_bmp_ok));
                            } catch (IOException e) {
                                e.printStackTrace();
                            }
                            report.setText("Verifying...");

                            //create Gb fingerprint class
                            GbExampleGrayScaleBitmapClass bmpCls =
                                    new GbExampleGrayScaleBitmapClass(
                                            bmp,
                                            capturedImage.getWidth(), capturedImage.getHeight(),
                                            false,
                                            false
                                    );
                            Log.i("Check img size", "Real SizeX = " + (
                                    bmpCls.sx));
                            try {
                                //      bmpCls.GBBmpFromJpegBuffer(bmp, false, false, Verify.this);

                                //                                boolean ret = bmpCls.TestLfsBozorth1();
                                boolean ret = bmpCls.VerifyByString(Verify.this);
                                //    Toast.makeText(getApplicationContext(), "RetVal = " + ret, Toast.LENGTH_SHORT).show();
                                if (ret) {
                                    printState("Enrolled Successfully");
                                    report.setText("Verified Successfully!");
                                    Intent a=new Intent(Verify.this,VerifiedDetails.class);
                                    startActivity(a);

                                } else {
                                    printState("Not Enrolled Successfully");
                                    report.setText("Not Enrolled");
                                }
                            } catch (Exception e) {
                                e.printStackTrace();
                            }


                        }
                    }
                }
            });

            return true;
        }

        @Override
        public void onCaptureError(Object contest, int errorCode, String error) {
            log("onCaptureError : " + error);
            if (errorCode != IBioMiniDevice.ErrorCode.OK.value()) {

                printState(getResources().getText(R.string.capture_single_fail) + "(" + error + ")");
                //                report.setText("Error: Capture again");
            }

        }
    };

    synchronized public void printState(final CharSequence str) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                //                ((TextView) findViewById(R.id.tv)).setText(str);
                log(str.toString());
            }
        });

    }

    synchronized public void log(final String msg) {
        Log.d(TAG, msg);
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Log.d(TAG, msg);
            }
        });
    }


    private final BroadcastReceiver mUsbReceiver = new BroadcastReceiver() {
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if (ACTION_USB_PERMISSION.equals(action)) {
                synchronized (this) {
                    UsbDevice device = (UsbDevice) intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);
                    if (intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false)) {
                        if (device != null) {
                            if (mBioMiniFactory == null) return;
                            mBioMiniFactory.addDevice(device);
                            log(String.format(Locale.ENGLISH, "Initialized device count- BioMiniFactory (%d)", mBioMiniFactory.getDeviceCount()));
                        }
                    } else {
                        Log.d(TAG, "permission denied for device " + device);
                    }
                }
            }
        }
    };

    public void checkDevice() {
        if (mUsbManager == null) return;
        log("checkDevice");
        HashMap<String, UsbDevice> deviceList = mUsbManager.getDeviceList();
        Iterator<UsbDevice> deviceIter = deviceList.values().iterator();
        while (deviceIter.hasNext()) {
            UsbDevice _device = deviceIter.next();
            if (_device.getVendorId() == 0x16d1) {
                //Suprema vendor ID
                mUsbManager.requestPermission(_device, mPermissionIntent);
            } else {
            }
        }

    }

    @SuppressLint("RestrictedApi")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        FingerPrint.fingerPrintPower(1);


        GB_AcquisitionOptionsGlobals.GBMSAPI_Jw = new GBMSAPIJavaWrapperLibrary();
        GB_AcquisitionOptionsGlobals.WSQ_Jw = new WsqJavaWrapperLibrary();
        GB_AcquisitionOptionsGlobals.GBFRSW_Jw = new GbfrswJavaWrapperLibrary();
        GB_AcquisitionOptionsGlobals.GBFINIMG_Jw = new GbfinimgJavaWrapperLibrary();
        GB_AcquisitionOptionsGlobals.Jpeg_Jw = new GbjpegJavaWrapperLibrary();
        GB_AcquisitionOptionsGlobals.AN2000_Jw = new Gban2000JavaWrapperLibrary();
        GB_AcquisitionOptionsGlobals.GBFIR_Jw = new GbfirJavaWrapperLibrary();
        GB_AcquisitionOptionsGlobals.GBNFIQ_Jw = new GbNfiqJavaWrapperLibrary();
        GB_AcquisitionOptionsGlobals.GBNFIQ2_Jw = new GbNfiq2JavaWrapperLibrary();
        GB_AcquisitionOptionsGlobals.LFS_Jw = new LfsJavaWrapperLibrary();
        GB_AcquisitionOptionsGlobals.BOZORTH_Jw = new BozorthJavaWrapperLibrary();

        setContentView(R.layout.activity_enroll);
        sharedPref =  PreferenceManager.getDefaultSharedPreferences(getApplicationContext());
        editor = sharedPref.edit();
        fromSharedPref = sharedPref.getString("address", "nothing is here");

        mCaptureOptionDefault.frameRate = IBioMiniDevice.FrameRate.SHIGH;
        bippiis_number = getIntent().getStringExtra("bippiis_number");
        token = getIntent().getStringExtra("token");
        fullname = getIntent().getStringExtra("fullname");

//        Log.d("fingerprint", "B: " + bippiis_number + ", BE: " + bippiis_number_edited + ", T: " + token + ", F: " + fullname);

        img = findViewById(R.id.logo);
        report = findViewById(R.id.tv);
        name = findViewById(R.id.fullname);
        upload = findViewById(R.id.upload);
        upload.setVisibility(View.GONE);
        name.setText("Welcome, " + fullname);

        final Animation animation = new AlphaAnimation(1, 0); // Change alpha from fully visible to invisible
        new Transition() {
            @Override
            public void captureEndValues(@NonNull TransitionValues transitionValues) {

            }

            @Override
            public void captureStartValues(@NonNull TransitionValues transitionValues) {

            }
        };
        animation.setDuration(2000); // duration - 2 seconds
        animation.setInterpolator(new LinearInterpolator()); // do not alter animation rate
        animation.setRepeatCount(300); // Repeat animation infinitely
        animation.setRepeatMode(Animation.REVERSE); // Reverse animation at the end so the button will fade back in
        img.startAnimation(animation);

 /*
         int RetVal = GB_AcquisitionOptionsGlobals.Jpeg_Jw.Load();
         if (RetVal == GbjpegJavaWrapperDefinesReturnCodes.GBJPEG_OK) {
             Log.d("fingerprint", "Jpeg Load ok");
         } else {
             Log.d("fingerprint", "Jpeg Load Failure: " + GB_AcquisitionOptionsGlobals.Jpeg_Jw.GetLastErrorString());
         }
 */

        // Auto generated above
        mainContext = this;

        findViewById(R.id.start).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                report.setText("Place a finger on the scanner");
                ((ImageView) findViewById(R.id.imagePreview)).setImageBitmap(null);
                if (mCurrentDevice != null) {
                    //mCaptureOptionDefault.captureTimeout = (int)mCurrentDevice.getParameter(IBioMiniDevice.ParameterType.TIMEOUT).value;
                    mCurrentDevice.captureSingle(
                            mCaptureOptionDefault,
                            mCaptureResponseDefault,
                            true);
                }
            }
        });

        findViewById(R.id.stop).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (mCurrentDevice != null) {
                    new Thread(new Runnable() {
                        @Override
                        public void run() {
                            mCurrentDevice.abortCapturing();
                            int nRetryCount = 0;
                            while (mCurrentDevice != null && mCurrentDevice.isCapturing()) {
                                SystemClock.sleep(10);
                                nRetryCount++;
                            }
                            Log.d("AbortCapturing", String.format(Locale.ENGLISH,
                                    "IsCapturing return false.(Abort-lead time: %dms) ",
                                    nRetryCount * 10));
                        }
                    }).start();
                    report.setText("Scanning aborted");
                }
            }
        });

        if (mBioMiniFactory != null) {
            mBioMiniFactory.close();
        }

        restartBioMini();

        Log.d("fingerprint", "" + mBioMiniFactory.getSDKInfo());

    }

    void handleDevChange(IUsbEventHandler.DeviceChangeEvent event, Object dev) {
        //  Toast.makeText(getApplicationContext(), "handleDevChange", Toast.LENGTH_SHORT).show();

        if (event == IUsbEventHandler.DeviceChangeEvent.DEVICE_ATTACHED && mCurrentDevice == null) {
            //  Toast.makeText(getApplicationContext(), "attached", Toast.LENGTH_SHORT).show();
            new Thread(new Runnable() {
                @Override
                public void run() {
                    int cnt = 0;
                    while (mBioMiniFactory == null && cnt < 20) {
                        SystemClock.sleep(1000);
                        cnt++;
                    }
                    if (mBioMiniFactory != null) {
                        mCurrentDevice = mBioMiniFactory.getDevice(0);
                        Log.d(TAG, "mCurrentDevice attached : " + mCurrentDevice);
                        if (mCurrentDevice != null) {
                            log(" DeviceName : " + mCurrentDevice.getDeviceInfo().deviceName);
                            log("         SN : " + mCurrentDevice.getDeviceInfo().deviceSN);
                            log("SDK version : " + mCurrentDevice.getDeviceInfo().versionSDK);
                        }
                    }
                }
            }).start();
        } else if (mCurrentDevice != null && event == IUsbEventHandler.DeviceChangeEvent.DEVICE_DETACHED && mCurrentDevice.isEqual(dev)) {
            Toast.makeText(getApplicationContext(), "Device has been detached", Toast.LENGTH_SHORT).show();
            Log.d(TAG, "mCurrentDevice removed : " + mCurrentDevice);
            mCurrentDevice = null;
        }
    }

    void restartBioMini() {
        if (mBioMiniFactory != null) {
            mBioMiniFactory.close();
        }
        if (mbUsbExternalUSBManager) {
            //    Toast.makeText(getApplicationContext(), "mbUsbExternalUSBManager = true", Toast.LENGTH_SHORT).show();
            mUsbManager = (UsbManager) getSystemService(Context.USB_SERVICE);
            mBioMiniFactory = new BioMiniFactory(mainContext, mUsbManager) {
                @Override
                public void onDeviceChange(DeviceChangeEvent event, Object dev) {
                    log("----------------------------------------");
                    log("onDeviceChange : " + event + " using external usb-manager");
                    log("----------------------------------------");
                    handleDevChange(event, dev);
                }
            };
            //
            mPermissionIntent = PendingIntent.getBroadcast(this, 0, new Intent(ACTION_USB_PERMISSION), 0);
            IntentFilter filter = new IntentFilter(ACTION_USB_PERMISSION);
            registerReceiver(mUsbReceiver, filter);
            checkDevice();
        } else {
            //            Toast.makeText(getApplicationContext(), "USBManager: Permission not given", Toast.LENGTH_SHORT).show();
            mBioMiniFactory = new BioMiniFactory(mainContext) {
                @Override
                public void onDeviceChange(DeviceChangeEvent event, Object dev) {
                    log("----------------------------------------");
                    log("onDeviceChange : " + event);
                    log("----------------------------------------");
                    handleDevChange(event, dev);
                }
            };
        }
        //mBioMiniFactory.setTransferMode(IBioMiniDevice.TransferMode.MODE2);
    }

    @Override
    protected void onPostResume() {
        super.onPostResume();
    }

    @Override
    protected void onDestroy() {
        if (mBioMiniFactory != null) {
            mBioMiniFactory.close();
            mBioMiniFactory = null;
        }
        if (mbUsbExternalUSBManager) {
            unregisterReceiver(mUsbReceiver);
        }
        FingerPrint.fingerPrintPower(0);
        super.onDestroy();
    }

    public void setMenuPicker(int idx) {
        if (idx > mPager.getChildCount()) {
            return;
        }
        for (int i = 0; i < mNaviPicks.length; i++) {
            ImageView img_view = (ImageView) findViewById(mNaviPicks[i]);
            if (idx == i) {
                img_view.setImageResource(R.drawable.ic_place_grey600_48dp);
            } else {
                img_view.setImageResource(R.drawable.ic_pin_drop_grey_underbar);
            }
        }
    }

    public void clearState() {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                ((TextView) findViewById(R.id.tv)).clearComposingText();
            }
        });

    }

    private void requestPermission() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            requestPermissions(new String[]{android.Manifest.permission.WRITE_EXTERNAL_STORAGE}, REQUEST_WRITE_PERMISSION);
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        if (requestCode == REQUEST_WRITE_PERMISSION && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
            log("permission granted");
        }
    }

    @Override
    public void onPostCreate(Bundle savedInstanceState) {
        requestPermission();
        super.onPostCreate(savedInstanceState);
    }

    private long exitTime = 0;

    public void doExitApp() {
        if ((System.currentTimeMillis() - exitTime) > 1500) {
            Tools.toast("Press again to CANCEL enrollment", Verify.this);
            exitTime = System.currentTimeMillis();
        } else {
            finishAffinity();
        }
    }

    @Override
    public void onBackPressed() {
        goingBack();
    }

    public void goingBack() {
        AlertDialog.Builder builder
                = new AlertDialog
                .Builder(Verify.this);

        // Set the message show for the Alert time
        builder.setMessage("Are you done?");

        // Set Alert Title
        builder.setTitle("Alert !");

        // Set Cancelable false
        // for when the user clicks on the outside
        // the Dialog Box then it will remain show
        builder.setCancelable(true);

        // Set the positive button with yes name
        // OnClickListener method is use of
        // DialogInterface interface.

        builder
                .setPositiveButton("Yes",
                        (dialog, which) -> finish());

        builder
                .setNegativeButton(
                        "No",
                        (dialog, which) -> {

                            // If user click no
                            // then dialog box is canceled.
                            dialog.cancel();
                        });

        // Create the Alert dialog
        AlertDialog alertDialog = builder.create();

        // Show the Alert Dialog box
        alertDialog.show();

    }

    public void upload(View v) {
        if (storageFile.fingerPrint.getAllFingerprints().size() > 4) {
            AlertDialog.Builder builder
                    = new AlertDialog
                    .Builder(Verify.this);

            // Set the message show for the Alert time
            builder.setMessage("Are you done and want to upload?");

            // Set Alert Title
            builder.setTitle("Alert !");

            // Set Cancelable false
            // for when the user clicks on the outside
            // the Dialog Box then it will remain show
            builder.setCancelable(false);

            // Set the positive button with yes name
            // OnClickListener method is use of
            // DialogInterface interface.

            builder
                    .setPositiveButton("Yes",
                            (dialog, which) -> upload());

            builder
                    .setNegativeButton(
                            "No",
                            (dialog, which) -> {

                                // If user click no
                                // then dialog box is canceled.
                                dialog.cancel();
                            });

            // Create the Alert dialog
            AlertDialog alertDialog = builder.create();

            // Show the Alert Dialog box
            alertDialog.show();

        } else {
            Toast.makeText(getApplicationContext(), "Capture a minimum of 5 fingers", Toast.LENGTH_SHORT).show();
        }


    }

    @Override
    protected void onPause() {
        super.onPause();
    }

    public void upload() {
        report.setText("Uploading.....");
        img.setImageResource(R.drawable.processing);
        fingerprints_array = storageFile.fingerPrint.getAllFingerprints();
        fingerprints_images_array = storageFileImages.fingerPrintImages.getAllFingerprintsImages();
        Log.d("fingerprint", "Number of fingerprints = " + fingerprints_array.size());
        Log.d("fingerprint", "Number of fingerprints Images= " + fingerprints_images_array.size());
        //retrofit

        OkHttpClient client = new OkHttpClient.Builder().addInterceptor(chain -> {
            Request newRequest = chain.request().newBuilder()
                    .addHeader("Authorization", "Bearer " + token)
                    .build();
            return chain.proceed(newRequest);
        }).build();

        Retrofit retrofit = new Retrofit.Builder().client(client)
                .baseUrl(fromSharedPref)
                .addConverterFactory(GsonConverterFactory.create()).build();
        NAIC service = retrofit.create(NAIC.class);

        Log.d("fingerprint", "NAIC NUMBER: " + bippiis_number);
        FingerprintRequest fingerprintRequest = new FingerprintRequest();
        //fingerprintRequest.setBippiis_number(bippiis_number_edited);
//        fingerprintRequest.set.setBippiis_number(bippiis_number);

        fingerprintRequest.setFingerprints(storageFile.fingerPrint.allFingerprints);
        fingerprintRequest.setFingerprintsImages(storageFileImages.fingerPrintImages.allFingerprintsImages);
        Log.d("fingerprint", "fingerprintRequest ArrayListString: " + storageFile.fingerPrint.allFingerprints.toArray().toString());
        Log.d("fingerprint", "fingerprintRequest Number of fingers: " + storageFile.fingerPrint.allFingerprints.size());

        Log.d("fingerprint", "fingerPrintImagesResponse ArrayListString: " + storageFileImages.fingerPrintImages.allFingerprintsImages.toArray().toString());
        Log.d("fingerprint", "fingerPrintImagesResponse Number of fingersImages: " + storageFileImages.fingerPrintImages.allFingerprintsImages.size());


        Call<FingerprintResponse> fingerprintResponseCall = service.getFingerprintResponse(fingerprintRequest);
        fingerprintResponseCall.enqueue(new Callback<FingerprintResponse>() {
            @Override
            public void onResponse
                    (Call<FingerprintResponse> call, Response<FingerprintResponse> response) {
                // TODO: still need to catch errors properly from accurate response filters

                FingerprintResponse fingerPrintResponse = response.body();
                try {
                    Log.d("fingerprint", "fingerPrintResponse success: " + response.isSuccessful());
                    Log.d("fingerprint", "fingerPrintResponse RESPONSE: " + response.toString());
                    //                    Log.d("fingerprint", "fingerPrintResponse TOKEN: " + response.body().getToken());
                    //                    Log.d("fingerprint", "fingerPrintResponse BODY: " + response.body().toString());


                    //TODO: remove later
                    Log.d("fingerprint", "fingerPrintResponse RESPONSE Error Body: " + response.errorBody().toString());
                    Log.d("fingerprint", "fingerPrintResponse RESPONSE Error Body: " + response.errorBody().source().readUtf8());
                } catch (Exception e) {
                    e.printStackTrace();
                }

                //validate response
                try {
                    if (fingerPrintResponse.getStatus().equalsIgnoreCase("success")) {
                        //    uploaded = true; //end of retrofit
                        report.setText("All Done.");
                        Log.d("fingerprint", "Uploaded successfully " + response);
                        Log.d("fingerprint", "Response: " + response.body().getData().toString());
                        String access_token = fingerPrintResponse.getToken();

                        Log.d("fingerprint", "Response: token " + access_token);

                        token = access_token;
                        uploaded = true;
                        startActivity(new Intent(getApplicationContext(), CameraCapture.class).putExtra("token", token));
                    } else {
                        // go to login then camera
                        img.setImageResource(R.drawable.unsuccessful);
                        report.setTextColor(getResources().getColor(R.color.colorAccent));
                        report.setText("Something went wrong. Click on retry");
                    }
                } catch (NullPointerException n) {
                    n.printStackTrace();
                    img.setImageResource(R.drawable.unsuccessful);
                    report.setTextColor(getResources().getColor(R.color.colorAccent));
                    report.setText("Upload Failed. Click on upload again");
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }

            @Override
            public void onFailure(Call<FingerprintResponse> call, Throwable t) {
                report.setTextColor(getResources().getColor(R.color.colorAccent));
                report.setText("Upload Failed. Click on retry");
                img.setImageResource(R.drawable.unsuccessful);
                uploaded = false;
                Log.d("fingerprint", "Failed to Upload");
            }
        });


    }

    //DONE
    public void fab(View view) {
        // do something
        startActivity(new Intent(getApplicationContext(), Verify.class));

        //        if (mCurrentDevice != null) {
        //            new Thread(new Runnable() {
        //                @Override
        //                public void run() {
        //                    mCurrentDevice.abortCapturing();
        //                    int nRetryCount = 0;
        //                    while (mCurrentDevice != null && mCurrentDevice.isCapturing()) {
        //                        SystemClock.sleep(10);
        //                        nRetryCount++;
        //                    }
        //                    Log.d("AbortCapturing", String.format(Locale.ENGLISH,
        //                            "IsCapturing return false.(Abort-lead time: %dms) ",
        //                            nRetryCount * 10));
        //                }
        //            }).start();
        //        }
    }


    /**
     * Get grayscale data from argb image to byte array
     */
    public static byte[] ARGB2Gray(Bitmap img) {

        int width = img.getWidth();
        int height = img.getHeight();

        int[] pixels = new int[height * width];
        byte grayIm[] = new byte[height * width];

        img.getPixels(pixels, 0, width, 0, 0, width, height);

        int pixel = 0;
        int count = width * height;

        while (count-- > 0) {
            int inVal = pixels[pixel];

            //Get the pixel channel values from int
            double r = (double) ((inVal & 0x00ff0000) >> 16);
            double g = (double) ((inVal & 0x0000ff00) >> 8);
            double b = (double) (inVal & 0x000000ff);

            grayIm[pixel++] = (byte) (0.2989 * r + 0.5870 * g + 0.1140 * b);
        }

        return grayIm;
    }

    public void LoadBmp(Bitmap bitmap, String filename) {
        String funcName = "LoadBmp";
        try {
            GbExampleGrayScaleBitmapClass bmpToShow = new GbExampleGrayScaleBitmapClass();
            boolean loaded = bmpToShow.GbBmpFromBmpFile(filename, true, true);
            //  boolean loaded = bmpToShow.GbBmpFromBmp(bitmap, true, true);
            if (loaded) {
                //                LogBitmap(bmpToShow);
                LogAsDialog(funcName + ": Loaded from bmp");
                bmpToShow.TestLfsBozorth1();
            } else {
                LogAsDialog(funcName + ": Failure in open bmp");
            }
        } catch (Exception ex) {
            LogAsDialog(funcName + ": " + ex.getMessage());
        }
    }

}
