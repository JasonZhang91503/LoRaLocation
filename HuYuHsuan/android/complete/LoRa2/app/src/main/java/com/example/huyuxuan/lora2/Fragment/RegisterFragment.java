package com.example.huyuxuan.lora2.Fragment;

import android.support.v4.app.Fragment;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.icu.text.SimpleDateFormat;
import android.os.Bundle;
import android.os.IBinder;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.Spinner;

import com.example.huyuxuan.lora2.ConnectService;
import com.example.huyuxuan.lora2.R;
import com.example.huyuxuan.lora2.User;

import java.util.Calendar;
import java.util.Locale;

/**
 * Created by huyuxuan on 2017/4/28.
 */

public class RegisterFragment extends Fragment {
    private View myview;
    private static boolean isBind;
    private Button btnOk;
    private Button btnSelectTime;
    private Spinner spnReceiver;
    private Spinner spnStart;
    private Spinner spnDes;

    User mUser;
    private String myName;
    private boolean getBuildingArray;
    private String des;
    private String start;
    private String receiverName;
    private String time;

    static ConnectService mBoundService;
    private ConnectServiceReceiver receiver;
    private static final String ACTION_RECV_MSG = "com.example.huyuxuan.lora.intent.action.RECEIVE_MESSAGE";
    static java.text.SimpleDateFormat dayDateFormat = new java.text.SimpleDateFormat("yyyy-MM-dd", Locale.getDefault());

    @Override
    public void onCreate(Bundle savedInstances){
        super.onCreate(savedInstances);
        isBind = false;
        mUser = (User) getContext().getApplicationContext();
        myName = mUser.UserName;
        getBuildingArray = false;


        //向server要有空時段資料
        Intent intent = new Intent(getActivity(),ConnectService.class);
        intent.putExtra(getString(R.string.activity),"RegisterFragment");
        intent.putExtra(getString(R.string.id),"10");
        intent.putExtra(getString(R.string.name),myName);
        if(!isBind){
            getActivity().bindService(intent,mConnection, Context.BIND_AUTO_CREATE);
            isBind=true;
            Log.d("RegisterFragment:", "checkSR->bind");
        }
        else{
            mBoundService.sendToServer(intent);
            Log.d("RegisterFragment:", "checkSR->sendToService");
        }
        setReceiver();

    }
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState){
        //inflate layout for this fragment
        myview = inflater.inflate(R.layout.fragment_register,container,false);
        btnOk = (Button)getView().findViewById(R.id.btnRegisterOk);
        btnSelectTime = (Button)getView().findViewById(R.id.btnSelectTime);
        spnReceiver = (Spinner)getView().findViewById(R.id.spinnerReceiver);
        spnStart = (Spinner)getView().findViewById(R.id.spinnerStart);
        spnDes = (Spinner)getView().findViewById(R.id.spinnerDes);

        spnReceiver.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                receiverName = parent.getItemAtPosition(position).toString();
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });
        spnStart.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                start = parent.getItemAtPosition(position).toString();
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });
        spnDes.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                des = parent.getItemAtPosition(position).toString();
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });

        btnOk.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //登記
                Intent intent = new Intent(getActivity(),ConnectService.class);
                intent.putExtra(getString(R.string.activity),"RegisterFragment");
                intent.putExtra(getString(R.string.id),"4");
                //等挑時間的介面寫完再改
                intent.putExtra(getString(R.string.requireTime),"10:30:00");
                intent.putExtra(getString(R.string.sender),myName);
                intent.putExtra(getString(R.string.receiver),receiverName);
                intent.putExtra(getString(R.string.startLocation),des);
                intent.putExtra(getString(R.string.desLocation),start);

                if(!isBind){
                    getActivity().bindService(intent,mConnection,Context.BIND_AUTO_CREATE);
                    isBind=true;
                    Log.d("RegisterFragment:", "apply->bind");
                }
                else{
                    if(mBoundService == null){
                        Log.e("RegisterFragment","mBoundService is null");
                    }
                    else{
                        Log.d("MainActivity:", "apply->sendToService");
                        mBoundService.sendToServer(intent);
                    }

                }
                setReceiver();
            }
        });

        btnSelectTime.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //抓手機現在時間
                Calendar c = Calendar.getInstance();
                String formattedDate = dayDateFormat.format(c.getTime());
                Log.d("RegisterFragment","formattedDate:"+formattedDate);

                //向server要有空時段資料
                Intent intent = new Intent(getActivity(),ConnectService.class);
                intent.putExtra(getString(R.string.activity),"RegisterFragment");
                intent.putExtra(getString(R.string.id),"5");
                intent.putExtra(getString(R.string.requireTime),formattedDate);
                if(!isBind){
                    getActivity().bindService(intent,mConnection, Context.BIND_AUTO_CREATE);
                    isBind=true;
                    Log.d("RegisterFragment:", "checkSR->bind");
                }
                else{
                    mBoundService.sendToServer(intent);
                    Log.d("RegisterFragment:", "checkSR->sendToService");
                }
                setReceiver();
            }
        });

        return myview;
    }
    private static ServiceConnection mConnection = new ServiceConnection() {
        //EDITED PART
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            // TODO Auto-generated method stub
            mBoundService = ((ConnectService.LocalBinder)service).getService();
            isBind=true;
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            // TODO Auto-generated method stub
            Log.d("RegisterFragment","onServiceDisconnected");
            //mBoundService = null;
            isBind=false;
        }

    };

    //接收广播类
    public class ConnectServiceReceiver extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
            if(intent.getStringExtra("activity").equals("RegisterFragment")){
                getActivity().unregisterReceiver(receiver);
                Bundle bundle = intent.getExtras();
                String id = bundle.getString(getString(R.string.id));
                switch(id){

                    case "5":
                        String msg = bundle.getString("message");
                        //這裡要把資料放進介面
                       // char tmp[]=msg.toCharArray();
                        /*
                        for(int i=0;i<tmp.length;i++){

                        }
                        */
                        Log.d("RegisterFragment","id=5,msg="+msg);
                        break;

                    case "10"://要使用者名單
                        String nameArray[]=bundle.getStringArray(getString(R.string.nameArray));
                        ArrayAdapter<String > nameList = new ArrayAdapter<>(getActivity(),
                                R.layout.support_simple_spinner_dropdown_item,nameArray);
                        spnReceiver.setAdapter(nameList);
                        if(!getBuildingArray){
                            Intent tmpIntent = new Intent(getContext(),ConnectService.class);
                            tmpIntent.putExtra(getString(R.string.activity),"RegisterFragment");
                            tmpIntent.putExtra(getString(R.string.id),"11");
                            mBoundService.sendToServer(tmpIntent);
                            setReceiver();
                        }
                        break;
                    case "11"://要大樓資訊
                        String buildingArray[]=bundle.getStringArray(getString(R.string.buildingArray));
                        ArrayAdapter<String > buildingList = new ArrayAdapter<>(getActivity(),
                                R.layout.support_simple_spinner_dropdown_item,buildingArray);
                        spnStart.setAdapter(buildingList);
                        spnDes.setAdapter(buildingList);
                        getBuildingArray = true;
                        break;
                }

            }
        }
    }

    private void setReceiver(){
        //动态注册receiver
        IntentFilter filter = new IntentFilter(ACTION_RECV_MSG);
        filter.addCategory(Intent.CATEGORY_DEFAULT);
        receiver = new ConnectServiceReceiver();
        getActivity().registerReceiver(receiver, filter);
        Log.d("RegisterFragment:","register receiver");
    }


}
