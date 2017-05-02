package com.example.huyuxuan.lora2.Fragment;

import android.support.v4.app.Fragment;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
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
import java.util.Calendar;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Locale;

/**
 * Created by huyuxuan on 2017/4/28.
 */

public class HomeFragment extends Fragment {

    private View myview;
    private Button btnProfile;
    private Button btnRegister;
    private ListView lv;

    private static boolean isBind;
    static ConnectService mBoundService;
    private ConnectServiceReceiver receiver;
    private static final String ACTION_RECV_MSG = "com.example.huyuxuan.lora.intent.action.RECEIVE_MESSAGE";
    static java.text.SimpleDateFormat dayDateFormat = new java.text.SimpleDateFormat("yyyy-MM-dd", Locale.getDefault());


    @Override
    public void onCreate(Bundle savedInstances){
        super.onCreate(savedInstances);
    }
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState){
        Log.d("HomeFragment","onCreateView");
        //inflate layout for this fragment
        myview = inflater.inflate(R.layout.fragment_home,container,false);
        lv = (ListView)myview.findViewById(R.id.home_listview);

        btnProfile = (Button)myview.findViewById(R.id.btnProfile);
        btnRegister = (Button)myview.findViewById(R.id.btnHomeToRegister);
        isBind = false;
        updateListView();

        btnProfile.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //跳到個人資料介面
                ProfileFragment profileFragment = new ProfileFragment();
                profileFragment.setTargetFragment(HomeFragment.this,0);
                getFragmentManager().beginTransaction()
                        .addToBackStack(null)
                        .replace(R.id.fragment_container,profileFragment).commit();
            }
        });
        btnRegister.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //跳到登記寄件
                RegisterFragment registerFragment = new RegisterFragment();
                registerFragment.setTargetFragment(HomeFragment.this, 0);
                getFragmentManager().beginTransaction()
                        .addToBackStack(null)
                        .replace(R.id.fragment_container,registerFragment).commit();
            }
        });
        return myview;
    }

    public void updateListView(){

        Calendar c = Calendar.getInstance();
        String formattedDate = dayDateFormat.format(c.getTime());
        Log.d("HomeFragment","updateListView:formattedDate:"+formattedDate);

        Intent intent = new Intent(getActivity(),ConnectService.class);
        intent.putExtra(getString(R.string.activity),"HomeFragment");
        intent.putExtra(getString(R.string.id),"9");
        intent.putExtra(getString(R.string.requireTime),formattedDate);

        if(!isBind){
            getActivity().getApplicationContext().bindService(intent,mConnection, Context.BIND_AUTO_CREATE);
            isBind=true;
            Log.d("HomeFragment:", "checkSR->bind");
        }
        else{
            mBoundService.sendToServer(intent);
            Log.d("HomeFragment:", "checkSR->sendToService");
        }
        setReceiver();

    }

    //接收广播类
    public class ConnectServiceReceiver extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
            if(intent.getStringExtra("activity").equals("HomeFragment")){
                getActivity().getApplicationContext().unregisterReceiver(receiver);
                getActivity().getApplicationContext().unbindService(mConnection);
                Bundle bundle = intent.getExtras();
                ArrayList<HashMap<String, String>> DataList = ((ArrayList<HashMap<String, String>>) bundle.getSerializable("arrayList"));;
               // ArrayList list = bundle.getParcelableArrayList("list");
               // DataList = (ArrayList<HashMap<String, String>>)list.get(0);
                ListAdapter adapter = new SimpleAdapter(
                        getActivity(), DataList,
                        R.layout.list_item, new String[] {getString(R.string.requireTime),getString(R.string.state),
                        getString(R.string.sender),getString(R.string.receiver),getString(R.string.desLocation),getString(R.string.key)},
                        new int[] {R.id.requireTime,R.id.state,R.id.sender,R.id.receiver,R.id.des_id,R.id.key});
                lv.setAdapter(adapter);

            }
        }
    }

    private static ServiceConnection mConnection = new ServiceConnection() {
        //EDITED PART
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            // TODO Auto-generated method stub
            mBoundService = ((ConnectService.LocalBinder)service).getService();
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            // TODO Auto-generated method stub
            Log.d("HomeFragment","onServiceDisconnected");
            mBoundService = null;
            isBind=false;
        }

    };

    private void setReceiver(){
        //动态注册receiver
        IntentFilter filter = new IntentFilter(ACTION_RECV_MSG);
        filter.addCategory(Intent.CATEGORY_DEFAULT);
        receiver = new ConnectServiceReceiver();
        getActivity().getApplicationContext().registerReceiver(receiver, filter);
        Log.d("HomeFragment:","register receiver");
    }
    @Override
    public void onResume(){
        super.onResume();
        Log.d("Home Fragment","onResume");
    }

    @Override
    public void onPause(){
        super.onPause();
        Log.d("HomeFragment","onPause");
    }
}
