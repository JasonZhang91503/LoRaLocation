package com.example.keng.main;

import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentTransaction;
import android.util.Log;
import android.view.View;
import android.support.design.widget.NavigationView;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.Menu;
import android.view.MenuItem;

import java.util.ArrayList;

public class Mainpage extends AppCompatActivity
        implements NavigationView.OnNavigationItemSelectedListener ,NewOrderFragment3.ListComplete,HomeFragment.HFinterface,HistoryMainFragment.HistMaininterface{
        int index=0;
    HomeFragment homeFragment;
    NewOrderFragment newOrderFragment;
    HistoryMainFragment historyFragment;
    HistoryMain_RecvFragment historyMain_recvFragment;
    AccountFragment accountFragment;
    ArrayList<Order> dataset;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_mainpage);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(
                this, drawer, toolbar, R.string.navigation_drawer_open, R.string.navigation_drawer_close);
        drawer.addDrawerListener(toggle);
        toggle.syncState();

        NavigationView navigationView = (NavigationView) findViewById(R.id.nav_view);
        navigationView.setNavigationItemSelectedListener(this);

        //在Mainpage讀資料，一一將每一筆資料傳遞到每個fragment
         dataset = new ArrayList<Order>();

        Order[] order=new Order[20];

        order[0]=new Order(3,4,50,"耿楷寗","胡育旋","2017-5-01 17:00:00","","1234",1);
        order[1]=new Order(0,2,30,"張紘綸","耿楷寗","2017-4-28 11:00:00","記得要做考古題","5678",2);
        order[2]=new Order(1,5,40,"耿楷寗","張主任","2017-4-28 09:00:00","內有公文記得簽收","1213",3);
        order[3]=new Order(0,4,20,"卓書宇","耿楷寗","2017-4-17 17:30:00","","2042",4);
        order[4]=new Order(4,2,20,"耿楷寗","謝正鴻","2017-4-17 14:30:00","","1356",1);
        order[5]=new Order(1,3,20,"周聿晟","耿楷寗","2017-4-17 10:30:00","","9073",2);
        order[6]=new Order(3,5,20,"張晏誠","耿楷寗","2017-4-10 14:00:00","","7658",3);
        order[7]=new Order(5,1,20,"耿楷寗","謝正鴻","2017-4-10 13:30:00","吃皮蛋麵","2341",4);
        order[8]=new Order(0,5,20,"卓書宇","耿楷寗","2017-4-10 10:00:00","吃老虎麵","6455",1);
        order[9]=new Order(3,1,20,"卓書宇","耿楷寗","2017-4-10 09:30:00","郎有會","9527",4);
        for(int i=0;i<10;i++){
            dataset.add(order[i]);
            Log.d("tag",String.valueOf(i));
        }

        if(savedInstanceState==null){
            homeFragment=new HomeFragment(dataset);
            newOrderFragment=new NewOrderFragment();
            historyFragment=new HistoryMainFragment(dataset);
            historyMain_recvFragment=new HistoryMain_RecvFragment(dataset);
            accountFragment=new AccountFragment();
            getSupportFragmentManager().beginTransaction().add(R.id.frame,homeFragment).commit();
        }
    }

    @Override
    public void onBackPressed() {
        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        if (drawer.isDrawerOpen(GravityCompat.START)) {
            drawer.closeDrawer(GravityCompat.START);
        } else {
            super.onBackPressed();
        }
    }


    @Override
    public boolean onNavigationItemSelected(MenuItem item) {
        // Handle navigation view item clicks here.
        int id = item.getItemId();


        if(id==R.id.nav_home){
            if(index!=0){
                switchFragment(homeFragment,index);
                Log.d(getClass().getSimpleName()+" navi","0");
                index=0;
            }
        }else if(id==R.id.nav_newOrder){
            if(index!=1){
                switchFragment(newOrderFragment,index);
                index=1;
                Log.d(getClass().getSimpleName()+" navi","1");
            }

        }else if(id==R.id.nav_sendhistory){
            if(index!=2){
                switchFragment(historyFragment,index);
                Log.d(getClass().getSimpleName()+" navi","2");
                index=2;
            }

        }else if(id==R.id.nav_recvhistory){
            if(index!=3) {
                switchFragment(historyMain_recvFragment, index);
                Log.d(getClass().getSimpleName()+" navi","3");
                index=3;
            }
        }
        else if(id==R.id.nav_setting){
            if(index!=4) {
                switchFragment(accountFragment, index);
                Log.d(getClass().getSimpleName()+" navi","3");
                index=4;
            }

        }else if(id==R.id.nav_logout){

        }

        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        drawer.closeDrawer(GravityCompat.START);
        return true;
    }
    public void switchFragment(Fragment to,int index){//目的地 , 出發地
        FragmentManager fm = getSupportFragmentManager();
        FragmentTransaction transaction = fm.beginTransaction();

        if(!to.isAdded()){
            transaction.add(R.id.frame,to);
        }
        switch (index){
            case 0:
                transaction.addToBackStack(null);
                transaction.hide(homeFragment);
                break;
            case 1:
                transaction.addToBackStack(null);
                transaction.hide(newOrderFragment);
                break;
            case 2:
                transaction.addToBackStack(null);
                transaction.hide(historyFragment);
                break;
            case 3:
                transaction.addToBackStack(null);
                transaction.hide(historyMain_recvFragment);
                break;
            case 4:
                transaction.addToBackStack(null);
                transaction.hide(accountFragment);
        }
        transaction.show(to).commit();
    }

    @Override
    public void setIndex(int i){
        index=i;
        Log.d(getClass().getSimpleName(),"index 已更新");
    }

    @Override
    public void setNew_order(Order new_order) {
        dataset.add(new_order);
        Log.d("size"," "+dataset.size());
        //orderArrayList();
        Log.d(getClass().getSimpleName(),"新增訂單事件");
    }
    @Override
    public ArrayList<Order> getDataset() {
        return dataset;
    }
}
