package com.example.huyuxuan.lora2.Background;

import android.app.AlarmManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.os.SystemClock;
import android.support.v4.content.WakefulBroadcastReceiver;
import android.util.Log;

import static android.content.Context.MODE_PRIVATE;

/**
 * Created by huyuxuan on 2017/4/26.
 */

public class MyAlarmReceiver extends WakefulBroadcastReceiver {
    private AlarmManager alarmManager;
    private PendingIntent pendingIntent;

    static private int serviceCounter;
    private BgServiceRecver mServiceBroadcastReceiver;
    private static final String ACTION_RECV_SER_BROD = "com.example.huyuxuan.lora.RECV_SERVER_BROADCAST";
    private SharedPreferences sharedPreferences;

    @Override
    public void onReceive(Context context, Intent intent) {
        Log.i("MyAlarmReceiver","onReceive");
        sharedPreferences = context.getSharedPreferences("data" , MODE_PRIVATE);
        serviceCounter = sharedPreferences.getInt("BGServiceCount",0);
        Log.d("MyAlatmReceiver","serviceCounter="+serviceCounter);
        if(serviceCounter == 0){
            Intent service = new Intent(context, BackgroundRecvService.class);
            startWakefulService(context, service);
            serviceCounter++;
            sharedPreferences.edit().putInt("BGServiceCount",serviceCounter).apply();
        }
    }

    public void setAlarm(Context context) {
        Log.i("MyAlarmReceiver","setAlarm");
        alarmManager = (AlarmManager) context.getSystemService(Context.ALARM_SERVICE);
        Intent intent = new Intent(context, MyAlarmReceiver.class);
        pendingIntent = PendingIntent.getBroadcast(context, 0, intent, 0);

        alarmManager.setRepeating(AlarmManager.ELAPSED_REALTIME_WAKEUP,
                SystemClock.elapsedRealtime(), 10, pendingIntent);

        // Enable {@code MyBootReceiver} to automatically restart the alarm when the
        // device is rebooted.
        ComponentName receiver = new ComponentName(context, BackgroundReceiver.class);
        PackageManager pm = context.getPackageManager();

        pm.setComponentEnabledSetting(receiver,
                PackageManager.COMPONENT_ENABLED_STATE_ENABLED,
                PackageManager.DONT_KILL_APP);

        //动态注册receiver
        IntentFilter filter = new IntentFilter(ACTION_RECV_SER_BROD);
        filter.addCategory(Intent.CATEGORY_DEFAULT);
        mServiceBroadcastReceiver = new BgServiceRecver();
        context.getApplicationContext().registerReceiver(mServiceBroadcastReceiver, filter);
        Log.d("MyAlarmReceiver","register receiver");
    }
    /*
    public void cancelAlarm(Context context) {
        if (alarmManager != null) {
            alarmManager.cancel(pendingIntent);
        }

        // Disable {@code MyBootReceiver} so that it doesn't automatically restart the
        // alarm when the device is rebooted.
        ComponentName receiver = new ComponentName(context, BackgroundReceiver.class);
        PackageManager pm = context.getPackageManager();

        pm.setComponentEnabledSetting(receiver,
                PackageManager.COMPONENT_ENABLED_STATE_DISABLED,
                PackageManager.DONT_KILL_APP);
    }
    */

    public class BgServiceRecver extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {

            String state = intent.getStringExtra("state");
            // 如果傳回成功
            if(state.equals("true")){
                Log.d("BgServiceReceiver:","state-> "+ state);
                //開啟notification
                serviceCounter--;
                sharedPreferences.edit().putInt("BGServiceCount",serviceCounter).apply();
            }

        }
    }
}
