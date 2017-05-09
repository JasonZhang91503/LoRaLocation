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
import android.widget.Toast;

import com.example.huyuxuan.lora2.ConnectService;
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
    String timeText;
    boolean is_setTime=false;
    Spinner spnStart;
    Spinner spnDest;
    TextView orderTime;
    private String des;
    private String start;
    String buildingArray[];
    private static boolean isBind;
    private String time;
    String formattedDate;
    FragmentTransaction ft;

    static ConnectService mBoundService;
    private ConnectServiceReceiver receiver;
    private static final String ACTION_RECV_MSG = "com.example.huyuxuan.lora.intent.action.RECEIVE_MESSAGE";
    static java.text.SimpleDateFormat dayDateFormat = new java.text.SimpleDateFormat("yyyy-MM-dd", Locale.getDefault());
    final String AVAILABLE_TIME_FRAGMENT = "available_fragment";

    public NewOrderFragment() {
        // Required empty public constructor
    }
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment

        View view=inflater.inflate(R.layout.fragment_new_order, container, false);
        //得到日期資訊並且顯示於Text中
        LinearLayout time=(LinearLayout)view.findViewById(R.id.time);
        orderTime=(TextView)view.findViewById(R.id.orderTime);

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


        //設定下一步Btn的內容
        Button btn=(Button)view.findViewById(R.id.btn_next);
        btn.setOnClickListener(this);
        spnStart=(Spinner)view.findViewById(R.id.start);
        spnDest=(Spinner)view.findViewById(R.id.destination);
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
        //替time Linearlayout 設定ClickListener
        time.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Calendar c = Calendar.getInstance();
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
            NewOrderFragment2 fragment2 = NewOrderFragment2.newInstance(location, timeText);
            FragmentManager fm = getActivity().getSupportFragmentManager();
            FragmentTransaction trans = fm.beginTransaction();
            trans.addToBackStack("null");
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
            if(intent.getStringExtra("activity").equals("NewOrderFragment")){
                getActivity().getApplicationContext().unregisterReceiver(receiver);
                //getActivity().getApplicationContext().unbindService(mConnection);
                Bundle bundle = intent.getExtras();
                String id = bundle.getString(getString(R.string.id));
                switch(id){

                    case "5":
                        String msg = bundle.getString("message");

                        //創basic dialog
                        ft = getFragmentManager().beginTransaction();
                        DialogFragment mDialogFragment;
                        mDialogFragment = new BasicDialogFragment();
                        mDialogFragment.setArguments(bundle);
                        mDialogFragment.setTargetFragment(NewOrderFragment.this, 0);
                      //  mDialogFragment.setRetainInstance(true); // <-- this is important - otherwise the fragment manager will crash when readding the fragment
                       // mDialogFragment.show(ft,AVAILABLE_TIME_FRAGMENT);
                        ft.add(R.id.fragment_container,mDialogFragment);
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
        Log.d("NewOrderFragment","register receiver");
    }

    public void setTime(String tmp){
        time=tmp;
    }

    @SuppressLint("ValidFragment")
    public class BasicDialogFragment extends DialogFragment {
        View v;
        ListView lv;
        private String[] list={"09:00","09:30","10:00","10:30","11:00","11:30","12:00","12:30","13:00","13:30"
                ,"14:00","14:30","15:00","15:30","16:00","16:30","17:00","17:30"};
        String tmp;
        private String dialogMessage;

        @Override
        public View onCreateView(LayoutInflater inflater, ViewGroup container,
                                 Bundle savedInstanceState) {
            v = inflater.inflate(R.layout.fragment_available_time, container, false);
            lv = (ListView) v.findViewById(R.id.available_listview);
            tmp = getResources().getString(R.string.message);

            return v;
        }

        /*
        public BasicDialogFragment newInstance(String dialogMessage){
            BasicDialogFragment fragment = new BasicDialogFragment();
            Bundle args = new Bundle();
            args.putString("message", dialogMessage);
            fragment.setArguments(args);
            return fragment;
        }
        */


        public BasicDialogFragment(){
            Bundle bundle = getArguments();
            if(bundle != null) {
                dialogMessage = bundle.getString("message");
            }

            List<Map<String, String>> items = new ArrayList<Map<String,String>>();
            for(int i=0;i<dialogMessage.length();i++){
                Map<String, String> item = new HashMap<String, String>();
                item.put("time",list[i]);
                if(dialogMessage.charAt(i)=='1'){
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
            for(int i=0;i<dialogMessage.length();i++){
                TextView tmpView;
                tmpView = (TextView)getViewByPosition(i,lv);
                if(dialogMessage.charAt(i)=='0'){
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
                                    NewOrderFragment.this.setTime(list[position]);
                                    timeText=list[position];
                                    orderTime.setText(timeText);
                                    is_setTime=true;
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
