package com.example.huyuxuan.lora2;

import android.app.Service;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Binder;
import android.os.Bundle;
import android.os.IBinder;
import android.util.Log;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;
import java.util.ArrayList;
import java.util.HashMap;

/**
 * Created by huyuxuan on 2017/4/26.
 */

public class ConnectService extends Service {
    static Socket mSocket;
    InetAddress serverAddr;
    SocketAddress sc_add;
    static BufferedReader in;
    static BufferedWriter out;

    String id; //識別碼
    String msg;
    String activityName; //哪個activity傳來的請求

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
        Log.i("ConnectService:","onCreate");
        if(!MyBoundedService.isConnect){
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
                        MyBoundedService.isConnect=true;
                        in = new BufferedReader(new InputStreamReader(mSocket.getInputStream(), "utf8"));
                        out = new BufferedWriter(new OutputStreamWriter(mSocket.getOutputStream(), "utf8"));
                        Log.i("Service", "BufferedReader and PrintWriter ready.");
                        out.write("1");
                        out.flush();
                        Log.d("Service", "write 1 to server");

                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    return null;
                }
            }.execute();
        }


    }

    @Override
    public IBinder onBind(final Intent intent) {
        Log.i("Service:","onBind called from "+intent.getExtras().getString("activity"));

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

      //  sendToServer(intent);
        return START_STICKY;
    }

    public void sendToServer(final Intent intent){

        new AsyncTask<String,String,String>() {
            @Override
            protected String doInBackground(String... strings) {
                id = intent.getExtras().getString("id");
                activityName = intent.getExtras().getString("activity");
                Log.i("Service:","send to server from "+activityName);
                Bundle bundle = new Bundle();
                Log.i("Service", "id = " + id);

                switch (id) {
                    case "2"://註冊
                        String account = intent.getStringExtra("account");
                        String password = intent.getStringExtra("password");
                        String name = intent.getStringExtra("name");
                        String email = intent.getStringExtra("email");
                        msg = id +","+ account + "," + password + "," + name + "," + email + ",";
                        break;
                    case "3"://登入
                        account = intent.getStringExtra("account");
                        password = intent.getStringExtra("password");
                        msg = id+","+ account + "," + password + ",";
                        break;
                    case "4"://登記寄件
                        String time = intent.getStringExtra(getString(R.string.requireTime));
                        String sender = intent.getStringExtra(getString(R.string.sender));
                        String receiver = intent.getStringExtra(getString(R.string.receiver));
                        String StartId = intent.getStringExtra(getString(R.string.startLocation));
                        String destinationId = intent.getStringExtra(getString(R.string.desLocation));
                        msg = id +","+ time + "," + sender + "," + receiver + "," + StartId + "," + destinationId + ",";
                        break;
                    case "5"://詢問車子有空時間
                    case "7":
                    case "8":
                    case "9":
                        Log.d("ConnectService","requireTime="+intent.getStringExtra(getString(R.string.requireTime)));
                        time = intent.getStringExtra(getString(R.string.requireTime));
                        msg = id+","+ time + ",";
                        break;
                    case "10":
                        name = intent.getStringExtra(getString(R.string.name));
                        msg = id +","+ name + ",";
                        break;
                    case "11"://要大樓資訊
                        msg = id+",";
                        break;
                }

                if (!mSocket.isOutputShutdown() && msg.length() > 0 && !mSocket.isInputShutdown()) {
                    try {
                        if (out != null) {//傳送給server，接收server回應
                            rcvMessage="";
                            out.write(msg);
                            out.flush();
                            Log.d("Service", "write " + msg + " to server");
                            rcvMessage = in.readLine();
                            rcvMessage.concat("\0");    //*****後面一定要加\0不然會是亂碼
                            Log.d("Service", "receive " + rcvMessage + " from server");
                            bundle = Analyze(rcvMessage);
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
                Intent broadcastIntent = new Intent();
                broadcastIntent.setAction(ACTION_RECV_MSG);
                broadcastIntent.addCategory(Intent.CATEGORY_DEFAULT);
                broadcastIntent.putExtra("result", flag.toString());
                broadcastIntent.putExtra("activity",activityName);//決定要傳給哪個activity
                broadcastIntent.putExtras(bundle);
                sendBroadcast(broadcastIntent);
                Log.i("Service:","sendbroadcast to  "+activityName);

                rcvMessage="";
                return  null;
            }
        }.execute();
    }

    public Bundle Analyze(String mes){
        Bundle dataBundle = new Bundle();
        int commaIndex = mes.indexOf(',');
        String RcvId = mes.substring(0,commaIndex);
        dataBundle.putString("id",RcvId);
        Log.d("ConnectService","receive id="+RcvId);
        switch (RcvId){

            case "2":   //註冊是否成功
                String type=mes.substring(commaIndex+1,3);
                Log.d("ana:","id="+id+"type="+type);
                dataBundle.putString(getString(R.string.type),type);
                break;
            case "3":   //登入是否成功
                type=mes.substring(commaIndex+1,3);
                if(type.compareTo("1")==0){
                    String name = mes.substring(3,mes.indexOf('^'));
                    String email = mes.substring(mes.indexOf('^')+1);
                    dataBundle.putString(getString(R.string.type),type);
                    dataBundle.putString(getString(R.string.name),name);
                    dataBundle.putString(getString(R.string.email),email);
                    Log.d("ana:","id="+id+"type="+type+"name="+name+"email="+email);
                }
                else{
                    String error = mes.substring(3);
                    dataBundle.putString(getString(R.string.type),type);
                    dataBundle.putString(getString(R.string.errorMsg),error);
                    Log.d("ana:","id="+id+"type="+type+"errorMsg="+error);
                }
                dataBundle.putString("type",type);
                break;
            case "4":   //登記寄件是否成功
                type=mes.substring(commaIndex+1,3);
                if(type.compareTo("0")==0){
                    String error = mes.substring(3);
                    dataBundle.putString(getString(R.string.errorMsg),error);
                    Log.d("ana:","id="+id+"type="+type+"errorMsg="+error);
                }
                Log.d("ana:","id="+id+"type="+type);
                dataBundle.putString(getString(R.string.type),type);
                break;
            case "5":   //車子有空時段
                dataBundle.putString("message",mes.substring(commaIndex+1));
                Log.d("ana:","id="+id+"msg="+mes.substring(commaIndex+1));
                break;
            case "6":   //使用者資料
                String name = mes.substring(commaIndex+1,mes.indexOf('^'));
                String email = mes.substring(mes.indexOf('^')+1,mes.indexOf('*'));
                dataBundle.putString(getString(R.string.name),name);
                dataBundle.putString(getString(R.string.email),email);
                Log.d("ana:","id="+id+"name="+name+"email="+email);
                break;
            case "7":   //寄件資料
                String numStr = mes.substring(commaIndex+1,3);//抓資料數量
                int num = Integer.valueOf(numStr);
                ArrayList<HashMap<String, String>> DataList = new ArrayList<HashMap<String, String>>();
                String[] mesArray = mes.split("\\*");//把每筆用＊分開的資料分別抓出來存進array
                for(int i = 0; i < num ; i++){
                    String curStr = mesArray[i]; //抓每筆資料
                    HashMap<String, String> map = new HashMap<String, String>();
                    if(i==0){
                        map.put(getString(R.string.receiver),curStr.substring(3,curStr.indexOf('~')));
                    }
                    else{
                        map.put(getString(R.string.receiver),curStr.substring(0,curStr.indexOf('~')));
                    }
                    map.put(getString(R.string.requireTime),curStr.substring(curStr.indexOf('~')+1,curStr.indexOf('^')));
                    map.put(getString(R.string.arriveTime),curStr.substring(curStr.indexOf('^')+1,curStr.indexOf(';')));
                    map.put(getString(R.string.startLocation),curStr.substring(curStr.indexOf(';')+1,curStr.indexOf('/')));
                    map.put(getString(R.string.desLocation),curStr.substring(curStr.indexOf('/')+1,curStr.indexOf('!')));
                    map.put(getString(R.string.state),curStr.substring(curStr.indexOf('!')+1,curStr.indexOf('#')));
                    map.put(getString(R.string.key),curStr.substring(curStr.indexOf('#')+1));
                    DataList.add(map);
                    Log.d("ana:","id="+id+"第"+i+"筆"+"requireTime="+curStr.substring(curStr.indexOf('~')+1,curStr.indexOf('^'))+"key="+curStr.substring(curStr.indexOf('#')+1));
                }
                dataBundle.putSerializable("arrayList",DataList);
                break;
            case "8":   //收件資料
                numStr = mes.substring(commaIndex+1,3);//抓資料數量
                num = Integer.valueOf(numStr);
                DataList = new ArrayList<HashMap<String, String>>();
                mesArray = mes.split("\\*");//把每筆用＊分開的資料分別抓出來存進array
                for(int i = 0; i < num ; i++){
                    String curStr = mesArray[i]; //抓每筆資料
                    Log.d("ana","curStr="+curStr);
                    HashMap<String, String> map = new HashMap<String, String>();
                    if(i==0){
                        map.put(getString(R.string.sender),curStr.substring(3,curStr.indexOf('~')));
                    }
                    else{
                        map.put(getString(R.string.sender),curStr.substring(0,curStr.indexOf('~')));
                    }
                    map.put(getString(R.string.requireTime),curStr.substring(curStr.indexOf('~')+1,curStr.indexOf('^')));
                    map.put(getString(R.string.arriveTime),curStr.substring(curStr.indexOf('^')+1,curStr.indexOf(';')));
                    map.put(getString(R.string.startLocation),curStr.substring(curStr.indexOf(';')+1,curStr.indexOf('/')));
                    map.put(getString(R.string.desLocation),curStr.substring(curStr.indexOf('/')+1,curStr.indexOf('!')));
                    map.put(getString(R.string.state),curStr.substring(curStr.indexOf('!')+1,curStr.indexOf('#')));
                    map.put(getString(R.string.key),curStr.substring(curStr.indexOf('#')+1));
                    DataList.add(map);
                    Log.d("ana:","id="+id+"第"+i+"筆"+"requireTime="+curStr.substring(curStr.indexOf('~')+1,curStr.indexOf('^'))+"key="+curStr.substring(curStr.indexOf('#')+1));
                }

                dataBundle.putSerializable("arrayList",DataList);
                break;
            case "9":
                numStr = mes.substring(commaIndex+1,3);//抓資料數量
                num = Integer.valueOf(numStr);
                DataList = new ArrayList<HashMap<String, String>>();
                mesArray = mes.split("\\*");//把每筆用＊分開的資料分別抓出來存進array
                for(int i = 0; i < num ; i++){
                    String curStr = mesArray[i]; //抓每筆資料
                    HashMap<String, String> map = new HashMap<String, String>();
                    if(i==0){
                        map.put(getString(R.string.packetType),curStr.substring(3,4));//是寄件或收件資料
                        map.put(getString(R.string.sender),curStr.substring(4,curStr.indexOf('~')));
                    }
                    else{
                        map.put(getString(R.string.packetType),curStr.substring(0,1));
                        map.put(getString(R.string.sender),curStr.substring(1,curStr.indexOf('~')));
                    }
                    map.put(getString(R.string.receiver),curStr.substring(curStr.indexOf('~')+1,curStr.indexOf('^')));
                    map.put(getString(R.string.requireTime),curStr.substring(curStr.indexOf('^')+1,curStr.indexOf(';')));
                    map.put(getString(R.string.arriveTime),curStr.substring(curStr.indexOf(';')+1,curStr.indexOf('/')));
                    map.put(getString(R.string.startLocation),curStr.substring(curStr.indexOf('/')+1,curStr.indexOf('!')));
                    map.put(getString(R.string.desLocation),curStr.substring(curStr.indexOf('!')+1,curStr.indexOf('#')));
                    map.put(getString(R.string.state),curStr.substring(curStr.indexOf('#')+1,curStr.indexOf('$')));
                    map.put(getString(R.string.key),curStr.substring(curStr.indexOf('$')+1));
                    DataList.add(map);
                    Log.d("ana:","id="+id+"第"+i+"筆"+"requireTime="+curStr.substring(curStr.indexOf('~')+1,curStr.indexOf('^'))+"key="+curStr.substring(curStr.indexOf('#')+1));
                }
                //ArrayList tmpList = new ArrayList(); //这个list用于在budnle中传递 需要传递的ArrayList<Object>
                //tmpList.add(DataList);
                dataBundle.putSerializable("arrayList",DataList);
                //dataBundle.putParcelableArrayList("list",tmpList);
                break;
            case "10":
                mes = mes.substring(mes.indexOf('^')+1);
                mesArray = mes.split("\\*");//把每筆用＊分開的資料分別抓出來存進array
                dataBundle.putStringArray(getString(R.string.nameArray),mesArray);
                Log.d("ana:","第二筆"+mesArray[2]);
                break;
            case "11":
                mes = mes.substring(mes.indexOf('^')+1);
                mesArray = mes.split("\\*");//把每筆用＊分開的資料分別抓出來存進array
                dataBundle.putStringArray(getString(R.string.buildingArray),mesArray);
                Log.d("ana:","第二筆"+mesArray[2]);
                break;
            default:
                dataBundle=null;
        }
        return  dataBundle;
    }
}
