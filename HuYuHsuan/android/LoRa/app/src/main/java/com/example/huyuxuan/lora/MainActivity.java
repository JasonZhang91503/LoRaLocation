package com.example.huyuxuan.lora;

import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.TaskStackBuilder;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.IBinder;
import android.support.v4.app.NotificationCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import com.example.huyuxuan.lora.Background.MyAlarmReceiver;

public class MainActivity extends AppCompatActivity {

    private Button btnLogin;
    private Button btnSignUp;
    private Button btnApplySend;
    private Button btnCheckCar;
    private Button btnCheckSend;
    private Button btnCheckReceive;
    private Button btnCheckSR;
    private Button btnSetAlarm;


    
    static ConnectService mBoundService;
    static boolean isBind;
    MyAlarmReceiver alarm = new MyAlarmReceiver();

    private ConnectServiceReceiver receiver;
    private static final String ACTION_RECV_MSG = "com.example.huyuxuan.lora.intent.action.RECEIVE_MESSAGE";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        btnLogin = (Button) findViewById(R.id.btnLogin);
        btnSignUp = (Button) findViewById(R.id.btnSignUp);
        btnApplySend = (Button)findViewById(R.id.btnApplySend);
        btnCheckCar = (Button)findViewById(R.id.btnCheckCar);
        btnCheckSend = (Button)findViewById(R.id.btnCheckSend);
        btnCheckReceive = (Button)findViewById(R.id.btnCheckReceive);
        btnCheckSR = (Button)findViewById(R.id.btnCheckSR);
        btnSetAlarm = (Button)findViewById(R.id.btnSetAlarm);

        isBind = false;


        //登入
        btnLogin.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(MainActivity.this,ConnectService.class);
                intent.putExtra(getString(R.string.id),"3");
                intent.putExtra(getString(R.string.account),"dfdf");
                intent.putExtra(getString(R.string.password),"1123");

