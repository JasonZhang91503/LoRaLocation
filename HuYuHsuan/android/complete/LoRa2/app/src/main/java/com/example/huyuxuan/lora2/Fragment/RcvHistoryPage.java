package com.example.huyuxuan.lora2.Fragment;

import android.app.DialogFragment;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.icu.text.SimpleDateFormat;
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
import java.util.TimeZone;

/**
 * Created by huyuxuan on 2017/4/29.
 */

public class RcvHistoryPage extends Fragment implements Serializable,DatePickerFragment.PassOnDateSetListener{

    private static  View myView;
    private ListView lv;
    private Button btnPickDate;

    private Calendar c;
    int myYear,myMonth,myDay;
    String formattedDate;

    private static boolean isBind;
    static ConnectService mBoundService;
    private ConnectServiceReceiver receiver;
    private static final String ACTION_RECV_MSG = "com.example.huyuxuan.lora.intent.action.RECEIVE_MESSAGE";
    static java.text.SimpleDateFormat dayDateFormat = new java.text.SimpleDateFormat("yyyy-MM-dd", Locale.getDefault());
    @Override
    public void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        isBind = false;

    }
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState){
        myView = inflater.inflate(R.layout.fragment_recv_history, container, false);
        lv = (ListView)myView.findViewById(R.id.rcvHistoryListView);
        c = Calendar.getInstance();
        myYear = c.get(Calendar.YEAR);
        myMonth = c.get(Calendar.MONTH) + 1;
        myDay = c.get(Calendar.DAY_OF_MONTH);
       // updateListView();
        btnPickDate = (Button)myView.findViewById(R.id.btnSelectTime);
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
        Log.d("RcvHistoryPage:", "pickdate set getTimeInMillis");

        return myView;
    }

    public void updateListView(){

        //c = Calendar.getInstance(TimeZone.getDefault());
        formattedDate = dayDateFormat.format(c.getTime());
        Log.d("RcvHistoryPage:","updateLV formatted="+formattedDate);

        Intent intent = new Intent(getActivity(),ConnectService.class);
        intent.putExtra(getString(R.string.activity),"RcvHistoryPage");
        intent.putExtra(getString(R.string.id),"8");
        intent.putExtra(getString(R.string.requireTime),formattedDate);
        if(!isBind){
            getActivity().bindService(intent,mConnection, Context.BIND_AUTO_CREATE);
            isBind=true;
            Log.d("RcvHistoryPage:", "checkSR->bind");
        }
        else{
            mBoundService.sendToServer(intent);
            Log.d("RcvHistoryPage:", "checkSR->sendToService");
        }
        setReceiver();

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
            Log.d("RcvHistoryPage","onServiceDisconnected");
            //mBoundService = null;
            isBind=false;
        }

    };

    //接收广播类
    public class ConnectServiceReceiver extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
            if(intent.getStringExtra("activity").equals("RcvHistoryPage")){
                Log.d("RcvHistoryPage:","receiver on receive");
                getActivity().unregisterReceiver(receiver);
                Bundle bundle = intent.getExtras();
                ArrayList<HashMap<String, String>> DataList = ((ArrayList<HashMap<String, String>>) bundle.getSerializable("arrayList"));;
                // ArrayList list = bundle.getParcelableArrayList("list");
                // DataList = (ArrayList<HashMap<String, String>>)list.get(0);
                ListAdapter adapter = new SimpleAdapter(
                        getActivity(), DataList,
                        R.layout.rcvhistory_list_item, new String[] {getString(R.string.requireTime),getString(R.string.arriveTime),getString(R.string.state),
                        getString(R.string.sender),getString(R.string.desLocation),getString(R.string.startLocation),getString(R.string.key)},
                        new int[] {R.id.RcvrequireTime,R.id.RcvarriveTime,R.id.Rcvstate,R.id.Rcvsender,R.id.Rcvdes_id,R.id.Rcvstart,R.id.Rcvkey});
                lv.setAdapter(adapter);

            }
        }
    }

    private void setReceiver(){
        //动态注册receiver
        IntentFilter filter = new IntentFilter(ACTION_RECV_MSG);
        filter.addCategory(Intent.CATEGORY_DEFAULT);
        receiver = new ConnectServiceReceiver();
        getActivity().registerReceiver(receiver, filter);
        Log.d("RcvHistoryPage:","register receiver");
    }

    public void passOnDateSet(int year, int month, int day) {
        Log.d("RcvHistoryPage","curFragment.passOnDateSet");
        myYear = year;
        myMonth = month + 1;
        myDay = day;
        c.set(year,month,day);
        formattedDate = dayDateFormat.format(c.getTimeInMillis());
        btnPickDate.setText(formattedDate);
        Log.d("RcvHistoryPage:","passOn formatted="+formattedDate);
        updateListView();
    }
}
