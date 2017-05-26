package com.example.huyuxuan.lora2.Fragment;

import android.annotation.SuppressLint;
import android.app.Dialog;
import android.app.TimePickerDialog;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.support.v4.app.DialogFragment;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentTransaction;
import android.support.v7.app.AlertDialog;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.SimpleAdapter;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.TimePicker;
import android.widget.Toast;

import com.example.huyuxuan.lora2.ConnectService;
import com.example.huyuxuan.lora2.MyBoundedService;
import com.example.huyuxuan.lora2.R;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.HashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;

/**
 * Created by huyuxuan on 2017/5/7.
 */

public class NewOrderFragment extends Fragment implements View.OnClickListener{
    private final String TAG=this.getClass().getSimpleName();
    String[] location={"",""};
    boolean is_setTime=false;
    Spinner spnStart;
    Spinner spnDest;
    TextView orderTime;
    private String des;
    private String start;
    String buildingArray[];
    private static boolean isBind=false;
    private String time;
    String formattedDate;
    FragmentTransaction ft;
    private String str_show_time="請選擇時間";

    static ConnectService mBoundService;
    private ConnectServiceReceiver receiver;
    private static final String ACTION_RECV_MSG = "com.example.huyuxuan.lora.intent.action.RECEIVE_MESSAGE";
    static java.text.SimpleDateFormat dayDateFormat = new java.text.SimpleDateFormat("yyyy-MM-dd", Locale.getDefault());
    public int MY_REQUEST_CODE=11;


    public NewOrderFragment() {
        // Required empty public constructor
    }
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        Log.d("NewOrderFragment","onCreateView");
        // Inflate the layout for this fragment

        View view=inflater.inflate(R.layout.fragment_new_order, container, false);

        spnStart=(Spinner)view.findViewById(R.id.start);
        spnDest=(Spinner)view.findViewById(R.id.destination);
        //得到日期資訊並且顯示於Text中
        LinearLayout time=(LinearLayout)view.findViewById(R.id.time);
        orderTime=(TextView)view.findViewById(R.id.orderTime);
        orderTime.setText(str_show_time);
        MyBoundedService.fragmentID=2;
        MyBoundedService.curFragment=this;

        //要大樓資料
        Intent tmpIntent = new Intent(getContext(),ConnectService.class);
        tmpIntent.putExtra(getString(R.string.activity),"NewOrderFragment");
        tmpIntent.putExtra(getString(R.string.id),"11");
        if(!isBind){
            getActivity().getApplicationContext().bindService(tmpIntent,mConnection, Context.BIND_AUTO_CREATE);
            isBind=true;
            Log.d("NewOrderFragment:", "checkSR->bind");
        }
        else{
            mBoundService.sendToServer(tmpIntent);
            Log.d("NewOrderFragment:", "checkSR->sendToService");
        }
        setReceiver();
        //Toast.makeText(getContext(),"請求資料中",Toast.LENGTH_SHORT).show();


        //設定下一步Btn的內容
        Button btn=(Button)view.findViewById(R.id.btn_next);
        btn.setOnClickListener(this);


        //替time Linearlayout 設定ClickListener
        time.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                final Calendar c = Calendar.getInstance();
                formattedDate = dayDateFormat.format(c.getTime());
                Log.d("NewOrderFragment","formattedDate:"+formattedDate);

