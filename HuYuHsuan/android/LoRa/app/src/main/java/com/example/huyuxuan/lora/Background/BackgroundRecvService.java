package com.example.huyuxuan.lora.Background;

import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.app.TaskStackBuilder;
import android.content.Context;
import android.content.Intent;
import android.os.IBinder;
import android.os.PowerManager;
import android.support.annotation.Nullable;
import android.support.v4.app.NotificationCompat;
import android.util.Log;

import com.example.huyuxuan.lora.MainActivity;
import com.example.huyuxuan.lora.R;

/**
 * Created by huyuxuan on 2017/3/19.
 */

public class BackgroundRecvService extends Service {

    private static final String ACTION_RECV_MSG = "com.example.huyuxuan.lora.intent.action.RECEIVE_MESSAGE";
    Boolean flag = false;
    private final int notifyId = 1;
    PowerManager.WakeLock mWakeLock;

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        acquireWakeLock(1);
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        releaseWakeLock();
    }

    @Override
    public int onStartCommand(final Intent intent, int flags, int startId){


        //接收server傳來的東西
        Log.e("BackgroundRecvService:","已經接收到server的東西了");



        //開啟通知
        createSimleNotification();
        //強迫螢幕亮起
        acquireWakeLock(2);

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

    private void createSimleNotification(){
        //開啟notification
        NotificationCompat.Builder mBuilder =
                new NotificationCompat.Builder(this)
                        .setSmallIcon(R.drawable.ic_launcher)
                        .setContentTitle("My notification")
                        .setContentText("Hello World!")
                        .setAutoCancel(true);
        // Creates an explicit intent for an Activity in your app
        Intent resultIntent = new Intent(this, MainActivity.class);

        // The stack builder object will contain an artificial back stack for the
        // started Activity.
        // This ensures that navigating backward from the Activity leads out of
        // your application to the Home screen.
        TaskStackBuilder stackBuilder = TaskStackBuilder.create(this);
        // Adds the back stack for the Intent (but not the Intent itself)
        stackBuilder.addParentStack(MainActivity.class);
        // Adds the Intent that starts the Activity to the top of the stack
        stackBuilder.addNextIntent(resultIntent);
        PendingIntent resultPendingIntent =
                stackBuilder.getPendingIntent(
                        0,
                        PendingIntent.FLAG_UPDATE_CURRENT
                );
        mBuilder.setContentIntent(resultPendingIntent);
        NotificationManager mNotificationManager =
                (NotificationManager) getSystemService(Context.NOTIFICATION_SERVICE);
        // mId allows you to update the notification later on.
        mNotificationManager.notify(notifyId, mBuilder.build());


    }

    private void acquireWakeLock(int type) {
        Log.i("BgService","正在申請電源鎖");
        if (mWakeLock == null) {
            PowerManager pm = (PowerManager) this.getSystemService(Context.POWER_SERVICE);
            if(type == 1){
                mWakeLock = pm.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK , "");
            }
            else{
                mWakeLock = pm.newWakeLock(PowerManager.ACQUIRE_CAUSES_WAKEUP , "");
            }
            if (null != mWakeLock) {
                mWakeLock.acquire();
                Log.e("BgService","電源鎖申請成功");
            }
         }
    }

    private void releaseWakeLock(){
        Log.i("BgService","正在釋放電源鎖");
        if (null != mWakeLock) {
            mWakeLock.release();
            mWakeLock = null;
            Log.e("BgService","電源鎖釋放成功");
        }
    }


}
