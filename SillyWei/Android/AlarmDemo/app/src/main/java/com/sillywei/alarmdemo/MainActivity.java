package com.sillywei.alarmdemo;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;

import com.sillywei.alarmdemo.scheduler.MyAlarmReceiver;


public class MainActivity extends AppCompatActivity {
    MyAlarmReceiver alarm = new MyAlarmReceiver();


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }



    public void openAlarmButtonOnClick(View view) {
        alarm.setAlarm(this);
    }

    public void closeAlarmButtonOnClick(View view) {
        alarm.cancelAlarm(this);
    }
}
