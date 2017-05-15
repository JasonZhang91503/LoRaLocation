package com.example.huyuxuan.lora2.Fragment;

import android.app.AlertDialog;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.os.Environment;
import android.os.IBinder;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.example.huyuxuan.lora2.ConnectService;
import com.example.huyuxuan.lora2.MyBoundedService;
import com.example.huyuxuan.lora2.R;
import com.example.huyuxuan.lora2.RoundImageView;

import static android.content.Context.MODE_PRIVATE;

/**
 * Created by huyuxuan on 2017/5/15.
 */

public class AccountFragment extends Fragment {

    ImageView editMail,editPwd;
    TextView pwd,mail;
    TextView myNameView;
    TextView myAccountView;
    RoundImageView photoView;

    String password;
    String email;

    private SharedPreferences sharedPreferences;
    private static boolean isBind;
    static ConnectService mBoundService;
    private ConnectServiceReceiver receiver;
    private static final String ACTION_RECV_MSG = "com.example.huyuxuan.lora.intent.action.RECEIVE_MESSAGE";
    public AccountFragment() {
        // Required empty public constructor
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment

        View view=inflater.inflate(R.layout.fragment_account, container, false);

        sharedPreferences = getActivity().getSharedPreferences("data" , MODE_PRIVATE);
        password = sharedPreferences.getString(getString(R.string.password),"");
        email = sharedPreferences.getString(getString(R.string.email),"");

        editMail=(ImageView)view.findViewById(R.id.editmail);
        editPwd=(ImageView)view.findViewById(R.id.editpwd);
        myNameView = (TextView)view.findViewById(R.id.name);
        myAccountView = (TextView)view.findViewById(R.id.account);
        pwd=(TextView)view.findViewById(R.id.password);
        mail=(TextView)view.findViewById(R.id.mail);
        photoView = (RoundImageView) view.findViewById(R.id.acc_roundImage);

        myNameView.setText(sharedPreferences.getString(getString(R.string.name),""));
        myAccountView.setText(sharedPreferences.getString(getString(R.string.account),""));
        pwd.setText(sharedPreferences.getString(getString(R.string.password),""));
        mail.setText(sharedPreferences.getString(getString(R.string.email),""));

        String sd = Environment.getExternalStorageDirectory().toString();
        Bitmap bitmap = BitmapFactory.decodeFile(sd + "/mypic.png");
        if(bitmap != null){
            photoView.setImageBitmap(bitmap);
        }


        editMail.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                View mView =getActivity().getLayoutInflater().inflate(R.layout.change_mail_dialog,null);
                final EditText editmail=(EditText)mView.findViewById(R.id.edit_mail);
                final AlertDialog mailbuilder=new AlertDialog.Builder(getContext()).setView(mView).setPositiveButton("確認",null).setNegativeButton("取消",null).create();
                mailbuilder.setOnShowListener(new DialogInterface.OnShowListener() {
                    @Override
                    public void onShow(final DialogInterface dialog) {
                        Button btn_confirm=mailbuilder.getButton(AlertDialog.BUTTON_POSITIVE);
                        btn_confirm.setOnClickListener(new View.OnClickListener() {
                            @Override
                            public void onClick(View v) {
                                if(editmail.getText().length()==0){
                                    Toast.makeText(getContext(),"請輸入新信箱",Toast.LENGTH_SHORT).show();
                                }else{
                                    //傳入更新信箱至資料庫
                                    email = editmail.getText().toString();
                                    sendToService();
                                    mail.setText(editmail.getText());
                                    dialog.dismiss();
                                }
                            }
                        });
                        Button btn_cancel=mailbuilder.getButton(AlertDialog.BUTTON_NEGATIVE);
                        btn_cancel.setOnClickListener(new View.OnClickListener() {
                            @Override
                            public void onClick(View v) {
                                dialog.dismiss();
                            }
                        });
                    }
                });
                mailbuilder.show();

            }
        });
        editPwd.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                View mView =getActivity().getLayoutInflater().inflate(R.layout.change_pwd_dialog,null);
                final EditText editPwd=(EditText)mView.findViewById(R.id.edit_pwd);
                final EditText editConfirmPwd=(EditText)mView.findViewById(R.id.edit_confirm_pwd);
                final AlertDialog pwdbuilder=new AlertDialog.Builder(getContext()).setView(mView).setPositiveButton("確認",null).setNegativeButton("取消",null).create();
                pwdbuilder.setOnShowListener(new DialogInterface.OnShowListener() {
                    @Override
                    public void onShow(final DialogInterface dialog) {
                        Button btn_confirm=pwdbuilder.getButton(AlertDialog.BUTTON_POSITIVE);
                        btn_confirm.setOnClickListener(new View.OnClickListener() {
                            @Override
                            public void onClick(View v) {
                                if(editConfirmPwd.getText().length()==0||editPwd.getText().length()==0){
                                    Toast.makeText(getContext(),"請檢查是否有空白欄位",Toast.LENGTH_SHORT).show();
                                }else if(!editPwd.getText().toString().equals(editConfirmPwd.getText().toString())){
                                    Toast.makeText(getContext(),"請檢查密碼是否相同",Toast.LENGTH_SHORT).show();
                                }else{
                                    //傳入更新密碼至資料庫
                                    pwd.setText(editPwd.getText());
                                    password=editPwd.getText().toString();
                                    sendToService();
                                    dialog.dismiss();
                                }
                            }
                        });
                        Button btn_cancel=pwdbuilder.getButton(AlertDialog.BUTTON_NEGATIVE);
                        btn_cancel.setOnClickListener(new View.OnClickListener() {
                            @Override
                            public void onClick(View v) {
                                dialog.dismiss();
                            }
                        });
                    }
                });
                pwdbuilder.show();
            }


        });
        return view;
    }

    public void sendToService(){
        //傳給server要求
        Intent intent = new Intent(getActivity(),ConnectService.class);
        intent.putExtra(getString(R.string.activity),"AccountFragment");
        intent.putExtra(getString(R.string.id),"12");
        intent.putExtra(getString(R.string.password),password);
        intent.putExtra(getString(R.string.email),email);

        if(!isBind){
            getActivity().getApplicationContext().bindService(intent,mConnection, Context.BIND_AUTO_CREATE);
            isBind=true;
            Log.d("AccountFragment:", "checkSR->bind");
        }
        else{
            mBoundService.sendToServer(intent);
            Log.d("AccountFragment:", "checkSR->sendToService");
        }
        setReceiver();
    }

    //接收广播类
    public class ConnectServiceReceiver extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
            if(intent.getStringExtra("activity").equals("AccountFragment")){
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
                        Log.d("AccountFragment","type error");
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
        Log.d("AccountFragment:","register receiver");
    }
}