                if(!isBind){
                    getApplicationContext().bindService(intent,mConnection,Context.BIND_AUTO_CREATE);
                    isBind=true;
                    Log.d("MainActivity:", "login->bind");
                }
                else{
                    mBoundService.sendToServer(intent);
                    Log.d("MainActivity:", "login->sendToService");
                }

            }
        });

        //註冊
        btnSignUp.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(MainActivity.this,ConnectService.class);
                intent.putExtra(getString(R.string.id), "2");
                intent.putExtra(getString(R.string.account), "dfdf");
                intent.putExtra(getString(R.string.password), "1123");
                intent.putExtra(getString(R.string.name), "huhu");
                intent.putExtra(getString(R.string.email), "ssdd@gmail.com");

                if(!isBind){
                    getApplicationContext().bindService(intent,mConnection,Context.BIND_AUTO_CREATE);
                    isBind=true;
                    Log.d("MainActivity:", "apply->bind");
                }
                else{
                    mBoundService.sendToServer(intent);
                    Log.d("MainActivity:", "sign->sendToService");
                }
            }
        });

        //登記寄件
        btnApplySend.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(MainActivity.this,ConnectService.class);
                intent.putExtra(getString(R.string.id),"4");
                intent.putExtra(getString(R.string.requireTime),"2017-03-15 10:30:00");
                intent.putExtra(getString(R.string.sender),"我是寄件者");
                intent.putExtra(getString(R.string.receiver),"我是收件者");
                intent.putExtra(getString(R.string.startLocation),"我是寄件地Id");
                intent.putExtra(getString(R.string.desLocation),"我是收件地Id");

                if(!isBind){
                    getApplicationContext().bindService(intent,mConnection,Context.BIND_AUTO_CREATE);
                    isBind=true;
                    Log.d("MainActivity:", "apply->bind");
                }
                else{
                    if(mBoundService == null){
                        Log.e("MainActivity","mBoundService is null");
                    }
                    else{
                        Log.d("MainActivity:", "apply->sendToService");
                        mBoundService.sendToServer(intent);
                    }

                }
            }
        });

        //詢問車子有空時間
        btnCheckCar.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(MainActivity.this,ConnectService.class);
                intent.putExtra(getString(R.string.id),"5");
                intent.putExtra(getString(R.string.requireTime),"2017-03-15");

                Log.d("check:","isBind="+isBind);
                if(!isBind){
                    getApplicationContext().bindService(intent,mConnection,Context.BIND_AUTO_CREATE);
                    isBind=true;
                    Log.d("MainActivity:", "checkCar->bind");
                }
                else{
                    mBoundService.sendToServer(intent);
                    Log.d("MainActivity:", "checkCar->sendToService");
                }
            }
        });

        //詢問寄件資料
        btnCheckSend.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(MainActivity.this,ConnectService.class);
                intent.putExtra(getString(R.string.id),"7");
                intent.putExtra(getString(R.string.requireTime),"2017-03-15");

                if(!isBind){
                    getApplicationContext().bindService(intent,mConnection,Context.BIND_AUTO_CREATE);
                    isBind=true;
                    Log.d("MainActivity:", "checkSend->bind");
                }
                else{
                    mBoundService.sendToServer(intent);
                    Log.d("MainActivity:", "checkSend->sendToService");
                }
            }
        });

        btnCheckReceive.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(MainActivity.this,ConnectService.class);
                intent.putExtra(getString(R.string.id),"8");
                intent.putExtra(getString(R.string.requireTime),"2017-03-15");

                if(!isBind){
                    getApplicationContext().bindService(intent,mConnection,Context.BIND_AUTO_CREATE);
                    isBind=true;
                    Log.d("MainActivity:", "checkRcv->bind");
                }
                else{
                    mBoundService.sendToServer(intent);
                    Log.d("MainActivity:", "checkRcv->sendToService");
                }
            }
        });

        btnCheckSR.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(MainActivity.this,ConnectService.class);
                intent.putExtra(getString(R.string.id),"9");
                intent.putExtra(getString(R.string.requireTime),"2017-03-15");


                if(!isBind){
                    getApplicationContext().bindService(intent,mConnection,Context.BIND_AUTO_CREATE);
                    isBind=true;
                    Log.d("MainActivity:", "checkSR->bind");
                }
                else{
                    mBoundService.sendToServer(intent);
                    Log.d("MainActivity:", "checkSR->sendToService");
                }
            }
        });

        btnSetAlarm.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                alarm.setAlarm(MainActivity.this);
            }
        });

        //动态注册receiver
        IntentFilter filter = new IntentFilter(ACTION_RECV_MSG);
        filter.addCategory(Intent.CATEGORY_DEFAULT);
        receiver = new ConnectServiceReceiver();
        registerReceiver(receiver, filter);
    }

    @Override
    protected void onDestroy(){
        super.onDestroy();
        unregisterReceiver(receiver);
    }

    @Override
    protected void onResume(){
        super.onResume();
        IntentFilter filter = new IntentFilter(ACTION_RECV_MSG);
        filter.addCategory(Intent.CATEGORY_DEFAULT);
        receiver = new ConnectServiceReceiver();
        registerReceiver(receiver, filter);
    }

    //接收广播类
    public class ConnectServiceReceiver extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
            //String result = intent.getStringExtra("result");
            if(intent.getStringExtra("service").equals("ConnectService")){
                String message = intent.getStringExtra("message");
                // 如果傳回成功
                if(message.equals("true")){
                    Log.d("ConnectServiceReceiver:","receive-> "+ message);
                }
            }
        }
    }

    //
    private static ServiceConnection mConnection = new ServiceConnection() {
        //EDITED PART
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            // TODO Auto-generated method stub
            mBoundService = ((ConnectService.LocalBinder)service).getService();
            isBind=true;
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            // TODO Auto-generated method stub
            Log.d("MainActivity","onServiceDisconnected");
            //mBoundService = null;
            isBind=false;
        }

    };


}