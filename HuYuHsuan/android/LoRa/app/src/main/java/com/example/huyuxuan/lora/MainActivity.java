package com.example.huyuxuan.lora;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

public class MainActivity extends AppCompatActivity {

    private Button btnLogin;
    private Button btnSignUp;

    private ConnectServiceReceiver receiver;
    private static final String ACTION_RECV_MSG = "com.example.huyuxuan.lora.intent.action.RECEIVE_MESSAGE";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        btnLogin = (Button) findViewById(R.id.btnLogin);
        btnSignUp = (Button) findViewById(R.id.btnSignUp);

        btnLogin.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(MainActivity.this,ConnectService.class);

                intent.putExtra("type","out");
                intent.putExtra("id","3");
                intent.putExtra("account","dfdf");
                intent.putExtra("password","1123");

                startService(intent);
                Log.d("MainActivity:", "start ConnectService");
            }
        });
        btnSignUp.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(ConnectService.class.getName());
                intent.putExtra("type", "out");
                intent.putExtra("id", "2");
                intent.putExtra("account", "dfdf");
                intent.putExtra("password", "1123");
                intent.putExtra("name", "huhu");
                intent.putExtra("email", "ssdd@gmail.com");
                startService(intent);
            }
        });

        //动态注册receiver
        IntentFilter filter = new IntentFilter(ACTION_RECV_MSG);
        filter.addCategory(Intent.CATEGORY_DEFAULT);
        receiver = new ConnectServiceReceiver();
        registerReceiver(receiver, filter);
    }

    //接收广播类
    public class ConnectServiceReceiver extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
            String message = intent.getStringExtra("result");
            Log.d("ConnectServiceReceiver", message);

            // 如果傳回成功
            if(message.equals("true")){

            }

        }
    }
}