package com.example.huyuxuan.lora2;

import android.app.FragmentManager;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.IBinder;
import android.support.v4.app.Fragment;
import android.content.SharedPreferences;
import android.os.Bundle;

import android.support.design.widget.NavigationView;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.MenuItem;
import android.widget.Toast;

import com.example.huyuxuan.lora2.Background.MyAlarmReceiver;
import com.example.huyuxuan.lora2.Fragment.AccountFragment;
import com.example.huyuxuan.lora2.Fragment.HomeFragment;
import com.example.huyuxuan.lora2.Fragment.NewOrderFragment;
import com.example.huyuxuan.lora2.Fragment.RecvHistoryFragment;
import com.example.huyuxuan.lora2.Fragment.SendHistoryFragment;

import java.util.Calendar;
import java.util.Locale;

/**
 * Created by huyuxuan on 2017/4/27.
 */

public class NavigationActivity extends AppCompatActivity implements NavigationView.OnNavigationItemSelectedListener{
    public Fragment myFragment;
    static ConnectService mBoundService;
    private static final String ACTION_RECV_MSG = "com.example.huyuxuan.lora.intent.action.RECEIVE_MESSAGE";
    private static boolean isBind;
    private ConnectServiceReceiver receiver;
    public HomeFragment firstFragment;


    private Calendar c;
    String formattedDate;
    static java.text.SimpleDateFormat dayDateFormat = new java.text.SimpleDateFormat("yyyy-MM-dd", Locale.getDefault());

    MyAlarmReceiver alarm = new MyAlarmReceiver();
    private SharedPreferences sharedPreferences;
    Bundle tmp;

    @Override
    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        tmp = savedInstanceState;
        Log.d("NavigationActivity","onCreate");
        sharedPreferences = getSharedPreferences("data" , MODE_PRIVATE);
        setContentView(R.layout.activity_navigation);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(
                this, drawer, toolbar, R.string.navigation_drawer_open, R.string.navigation_drawer_close);
        drawer.setDrawerListener(toggle);
        toggle.syncState();

        c = Calendar.getInstance();

        NavigationView navigationView = (NavigationView) findViewById(R.id.nav_view);
        navigationView.setNavigationItemSelectedListener(this);
        navigationView.getMenu().getItem(0).setChecked(true);

        alarm.setAlarm(this);

