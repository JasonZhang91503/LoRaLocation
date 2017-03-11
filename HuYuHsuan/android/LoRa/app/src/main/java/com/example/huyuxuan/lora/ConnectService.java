package com.example.huyuxuan.lora;

import android.app.IntentService;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.Socket;

/**
 * Created by huyuxuan on 2017/3/11.
 */

public class ConnectService extends IntentService {

    Socket mSocket;
    BufferedReader in;
    BufferedWriter out;
    String type;
    int id;


    public ConnectService(){
        super("ConnectService");
    }

    public void onCreate(){
        try {
            mSocket = new Socket(getString(R.string.ip),R.string.port);
            if (mSocket.isConnected()) Log.i("Service", "Socket Connected");
            in = new BufferedReader(new InputStreamReader(mSocket.getInputStream()));
            out = new BufferedWriter(new OutputStreamWriter(mSocket.getOutputStream(),"utf8"));
            Log.i("Service","BufferedReader and PrintWriter ready.");
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    protected void onHandleIntent(Intent intent) {
        type = intent.getExtras().getString("type");
        id = intent.getExtras().getInt("id");
        String msg = null;
        if(type == "out"){
            Log.i("Service", "type = out");
            switch (id){
                case 2://註冊
                    String account = intent.getExtras().getString("account");
                    String password = intent.getExtras().getString("password");
                    String name = intent.getExtras().getString("name");
                    String email = intent.getExtras().getString("email");
                    msg = String.valueOf(id)+account+","+password+","+name+","+email+",";
                    break;
                case 3://登入
                    account = intent.getExtras().getString("account");
                    password = intent.getExtras().getString("password");
                    msg = String.valueOf(id)+account+","+password+",";
                    break;
            }
            if (!mSocket.isOutputShutdown() && msg.length() > 0){
                try {
                    out.write(msg);
                    out.flush();
                    Log.d("Service","write "+msg+" to server");
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }else if(type == "in"){
            //要接收
        }
    }




}
