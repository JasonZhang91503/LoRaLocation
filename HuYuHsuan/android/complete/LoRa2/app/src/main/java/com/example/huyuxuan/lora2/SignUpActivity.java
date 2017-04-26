package com.example.huyuxuan.lora2;

import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.IBinder;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

/**
 * Created by huyuxuan on 2017/4/26.
 */

public class SignUpActivity extends AppCompatActivity {
    protected EditText editTextAccount;
    protected EditText editTextPassword;
    protected Button btnLogin;
    protected Button btnSignUp;

    protected String account;
    protected String password;

    static boolean isBind;

    static ConnectService mBoundService;
    private ConnectServiceReceiver receiver;
    private static final String ACTION_RECV_MSG = "com.example.huyuxuan.lora.intent.action.RECEIVE_MESSAGE";

    //用來存程式關掉之後還要存在的資料、狀態
    private SharedPreferences sharedPreferences;

    @Override
    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        //setcontextView

        editTextAccount = (EditText)findViewById(R.id.editTextAccount);
        editTextPassword = (EditText)findViewById(R.id.editTextPassword);

        isBind=false;

        btnLogin.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                //跳到登入畫面
            }
        });

        btnSignUp.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                //抓資料

            }
        });

    }

    //接收广播类
    public class ConnectServiceReceiver extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
            if(intent.getStringExtra("activity").equals("SignUpActivity")){
                unregisterReceiver(receiver);
                Bundle bundle = intent.getExtras();
                String type = bundle.getString(getString(R.string.type));

            }
        }
    }

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
            Log.d("SignUpActivity","onServiceDisconnected");
            //mBoundService = null;
            isBind=false;
        }

    };

    private void setReceiver(){
        //动态注册receiver
        IntentFilter filter = new IntentFilter(ACTION_RECV_MSG);
        filter.addCategory(Intent.CATEGORY_DEFAULT);
        receiver = new ConnectServiceReceiver();
        registerReceiver(receiver, filter);
        Log.d("SignUpActivity:","register receiver");
    }
}
