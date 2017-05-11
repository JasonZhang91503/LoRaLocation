package com.example.huyuxuan.lora2.Fragment;

import android.app.FragmentManager;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.ContentResolver;
import android.content.ContentValues;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Bundle;
import android.os.IBinder;
import android.provider.MediaStore;
import android.support.v4.app.Fragment;
import android.support.v7.app.AlertDialog;
import android.text.InputType;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;

import com.example.huyuxuan.lora2.ConnectService;
import com.example.huyuxuan.lora2.MyBoundedService;
import com.example.huyuxuan.lora2.R;

import org.w3c.dom.Text;

import java.io.FileNotFoundException;

import static android.content.Context.MODE_PRIVATE;

/**
 * Created by huyuxuan on 2017/5/1.
 */

public class ProfileFragment extends Fragment{
    private View myview;
    private TextView myNameView;
    private TextView myAccountView;
    private TextView myEmailView;
    private TextView myPasswordView;
    private Button btnChangePassword;
    private Button btnChangeEmail;
    private Button btnChangeOk;
    String password;
    String email;

    private SharedPreferences sharedPreferences;
    private static boolean isBind;
    static ConnectService mBoundService;
    private ConnectServiceReceiver receiver;
    private static final String ACTION_RECV_MSG = "com.example.huyuxuan.lora.intent.action.RECEIVE_MESSAGE";


    @Override
    public void onCreate(Bundle savedInstances){
        super.onCreate(savedInstances);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState){
        Log.d("ProfileFragment","onCreateView");
        myview = inflater.inflate(R.layout.fragment_profile,container,false);
        myNameView = (TextView)myview.findViewById(R.id.tvProfileName);
        myAccountView = (TextView)myview.findViewById(R.id.tvProfileAccount);
        myEmailView = (TextView)myview.findViewById(R.id.tvProfileEmail);
        myPasswordView = (TextView)myview.findViewById(R.id.tvProfilePassword);
        btnChangePassword = (Button)myview.findViewById(R.id.btnChangePassword);
        btnChangeEmail = (Button)myview.findViewById(R.id.btnChangeEmail);
        btnChangeOk = (Button)myview.findViewById(R.id.btnChangeOk);

        sharedPreferences = getActivity().getSharedPreferences("data" , MODE_PRIVATE);
        password = sharedPreferences.getString(getString(R.string.password),"");
        email = sharedPreferences.getString(getString(R.string.email),"");

        myNameView.setText(sharedPreferences.getString(getString(R.string.name),""));
        myAccountView.setText(sharedPreferences.getString(getString(R.string.account),""));
        myEmailView.setText(email);
        myPasswordView.setText(password);

        /*
        final AlertDialog.Builder build = new AlertDialog.Builder(getContext());
        final AlertDialog alert;
        //final LayoutInflater inflater = LayoutInflater.from(getActivity());
        final View tmp = inflater.inflate(R.layout.edit_dialog, null);
        final EditText editText = (EditText) (tmp.findViewById(R.id.editTextChange));
        */

        btnChangePassword.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                final LayoutInflater inflater = LayoutInflater.from(getActivity());
                final View tmp = inflater.inflate(R.layout.edit_dialog, null);
                final EditText editText = (EditText) (tmp.findViewById(R.id.editTextChange));
                editText.setText(password);

                new AlertDialog.Builder(getContext())
                        .setTitle("請輸入新密碼")
                        .setView(tmp)
                        .setPositiveButton("確定", new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int which) {
                                password = editText.getText().toString();
                                myPasswordView.setText(password);
                                Log.d("changeDialog","new password="+password);
                                ((ViewGroup) tmp.getParent()).removeView(tmp);
                                dialog.dismiss();

                            }
                        })
                        .show();
            }
        });
        btnChangeEmail.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                LayoutInflater inflater = LayoutInflater.from(getActivity());
                final View tmp2 = inflater.inflate(R.layout.edit_dialog, null);
                final EditText editText2 = (EditText) (tmp2.findViewById(R.id.editTextChange));
                editText2.setText(email);
                editText2.setInputType(InputType.TYPE_TEXT_VARIATION_EMAIL_ADDRESS);
                new AlertDialog.Builder(getContext())
                        .setTitle("請輸入新信箱")
                        .setView(tmp2)
                        .setPositiveButton("確定", new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int which) {
                                email = editText2.getText().toString();
                                myEmailView.setText(email);
                                Log.d("changeDialog","new email="+email);
                                ((ViewGroup) tmp2.getParent()).removeView(tmp2);
                                dialog.dismiss();
                            }
                        })
                        .show();


                /*
                editText.setText(email);
                editText.setInputType(InputType.TYPE_TEXT_VARIATION_EMAIL_ADDRESS);
                build.setTitle("請輸入新密碼")
                        .setView(tmp)
                        .setPositiveButton("確定", new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int which) {
                                password = editText.getText().toString();
                                myPasswordView.setText(password);
                                Log.d("changeDialog","new password="+password);
                                dialog.dismiss();

                            }
                        })
                        .create();
                alert.show();
                */
            }
        });

        btnChangeOk.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(!password.equals(sharedPreferences.getString(getString(R.string.password),"")) ||
                        !email.equals(sharedPreferences.getString(getString(R.string.email),""))){
                    //傳給server要求
                    Intent intent = new Intent(getActivity(),ConnectService.class);
                    intent.putExtra(getString(R.string.activity),"ProfileFragment");
                    intent.putExtra(getString(R.string.id),"12");
                    intent.putExtra(getString(R.string.password),password);
                    intent.putExtra(getString(R.string.email),email);

                    if(!isBind){
                        getActivity().getApplicationContext().bindService(intent,mConnection, Context.BIND_AUTO_CREATE);
                        isBind=true;
                        Log.d("ProfileFragment:", "checkSR->bind");
                    }
                    else{
                        mBoundService.sendToServer(intent);
                        Log.d("ProfileFragment:", "checkSR->sendToService");
                    }
                    setReceiver();
                }else{
                    //返回上一頁
                    FragmentManager fm = getActivity().getFragmentManager();
                    if (fm.getBackStackEntryCount() != 0) {
                        fm.popBackStack();
                    }
                }
            }
        });


        return myview;
    }
    //接收广播类
    public class ConnectServiceReceiver extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
            if(intent.getStringExtra("activity").equals("ProfileFragment")){
                getActivity().getApplicationContext().unregisterReceiver(receiver);
                getActivity().getApplicationContext().unbindService(mConnection);
                Bundle bundle = intent.getExtras();
                if(bundle != null){
                    String type = bundle.getString(getString(R.string.type));
                    if(type.compareTo("1")==0){
                        //更改資料成功
                        sharedPreferences.edit()
                                .putString(getString(R.string.password),password)
                                .putString(getString(R.string.email),email)
                                .apply();
                    }else if(type.compareTo("0")==0){
                        //更改資料失敗
                        Toast.makeText(getActivity().getApplicationContext(),"更改資料失敗",Toast.LENGTH_LONG).show();

                    }else{
                        Log.d("ProfileFragment","type error");
                    }
                }else{

                    Toast.makeText(getActivity().getApplicationContext(),"bundle null",Toast.LENGTH_LONG).show();
                }
            }
        }
    }

    private static ServiceConnection mConnection = new ServiceConnection() {
        //EDITED PART
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            // TODO Auto-generated method stub

            if(MyBoundedService.myService!=null){
                mBoundService = MyBoundedService.myService;
            }
            else{
                mBoundService = ((ConnectService.LocalBinder)service).getService();
            }

            Log.d("mConnection","onServiceConnected");
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


}
