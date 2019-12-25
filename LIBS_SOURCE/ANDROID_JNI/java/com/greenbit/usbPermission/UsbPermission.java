package com.greenbit.usbPermission;

import java.util.ArrayList;

import android.app.Activity;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbManager;
import android.util.Log;
import android.net.Uri;
import android.media.MediaScannerConnection;

public class UsbPermission {
    private static final String ACTION_USB_PERMISSION_SUFIX =
            ".USB_PERMISSION";
    private static String ACTION_USB_PERMISSION = "";

    public static void SetActionUsbPermissionString(String packageAndNameOfTheAppRequesting)
    {
        ACTION_USB_PERMISSION = packageAndNameOfTheAppRequesting + ACTION_USB_PERMISSION_SUFIX;
    }

    private static UsbManager mUsbManager;
    private static PendingIntent mPermissionIntent;
    private static ArrayList<UsbDevice> GBDeviceList = new ArrayList<UsbDevice>();
    private static int GBDeviceIndex = 0;
    private static Context context; // save the context
    private static IGreenbitLogger MyMainActivity; // save the activity
    private static int NumFD = 0;
    private static int[] OpenedFD = new int[10];
    private static int[] DeviceID = new int[10];
    private static UsbDeviceConnection[] OpenedConnections = new UsbDeviceConnection[10];
    private static int NumDevices = 0;
    private static int NumFinished = 0;

    private static final BroadcastReceiver mUsbReceiver = new BroadcastReceiver()
    {
        public void onReceive(Context context, Intent intent)
        {
            String action = intent.getAction();
            if (ACTION_USB_PERMISSION.equals(action))
            {
                synchronized (this)
                {
                    UsbDevice device =
                            (UsbDevice)intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);

                    Log.d("onReceive", "Received permission request for device " + device.getDeviceName());

                    if (intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false))
                    {
                        if(device != null)
                        {
                            openDevice ( device );

                            // request permission for next device
                            if (GBDeviceIndex < (GBDeviceList.size() - 1))
                            {
                                String logStr;
                                GBDeviceIndex++;
                                UsbDevice dev = GBDeviceList.get(GBDeviceIndex);
                                MyMainActivity.LogOnScreen("Usb Receive: requestPermission for next device: " + dev.getDeviceName());
                                Log.d("Usb Receive", "requestPermission for next device: " + dev.getDeviceName());
                                mUsbManager.requestPermission (dev, mPermissionIntent);
                            }
                        }
                    }
                    else
                    {
                        Log.d("Usb Receive", "permission denied for device " + device.getDeviceName());
                        MyMainActivity.LogOnScreen("Usb Receive: permission denied for device: " + device.getDeviceName());
                    }
                    NumFinished++;
                    Log.d("Usb Receive", "NumFinished = " + NumFinished);
                    MyMainActivity.LogOnScreen("Usb Receive: NumFinished = " + NumFinished);
                }
            }
        }
    };


    public static void SetMainActivity ( IGreenbitLogger act  )
    {
        // save the Activity for later uses...
        MyMainActivity = act;
    }

    //public static void LoadGBMSAPI ( Activity act  )
    public static void GetDevicesAndPermissions ( Activity act  )
    {
        NumFD = 0;
        NumDevices = 0;
        NumFinished = 0;
        GBDeviceList.clear();

        Context ctx = act.getApplicationContext();
        mUsbManager = ( UsbManager )ctx.getSystemService( Context.USB_SERVICE );

        ArrayList<UsbDevice> deviceList = new ArrayList<UsbDevice> ( mUsbManager.getDeviceList().values() );

        // register receiver
        mPermissionIntent = PendingIntent.getBroadcast(ctx, 0, new Intent(ACTION_USB_PERMISSION), 0);
        IntentFilter filter = new IntentFilter(ACTION_USB_PERMISSION);
        ctx.registerReceiver(mUsbReceiver, filter);

        for( UsbDevice dev : deviceList )
        {
            MyMainActivity.LogOnScreen("GetDevicesAndPerm: Device ID: " + dev.getDeviceId());
            MyMainActivity.LogOnScreen("GetDevicesAndPerm: Device name: " + dev.getDeviceName());
            Log.d("GetDevicesAndPerm", "Device ID: " + dev.getDeviceId());
            Log.d("GetDevicesAndPerm", "Device name: " + dev.getDeviceName());

            // filter Green Bit devices
            if (dev.getVendorId() == 6073)
            {
                // add to GB list
                MyMainActivity.LogOnScreen("GetDevicesAndPerm: Add device to GBDeviceList");
                Log.d("GetDevicesAndPerm", "Add device to GBDeviceList");
                GBDeviceList.add(dev);
                NumDevices++;
            }
        }
        Log.d("GetDevicesAndPerm", "NumDevices = " + NumDevices);
        MyMainActivity.LogOnScreen("GetDevicesAndPerm: NumDevices = " + NumDevices);

        // save context
        context = ctx;

        if (NumDevices != 0)
        {
            // request permission for first GB device
            GBDeviceIndex = 0;
            UsbDevice device = GBDeviceList.get(GBDeviceIndex);
            Log.d("GetDevicesAndPerm", "requestPermission for first device: " + device.getDeviceName());
            MyMainActivity.LogOnScreen("GetDevicesAndPerm: requestPermission for first device: " + device.getDeviceName());
            mUsbManager.requestPermission (device, mPermissionIntent);
        }
    }

    // Functions called from mainwindow.cpp to get opened FDs and devices IDs
    public static int GetNumOpenedFD ()
    {
        return NumFD;
    }
    public static int GetOpenedFD ( int Index)
    {
        return OpenedFD[Index];
    }
    public static int GetDeviceID ( int Index)
    {
        return DeviceID[Index];
    }

    private static void openDevice( UsbDevice device )
    {
        UsbDeviceConnection connection = mUsbManager.openDevice( device );
        int FD;
        FD = connection.getFileDescriptor();
        Log.d("openDevice","device: " + device.getDeviceName() + " fd: " + FD );

        // save opened FD and DeviceId
        OpenedFD[NumFD] = FD;
        DeviceID[NumFD] = device.getDeviceId();
        OpenedConnections[NumFD] = connection;
        NumFD++;
    }

    // called from mainwindow.cpp before LoadDevicesList and on application exit
    public static void CloseConnections()
    {
        int i;
        for (i=0; i<NumFD; i++)
        {
            Log.d( "CloseConnections", "Close connection: " + OpenedConnections[i].getFileDescriptor() );
            OpenedConnections[i].close();
        }

        NumFD = 0;
        NumDevices = 0;
        NumFinished = 0;
    }

    public static int GetUsbPermissionFinished()
    {
        return NumFinished;
    }

    public static int IsUSBPermissionFinished()
    {
        if (NumFinished == NumDevices)
            return 1;
        else
            return 0;
    }

    public static void AddFileToGallery(String FilePath)
    {
        MediaScannerConnection.scanFile(context,
                new String[]{FilePath},
                null,
                new MediaScannerConnection.OnScanCompletedListener() {
                    public void onScanCompleted(String path, Uri uri) {
                        Log.i("ExternalStorage", "Scanned " + path + ":");
                        Log.i("ExternalStorage", "-> uri=" + uri);
                    }
                });
    }
}
