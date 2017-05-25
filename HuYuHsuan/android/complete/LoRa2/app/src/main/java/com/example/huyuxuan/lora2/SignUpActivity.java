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
    protected EditText editTextName;
    protected EditText editTextEmail;
    protected Button btnSignUp;

    protected String account;
    protected String password;
    protected String name;
    protected String email;


    static boolean isBind;

    static ConnectService mBoundService;
    private ConnectServiceReceiver receiver;
    private static final String ACTION_RECV_MSG = "com.example.huyuxuan.lora.intent.action.RECEIVE_MESSAGE";

    //用來存程式關掉之後還要存在的資料、狀態
    private SharedPreferences sharedPreferences;

    @Override
    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_sign_up);

        editTextAccount = (EditText)findViewById(R.id.etSignAccount);
        editTextPassword = (EditText)findViewById(R.id.etSignPassword);
        editTextName = (EditText)findViewById(R.id.etSignName);
        editTextEmail = (EditText)findViewById(R.id.etSignEmail);
        sharedPreferences = getSharedPreferences("data" , MODE_PRIVATE);
        btnSignUp = (Button)findViewById(R.id.btnSignUp);
        editTextPassword.setHintTextColor(getResources().getColor(R.color.colorHint));
        editTextAccount.setHintTextColor(getResources().getColor(R.color.colorHint));
        editTextName.setHintTextColor(getResources().getColor(R.color.colorHint));
        editTextEmail.setHintTextColor(getResources().getColor(R.color.colorHint));

        isBind=false;



        btnSignUp.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                //抓資料
                account = editTextAccount.getText().toString();
                password = editTextPassword.getText().toString();
                name = editTextName.getText().toString();
                email = editTextEmail.getText().toString();


                Intent intent = new Intent(SignUpActivity.this,ConnectService.class);
                intent.putExtra(getString(R.string.activity),"SignUpActivity");
                intent.putExtra(getString(R.string.id), "2");
                intent.putExtra(getString(R.string.account),account);
                intent.putExtra(getString(R.string.password),password);
                intent.putExtra(getString(R.string.name), name);
                intent.putExtra(getString(R.string.email), email);

                if(!isBind){
                    getApplicationContext().bindService(intent,mConnection,Context.BIND_AUTO_CREATE);
                    isBind=true;
                    Log.d("SignUpActivity:", "bindService");
                }
                else{
                    mBoundService.sendToServer(intent);
                    Log.d("SignUpActivity:", "sendToService");
                }
                setReceiver();

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
                switch (type){
                    case "1":
                        sharedPreferences.edit()
                                .putString(getString(R.string.account),account)
                                .putString(getString(R.string.password),password)
                                .putString(getString(R.string.name),name)
                                .putString(getString(R.string.email),email)
                                .putString(getString(R.string.isLogin),"true")
                                .apply();
                        Log.d("SignUpActivity:", "account:"+account+"password:"+password+"name:"+name+"email:"+email);
                        //跳到主畫面
                        Intent intentToMain = new Intent();
                        intentToMain.setClass(SignUpActivity.this,NavigationActivity.class);
                        startActivity(intentToMain);
                        SignUpActivity.this.finish();
                        Log.d("SignUpActivity","跳到主畫面");
                        break;
                    case "2":
                        Toast.makeText(SignUpActivity.this,"帳號重複", Toast.LENGTH_LONG).show();
                        editTextAccount.setText("");
                        break;
                    case "3":
                        Toast.makeText(SignUpActivity.this,"密碼重複", Toast.LENGTH_LONG).show();
                        editTextPassword.setText("");
                        break;
                    case "4":
                        Toast.makeText(SignUpActivity.this,"帳號密碼都重複", Toast.LENGTH_LONG).show();
                        editTextAccount.setText("");
                        editTextPassword.setText("");
                        break;
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
