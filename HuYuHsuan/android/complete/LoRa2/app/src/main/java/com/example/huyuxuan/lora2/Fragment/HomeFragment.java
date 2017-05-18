package com.example.huyuxuan.lora2.Fragment;

import android.Manifest;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Environment;
import android.provider.MediaStore;
import android.support.annotation.NonNull;
import android.support.v4.app.Fragment;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;

import com.example.huyuxuan.lora2.ConnectService;
import com.example.huyuxuan.lora2.MyBoundedService;
import com.example.huyuxuan.lora2.Order;
import com.example.huyuxuan.lora2.R;
import com.example.huyuxuan.lora2.RoundImageView;
import com.example.huyuxuan.lora2.SelectDialog;
import com.example.huyuxuan.lora2.SelectDialog.SelectDialogListener;
import com.example.huyuxuan.lora2.UIUtil;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Calendar;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;

import static android.app.Activity.RESULT_OK;
import static android.content.Context.MODE_PRIVATE;
import static android.support.v4.content.ContextCompat.checkSelfPermission;

/**
 * Created by huyuxuan on 2017/4/28.
 */

public class HomeFragment extends Fragment {

    private View myview;
    private ImageButton btnProfile;
    private ImageButton btnRegister;
    private RecyclerView recyclerView;
    private TextView tvName;
    private SharedPreferences sharedPreferences;


    private static boolean isBind;
    static ConnectService mBoundService;
    private ConnectServiceReceiver receiver;
    private static final String ACTION_RECV_MSG = "com.example.huyuxuan.lora.intent.action.RECEIVE_MESSAGE";
    static java.text.SimpleDateFormat dayDateFormat = new java.text.SimpleDateFormat("yyyy-MM-dd", Locale.getDefault());
    final private int MY_REQUEST_CODE = 123;
    final private int REQUEST_WRITE_EXTERNAL_CODE = 213;
    final private int REQUEST_READ_EXTERNAL_CODE = 230;

    private RoundImageView mHeadImage;
    private final int PHOTO_PICKED_FROM_CAMERA = 1; // 用来标识头像来自系统拍照
    private final int PHOTO_PICKED_FROM_FILE = 2; // 用来标识从相册获取头像
    private final int CROP_FROM_CAMERA = 3;
    private Uri imgUri; // 由于android手机的图片基本都会很大，所以建议用Uri，而不用Bitmap

    public static ArrayList<Order> orderlist;
    boolean resum=false;


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
        recyclerView = (RecyclerView) myview.findViewById(R.id.recyclerView);
        btnProfile = (ImageButton) myview.findViewById(R.id.btnProfile);
        btnRegister = (ImageButton) myview.findViewById(R.id.btnHomeToRegister);
        mHeadImage = (RoundImageView)myview.findViewById(R.id.main_roundImage);
        tvName = (TextView)myview.findViewById(R.id.textViewName);
        sharedPreferences = getActivity().getSharedPreferences("data" , MODE_PRIVATE);
        tvName.setText(sharedPreferences.getString(getString(R.string.name),"使用者"));


        isBind = false;
        updateListView();

        if (checkSelfPermission(getActivity().getApplicationContext(),Manifest.permission.READ_EXTERNAL_STORAGE)
                != PackageManager.PERMISSION_GRANTED) {

            requestPermissions(new String[]{Manifest.permission.READ_EXTERNAL_STORAGE},
                    REQUEST_READ_EXTERNAL_CODE);
        }

        String sd = Environment.getExternalStorageDirectory().toString();

        Bitmap bitmap = BitmapFactory.decodeFile(sd + "/mypic.png");
        if(bitmap != null){
            mHeadImage.setImageBitmap(bitmap);
        }