                Intent intent = new Intent(getActivity(),ConnectService.class);
                intent.putExtra(getString(R.string.activity),"NewOrderFragment");
                intent.putExtra(getString(R.string.id),"5");
                intent.putExtra(getString(R.string.requireTime),formattedDate);
                if(!isBind){
                    getActivity().getApplicationContext().bindService(intent,mConnection, Context.BIND_AUTO_CREATE);
                    isBind=true;
                    Log.d("NewOrderFragment:", "checkSR->bind");
                }
                else{
                    mBoundService.sendToServer(intent);
                    Log.d("NewOrderFragment:", "checkSR->sendToService");
                }
                setReceiver();

            }
        });

        return view;
    }

    @Override
    public void onClick(View v) {

        //創造一個可以傳遞參數的Fragment，並且將地點和時間的參數傳遞到下一個Fragment讓最後確認時可以有資訊
        //傳遞 Location 編號->可以從 string.xml 找到
        //傳遞 timeText 時間資訊->看看是否要改格式(方便傳遞)
        if(!is_setTime){
            //避免沒有選填時間
            Toast.makeText(getContext(),"請填選時間",Toast.LENGTH_SHORT).show();
        }else if(location[0]==location[1]){
            //避免相同收件地
            Toast.makeText(getContext(),"寄件地收件地不可相同",Toast.LENGTH_SHORT).show();
        }
        else {
          //  getActivity().getApplicationContext().unbindService(mConnection);
            String timeText = str_show_time.substring(str_show_time.indexOf(" ")+1);
            NewOrderFragment2 fragment2 = NewOrderFragment2.newInstance(location, timeText);
            FragmentManager fm = getActivity().getSupportFragmentManager();
            FragmentTransaction trans = fm.beginTransaction();
            trans.addToBackStack(null);
            trans.replace(R.id.fragment_container, fragment2, fragment2.getTag());
            trans.commit();
        }

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
            Log.d("NewOrderFragment","onServiceDisconnected");
            //mBoundService = null;
            isBind=false;
        }

    };


    //接收广播类
    public class ConnectServiceReceiver extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
            if(intent.getStringExtra("result").equals("true")){
                if(intent.getStringExtra("activity").equals("NewOrderFragment")){
                    if(isAdded()){
                        getActivity().getApplicationContext().unregisterReceiver(receiver);
                    }/*
                    if(isBind){
                        getActivity().getApplicationContext().unbindService(mConnection);
                        isBind=false;
                    }
                    */
                    Bundle bundle = intent.getExtras();
                    String id = bundle.getString(getString(R.string.id));
                    switch(id){

                        case "5":
                            String msg = bundle.getString("message");

                            //創basic dialog
                            ft = getFragmentManager().beginTransaction();
                            DialogFragment mDialogFragment;
                            mDialogFragment = BasicDialogFragment.newInstance(msg);
                            mDialogFragment.setTargetFragment(NewOrderFragment.this, MY_REQUEST_CODE);
                            ft.replace(R.id.fragment_container,mDialogFragment);
                            ft.addToBackStack(null);
                            ft.commit();

                            Log.d("NewOrderFragment","id=5,msg="+msg);
                            break;

                        case "11"://要大樓資訊
                            buildingArray=bundle.getStringArray(getString(R.string.buildingArray));
                            ArrayAdapter<String > buildingList = new ArrayAdapter<>(getActivity(),
                                    R.layout.support_simple_spinner_dropdown_item,buildingArray);
                            spnStart.setAdapter(buildingList);
                            spnDest.setAdapter(buildingList);

                            spnStart.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
                                @Override
                                public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {

                                    location[0] = buildingArray[position];
                                    Log.d(TAG,buildingArray[position]);
                                }

                                @Override
                                public void onNothingSelected(AdapterView<?> parent) {

                                }
                            });
                            spnDest.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
                                @Override
                                public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                                    location[1] = buildingArray[position];
                                    Log.e(TAG,buildingArray[position]);
                                }

                                @Override
                                public void onNothingSelected(AdapterView<?> parent) {

                                }
                            });
                            break;
                    }

                }
                else{
                   // Toast.makeText(getActivity(),"伺服器維護中,請稍候再試",Toast.LENGTH_LONG).show();
                }
            }

        }
    }

    private void setReceiver(){
        //动态注册receiver
        IntentFilter filter = new IntentFilter(ACTION_RECV_MSG);
        filter.addCategory(Intent.CATEGORY_DEFAULT);
        receiver = new ConnectServiceReceiver();
        getActivity().getApplicationContext().registerReceiver(receiver, filter);
        Log.d("NewOrderFragment","register receiver");
    }

    public void setTime(String tmp){
        time=tmp;
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        Log.d("NewOrderFragment","onActivityResult");
        super.onActivityResult(requestCode, resultCode, data);
        String str_time = data.getStringExtra("time");
        Calendar c = Calendar.getInstance();
        int year=c.get(Calendar.YEAR);
        int month=c.get(Calendar.MONTH)+1;
        int day=c.get(Calendar.DAY_OF_MONTH);
        str_show_time=String.valueOf(year)+"年"+String.valueOf(month)+"月"+String.valueOf(day)+"日 "+str_time;
        orderTime.setText(str_show_time);
        is_setTime=true;
        MyBoundedService.fragmentID=2;
        MyBoundedService.curFragment=this;
    }

    @Override
    public void onDestroy() {
        Log.d("NewOrderFragment:", "onDestroy");
        super.onDestroy();
    }

    @Override
    public void onStop() {
        Log.d("NewOrderFragment:", "onStop");
        if(isBind){
            getActivity().getApplicationContext().unbindService(mConnection);
            isBind=false;
        }

        super.onStop();
    }

    @Override
    public void onResume(){
        Log.d("NewOrderFragment","onResume");
        MyBoundedService.fragmentID=2;
        MyBoundedService.curFragment=this;
        super.onResume();
    }

}
