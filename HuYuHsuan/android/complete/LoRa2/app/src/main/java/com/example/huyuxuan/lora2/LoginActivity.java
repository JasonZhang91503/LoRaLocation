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

public class LoginActivity extends AppCompatActivity {
    protected EditText editTextAccount;
    protected EditText editTextPassword;
    protected Button btnLogin;

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
        setContentView(R.layout.activity_login);
        Log.d("LoginActivity","onCreate");

        editTextAccount = (EditText)findViewById(R.id.editTextAccount);
        editTextPassword = (EditText)findViewById(R.id.editTextPassword);
        btnLogin = (Button)findViewById(R.id.btnLogin);
        sharedPreferences = getSharedPreferences("data" , MODE_PRIVATE);
        editTextAccount.setHintTextColor(getResources().getColor(R.color.colorHint));
        editTextPassword.setHintTextColor(getResources().getColor(R.color.colorHint));


        isBind=false;

        btnLogin.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                account = editTextAccount.getText().toString();
                password = editTextPassword.getText().toString();
                if(account != null && password != null){
                    btnLogin.setClickable(false);
                    Intent intent = new Intent(LoginActivity.this,ConnectService.class);
                    intent.putExtra(getString(R.string.activity),"LoginActivity");
                    intent.putExtra(getString(R.string.id),"3");
                    intent.putExtra(getString(R.string.account),account);
                    intent.putExtra(getString(R.string.password),password);
                    Log.d("LoginActivity","account="+account+"password="+password);

                    if(!isBind){
                        getApplicationContext().bindService(intent,mConnection, Context.BIND_AUTO_CREATE);
                        isBind=true;
                        Log.d("LoginActivity:", "login->bind");
                    }
                    else{
                        mBoundService.sendToServer(intent);
                        Log.d("LoginActivity:", "login->sendToService");
                    }
                    setReceiver();
                }else{
                    Toast.makeText(LoginActivity.this,"請輸入帳號密碼",Toast.LENGTH_SHORT).show();
                }
            }
        });

    }

    //接收广播类
    public class ConnectServiceReceiver extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
            if(intent.getStringExtra("activity").equals("LoginActivity")){
                if(intent.getStringExtra("result").equals("true")){
                    try{
                        getApplicationContext().unbindService(mConnection);
                        getApplicationContext().unregisterReceiver(receiver);
                    }catch(Exception e){
                        e.printStackTrace();
                    }

                    Bundle bundle = intent.getExtras();
                    String type = bundle.getString(getString(R.string.type));
                    if(type.compareTo("1")==0){
                        String name = bundle.getString(getString(R.string.name));
                        String mail = bundle.getString(getString(R.string.email));
                        sharedPreferences.edit()
                                .putString(getString(R.string.account),account)
                                .putString(getString(R.string.password),password)
                                .putString(getString(R.string.name),name)
                                .putString(getString(R.string.email),mail)
                                .putString(getString(R.string.isLogin),"true")
                                .putString("BGLogin","false")
                                .apply();
                        Log.d("LoginActivity:", "account:"+account+"password:"+password+"name:"+name+"email:"+mail);
                        Log.d("LoginActivity","sharedpreference isLogin="+sharedPreferences.getString("isLogin",""));
                        //跳到主畫面
                        Intent intentToMain = new Intent();
                        intentToMain.setClass(LoginActivity.this,NavigationActivity.class);
                        startActivity(intentToMain);
                        LoginActivity.this.finish();
                        Log.d("LoginActivity","跳到主畫面");

                    }else{
                        //登入失敗

                        Toast.makeText(LoginActivity.this,"帳號或密碼錯誤，請重新輸入", Toast.LENGTH_SHORT).show();
                        btnLogin.setClickable(true);
                    }
                }
                else{
                    //連線有問題
                    //Toast.makeText(LoginActivity.this,"伺服器維護中,請稍候再試",Toast.LENGTH_LONG).show();
                }
            }
        }
    }

    private static ServiceConnection mConnection = new ServiceConnection() {
        //EDITED PART
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            // TODO Auto-generated method stub
            mBoundService = ((ConnectService.LocalBinder)service).getService();
            MyBoundedService.myService=mBoundService;
            isBind=true;
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            // TODO Auto-generated method stub
            Log.d("LoginActivity","onServiceDisconnected");
            mBoundService = null;
            isBind=false;
        }

    };

    private void setReceiver(){
        //动态注册receiver
        IntentFilter filter = new IntentFilter(ACTION_RECV_MSG);
        filter.addCategory(Intent.CATEGORY_DEFAULT);
        receiver = new ConnectServiceReceiver();
        registerReceiver(receiver, filter);
        Log.d("LoginActivity:","register receiver");
    }

    @Override
    public void onDestroy(){
        Log.d("LoginActivity","onDestroy");
        try{
            getApplicationContext().unbindService(mConnection);
            unregisterReceiver(receiver);
        }catch(Exception e){
            e.printStackTrace();
        }
        super.onDestroy();
    }

    @Override
    public void onPause(){
        Log.d("LoginActivity","onPause");
        super.onPause();

    }
}
