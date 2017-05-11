package com.example.keng.main;

import android.app.Activity;
import android.content.Intent;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;

//記得要修改成extends Activity
public class SplashScreen extends Activity {


    private static int SPLASH_TIME_OUT=1500; //執行1.5秒動畫時間
    private PrefManager prefManager;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_splash_screen);
        prefManager=new PrefManager(this);
        /* 透過延遲啟動MainActivity
         * 透過intent 啟動Activity的方法:
         * Intent i =new Intent(this,開啟動的類別)
         * startActivity(i)
         * finish()
         */
        new Handler().postDelayed(new Runnable() {
            @Override
            public void run() {
                Intent i;

                if (!prefManager.isFirstTimeLaunch()) {
                    //第一次啟動app，進行註冊程序
                    i = new Intent(SplashScreen.this, Mainpage.class);
                    startActivity(i);
                    finish();
                } else {
                    //之後啟動ㄧ率跳進登入畫面
                    i = new Intent(SplashScreen.this, IntroSlider.class);
                    startActivity(i);
                    finish();
                }//結束目前的Activity
            }
        },SPLASH_TIME_OUT);//間隔1.5秒後開啟程式
    }
}
