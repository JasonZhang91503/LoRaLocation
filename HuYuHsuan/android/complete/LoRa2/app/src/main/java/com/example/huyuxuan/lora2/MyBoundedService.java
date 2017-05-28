package com.example.huyuxuan.lora2;

import android.app.Application;
import android.support.v4.app.Fragment;

import com.example.huyuxuan.lora2.Background.BackgroundRecvService;
import com.example.huyuxuan.lora2.Fragment.AccountFragment;
import com.example.huyuxuan.lora2.Fragment.HomeFragment;

/**
 * Created by huyuxuan on 2017/5/4.
 */

public class MyBoundedService extends Application {
    public static ConnectService myService;
    public static boolean isConnect;
    public static BackgroundRecvService myBGService;
    public static int fragmentID;//0主畫面 1個人資料 2登記寄件(1) 3登記寄件(2) 4登記寄件(3) 5寄件歷史 6收件歷史 7選時段 8改照片
    public static Fragment curFragment;
}
