package com.greenbit.MultiscanJNIGuiJavaAndroid.broadcastreceiver;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

import com.greenbit.MultiscanJNIGuiJavaAndroid.services.MyService;

/**
 * Created by TECH VIBES on 01/01/2017.
 */
public class AutoStart extends BroadcastReceiver {
    @Override
    public void onReceive(Context cx, Intent im)
    {cx.startService(new Intent(cx, MyService.class));}
}