        btnProfile.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //跳到個人資料介面
                AccountFragment accountFragment = new AccountFragment();
                accountFragment.setTargetFragment(HomeFragment.this,0);
                getFragmentManager().beginTransaction()
                        .addToBackStack(null)
                        .replace(R.id.fragment_container,accountFragment).commit();
            }
        });
        btnRegister.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //跳到登記寄件
                NewOrderFragment newOrderFragment = new NewOrderFragment();
                newOrderFragment.setTargetFragment(HomeFragment.this, 0);
                getFragmentManager().beginTransaction()
                        .addToBackStack(null)
                        .replace(R.id.fragment_container,newOrderFragment).commit();

            }
        });
        mHeadImage.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                List<String> list = new ArrayList<>();
                list.add("拍照");
                list.add("相册");
                showDialog(new SelectDialogListener() {
                    @Override
                    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                        switch (position){
                            case 0:
                                //用系統相機拍照

                                if (checkSelfPermission(getActivity().getApplicationContext(),Manifest.permission.CAMERA)
                                        != PackageManager.PERMISSION_GRANTED) {

                                    requestPermissions(new String[]{Manifest.permission.CAMERA},
                                            MY_REQUEST_CODE);
                                }else{
                                    openCamera();
                                }

                                Log.d("HomeFragment","PHOTO-PICK-FROM-CAMERA");
                                break;
                            case 1:
                                //開啟相簿相片集，須由startActivityForResult且帶入requestCode進行呼叫，原因
                                //為點選相片後返回程式呼叫onActivityResult
                                Intent intent = new Intent(Intent.ACTION_GET_CONTENT, MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
                                intent.setType("image/*");
                                startActivityForResult(intent, PHOTO_PICKED_FROM_FILE);
                                Log.d("HomeFragment","PHOTO-PICK-FROM-FILE");
                                break;
                            default:
                                break;
                        }
                    }
                },list);
            }
        });

        if(orderlist!=null){
            MyAdapter myAdapter=new MyAdapter(orderlist);
            LinearLayoutManager layoutManager=new LinearLayoutManager(getActivity());
            layoutManager.setOrientation(LinearLayoutManager.VERTICAL);
            recyclerView.setLayoutManager(layoutManager);
            recyclerView.setAdapter(myAdapter);

        }

        return myview;
    }

    private SelectDialog showDialog(SelectDialogListener listener, List<String> list){
        Log.d("HomeFragment","showDialog");
        //getActivity?
        SelectDialog dialog = new SelectDialog(getActivity(),
                R.style.transparentFrameWindowStyle,listener,list);
        dialog.show();
        return dialog;
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
                if(bundle != null){
                    orderlist = (ArrayList<Order>)bundle.getSerializable("arrayList");
                    MyAdapter myAdapter=new MyAdapter(orderlist);
                    LinearLayoutManager layoutManager=new LinearLayoutManager(getActivity());
                    layoutManager.setOrientation(LinearLayoutManager.VERTICAL);
                    recyclerView.setLayoutManager(layoutManager);
                    recyclerView.setAdapter(myAdapter);
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
    @Override
    public void onResume(){
        super.onResume();
        if(resum){
           // updateListView();
        }
        Log.d("Home Fragment","onResume");
    }

    @Override
    public void onPause(){
        super.onPause();
        resum=true;
        Log.d("HomeFragment","onPause");
    }

    //拍照完畢或選取圖片後呼叫此函式
    @Override
    public void onActivityResult(int requestCode, int resultCode,Intent data)
    {
        Log.d("HomeFragment","onActivityResult");
        super.onActivityResult(requestCode, resultCode, data);
        Log.d("HomeFragment","resultCode="+resultCode+"requestCode="+requestCode);
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
        Log.d("HomeFragment","doCrop");
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
        Log.d("HomeFragment","setCropImg");
        Bundle bundle = picData.getExtras();
        if (bundle != null){
            Bitmap mBitmap = bundle.getParcelable("data");
            mHeadImage.setImageBitmap(mBitmap);
            saveBitmap(Environment.getExternalStorageDirectory() + "/mypic.png",mBitmap);
        }
    }
    private void saveBitmap(String fileName,Bitmap bitmap){
        Log.d("HomeFragment","saveBitmap");
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
            }
        }

    }

    public void openCamera(){
        Intent intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
        imgUri = Uri.fromFile(new File(Environment.getExternalStorageDirectory(),"/mypic.png"));
        intent.putExtra(MediaStore.EXTRA_OUTPUT,imgUri);
        startActivityForResult(intent,PHOTO_PICKED_FROM_CAMERA);
        Log.d("HomeFragment","openCamera");
    }


}
