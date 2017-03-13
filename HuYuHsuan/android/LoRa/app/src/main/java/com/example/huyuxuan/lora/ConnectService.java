package com.example.huyuxuan.lora;

import android.app.IntentService;
import android.content.Intent;
import android.os.Bundle;
import android.os.IBinder;
import android.util.Log;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;

/**
 * Created by huyuxuan on 2017/3/11.
 */

public class ConnectService extends IntentService {

    Socket mSocket;
    InetAddress serverAddr;
    SocketAddress sc_add;
    BufferedReader in;
    BufferedWriter out;

    String type;
    String id;
    String account;
    String password;
    String name;
    String email;
    String msg;

    String rcvMessage;
    Boolean flag = false;

    private static final String ACTION_RECV_MSG = "com.example.huyuxuan.lora.intent.action.RECEIVE_MESSAGE";

    public ConnectService(){
        super("ConnectService");
    }

    @Override
    public void onCreate(){

        super.onCreate();
        Log.i("ConnectService:","onCreate");
        new Thread(new Runnable() {
            public void run() {
                try {
                    serverAddr = InetAddress.getByName(getString(R.string.ip));
                    mSocket = new Socket();
                    sc_add = new InetSocketAddress(serverAddr,Integer.parseInt(getString(R.string.port)));
                    if (mSocket.isConnected()){
                        Log.i("Service", "Socket Connected");
                    }else{
                        mSocket.connect(sc_add,2000);
                    }
                    in = new BufferedReader(new InputStreamReader(mSocket.getInputStream()));
                    out = new BufferedWriter(new OutputStreamWriter(mSocket.getOutputStream(),"utf8"));
                    Log.i("Service","BufferedReader and PrintWriter ready.");
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }).start();


    }

    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.d("Service:","onStartCommand called");

        type = intent.getExtras().getString("type");
        id = intent.getExtras().getString("id");

        return START_NOT_STICKY;
    }

    @Override
    protected void onHandleIntent(Intent intent) {
        Log.d("Service:","onHandleIntent called");

        if(type.equals("out")){
            Log.i("Service", "type = "+type+" id = "+id);

            switch (id) {
                case "2"://註冊
                    account = intent.getStringExtra("account");
                    password = intent.getStringExtra("password");
                    name = intent.getStringExtra("name");
                    email = intent.getStringExtra("email");
                    msg = String.valueOf(id) + account + "," + password + "," + name + "," + email + ",";
                    break;
                case "3"://登入
                    account = intent.getStringExtra("account");
                    password = intent.getStringExtra("password");
                    msg = String.valueOf(id) + account + "," + password + ",";
                    break;
            }
            if (!mSocket.isOutputShutdown() && msg.length() > 0){
                try {
                    out.write(msg);
                    out.flush();
                    Log.d("Service","write "+msg+" to server");
                    rcvMessage = in.readLine();
                    Log.d("Service","receive "+rcvMessage+" from server");
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }else if(type == "in"){
            //要接收
            try {
                rcvMessage = in.readLine();
                Log.d("Service","receive "+rcvMessage+" from server");
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        Intent broadcastIntent = new Intent();
        broadcastIntent.setAction(ACTION_RECV_MSG);
        broadcastIntent.addCategory(Intent.CATEGORY_DEFAULT);
        broadcastIntent.putExtra("result", flag.toString());
        sendBroadcast(broadcastIntent);
    }


}
