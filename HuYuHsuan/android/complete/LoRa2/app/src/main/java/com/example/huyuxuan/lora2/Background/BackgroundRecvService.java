package com.example.huyuxuan.lora2.Background;

import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.app.TaskStackBuilder;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.AsyncTask;
import android.os.IBinder;
import android.os.PowerManager;
import android.support.annotation.Nullable;
import android.support.v4.app.NotificationCompat;
import android.util.Log;

import com.example.huyuxuan.lora2.MainActivity;
import com.example.huyuxuan.lora2.MyBoundedService;
import com.example.huyuxuan.lora2.R;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;

/**
 * Created by huyuxuan on 2017/4/26.
 */

public class BackgroundRecvService extends Service {
    private static final String ACTION_RECV_SER_BROD = "com.example.huyuxuan.lora.RECV_SERVER_BROADCAST";
    Boolean flag = false;
    private final int notifyId = 1;
    PowerManager.WakeLock mWakeLock;

    static Socket mSocket;
    InetAddress serverAddr;
    SocketAddress sc_add;
    static BufferedReader in;
    static BufferedWriter out;

    String rcvMessage;
    private SharedPreferences sharedPreferences;
    NotificationCompat.Builder mBuilder;

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        acquireWakeLock(1);
        Log.d("BGService","onCreate");
        sharedPreferences = getSharedPreferences("data" , MODE_PRIVATE);
        MyBoundedService.myBGService = BackgroundRecvService.this;

