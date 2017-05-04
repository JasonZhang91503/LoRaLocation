package com.example.huyuxuan.lora2;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Handler;
import android.support.v4.view.PagerAdapter;
import android.support.v4.view.ViewPager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.RadioGroup;

import java.util.ArrayList;

public class MainActivity extends AppCompatActivity {

    private static final int SPLASH_TIME_OUT = 1500;
    //用來存程式關掉之後還要存在的資料、狀態
    private SharedPreferences sharedPreferences;

    protected Button btnLogin;
    protected Button btnSignUp;

    private ViewPager viewPager;//使用viewpager 要先將ViewPager 放在xml中
    private ArrayList<View> viewList;
    private RadioGroup indicator;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_splash_screen);

        sharedPreferences = getSharedPreferences("data" , MODE_PRIVATE);
        Log.d("MainActivity","sharedpreference isLogin="+sharedPreferences.getString("isLogin",""));
        Log.d("MainActivity","sharedpreference isFirstTimeOpen="+sharedPreferences.getString("isFirstTimeOpen",""));
        new Handler().postDelayed(new Runnable() {
            @Override
            public void run() {
                //是否第一次開app，若沒有值預設是第一次(true)
                if((sharedPreferences.getString(getString(R.string.isFirstTimeOpen),"").compareTo("false")==0) &&
                        sharedPreferences.getString(getString(R.string.isLogin),"").compareTo("true")==0){
                    //不是第一次開app且已登入
                    Log.d("MainActivity","不是第一次開且已登入");
                    //跳到主畫面
                    Intent intent = new Intent();
                    intent.setClass(MainActivity.this,NavigationActivity.class);
                    startActivity(intent);
                    MainActivity.this.finish();
                    Log.d("MainActivity","跳到主畫面");

                }else{
                    //是第一次開app或是還沒登入
                    //顯示介紹
                    setContentView(R.layout.activity_main);
                    Log.d("MainActivity","第一次開app或還沒登入");
                    btnLogin = (Button)findViewById(R.id.btnToLogin);
                    btnSignUp = (Button)findViewById(R.id.btnMaintoSign);
                    sharedPreferences.edit().putString(getString(R.string.isFirstTimeOpen),"false").apply();
                    Log.d("MainActivity","sharedpreference isFirstTimeOpen="+sharedPreferences.getString("isFirstTimeOpen",""));

                    btnLogin.setOnClickListener(new View.OnClickListener() {
                        @Override
                        public void onClick(View view) {
                            //跳到登入畫面
                            Intent intent = new Intent();
                            intent.setClass(MainActivity.this,LoginActivity.class);
                            startActivity(intent);
                           // MainActivity.this.finish();
                            Log.d("MainActivity","跳到登入畫面");
                        }
                    });

                    btnSignUp.setOnClickListener(new View.OnClickListener() {
                        @Override
                        public void onClick(View view) {
                            //跳到註冊畫面
                            Intent intent = new Intent();
                            intent.setClass(MainActivity.this,SignUpActivity.class);
                            startActivity(intent);
                           // MainActivity.this.finish();
                            Log.d("MainActivity","跳到註冊畫面");

                        }
                    });
                    LayoutInflater lf=getLayoutInflater().from(MainActivity.this);
                    viewPager=(ViewPager)findViewById(R.id.view_pager);

                    //透過ViewList決定要顯示的介紹
                    View intro1=lf.inflate(R.layout.activity_intro1,null);
                    View intro2=lf.inflate(R.layout.activity_intro2,null);
                    //View intro3=lf.inflate(R.layout.activity_intro3,null);

                    viewList=new ArrayList();
                    viewList.add(intro1);
                    viewList.add(intro2);
                    //viewList.add(intro3);
                    viewPager.setAdapter(new myPagerAdapter());
                    viewPager.setCurrentItem(0);
                    //使用radioGroup將index得以顯示
                    indicator=(RadioGroup)findViewById(R.id.indicator);
                    indicator.check(R.id.index1);
                    // radioButton.setChecked(true);
                    viewPager.addOnPageChangeListener(new ViewPager.OnPageChangeListener() {
                        @Override
                        public void onPageScrolled(int position, float positionOffset, int positionOffsetPixels) {

                        }
                        @Override
                        public void onPageSelected(int position) {
                            switch (position){
                                case 0:
                                    indicator.check(R.id.index1);
                                    break;
                                case 1:
                                    indicator.check(R.id.index2);
                                    break;
                                //case 2:

                                // break;
                            }
                        }

                        @Override
                        public void onPageScrollStateChanged(int state) {

                        }
                    });
                }
            }
        },SPLASH_TIME_OUT);

    }

    @Override
    public void onResume(){
        super.onResume();
        //sharedPreferences = getSharedPreferences("data" , MODE_PRIVATE);
    }

    //使用ViewPager進行調控就如同Fragment 需要FragmentPagerAdapter
    public class myPagerAdapter extends PagerAdapter {
        public myPagerAdapter(){

        }
        @Override
        public int getCount() {
            return viewList.size();
        }
        @Override
        public Object instantiateItem(ViewGroup container, int position) { //使用此方法建立物件
            container.addView(viewList.get(position), 0);//添加頁面
            return viewList.get(position);
        }

        @Override
        public void destroyItem(ViewGroup container, int position, Object object) {
            container.removeView( viewList.get(position));
        }
        @Override
        public boolean isViewFromObject(View arg0,Object arg1){
            return arg0==arg1;
        }
    }
}
