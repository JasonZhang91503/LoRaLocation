package com.example.huyuxuan.lora2.Fragment;

import android.annotation.SuppressLint;
import android.content.DialogInterface;
import android.content.SharedPreferences;
import android.support.v4.app.DialogFragment;
import android.support.v4.app.Fragment;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.support.v4.app.FragmentTransaction;
import android.support.v7.app.AlertDialog;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.SimpleAdapter;
import android.widget.Spinner;
import android.widget.TextView;

import com.example.huyuxuan.lora2.ConnectService;
import com.example.huyuxuan.lora2.R;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.HashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;

import static android.content.Context.MODE_PRIVATE;

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
    private SharedPreferences sharedPreferences;
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
    final String AVAILABLE_TIME_FRAGMENT = "available_fragment";

    @Override
    public void onCreate(Bundle savedInstances){
        super.onCreate(savedInstances);


    }
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState){
        //inflate layout for this fragment
        myview = inflater.inflate(R.layout.fragment_register,container,false);
        btnOk = (Button)myview.findViewById(R.id.btnRegisterOk);
        btnSelectTime = (Button)myview.findViewById(R.id.btnSelectTime);
        spnReceiver = (Spinner)myview.findViewById(R.id.spinnerReceiver);
        spnStart = (Spinner)myview.findViewById(R.id.spinnerStart);
        spnDes = (Spinner)myview.findViewById(R.id.spinnerDes);


        isBind = false;
        sharedPreferences = getActivity().getSharedPreferences("data" , MODE_PRIVATE);
        myName = sharedPreferences.getString(getString(R.string.name),"");
        getBuildingArray = false;


        //向server要有空時段資料
        Intent intent = new Intent(getActivity(),ConnectService.class);
        intent.putExtra(getString(R.string.activity),"RegisterFragment");
        intent.putExtra(getString(R.string.id),"10");
        intent.putExtra(getString(R.string.name),myName);
        if(!isBind){
            getActivity().getApplicationContext().bindService(intent,mConnection, Context.BIND_AUTO_CREATE);
            isBind=true;
            Log.d("RegisterFragment:", "checkSR->bind");
        }
        else{
            mBoundService.sendToServer(intent);
            Log.d("RegisterFragment:", "checkSR->sendToService");
        }
        setReceiver();

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
                    getActivity().getApplicationContext().bindService(intent,mConnection,Context.BIND_AUTO_CREATE);
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
                    getActivity().getApplicationContext().bindService(intent,mConnection, Context.BIND_AUTO_CREATE);
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

    public void setTime(String tmp){
        time=tmp;
    }


    //接收广播类
    public class ConnectServiceReceiver extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
            if(intent.getStringExtra("activity").equals("RegisterFragment")){
                getActivity().getApplicationContext().unregisterReceiver(receiver);
                //getActivity().getApplicationContext().unbindService(mConnection);
                Bundle bundle = intent.getExtras();
                String id = bundle.getString(getString(R.string.id));
                switch(id){

                    case "5":
                        String msg = bundle.getString("message");


                        //創basic dialog
                        FragmentTransaction ft = getFragmentManager().beginTransaction();
                        Fragment prev=getFragmentManager().findFragmentByTag(AVAILABLE_TIME_FRAGMENT);
                        if(prev != null){
                            ft.remove(prev);
                        }
                        ft.addToBackStack(null);
                        DialogFragment mDialogFragment = new BasicDialogFragment();
                        mDialogFragment.setArguments(bundle);
                        mDialogFragment.setRetainInstance(true); // <-- this is important - otherwise the fragment manager will crash when readding the fragment
                        mDialogFragment.show(ft,AVAILABLE_TIME_FRAGMENT);


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
        getActivity().getApplicationContext().registerReceiver(receiver, filter);
        Log.d("RegisterFragment","register receiver");
    }


    @SuppressLint("ValidFragment")
    public class BasicDialogFragment extends DialogFragment {
        View v;
        ListView lv;
        private String[] list={"09:00","09:30","10:00","10:30","11:00","11:30","12:00","12:30","13:00","13:30"
                ,"14:00","14:30","15:00","15:30","16:00","16:30","17:00","17:30"};

        @Override
        public View onCreateView(LayoutInflater inflater, ViewGroup container,
                                 Bundle savedInstanceState) {
            v = inflater.inflate(R.layout.fragment_available_time, container, false);
            lv = (ListView) v.findViewById(R.id.available_listview);

            return v;
        }

        public BasicDialogFragment(){
            //開啟畫面時把資料放進listView
            String msg = getArguments().getString(getString(R.string.message));
           // char tmp[]=msg.toCharArray();

            List<Map<String, String>> items = new ArrayList<Map<String,String>>();
            for(int i=0;i<msg.length();i++){
                Map<String, String> item = new HashMap<String, String>();
                item.put("time",list[i]);
                if(msg.charAt(i)=='1'){
                    item.put("text","可以預約");
                }else{
                    item.put("text","無法預約");
                }
                items.add(item);
            }
            SimpleAdapter adapter = new SimpleAdapter(getActivity().getApplicationContext(),items,
                    R.layout.available_time_list_item,new String[]{"time","text"}
                    ,new int[]{R.id.LI_time,R.id.LI_Choose});
            lv.setAdapter(adapter);
            for(int i=0;i<msg.length();i++){
                TextView tmpView;
                tmpView = (TextView)getViewByPosition(i,lv);
                if(msg.charAt(i)=='0'){
                    //不能預約，要把該textView的clickable設為false
                    tmpView.setClickable(false);
                }
            }
            lv.setOnItemClickListener(new AdapterView.OnItemClickListener() {
                @Override
                public void onItemClick(AdapterView<?> parent, View view, final int position, long id) {
                    new AlertDialog.Builder(getContext())
                            .setTitle("選擇時段")
                            .setMessage("確定要預約"+list[position]+"嗎？")
                            .setPositiveButton("是", new DialogInterface.OnClickListener() {
                                @Override
                                public void onClick(DialogInterface dialog, int which) {
                                    RegisterFragment.this.setTime(list[position]);
                                }
                            })
                            .setNegativeButton("取消", new DialogInterface.OnClickListener() {
                                @Override
                                public void onClick(DialogInterface dialog, int which) {

                                }
                            })
                            .show();
                }
            });
        }

        public View getViewByPosition(int pos, ListView listView) {
            final int firstListItemPosition = listView.getFirstVisiblePosition();
            final int lastListItemPosition = firstListItemPosition + listView.getChildCount() - 1;

            if (pos < firstListItemPosition || pos > lastListItemPosition ) {
                return listView.getAdapter().getView(pos, null, listView);
            } else {
                final int childIndex = pos - firstListItemPosition;
                return listView.getChildAt(childIndex);
            }
        }
    }
}
