package com.example.huyuxuan.mysql;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import com.example.huyuxuan.mysql.alarm.MyAlarmReceiver;

/**
 * Created by huyuxuan on 2016/12/28.
 */

public class FeaturesActivity extends Activity {
    Button mAddTransBtn;
    Button mCheckRecvBtn;
    Button mCheckSendBtn;
    int id;
    int type;
    MyAlarmReceiver alarm = new MyAlarmReceiver();

    @Override
    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        setContentView(R.layout.features_layout);
        Bundle bundle = this.getIntent().getExtras();
        id = bundle.getInt("id");

        mAddTransBtn = (Button)findViewById(R.id.btnAddTransport);
        mCheckRecvBtn = (Button)findViewById(R.id.btnCheckReceive);
        mCheckSendBtn = (Button)findViewById(R.id.btnCheckSend);
        mAddTransBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(getApplicationContext(),NewTransportActivity.class);
                Bundle bundle = new Bundle();
                bundle.putInt("id",id);

                intent.putExtras(bundle);
                startActivityForResult(intent,100);
            }
        });
        mCheckRecvBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                type = 1;
                Intent intent = new Intent(getApplicationContext(),CheckReceiveActivity.class);
                Bundle bundle = new Bundle();
                bundle.putInt("id",id);
                bundle.putInt("type",type);
                intent.putExtras(bundle);
                startActivityForResult(intent,200);
            }
        });

        mCheckSendBtn.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View view){
                type = 0;
                Intent intent = new Intent(getApplicationContext(),CheckReceiveActivity.class);
                Bundle bundle = new Bundle();
                bundle.putInt("id",id);
                bundle.putInt("type",type);
                intent.putExtras(bundle);
                startActivityForResult(intent,200);
            }
        });
        alarm.setID(id);
        Log.d("Features:","set alarm id = "+String.valueOf(id));
        alarm.setAlarm(this);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        // if result code 100
        if (resultCode == 200) {
            // if result code 100 is received
            // means user edited/deleted product
            // reload this screen again
            Intent intent = getIntent();

            Log.d("Features:","get type:"+String.valueOf(type));
            if(type == 1){
                Toast.makeText(FeaturesActivity.this,"沒有要寄給你的信唷",Toast.LENGTH_SHORT).show();
            }else{
                Toast.makeText(FeaturesActivity.this,"你還沒有登記寄信唷",Toast.LENGTH_SHORT).show();
            }

        }

    }

    @Override
    protected void onStop(){
        super.onStop();
       // alarm.cancelAlarm(this);
    }

}
