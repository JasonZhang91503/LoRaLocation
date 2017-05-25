package com.example.huyuxuan.lora2.Fragment;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.DatePickerDialog;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.IBinder;
import android.support.design.widget.TabLayout;
import android.support.v4.app.Fragment;
import android.support.v4.view.ViewPager;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.DatePicker;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;

import com.example.huyuxuan.lora2.ConnectService;
import com.example.huyuxuan.lora2.Order;
import com.example.huyuxuan.lora2.R;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.Locale;

import static android.content.Context.MODE_PRIVATE;

/**
 * Created by keng on 2017/5/13.
 */
@SuppressLint("ValidFragment")
public class SendHistoryFragment extends Fragment {
    ArrayList<Order> dataset;
    ViewPager viewPager;
    View view;
    TabLayout tabLayout;
    int type=0;
    int index_DayOfMonth;
    int index_Month;
    int index_Year;
    ImageButton front,back;
    Calendar calendar;
    TextView date;
    boolean tmp=false;
    String str_date;
    PagerAdapter adapter;

    static java.text.SimpleDateFormat dayDateFormat = new java.text.SimpleDateFormat("yyyy-MM-dd", Locale.getDefault());
    String formattedDate;
    private static boolean isBind;
    static ConnectService mBoundService;
    private ConnectServiceReceiver receiver;
    private static final String ACTION_RECV_MSG = "com.example.huyuxuan.lora.intent.action.RECEIVE_MESSAGE";