        Log.d("NavigationActivity","BGLogin="+sharedPreferences.getString("BGLogin",""));
        if(sharedPreferences.getString("BGLogin","").equals("true")){
            //須偷偷登入才能載入HomeFragment
            Intent intent = new Intent(NavigationActivity.this,ConnectService.class);
            intent.putExtra(getString(R.string.activity),"NavigationActivity");
            intent.putExtra(getString(R.string.id),"3");
            intent.putExtra(getString(R.string.account),sharedPreferences.getString(getString(R.string.account),""));
            intent.putExtra(getString(R.string.password),sharedPreferences.getString(getString(R.string.password),""));
            Log.d("NavigationActivity","偷偷登入");

            if(!isBind){
                getApplicationContext().bindService(intent,mConnection, Context.BIND_AUTO_CREATE);
                isBind=true;
                Log.d("NavigationActivity:", "login->bind");
            }
            else{
                mBoundService.sendToServer(intent);
                Log.d("NavigationActivity:", "login->sendToService");
            }
            setReceiver();
        }else{
            //不用偷偷登入即可載入HomeFragment
            loadHome(savedInstanceState);
        }
    }

    public void loadHome(Bundle tmp2){
        // Check that the activity is using the layout version with
        // the fragment_container FrameLayout
        if (findViewById(R.id.fragment_container) != null) {

            // However, if we're being restored from a previous state,
            // then we don't need to do anything and should return or else
            // we could end up with overlapping fragments.
            if (tmp2 != null) {
                return;
            }

            // Create a new Fragment to be placed in the activity layout
            //這裡要放主畫面
            if(firstFragment==null){
                firstFragment = new HomeFragment();
            }
            myFragment = firstFragment;
            getSupportFragmentManager().beginTransaction().add(R.id.fragment_container,firstFragment).commit();
        }
    }

    @Override
    protected void onStop() {
        sharedPreferences.edit().putString("BGLogin","true").apply();
        sharedPreferences.edit().putString("hasStop","true").apply();
        Log.d("NavigationActivity","onStop");
        if(mBoundService!=null){
            MyBoundedService.isConnect=false;
            mBoundService.disconnect();
        }

        super.onStop();
    }

    @Override
    protected void onDestroy() {
        Log.d("NavigationActivity","onDestroy");
        sharedPreferences.edit().putString("BGLogin","true").apply();
        Log.d("NavigationActivity","BGLogin="+sharedPreferences.getString("BGLogin",""));
        if(mBoundService!=null){
            mBoundService.disconnect();
            unregisterReceiver(receiver);
            getApplicationContext().unbindService(mConnection);
        }

        super.onDestroy();
    }

    @Override
    protected void onStart() {
        Log.d("NavigationActivity","onStart");
        super.onStart();
    }

    @Override
    protected void onResume() {
        Log.d("NavigationActivity","onResume");
        sharedPreferences.edit().putString("hasStop","false").apply();
        alarm.setAlarm(this);
        super.onResume();
    }

    @Override
    protected void onPause() {
        Log.d("NavigationActivity","onPause");
        super.onPause();
    }


    @Override
    public void onBackPressed() {
        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        if (drawer.isDrawerOpen(GravityCompat.START)) {
            drawer.closeDrawer(GravityCompat.START);
        } else {
            FragmentManager fm = this.getFragmentManager();

            if (fm.getBackStackEntryCount() == 0) {
                super.onBackPressed();
                //   this.finish();
            } else {
                fm.popBackStack();
            }
        }

    }

    @Override
    public boolean onNavigationItemSelected(MenuItem item) {
        //handle
        int itemId = item.getItemId();
        if(itemId!=R.id.nav_logOut){
            DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
            drawer.closeDrawer(GravityCompat.START);
        }
        switch (itemId){
            case R.id.nav_home:
                HomeFragment firstFragment = new HomeFragment();
                myFragment = firstFragment;
                getSupportFragmentManager().beginTransaction()
                        .addToBackStack(null)
                        .replace(R.id.fragment_container,firstFragment).commit();
                break;
            case R.id.nav_register:
                getSupportFragmentManager().beginTransaction().remove(myFragment).commit();
                NewOrderFragment newOrderFragment = new NewOrderFragment();
                myFragment = newOrderFragment;
                getSupportFragmentManager().beginTransaction()
                        .addToBackStack(null)
                        .replace(R.id.fragment_container,newOrderFragment).commit();
                break;
            case R.id.nav_send_history:
                getSupportFragmentManager().beginTransaction().remove(myFragment).commit();
                SendHistoryFragment sendHistoryFragment = new SendHistoryFragment();
                myFragment = sendHistoryFragment;
                getSupportFragmentManager().beginTransaction()
                        .addToBackStack(null)
                        .replace(R.id.fragment_container,sendHistoryFragment).commit();
                break;
            case R.id.nav_recv_history:
                getSupportFragmentManager().beginTransaction().remove(myFragment).commit();
                RecvHistoryFragment recvHistoryFragment = new RecvHistoryFragment();
                myFragment = recvHistoryFragment;
                getSupportFragmentManager().beginTransaction()
                        .addToBackStack(null)
                        .replace(R.id.fragment_container,recvHistoryFragment).commit();
                break;
            case R.id.nav_profile:
                //getSupportFragmentManager().beginTransaction().remove(myFragment).commit();
                AccountFragment accountFragment = new AccountFragment();
                myFragment = accountFragment;
                getSupportFragmentManager().beginTransaction()
                        .addToBackStack(null)
                        .replace(R.id.fragment_container,accountFragment).commit();
                break;
            case R.id.nav_logOut:
                logOutDialog();
                break;
        }
        return false;
    }

    private void logOutDialog(){
        new AlertDialog.Builder(this)
                .setTitle("登出")
                .setMessage("確定要登出嗎？")
                .setPositiveButton("是", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        clearData();
                        Intent intent = new Intent();
                        intent.setClass(NavigationActivity.this,LoginActivity.class);
                        startActivity(intent);
                        Log.d("NavigationActivity","跳到登入畫面");
                        NavigationActivity.this.finish();
                    }
                })
                .setNegativeButton("取消", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {

                    }
                })
                .show();
    }

    private void clearData(){
        Log.d("NavigationActivity","clearData");
        SharedPreferences sharedPreferences = getSharedPreferences("data" , MODE_PRIVATE);
        sharedPreferences.edit()
                .putString(getString(R.string.account),"")
                .putString(getString(R.string.password),"")
                .putString(getString(R.string.name),"")
                .putString(getString(R.string.email),"")
                .putString(getString(R.string.isLogin),"false")
                .putString("BGLogin","false")
                .apply();


        //service的socket斷線
        if(mBoundService!=null){
            mBoundService.disconnect();
            alarm.cancelAlarm(NavigationActivity.this);
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
            Log.d("NavigationActivity","onServiceDisconnected");
            mBoundService = null;
            isBind=false;
        }

    };

    //接收广播类
    public class ConnectServiceReceiver extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
            if(intent.getStringExtra("result").equals("true")){
                if(intent.getStringExtra("activity").equals("NavigationActivity")){
                    getApplicationContext().unbindService(mConnection);
                    unregisterReceiver(receiver);
                    Bundle bundle = intent.getExtras();
                    String type = bundle.getString(getString(R.string.type));
                    if(type.compareTo("1")==0){
                        //偷偷登入成功，可以載入HomeFragment要資料了
                        loadHome(tmp);
                    }else{
                        String errorMsg=bundle.getString(getString(R.string.errorMsg));
                        Log.e("NavigationActivity",errorMsg);
                    }
                }
            }
           else{
                //連線有問題
                Toast.makeText(NavigationActivity.this,"伺服器維護中,請稍候再試",Toast.LENGTH_LONG).show();
            }
        }
    }

    private void setReceiver(){
        //动态注册receiver
        IntentFilter filter = new IntentFilter(ACTION_RECV_MSG);
        filter.addCategory(Intent.CATEGORY_DEFAULT);
        receiver = new ConnectServiceReceiver();
        registerReceiver(receiver, filter);
        Log.d("NavigationActivity:","register receiver");
    }

}
