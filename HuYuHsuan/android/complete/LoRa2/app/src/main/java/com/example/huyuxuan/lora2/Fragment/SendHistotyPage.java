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
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.SimpleAdapter;

import com.example.huyuxuan.lora2.ConnectService;
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
    private ListView lv;
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
        lv = (ListView)myView.findViewById(R.id.sendHistoryListView);
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
        ArrayList<HashMap<String, String>> DataList = ((ArrayList<HashMap<String, String>>) bundle.getSerializable("arrayList"));;
        ListAdapter adapter = new SimpleAdapter(
                getActivity(), DataList,
                R.layout.sendhistory_list_item, new String[] {getString(R.string.requireTime),getString(R.string.arriveTime),getString(R.string.state),
                getString(R.string.receiver),getString(R.string.desLocation),getString(R.string.startLocation),getString(R.string.key)},
                new int[] {R.id.SendrequireTime,R.id.SendarriveTime,R.id.Sendstate,R.id.Sendreceiver,R.id.Senddes_id,R.id.Sendstart,R.id.Sendkey});
        lv.setAdapter(adapter);


    }

}
