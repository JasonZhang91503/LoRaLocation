package com.sillywei.alarmdemo.scheduler;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

/**
 * Created by Silly Wei on 2017/1/17.
 */

public class MyBootReceiver extends BroadcastReceiver {
    MyAlarmReceiver alarm = new MyAlarmReceiver();

    @Override
    public void onReceive(Context context, Intent intent) {
        if (intent.getAction().equals("android.intent.action.BOOT_COMPLETED")) {
            alarm.setAlarm(context);
        }
    }
}
