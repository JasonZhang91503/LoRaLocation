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
import android.support.v4.app.DialogFragment;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentTransaction;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.Toast;

import com.example.huyuxuan.lora2.ConnectService;
import com.example.huyuxuan.lora2.MyBoundedService;
import com.example.huyuxuan.lora2.R;

import java.util.ArrayList;
import java.util.HashMap;

import static android.content.Context.MODE_PRIVATE;

/**
 * Created by huyuxuan on 2017/5/7.
 */

public class NewOrderFragment2 extends Fragment implements View.OnClickListener {
    private final String TAG=this.getClass().getSimpleName();
    // TODO: Rename parameter arguments, choose names that match
    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    private static final String ARG_PARAM1 = "param1";
    private static final String ARG_PARAM2 = "param2";

    // TODO: Rename and change types of parameters
    private String[] mParam1;
    private String mParam2;
    EditText note;
    Spinner spnName;
    String receiverName;
    private String myName;
    private SharedPreferences sharedPreferences;

    private static boolean isBind;
    static ConnectService mBoundService;
    private ConnectServiceReceiver receiver;
    private static final String ACTION_RECV_MSG = "com.example.huyuxuan.lora.intent.action.RECEIVE_MESSAGE";
    ArrayList<HashMap<String,String>> nameArrayList;

    String[] packet=new String[3];

    public NewOrderFragment2() {
        // Required empty public constructor
    }
    // TODO: Rename and change types and number of parameters
    public static NewOrderFragment2 newInstance(String[] param1, String param2) {
        NewOrderFragment2 fragment = new NewOrderFragment2();
        Bundle args = new Bundle();
        args.putStringArray(ARG_PARAM1, param1);
        args.putString(ARG_PARAM2, param2);
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (getArguments() != null) {
            mParam1 = getArguments().getStringArray(ARG_PARAM1);
            mParam2 = getArguments().getString(ARG_PARAM2);
        }
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        Log.d(TAG, mParam1[0]+" "+mParam1[1]);
        Log.d(TAG,mParam2);
        // Inflate the layout for this fragment
        View view=inflater.inflate(R.layout.fragment_new_order2, container, false);

        spnName=(Spinner)view.findViewById(R.id.Name);
        note=(EditText)view.findViewById(R.id.Note);
        Button btn_next=(Button)view.findViewById(R.id.btn_next);
        btn_next.setOnClickListener(this);
        isBind=false;
        MyBoundedService.fragmentID=3;
        MyBoundedService.curFragment=this;

        //向server要使用者名單
        sharedPreferences = getActivity().getSharedPreferences("data" , MODE_PRIVATE);
        myName = sharedPreferences.getString(getString(R.string.name),"");
        Intent intent = new Intent(getActivity(),ConnectService.class);
        intent.putExtra(getString(R.string.activity),"NewOrderFragment2");
        intent.putExtra(getString(R.string.id),"10");
        intent.putExtra(getString(R.string.name),myName);
        if(!isBind){
            getActivity().getApplicationContext().bindService(intent,mConnection, Context.BIND_AUTO_CREATE);
            isBind=true;
            Log.d("NewOrderFragment2:", "checkSR->bind");
        }
        else{
            mBoundService.sendToServer(intent);
            Log.d("NewOrderFragment2:", "checkSR->sendToService");
        }
        setReceiver();
        spnName.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                receiverName = parent.getItemAtPosition(position).toString();
                receiverName = receiverName.substring(receiverName.indexOf('(')+1,receiverName.indexOf(')'));
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });
        return view;
    }
    public void setPacket(){
        packet[0]=mParam2;
        packet[1]=receiverName;
        packet[2]=String.valueOf(note.getText());
    }
    @Override
    public void onClick(View v) {
        //前半部再做防呆
        if(receiverName==null){
            Toast.makeText(getContext(),"請填寫姓名",Toast.LENGTH_SHORT).show();
        }else {
            setPacket();
            NewOrderFragment3 fragment3 = NewOrderFragment3.newInstance(mParam1, packet);
            FragmentManager fm = getActivity().getSupportFragmentManager();
            FragmentTransaction trans = fm.beginTransaction();
            trans.addToBackStack("null");
            trans.replace(R.id.fragment_container, fragment3, fragment3.getTag());
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
            Log.d("NewOrderFragment2","onServiceDisconnected");
            mBoundService = null;
            isBind=false;
        }

    };



    //接收广播类
    public class ConnectServiceReceiver extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
            if(intent.getStringExtra("activity").equals("NewOrderFragment2")){
                if(isAdded()){
                    try{
                        getActivity().getApplicationContext().unregisterReceiver(receiver);
                    }catch (Exception e){
                        e.printStackTrace();
                    }
                }
                Bundle bundle = intent.getExtras();
                String id = bundle.getString(getString(R.string.id));
                switch(id){
                    case "10"://要使用者名單

                        if (bundle != null) {
                            nameArrayList=(ArrayList<HashMap<String,String>>) bundle.getSerializable("nameList");
                            String nameArray[]=new String[nameArrayList.size()];

                            for(int i=0;i<nameArrayList.size();i++){
                                HashMap<String,String> tmp = nameArrayList.get(i);
                                for(String key:tmp.keySet()){
                                    Log.d("NameArray","account="+key+"name="+tmp.get(key));
                                    nameArray[i]=tmp.get(key)+"("+key+")";
                                }
                            }
                            ArrayAdapter<String > nameList = new ArrayAdapter<>(getActivity(),
                                    R.layout.support_simple_spinner_dropdown_item,nameArray);
                            spnName.setAdapter(nameList);

                        } else {
                            Toast.makeText(getActivity().getApplicationContext(), "bundle null", Toast.LENGTH_LONG).show();
                        }
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
        Log.d("NewOrderFragment2","register receiver");
    }

    @Override
    public void onStop(){
        Log.d("NewOrderFragment2:", "onStop");
        if(isBind){
            try{
                getActivity().getApplicationContext().unbindService(mConnection);
            }catch (Exception e){
                e.printStackTrace();
            }
            isBind=false;
        }
        super.onStop();
    }

    @Override
    public void onResume(){
        Log.d("NewOrderFragment2","onResume");
        MyBoundedService.fragmentID=3;
        MyBoundedService.curFragment=this;
        super.onResume();
    }
}