        new AsyncTask<String,String,String>() {
            @Override
            protected String doInBackground(String... strings) {
                try {
                    serverAddr = InetAddress.getByName(getString(R.string.ip));
                    mSocket = new Socket();
                    sc_add = new InetSocketAddress(serverAddr, Integer.parseInt(getString(R.string.port)));
                    if (mSocket.isConnected()) {
                        Log.i("BGRService", "Socket Connected");
                    } else {
                        mSocket.connect(sc_add, 2000);
                    }
                    in = new BufferedReader(new InputStreamReader(mSocket.getInputStream(), "utf8"));
                    out = new BufferedWriter(new OutputStreamWriter(mSocket.getOutputStream(), "utf8"));
                    Log.i("BGRService", "BufferedReader and PrintWriter ready.");
                    String account = sharedPreferences.getString(getString(R.string.account),"");
                    String password = sharedPreferences.getString(getString(R.string.password),"");
                    out.write("3");
                    out.flush();
                    Log.d("Service", "write 3 to server");
                    out.write(account+","+password+",");
                    out.flush();
                    Log.d("Service", "write "+account+","+password+", to server");
                    if(in != null){
                        rcvMessage = in.readLine();
                        rcvMessage.concat("\0");
                        Log.d("BGRService", "receive " + rcvMessage + " from server");
                        if(rcvMessage!=null){
                            analyze(rcvMessage);
                        }
                    }
                } catch (IOException e) {
                    sharedPreferences.edit().putInt("BGServiceCount",0).apply();
                    BackgroundRecvService.this.stopSelf();
                    e.printStackTrace();
                }
                return null;
            }
        }.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);

    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        Log.d("BGService","onDestroy");
        //sharedPreferences.edit().putInt("BGServiceCount",0).apply();
        if(mWakeLock!=null){
            releaseWakeLock();
        }
    }

    @Override
    public int onStartCommand(final Intent intent, int flags, int startId){
        String account=sharedPreferences.getString(getString(R.string.account),"");
        if(account != null) {
            acquireWakeLock(1);
            Log.d("BGService", "onStartCommand");
            sharedPreferences.edit().putInt("BGServiceCount",1).apply();
            new AsyncTask<String, String, String>() {
                @Override
                protected String doInBackground(String... strings) {

                    try {
                        ensureConnected();
                        if (in != null) {
                            rcvMessage = in.readLine();
                            rcvMessage.concat("\0");
                            Log.d("BGRService", "receive " + rcvMessage + " from server");
                            if (rcvMessage != null) {
                                analyze(rcvMessage);

                            }
                        }
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                    return null;
                }

            }.execute();

            releaseWakeLock();
        }
        return START_STICKY;
    }

    private void analyze(String msg){
        char type = msg.charAt(0);
        if(type=='2'){
            String tmp = msg.substring(0,3);
            if(tmp.equals("2,1")){
                mBuilder =
                        new NotificationCompat.Builder(this)
                                .setSmallIcon(R.drawable.delivery_icon)
                                .setContentTitle(getString(R.string.app_name))
                                .setContentText("車子到了下來寄信")
                                .setAutoCancel(true);
                createSimleNotification();
                //接收完
                Intent broadcastIntent = new Intent(ACTION_RECV_SER_BROD);
                broadcastIntent.addCategory(Intent.CATEGORY_DEFAULT);
                broadcastIntent.putExtra("result", flag.toString());
                broadcastIntent.putExtra("state","true");
                sendBroadcast(broadcastIntent);
            }else if(tmp.equals("2,0")){
                mBuilder =
                        new NotificationCompat.Builder(this)
                                .setSmallIcon(R.drawable.delivery_icon)
                                .setContentTitle(getString(R.string.app_name))
                                .setContentText("車子到了下來收信")
                                .setAutoCancel(true);
                createSimleNotification();
                //接收完
                Intent broadcastIntent = new Intent(ACTION_RECV_SER_BROD);
                broadcastIntent.addCategory(Intent.CATEGORY_DEFAULT);
                broadcastIntent.putExtra("result", flag.toString());
                broadcastIntent.putExtra("state","true");
                sendBroadcast(broadcastIntent);
            }else{
                Log.e("BGService","接收到的訊息格式錯誤");
            }
            if(!mSocket.isOutputShutdown()){
                if(out!=null){
                    try {
                        out.write("2");
                        out.flush();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
                else{
                    try {
                        out = new BufferedWriter(new OutputStreamWriter(mSocket.getOutputStream(), "utf8"));
                        out.write("2");
                        out.flush();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        }else if(type == '1'){
            if(msg.equals("1,1")){
                Log.d("BGService","認證成功");
                //接收完
                Intent broadcastIntent = new Intent(ACTION_RECV_SER_BROD);
                broadcastIntent.addCategory(Intent.CATEGORY_DEFAULT);
                broadcastIntent.putExtra("result", flag.toString());
                broadcastIntent.putExtra("state","true");
                sendBroadcast(broadcastIntent);
            }else if(msg.equals("1,0")){
                Log.d("BGService","認證失敗");
                //接收完
                Intent broadcastIntent = new Intent(ACTION_RECV_SER_BROD);
                broadcastIntent.addCategory(Intent.CATEGORY_DEFAULT);
                broadcastIntent.putExtra("result", flag.toString());
                broadcastIntent.putExtra("state","false");
                sendBroadcast(broadcastIntent);
            }else{
                Log.e("BGService","接收到的訊息格式錯誤");
            }
        }
    }

    public void createSimleNotification(){
        //開啟notification

        //強迫螢幕亮起
        acquireWakeLock(2);


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
        releaseWakeLock();

    }

    private void acquireWakeLock(int type) {
        Log.i("BgService","正在申請電源鎖");
        if (mWakeLock == null) {
            PowerManager pm = (PowerManager) this.getSystemService(Context.POWER_SERVICE);
            if(type == 1){
                mWakeLock = pm.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK , "");
            }
            else{
                mWakeLock = pm.newWakeLock(PowerManager.ACQUIRE_CAUSES_WAKEUP | PowerManager.PROXIMITY_SCREEN_OFF_WAKE_LOCK , "");
            }
            if (null != mWakeLock) {
                mWakeLock.acquire();
                Log.e("BgService","電源鎖申請成功");
            }
        }
    }

    public void releaseWakeLock(){
        Log.i("BgService","正在釋放電源鎖");
        if (null != mWakeLock) {
            mWakeLock.release();
            mWakeLock = null;
            Log.e("BgService","電源鎖釋放成功");
        }
        sharedPreferences.edit().putInt("BGServiceCount",0).apply();
    }

    private void ensureConnected(){
        Log.e("BgService","ensureConnected");
        try {

                if(mSocket==null){
                    mSocket=new Socket();
                }
                if (mSocket.isConnected()) {
                    Log.i("BgService", "Socket Connected");
                }else if(mSocket.isClosed()){
                    Log.i("BgService", "Socket closed reconnect");
                    mSocket = new Socket();
                    mSocket.connect(sc_add, 2000);
                    in = new BufferedReader(new InputStreamReader(mSocket.getInputStream(), "utf8"));
                    out = new BufferedWriter(new OutputStreamWriter(mSocket.getOutputStream(), "utf8"));
                }else{
                    Log.e("EnsureConnected","not connected and not closed");
                }


        }catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void disconnect(){
        try {
            if(mSocket.isConnected()){
                Log.d("BGService","socket close");
                releaseWakeLock();
                mSocket.close();
                sharedPreferences.edit().putInt("BGServiceCount",0).apply();
                MyBoundedService.myBGService=null;
                this.stopSelf();
            }
        }catch (IOException e){
            e.printStackTrace();
        }
    }

}
