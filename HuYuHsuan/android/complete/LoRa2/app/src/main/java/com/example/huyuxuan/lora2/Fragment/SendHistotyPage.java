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

public class SendHistotyPage extends Fragment implements Serializable{
    private static View myView;
    private RecyclerView recyclerView;
    private Button btnPickTime;

    private Calendar c;
    int myYear,myMonth,myDay;
    String formattedDate;

    static java.text.SimpleDateFormat dayDateFormat = new java.text.SimpleDateFormat("yyyy-MM-dd", Locale.getDefault());

    @Override
    public void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);

    }
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState){
        myView = inflater.inflate(R.layout.fragment_send_history, container, false);
        recyclerView = (RecyclerView) myView.findViewById(R.id.sendHistoryRecycleView);
        btnPickTime = (Button)myView.findViewById(R.id.btnPickDate2);

        c = Calendar.getInstance();
        myYear = c.get(Calendar.YEAR);
        myMonth = c.get(Calendar.MONTH) + 1;
        myDay = c.get(Calendar.DAY_OF_MONTH);

        btnPickTime.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                DialogFragment newFragment = new DatePickerFragment();
                newFragment.show(getActivity().getFragmentManager(), "datePicker");
                Log.d("SendHistotyPage","show DialogFragment");
            }
        });
        btnPickTime.setText(dayDateFormat.format(c.getTimeInMillis()));
        Log.d("SendHistotyPage:", "onCreateView");

        return myView;
    }

    public void updateListView(Bundle bundle){

        //c = Calendar.getInstance(TimeZone.getDefault());
        formattedDate = dayDateFormat.format(c.getTime());
        Log.d("SendHistotyPage:","updateLV formatted="+formattedDate);
        ArrayList<Order> orderlist = (ArrayList<Order>)bundle.getSerializable("arrayList");
        MyAdapter myAdapter=new MyAdapter(orderlist);
        LinearLayoutManager layoutManager=new LinearLayoutManager(getActivity());
        layoutManager.setOrientation(LinearLayoutManager.VERTICAL);
        recyclerView.setLayoutManager(layoutManager);
        recyclerView.setAdapter(myAdapter);
    }

}
