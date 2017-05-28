package com.example.huyuxuan.lora2.Fragment;

import android.Manifest;
import android.app.AlertDialog;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.os.IBinder;
import android.provider.MediaStore;
import android.support.annotation.NonNull;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.example.huyuxuan.lora2.ConnectService;
import com.example.huyuxuan.lora2.MyBoundedService;
import com.example.huyuxuan.lora2.NavigationActivity;
import com.example.huyuxuan.lora2.R;
import com.example.huyuxuan.lora2.RoundImageView;
import com.example.huyuxuan.lora2.SelectDialog;
import com.example.huyuxuan.lora2.UIUtil;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import static android.app.Activity.RESULT_OK;
import static android.content.Context.MODE_PRIVATE;
import static android.support.v4.content.ContextCompat.checkSelfPermission;

/**
 * Created by huyuxuan on 2017/5/15.
 */

public class AccountFragment extends Fragment {

    ImageView editMail,editPwd;
    TextView pwd,mail;
    TextView myNameView;
    TextView myAccountView;
    RoundImageView photoView;
    TextView mMoneyView;
    Button btnStore;

    String password;
    String email;
    String mMoney;

    private SharedPreferences sharedPreferences;
    private static boolean isBind;
    static ConnectService mBoundService;
    private ConnectServiceReceiver receiver;
    private static final String ACTION_RECV_MSG = "com.example.huyuxuan.lora.intent.action.RECEIVE_MESSAGE";

    private final int PHOTO_PICKED_FROM_CAMERA = 1; // 用来标识头像来自系统拍照
    private final int PHOTO_PICKED_FROM_FILE = 2; // 用来标识从相册获取头像
    private final int CROP_FROM_CAMERA = 3;
    private Uri imgUri; // 由于android手机的图片基本都会很大，所以建议用Uri，而不用Bitmap
    final private int MY_REQUEST_CODE = 123;
    final private int REQUEST_WRITE_EXTERNAL_CODE = 213;
    final private int REQUEST_READ_EXTERNAL_CODE = 230;

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
        mMoney=sharedPreferences.getString("Money","100");

        editMail=(ImageView)view.findViewById(R.id.editmail);
        editPwd=(ImageView)view.findViewById(R.id.editpwd);
        myNameView = (TextView)view.findViewById(R.id.name);
        myAccountView = (TextView)view.findViewById(R.id.account);
        pwd=(TextView)view.findViewById(R.id.password);
        mail=(TextView)view.findViewById(R.id.mail);
        photoView = (RoundImageView) view.findViewById(R.id.acc_roundImage);
        mMoneyView = (TextView)view.findViewById(R.id.txtmoney);
        btnStore = (Button)view.findViewById(R.id.btn_Store);

        myNameView.setText(sharedPreferences.getString(getString(R.string.name),""));
        myAccountView.setText(sharedPreferences.getString(getString(R.string.account),""));
        pwd.setText(sharedPreferences.getString(getString(R.string.password),""));
        mail.setText(sharedPreferences.getString(getString(R.string.email),""));
        mMoneyView.setText(mMoney);
        MyBoundedService.fragmentID = 1;
        MyBoundedService.curFragment=this;

        String sd = Environment.getExternalStorageDirectory().toString();
        Bitmap bitmap = BitmapFactory.decodeFile(sd + "/mypic.png");
        if(bitmap != null){
            photoView.setImageBitmap(bitmap);
        }

