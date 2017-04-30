package com.example.huyuxuan.lora.Background;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

/**
 * Created by huyuxuan on 2017/3/19.
 */

public class BackgroundReceiver extends BroadcastReceiver {

    MyAlarmReceiver alarm = new MyAlarmReceiver();

    @Override
    public void onReceive(Context context, Intent intent) {
        if (intent.getAction().equals(Intent.ACTION_USER_PRESENT)
                || intent.getAction().equals(Intent.ACTION_BOOT_COMPLETED)) {
            Log.i("BackgroundRecver", "Receiver Action: Boot Completed");
            alarm.setAlarm(context);
            Log.d("BackgroundRecver","setAlarm.");
        }

    }

}
