package com.example.huyuxuan.lora2.Fragment;

import android.app.DialogFragment;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.support.v4.app.Fragment;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.SimpleAdapter;

import com.example.huyuxuan.lora2.ConnectService;
import com.example.huyuxuan.lora2.Order;
import com.example.huyuxuan.lora2.R;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.HashMap;
import java.util.Locale;

/**
 * Created by huyuxuan on 2017/4/29.
 */

public class RcvHistoryPage extends Fragment implements Serializable{

    private static  View myView;
    //private ListView lv;
    private RecyclerView recyclerView;
    private Button btnPickDate;

    private Calendar c;
    String formattedDate;

  static java.text.SimpleDateFormat dayDateFormat = new java.text.SimpleDateFormat("yyyy-MM-dd", Locale.getDefault());
    @Override
    public void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);


    }
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState){
        myView = inflater.inflate(R.layout.fragment_recv_history, container, false);
        //lv = (ListView)myView.findViewById(R.id.rcvHistoryListView);
        recyclerView = (RecyclerView)myView.findViewById(R.id.rcvHistoryRecycleView);
        c = Calendar.getInstance();
        Log.d("RcvhistoryPage","date="+dayDateFormat.format(c.getTime()));

        btnPickDate = (Button)myView.findViewById(R.id.btnPickDate);
        btnPickDate.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //開啟選日期
                DialogFragment newFragment = new DatePickerFragment();
                newFragment.show(getActivity().getFragmentManager(), "datePicker");
                Log.d("RcvHistoryPage","show DialogFragment");
            }
        });
        btnPickDate.setText(dayDateFormat.format(c.getTimeInMillis()));
        Log.d("RcvHistoryPage:", "onCreateView");

        return myView;
    }

    public void updateListView(Bundle bundle){

        //c = Calendar.getInstance(TimeZone.getDefault());
        formattedDate = dayDateFormat.format(c.getTime());
        Log.d("RcvHistoryPage:","updateLV formatted="+formattedDate);
        if(bundle != null){
            /*
            ArrayList<HashMap<String, String>> DataList = ((ArrayList<HashMap<String, String>>) bundle.getSerializable("arrayList"));;
            ListAdapter adapter = new SimpleAdapter(
                    getActivity(), DataList,
                    R.layout.rcvhistory_list_item, new String[] {getString(R.string.requireTime),getString(R.string.arriveTime),getString(R.string.state),
                    getString(R.string.sender),getString(R.string.desLocation),getString(R.string.startLocation),getString(R.string.key)},
                    new int[] {R.id.RcvrequireTime,R.id.RcvarriveTime,R.id.Rcvstate,R.id.Rcvsender,R.id.Rcvdes_id,R.id.Rcvstart,R.id.Rcvkey});
            lv.setAdapter(adapter);
            */
            ArrayList<Order> orderlist = (ArrayList<Order>)bundle.getSerializable("arrayList");
            MyAdapter myAdapter=new MyAdapter(orderlist);
            LinearLayoutManager layoutManager=new LinearLayoutManager(getActivity());
            layoutManager.setOrientation(LinearLayoutManager.VERTICAL);
            recyclerView.setLayoutManager(layoutManager);
            recyclerView.setAdapter(myAdapter);
        }
        else{
            Log.d("RcvHistPage","updateLV bundle is null");
        }

    }

}
