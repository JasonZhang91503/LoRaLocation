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
import android.support.v4.app.FragmentStatePagerAdapter;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.MenuItem;
import android.view.ViewGroup;
import android.widget.ListAdapter;
import android.widget.SimpleAdapter;
import android.widget.Toast;

import com.example.huyuxuan.lora2.Fragment.DatePickerFragment;
import com.example.huyuxuan.lora2.Fragment.HistoryFragment;
import com.example.huyuxuan.lora2.Fragment.HomeFragment;
import com.example.huyuxuan.lora2.Fragment.RcvHistoryPage;
import com.example.huyuxuan.lora2.Fragment.RegisterFragment;
import com.example.huyuxuan.lora2.Fragment.SendHistotyPage;
import com.example.huyuxuan.lora2.Fragment.SettingFragment;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.HashMap;
import java.util.Locale;

/**
 * Created by huyuxuan on 2017/4/27.
 */

public class NavigationActivity extends AppCompatActivity implements NavigationView.OnNavigationItemSelectedListener,DatePickerFragment.PassOnDateSetListener{
    public Fragment myFragment;
    static ConnectService mBoundService;
    private static final String ACTION_RECV_MSG = "com.example.huyuxuan.lora.intent.action.RECEIVE_MESSAGE";
    private static boolean isBind;
    private ConnectServiceReceiver receiver;

    private static RcvHistoryPage mRcvHistoryPage;
    private static SendHistotyPage mSendHistoryPage;

    private Calendar c;
    String formattedDate;
    static java.text.SimpleDateFormat dayDateFormat = new java.text.SimpleDateFormat("yyyy-MM-dd", Locale.getDefault());

    @Override
    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);

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



        // Check that the activity is using the layout version with
        // the fragment_container FrameLayout
        if (findViewById(R.id.fragment_container) != null) {

            // However, if we're being restored from a previous state,
            // then we don't need to do anything and should return or else
            // we could end up with overlapping fragments.
            if (savedInstanceState != null) {
                return;
            }

            // Create a new Fragment to be placed in the activity layout
            //這裡要放主畫面

            HomeFragment firstFragment = new HomeFragment();
            myFragment = firstFragment;
            getSupportFragmentManager().beginTransaction().add(R.id.fragment_container,firstFragment).commit();
        }


    }

    @Override
    public void onBackPressed() {
        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        if (drawer.isDrawerOpen(GravityCompat.START)) {
            drawer.closeDrawer(GravityCompat.START);
        } else {
          //  super.onBackPressed();
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
                RegisterFragment registerFragment = new RegisterFragment();
                myFragment = registerFragment;
                getSupportFragmentManager().beginTransaction()
                        .addToBackStack(null)
                        .replace(R.id.fragment_container,registerFragment).commit();
                break;
            case R.id.nav_history:
                HistoryFragment historyFragment = new HistoryFragment();
                myFragment = historyFragment;
                getSupportFragmentManager().beginTransaction()
                        .addToBackStack(null)
                        .replace(R.id.fragment_container,historyFragment).commit();
                break;
            case R.id.nav_setting:
                SettingFragment settingFragment = new SettingFragment();
                myFragment = settingFragment;
                getSupportFragmentManager().beginTransaction()
                        .addToBackStack(null)
                        .replace(R.id.fragment_container,settingFragment).commit();
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
        SharedPreferences sharedPreferences = getSharedPreferences("data" , MODE_PRIVATE);
        sharedPreferences.edit()
                .putString(getString(R.string.account),"")
                .putString(getString(R.string.password),"")
                .putString(getString(R.string.name),"")
                .putString(getString(R.string.email),"")
                .putString(getString(R.string.isLogin),"fasle")
                .apply();
    }


    @Override
    public void passOnDateSet(int year, int month, int day) {
        Log.d("NavigationActivity","passOnDateSet");
        c.set(year,month,day);
        formattedDate = dayDateFormat.format(c.getTime());

        Intent intent = new Intent(NavigationActivity.this,ConnectService.class);
        intent.putExtra(getString(R.string.activity),"RcvHistoryPage");
        intent.putExtra(getString(R.string.id),"8");
        intent.putExtra(getString(R.string.requireTime),formattedDate);
        if(!isBind){
            getApplicationContext().bindService(intent,mConnection, Context.BIND_AUTO_CREATE);
            isBind=true;
            Log.d("NavigationActivity:", "checkSR->bind");
        }
        else{
            mBoundService.sendToServer(intent);
            Log.d("NavigationActivity:", "checkSR->sendToService");
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
            Log.d("NavigationActivity","onServiceDisconnected");
            //mBoundService = null;
            isBind=false;
        }

    };

    //接收广播类
    public class ConnectServiceReceiver extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
            if(intent.getStringExtra("activity").equals("RcvHistoryPage")){
                Log.d("NavigationActivity:","receiver on receive");
                unregisterReceiver(receiver);
                //getApplicationContext().unbindService(mConnection);
                Bundle bundle = intent.getExtras();
                //把bundle傳給recvHistoryPage
                if(mRcvHistoryPage!=null){
                    mRcvHistoryPage.updateListView(bundle);
                }
                Intent SendIntent = new Intent(NavigationActivity.this,ConnectService.class);
                SendIntent.putExtra(getString(R.string.activity),"SendHistoryPage");
                SendIntent.putExtra(getString(R.string.id),"7");
                SendIntent.putExtra(getString(R.string.requireTime),formattedDate);
                if(!isBind){
                    getApplicationContext().bindService(SendIntent,mConnection, Context.BIND_AUTO_CREATE);
                    isBind=true;
                    Log.d("NavigationActivity:", "bind in receiver");
                }
                else{
                    mBoundService.sendToServer(SendIntent);
                    Log.d("NavigationActivity:", "sendToService in receiver");
                }
                setReceiver();

            }
            else if(intent.getStringExtra("activity").equals("SendHistoryPage")){
                Log.d("NavigationActivity:","receiver on receive");
                unregisterReceiver(receiver);
                getApplicationContext().unbindService(mConnection);
                Bundle bundle = intent.getExtras();
                //把bundle傳給sendHistoryPage
                if(mSendHistoryPage!=null){
                    mSendHistoryPage.updateListView(bundle);
                }

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

    public static class PaperAdapter extends FragmentStatePagerAdapter {

        public PaperAdapter(android.support.v4.app.FragmentManager fm) {
            super(fm);
        }

        @Override
        public Fragment getItem(int position) {
            switch (position){
                case 0:
                    //創收件歷史的fragment
                    RcvHistoryPage rcpPage = new RcvHistoryPage();
                    return rcpPage;
                case 1:
                    //創寄件歷史的fragment
                    SendHistotyPage sendPage = new SendHistotyPage();
                    return sendPage;
            }
            return null;
        }

        @Override
        public int getCount() {
            return 2;
        }

        @Override
        public CharSequence getPageTitle(int position) {
            Locale l = Locale.getDefault();
            switch (position) {
                case 0:
                    return "ReceiveHistory";
                case 1:
                    return "SendHistory";
            }
            return null;
        }

        @Override
        public Object instantiateItem(ViewGroup container, int position) {
            Fragment createdFragment = (Fragment) super.instantiateItem(container, position);
            // save the appropriate reference depending on position
            switch (position) {
                case 0:
                    mRcvHistoryPage = (RcvHistoryPage) createdFragment;
                    break;
                case 1:
                    mSendHistoryPage = (SendHistotyPage) createdFragment;
                    break;
            }
            return createdFragment;
        }

    }
}