    @Override
    public void onResume(){
        super.onResume();
        Log.d("histmain On resume","ok");
        if(tmp){
            sendToService();
            //setView();
            tmp=false;
        }
    }
    public void setView(){
        if(adapter == null){
            adapter=new PagerAdapter(getActivity().getSupportFragmentManager(),dataset,type);//傳入資料表和看是查看寄件紀錄還是收件紀錄
            viewPager.setAdapter(adapter);
            Log.d("SendHistoryFragment","adapter == null");
        }else{
            adapter = (PagerAdapter) viewPager.getAdapter();
            adapter.update(dataset);
            viewPager.setAdapter(adapter);
            Log.d("SendHistoryFragment","adapter != null");
        }
        viewPager.addOnPageChangeListener(new TabLayout.TabLayoutOnPageChangeListener(tabLayout));

        date.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                DatePickerDialog datePickerDialog=new DatePickerDialog(getContext()
                        , new DatePickerDialog.OnDateSetListener() {
                    @Override
                    public void onDateSet(DatePicker view, int year, int month, int dayOfMonth) {
                        Log.d("date:", year+"年"+(month+1)+"月"+dayOfMonth+"日");
                        index_DayOfMonth=dayOfMonth;
                        index_Month=month+1;
                        index_Year=year;
                        calendar.set(year,month,dayOfMonth);
                        str_date = index_Year+"年"+index_Month+"月"+index_DayOfMonth+"日";
                        date.setText(str_date);
                        Log.d("calendar set", year+"年"+month+"月"+dayOfMonth+"日");
                        Log.d("SendHistoryFragment:", "checkSR->bind");

                        //向資料庫要資料
                        formattedDate=index_Year+"-"+index_Month+"-"+index_DayOfMonth;
                        sendToService();
                    }
                },calendar.get(Calendar.YEAR),calendar.get(Calendar.MONTH),calendar.get(Calendar.DAY_OF_MONTH));
                datePickerDialog.show();
            }
        });

        back.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                dateDecrease();
                date.setText(index_Year+"年"+(index_Month)+"月"+index_DayOfMonth+"日");
                calendar.set(index_Year,index_Month-1,index_DayOfMonth);

                //向資料庫要資料
                formattedDate=index_Year+"-"+index_Month+"-"+index_DayOfMonth;
                sendToService();

            }
        });

        front.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                dateIncrease();
                date.setText(index_Year+"年"+(index_Month)+"月"+index_DayOfMonth+"日");
                calendar.set(index_Year,index_Month-1,index_DayOfMonth);

                //向資料庫要資料
                formattedDate=index_Year+"-"+index_Month+"-"+index_DayOfMonth;
                sendToService();
            }
        });
        tabLayout.addOnTabSelectedListener(new TabLayout.OnTabSelectedListener() {
            @Override
            public void onTabSelected(TabLayout.Tab tab) {
                viewPager.setCurrentItem(tab.getPosition());
            }

            @Override
            public void onTabUnselected(TabLayout.Tab tab) {

            }

            @Override
            public void onTabReselected(TabLayout.Tab tab) {

            }
        });

    }
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        view=inflater.inflate(R.layout.fragment_history_main, container, false);
        tabLayout=(TabLayout) view.findViewById(R.id.tabLayout);
        viewPager=(ViewPager)view.findViewById(R.id.pager);
        date=(TextView)view.findViewById(R.id.txtDate);
        back=(ImageButton)view.findViewById(R.id.back);
        front=(ImageButton)view.findViewById(R.id.front);

        setTime();
        tmp=false;
        formattedDate = dayDateFormat.format(calendar.getTime());
        sendToService();

        return view;
    }
    public void dateDecrease(){
        if(index_DayOfMonth>0){
            index_DayOfMonth--;
        }else{
            index_Month--;
            if(index_Month>0){
                switch (index_Month){
                    case 1:
                        index_DayOfMonth=31;
                        break;
                    case 2:
                        if(index_Year%4!=0) {
                            index_DayOfMonth = 28;
                        }else{
                            index_DayOfMonth=29;
                        }
                        break;
                    case 3:
                        index_DayOfMonth=31;
                        break;
                    case 4:
                        index_DayOfMonth=30;
                        break;
                    case 5:
                        index_DayOfMonth=31;
                        break;
                    case 6:
                        index_DayOfMonth=30;
                        break;
                    case 7:
                        index_DayOfMonth=31;
                        break;
                    case 8:
                        index_DayOfMonth=31;
                        break;
                    case 9:
                        index_DayOfMonth=30;
                        break;
                    case 10:
                        index_DayOfMonth=31;
                        break;
                    case 11:
                        index_DayOfMonth=30;
                        break;
                }
            }else{
                index_Month=12;
                index_DayOfMonth=31;
                index_Year--;
            }
        }
    }
    public void dateIncrease(){
        int MAX_date=0;
        switch (index_Month){
            case 1:
                MAX_date=31;
                break;
            case 2:
                if(index_Year%4!=0) {
                    MAX_date = 28;
                }else{
                    MAX_date=29;
                }
                break;
            case 3:
                MAX_date=31;
                break;
            case 4:
                MAX_date=30;
                break;
            case 5:
                MAX_date=31;
                break;
            case 6:
                MAX_date=30;
                break;
            case 7:
                MAX_date=31;
                break;
            case 8:
                MAX_date=31;
                break;
            case 9:
                MAX_date=30;
                break;
            case 10:
                MAX_date=31;
                break;
            case 11:
                MAX_date=30;
                break;
            case 12:
                MAX_date=31;
                break;

        }
        if(index_DayOfMonth<MAX_date){
            index_DayOfMonth++;
        }else{
            index_Month++;
            if(index_Month<=12){
                index_DayOfMonth=1;
            }else{
                index_Month=1;
                index_DayOfMonth=1;
                index_Year++;
            }
        }
    }
    public void setTime(){
        calendar=Calendar.getInstance();
        index_DayOfMonth=calendar.get(Calendar.DAY_OF_MONTH);
        index_Month=calendar.get(Calendar.MONTH)+1;
        index_Year=calendar.get(Calendar.YEAR);
        str_date = index_Year+"年"+index_Month+"月"+index_DayOfMonth+"日";
        date.setText(str_date);

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
            Log.d("SendHistoryFragment","onServiceDisconnected");
            mBoundService = null;
            isBind=false;
        }

    };

    //接收广播类
    public class ConnectServiceReceiver extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
            if(intent.getStringExtra("activity").equals("SendHistoryFragment")){
                if(intent.getStringExtra("result").equals("true")){
                   Log.d("SendHistoryFragment:","receiver on receive");
                   Bundle bundle = intent.getExtras();
                   dataset = (ArrayList<Order>)bundle.getSerializable("arrayList");
                   setView();

                }
                else{
                    //連線有問題
                    Toast.makeText(getContext(),"伺服器維護中,請稍候再試",Toast.LENGTH_LONG).show();
                }
            }
        }
    }

    public void sendToService(){
        Intent intent = new Intent(getActivity(),ConnectService.class);
        intent.putExtra(getString(R.string.activity),"SendHistoryFragment");
        intent.putExtra(getString(R.string.id),"7");
        intent.putExtra(getString(R.string.requireTime),formattedDate);
        if(!isBind){
            getActivity().getApplicationContext().bindService(intent,mConnection, Context.BIND_AUTO_CREATE);
            isBind=true;
            Log.d("SendHistoryFragment:", "checkSR->bind");
        }
        else{
            mBoundService.sendToServer(intent);
            Log.d("SendHistoryFragment:", "checkSR->sendToService");
        }
        setReceiver();
    }

    private void setReceiver(){
        //动态注册receiver
        IntentFilter filter = new IntentFilter(ACTION_RECV_MSG);
        filter.addCategory(Intent.CATEGORY_DEFAULT);
        try{
            receiver = new ConnectServiceReceiver();
            getActivity().registerReceiver(receiver, filter);
        }catch(Exception e){
            e.printStackTrace();
            getActivity().unregisterReceiver(receiver);

        }

        Log.d("SendHistoryFragment:","register receiver");
    }



    @Override
    public void onStop() {
        Log.d("SendHistoryFragment:","onStop");
        SharedPreferences sharedPreferences= getActivity().getSharedPreferences("data" , MODE_PRIVATE);
        if(sharedPreferences.getString("hasStop","false").equals("false")){
            try{
                getActivity().getApplicationContext().unbindService(mConnection);
            }
            catch(Exception e){
                e.printStackTrace();
            }

        }

        tmp=true;
       // adapter=null;
        super.onStop();
    }

    @Override
    public void onDestroy() {
        Log.d("SendHistoryFragment:","onDestroy");
        super.onDestroy();
    }



}
