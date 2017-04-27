package com.example.huyuxuan.lora2.Background;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.util.Log;

import com.example.huyuxuan.lora2.R;

import static android.content.Context.MODE_PRIVATE;


/**
 * Created by huyuxuan on 2017/4/26.
 */

public class BackgroundReceiver extends BroadcastReceiver {
    MyAlarmReceiver alarm = new MyAlarmReceiver();
    private SharedPreferences sharedPreferences;

    @Override
    public void onReceive(Context context, Intent intent) {
        sharedPreferences = context.getSharedPreferences("data" , MODE_PRIVATE);

        if (intent.getAction().equals(Intent.ACTION_USER_PRESENT)
                || intent.getAction().equals(Intent.ACTION_BOOT_COMPLETED)) {
            if(sharedPreferences.getString(context.getString(R.string.isLogin),"false")=="true"){
                //如果已經登入才開啟backgroundService
                Log.i("BackgroundRecver", "Receiver Action: Boot Completed");
                alarm.setAlarm(context);
                Log.d("BackgroundRecver","setAlarm.");
            }
        }
    }
}
