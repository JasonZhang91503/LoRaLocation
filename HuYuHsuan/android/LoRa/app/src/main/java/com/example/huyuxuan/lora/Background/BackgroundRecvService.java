package com.example.huyuxuan.lora.Background;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.support.annotation.Nullable;

/**
 * Created by huyuxuan on 2017/3/19.
 */

public class BackgroundRecvService extends Service {

    private static final String ACTION_RECV_MSG = "com.example.huyuxuan.lora.intent.action.RECEIVE_MESSAGE";
    Boolean flag = false;


    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public int onStartCommand(final Intent intent, int flags, int startId){


        //接收server傳來的東西





        //接收完
        Intent broadcastIntent = new Intent();
        broadcastIntent.setAction(ACTION_RECV_MSG);
        broadcastIntent.addCategory(Intent.CATEGORY_DEFAULT);
        broadcastIntent.putExtra("result", flag.toString());
        broadcastIntent.putExtra("state","true");
        broadcastIntent.putExtra("service","BackgroundRecvService");
        sendBroadcast(broadcastIntent);


        return START_STICKY;
    }
}
