package com.example.huyuxuan.lora2.Fragment;

import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.IBinder;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.example.huyuxuan.lora2.ConnectService;
import com.example.huyuxuan.lora2.R;

import java.util.Calendar;

import static android.content.Context.MODE_PRIVATE;

/**
 * Created by huyuxuan on 2017/5/7.
 */

public class NewOrderFragment3 extends Fragment implements View.OnClickListener {
    private final String TAG=getClass().getSimpleName();
    // TODO: Rename parameter arguments, choose names that match
    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    private static final String ARG_PARAM1 = "param1";
    private static final String ARG_PARAM2 = "param2";

    // TODO: Rename and change types of parameters
    private String[] mParam1;
    private String[] mParam2;
    Button btn_confirm;
    private static boolean isBind;
    private String myName,myAccount;
    private SharedPreferences sharedPreferences;
    int year,month,day;

    TextView start,destination,name,note,time;

    static ConnectService mBoundService;
    private ConnectServiceReceiver receiver;
    private static final String ACTION_RECV_MSG = "com.example.huyuxuan.lora.intent.action.RECEIVE_MESSAGE";

    public NewOrderFragment3() {
        // Required empty public constructor
    }

    public static NewOrderFragment3 newInstance(String[] param1, String[] param2) {
        NewOrderFragment3 fragment = new NewOrderFragment3();
        Bundle args = new Bundle();
        args.putStringArray(ARG_PARAM1, param1);
        args.putStringArray(ARG_PARAM2, param2);
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (getArguments() != null) {
            mParam1 = getArguments().getStringArray(ARG_PARAM1);
            mParam2 = getArguments().getStringArray(ARG_PARAM2);
        }
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View view=inflater.inflate(R.layout.fragment_new_order3, container, false);
        for(int i=0;i<mParam2.length;i++){
            Log.d(TAG,mParam2[i]);
        }
        Log.d(TAG,mParam1[0]);
        Log.d(TAG,mParam1[1]);

        time=(TextView)view.findViewById(R.id.txtTime);
        start=(TextView)view.findViewById(R.id.Start);
        destination=(TextView)view.findViewById(R.id.Destination);
        name=(TextView)view.findViewById(R.id.txtName);
        note=(TextView)view.findViewById(R.id.txtNote);

        setText();
        btn_confirm=(Button)view.findViewById(R.id.btn_confim);
        btn_confirm.setOnClickListener(this);
        return view;
    }
    public void setText(){
        Calendar calendar=Calendar.getInstance();
        year=calendar.get(Calendar.YEAR);
        month=calendar.get(Calendar.MONTH)+1;
        day=calendar.get(Calendar.DAY_OF_MONTH);
        time.setText(year+"年"+month+"月"+day+"日 "+mParam2[0]);
        name.setText(mParam2[1]);
        if(mParam2[2]!=null){
            note.setText(mParam2[2]);
        }else{
            note.setText("");
        }
        start.setText(mParam1[0]);
        destination.setText(mParam1[1]);
    }
    @Override
    public void onClick(View v) {
        sharedPreferences = getActivity().getSharedPreferences("data" , MODE_PRIVATE);
        myName = sharedPreferences.getString(getString(R.string.name),"");
        myAccount = sharedPreferences.getString(getString(R.string.account),"");

        //向server登記
        Intent intent = new Intent(getActivity(),ConnectService.class);
        intent.putExtra(getString(R.string.activity),"NewOrderFragment3");
        intent.putExtra(getString(R.string.id),"4");
        intent.putExtra(getString(R.string.requireTime),year+"-0"+month+"-"+day+" "+mParam2[0]+":00");
        intent.putExtra(getString(R.string.sender),myAccount);
        intent.putExtra(getString(R.string.receiver),mParam2[1]);
        intent.putExtra(getString(R.string.startLocation),mParam1[0]);
        intent.putExtra(getString(R.string.desLocation),mParam1[1]);
        intent.putExtra(getString(R.string.note),mParam2[2]);

        if(!isBind){
            getActivity().getApplicationContext().bindService(intent,mConnection,Context.BIND_AUTO_CREATE);
            isBind=true;
            Log.d("NewOrderFragment3:", "apply->bind");
        }
        else{
            if(mBoundService == null){
                Log.e("NewOrderFragment3","mBoundService is null");
            }
            else{
                Log.d("NewOrderFragment3:", "apply->sendToService");
                mBoundService.sendToServer(intent);
            }

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
            Log.d("NewOrderFragment3","onServiceDisconnected");
            //mBoundService = null;
            isBind=false;
        }

    };



    //接收广播类
    public class ConnectServiceReceiver extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
            if(intent.getStringExtra("activity").equals("NewOrderFragment3")){
                getActivity().getApplicationContext().unregisterReceiver(receiver);
                getActivity().getApplicationContext().unbindService(mConnection);
                Bundle bundle = intent.getExtras();
                String id = bundle.getString(getString(R.string.id));
                if(id.equals("4")){//是登記寄件結果
                    String type = bundle.getString(getString(R.string.type));
                    if(type.compareTo("1")==0){
                        //登記寄件成功，回到主畫面
                        Toast.makeText(getContext(),"登記寄件成功",Toast.LENGTH_SHORT).show();
                        String money = sharedPreferences.getString("Money","100");
                        int mny = Integer.valueOf(money);
                        money = String.valueOf(mny-30);
                        sharedPreferences.edit().putString("Money",money).apply();

                        HomeFragment homeFragment = new HomeFragment();
                        homeFragment.setTargetFragment(NewOrderFragment3.this, 0);
                        getFragmentManager().beginTransaction()
                                .addToBackStack(null)
                                .replace(R.id.fragment_container,homeFragment).commit();


                    }else if(type.compareTo("0")==0){
                        //登記寄件失敗，回到主畫面
                        String errorMsg=bundle.getString(getString(R.string.errorMsg));
                        Toast.makeText(getContext(),"登記寄件失敗,"+errorMsg,Toast.LENGTH_SHORT).show();

                        HomeFragment homeFragment = new HomeFragment();
                        homeFragment.setTargetFragment(NewOrderFragment3.this, 0);
                        getFragmentManager().beginTransaction()
                                .addToBackStack(null)
                                .replace(R.id.fragment_container,homeFragment).commit();
                    }else{
                        Log.d("NewOrderFragment3","type error");
                    }
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
        Log.d("NewOrderFragment3","register receiver");
    }
}