        photoView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                List<String> list = new ArrayList<>();
                list.add("拍照");
                list.add("相簿");
                showDialog(new SelectDialog.SelectDialogListener() {
                    @Override
                    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                        MyBoundedService.fragmentID=8;
                        switch (position){
                            case 0:
                                //用系統相機拍照

                                if (checkSelfPermission(getActivity().getApplicationContext(), Manifest.permission.CAMERA)
                                        != PackageManager.PERMISSION_GRANTED) {

                                    requestPermissions(new String[]{Manifest.permission.CAMERA},
                                            MY_REQUEST_CODE);
                                }else{
                                    openCamera();
                                }

                                Log.d("AccountFragment","PHOTO-PICK-FROM-CAMERA");
                                break;
                            case 1:
                                //開啟相簿相片集，須由startActivityForResult且帶入requestCode進行呼叫，原因
                                //為點選相片後返回程式呼叫onActivityResult
                                Intent intent = new Intent(Intent.ACTION_GET_CONTENT, MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
                                intent.setType("image/*");
                                startActivityForResult(intent, PHOTO_PICKED_FROM_FILE);
                                Log.d("AccountFragment","PHOTO-PICK-FROM-FILE");
                                break;
                            default:
                                break;
                        }
                    }
                },list);
            }
        });


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

        btnStore.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                int tmp = Integer.valueOf(mMoney);
                tmp += 100;
                mMoney = String.valueOf(tmp);
                mMoneyView.setText(mMoney);
                sharedPreferences.edit().putString("Money",mMoney).apply();
                Toast.makeText(getContext(),"儲值成功",Toast.LENGTH_SHORT).show();
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
                        Toast.makeText(getActivity().getApplicationContext(),"更改成功",Toast.LENGTH_LONG).show();
                    }else if(type.compareTo("0")==0){
                        pwd.setText(sharedPreferences.getString(getString(R.string.password),""));
                        mail.setText(sharedPreferences.getString(getString(R.string.email),""));
                        //更改資料失敗
                        Toast.makeText(getActivity().getApplicationContext(),"更改失敗",Toast.LENGTH_LONG).show();

                    }else{
                        Log.e("AccountFragment","type error");
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
            Log.d("AccountFragment","onServiceDisconnected");
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

    //拍照完畢或選取圖片後呼叫此函式
    @Override
    public void onActivityResult(int requestCode, int resultCode,Intent data)
    {
        Log.d("AccountFragment","onActivityResult");
        super.onActivityResult(requestCode, resultCode, data);
        Log.d("AccountFragment","resultCode="+resultCode+"requestCode="+requestCode);
        if (resultCode != RESULT_OK){
            Log.d("onActivityResult","resultCode not ok");
            return;
        }
        //藉由requestCode判斷是否為開啟相機或開啟相簿而呼叫的，且data不為null
        switch (requestCode) {
            case PHOTO_PICKED_FROM_CAMERA:
                doCrop();
                break;
            case PHOTO_PICKED_FROM_FILE:
                imgUri = data.getData();
                doCrop();
                break;
            case CROP_FROM_CAMERA:
                if (data != null){
                    setCropImg(data);
                }else{
                    Log.d("onActivityResult","data = null");
                }
                break;
            default:
                break;
        }
    }

    //裁剪圖片
    private void doCrop(){
        Log.d("AccountFragment","doCrop");
        final Intent intent = new Intent("com.android.camera.action.CROP");
        //可以选择图片类型，如果是*表明所有类型的图片
        intent.setDataAndType(imgUri, "image/*");
        // 下面这个crop = true是设置在开启的Intent中设置显示的VIEW可裁剪
        intent.putExtra("crop", "true");
        // aspectX aspectY 是宽高的比例，这里设置的是正方形（长宽比为1:1）
        intent.putExtra("aspectX", 1);
        intent.putExtra("aspectY", 1);
        // outputX outputY 是裁剪图片宽高
        intent.putExtra("outputX", 300);
        intent.putExtra("outputY", 300);
        //裁剪时是否保留图片的比例，这里的比例是1:1
        intent.putExtra("scale", true);
        //是否是圆形裁剪区域，设置了也不一定有效
        //intent.putExtra("circleCrop", true);
        //设置输出的格式
        intent.putExtra("outputFormat", Bitmap.CompressFormat.JPEG.toString());
        //是否将数据保留在Bitmap中返回
        intent.putExtra("return-data", true);

        startActivityForResult(intent,CROP_FROM_CAMERA);


    }
    private void setCropImg(Intent picData){
        Log.d("AccountFragment","setCropImg");
        Bundle bundle = picData.getExtras();
        if (bundle != null){
            Bitmap mBitmap = bundle.getParcelable("data");
            saveBitmap(Environment.getExternalStorageDirectory() + "/mypic.png",mBitmap);

            String sd = Environment.getExternalStorageDirectory().toString();
            Bitmap bitmap = BitmapFactory.decodeFile(sd + "/mypic.png");
            photoView.setImageBitmap(bitmap);

        }
    }
    private void saveBitmap(String fileName,Bitmap bitmap){
        Log.d("AccountFragment","saveBitmap");
        if (checkSelfPermission(getActivity().getApplicationContext(),Manifest.permission.WRITE_EXTERNAL_STORAGE)
                != PackageManager.PERMISSION_GRANTED) {

            requestPermissions(new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE},
                    REQUEST_WRITE_EXTERNAL_CODE);
        }
        File file = new File(fileName);
        FileOutputStream fout = null;
        try {
            file.createNewFile();
            fout = new FileOutputStream(file);
            bitmap.compress(Bitmap.CompressFormat.PNG,100,fout);
            fout.flush();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                if (fout!=null){
                    fout.close();
                }
                UIUtil.showToast(getActivity(),"保存成功！");
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == MY_REQUEST_CODE) {
            if (grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                // Now user should be able to use camera
                openCamera();
            }
            else {
                // Your app will not have this permission. Turn off all functions
                // that require this permission or it will force close like your
                // original question
                Toast.makeText(getActivity(),"請開啟相機權限", Toast.LENGTH_LONG).show();
            }
        }else if(requestCode==REQUEST_WRITE_EXTERNAL_CODE){
            if (grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                // Now user should be able to use camera


            }
            else {
                // Your app will not have this permission. Turn off all functions
                // that require this permission or it will force close like your
                // original question
                Toast.makeText(getActivity(),"無法寫入外部SD", Toast.LENGTH_LONG).show();
            }
        }else if(requestCode==REQUEST_READ_EXTERNAL_CODE){
            if(grantResults[0]!=PackageManager.PERMISSION_GRANTED){
                Toast.makeText(getActivity(),"無法讀取外部SD", Toast.LENGTH_LONG).show();
            }else{
                String sd = Environment.getExternalStorageDirectory().toString();
                Bitmap bitmap = BitmapFactory.decodeFile(sd + "/mypic.png");
                if(bitmap != null){
                    photoView.setImageBitmap(bitmap);
                }
            }
        }

    }

    public void openCamera(){
        Intent intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
        imgUri = Uri.fromFile(new File(Environment.getExternalStorageDirectory(),"/mypic.png"));
        intent.putExtra(MediaStore.EXTRA_OUTPUT,imgUri);
        startActivityForResult(intent,PHOTO_PICKED_FROM_CAMERA);
        Log.d("AccountFragment","openCamera");
    }

    private SelectDialog showDialog(SelectDialog.SelectDialogListener listener, List<String> list){
        Log.d("AccountFragment","showDialog");
        SelectDialog dialog = new SelectDialog(getActivity(),
                R.style.transparentFrameWindowStyle,listener,list);
        dialog.show();
        return dialog;
    }

    @Override
    public void onResume(){
        MyBoundedService.fragmentID = 1;
        MyBoundedService.curFragment=this;
        super.onResume();
    }

}