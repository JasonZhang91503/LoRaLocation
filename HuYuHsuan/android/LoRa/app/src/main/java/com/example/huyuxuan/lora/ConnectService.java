package com.example.huyuxuan.lora;

import android.app.IntentService;
import android.app.Service;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Binder;
import android.os.Bundle;
import android.os.IBinder;
import android.support.annotation.Nullable;
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

public class ConnectService extends Service {

    static Socket mSocket;
    InetAddress serverAddr;
    SocketAddress sc_add;
    static BufferedReader in;
    static BufferedWriter out;

    String id; //識別碼
    String msg;

    String rcvMessage;
    Boolean flag = false;

    private final IBinder binder=new LocalBinder();

    private static final String ACTION_RECV_MSG = "com.example.huyuxuan.lora.intent.action.RECEIVE_MESSAGE";

   /*
    public ConnectService(){
        super("ConnectService");
    }
    */

    @Override
    public void onCreate(){
        super.onCreate();

        new AsyncTask<String,String,String>() {
            @Override
            protected String doInBackground(String... strings) {
                try {
                    serverAddr = InetAddress.getByName(getString(R.string.ip));
                    mSocket = new Socket();
                    sc_add = new InetSocketAddress(serverAddr, Integer.parseInt(getString(R.string.port)));
                    if (mSocket.isConnected()) {
                        Log.i("Service", "Socket Connected");
                    } else {
                        mSocket.connect(sc_add, 2000);
                    }
                    in = new BufferedReader(new InputStreamReader(mSocket.getInputStream(), "utf8"));
                    out = new BufferedWriter(new OutputStreamWriter(mSocket.getOutputStream(), "utf8"));
                    Log.i("Service", "BufferedReader and PrintWriter ready.");
                } catch (IOException e) {
                    e.printStackTrace();
                }
                return null;
            }
        }.execute();

        Log.i("ConnectService:","onCreate");
    }

    @Override
    public IBinder onBind(final Intent intent) {
        Log.i("Service:","onBind called");
        id = intent.getExtras().getString("id");

        new AsyncTask<String,String,String>(){
            @Override
            protected String doInBackground(String... strings) {
                sendToServer(intent);
                return null;
            }
        }.execute();


        return binder;
    }

    public class LocalBinder extends Binder {
        public ConnectService getService() {
            System.out.println("I am in Localbinder ");
            return ConnectService.this;
        }
    }

    @Override
    public int onStartCommand(final Intent intent, int flags, int startId) {
        Log.d("Service:","onHandleIntent called");

        sendToServer(intent);
        return START_STICKY;
    }


    public void sendToServer(final Intent intent){

        new AsyncTask<String,String,String>() {
            @Override
            protected String doInBackground(String... strings) {
                id = intent.getExtras().getString("id");
                Log.i("Service", "id = " + id);

                switch (id) {
                    case "2"://註冊
                        String account = intent.getStringExtra("account");
                        String password = intent.getStringExtra("password");
                        String name = intent.getStringExtra("name");
                        String email = intent.getStringExtra("email");
                        msg = id + account + "," + password + "," + name + "," + email + ",";
                        break;
                    case "3"://登入
                        account = intent.getStringExtra("account");
                        password = intent.getStringExtra("password");
                        msg = id+ account + "," + password + ",";
                        break;
                    case "4"://登記寄件
                        String time = intent.getStringExtra(getString(R.string.requireTime));
                        String sender = intent.getStringExtra(getString(R.string.sender));
                        String receiver = intent.getStringExtra(getString(R.string.receiver));
                        String StartId = intent.getStringExtra(getString(R.string.startLocation));
                        String destinationId = intent.getStringExtra(getString(R.string.desLocation));
                        msg = id + time + "," + sender + "," + receiver + "," + StartId + "," + destinationId + ",";
                        break;
                    case "5"://詢問車子有空時間
                    case "7":
                    case "8":
                        time = intent.getStringExtra(getString(R.string.requireTime));
                        msg = id+ time + ",";
                        break;
                }

                if (!mSocket.isOutputShutdown() && msg.length() > 0) {
                    try {
                        if (out != null) {//傳送給server，接收server回應
                            out.write(msg);
                            out.flush();
                            Log.d("Service", "write " + msg + " to server");
                            rcvMessage = in.readLine();
                            Log.d("Service", "receive " + rcvMessage + " from server");
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
                Intent broadcastIntent = new Intent();
                broadcastIntent.setAction(ACTION_RECV_MSG);
                broadcastIntent.addCategory(Intent.CATEGORY_DEFAULT);
                broadcastIntent.putExtra("result", flag.toString());
                broadcastIntent.putExtra("message",rcvMessage);
                broadcastIntent.putExtra("service","ConnectService");
                sendBroadcast(broadcastIntent);

                return  null;
            }
        }.execute();

        rcvMessage=null;
    }
}
