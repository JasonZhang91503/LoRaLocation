package com.example.huyuxuan.lora2;

import android.support.v4.app.Fragment;
import android.content.SharedPreferences;
import android.os.Bundle;

import android.support.design.widget.NavigationView;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.MenuItem;

import com.example.huyuxuan.lora2.Fragment.HomeFragment;
import com.example.huyuxuan.lora2.Fragment.RegisterFragment;

/**
 * Created by huyuxuan on 2017/4/27.
 */

public class NavigationActivity extends AppCompatActivity implements NavigationView.OnNavigationItemSelectedListener{

    public Fragment myFragment;


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
            super.onBackPressed();
        }
    }

    @Override
    public boolean onNavigationItemSelected(MenuItem item) {
        //handle
        int itemId = item.getItemId();
        switch (itemId){
            case R.id.nav_home:
                HomeFragment firstFragment = new HomeFragment();
                myFragment = firstFragment;
                getSupportFragmentManager().beginTransaction().add(R.id.fragment_container,firstFragment).commit();
                break;
            case R.id.nav_register:
                RegisterFragment registerFragment = new RegisterFragment();
                myFragment = registerFragment;
                getSupportFragmentManager().beginTransaction().add(R.id.fragment_container,registerFragment).commit();
                break;
            case R.id.nav_history:
                break;
            case R.id.nav_setting:
                break;
            case R.id.nav_logOut:
                break;
        }
        return false;
    }
}
