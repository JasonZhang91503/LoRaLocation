package com.example.huyuxuan.lora2;

import android.app.Application;

import com.example.huyuxuan.lora2.Background.BackgroundRecvService;

/**
 * Created by huyuxuan on 2017/5/4.
 */

public class MyBoundedService extends Application {
    public static ConnectService myService;
    public static boolean isConnect;
    public static BackgroundRecvService myBGService;
}
