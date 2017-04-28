package com.example.huyuxuan.lora2.Fragment;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.view.PagerTabStrip;
import android.support.v4.view.ViewPager;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.example.huyuxuan.lora2.ConnectService;
import com.example.huyuxuan.lora2.R;

/**
 * Created by huyuxuan on 2017/4/28.
 */

public class HistoryFragment extends Fragment {
    private View myview;
    ViewPager pager;
    PagerTabStrip tab_strp;


    private static boolean isBind;
    static ConnectService mBoundService;
    private ConnectServiceReceiver receiver;
    private static final String ACTION_RECV_MSG = "com.example.huyuxuan.lora.intent.action.RECEIVE_MESSAGE";

    @Override
    public void onCreate(Bundle savedInstances){
        super.onCreate(savedInstances);
        isBind = false;

    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState){
        myview = inflater.inflate(R.layout.fragment_history,container,false);



        return myview;
    }


    //接收广播类
    public class ConnectServiceReceiver extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
            if(intent.getStringExtra("activity").equals("HistoryFragment")){
                getActivity().unregisterReceiver(receiver);
                Bundle bundle = intent.getExtras();
                String id = bundle.getString(getString(R.string.id));



            }
        }
    }

    private void setReceiver(){
        //动态注册receiver
        IntentFilter filter = new IntentFilter(ACTION_RECV_MSG);
        filter.addCategory(Intent.CATEGORY_DEFAULT);
        receiver = new ConnectServiceReceiver();
        getActivity().registerReceiver(receiver, filter);
        Log.d("HistoryFragment:","register receiver");
    }
}
