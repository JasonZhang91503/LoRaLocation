package com.example.keng.main;

import android.content.Intent;
import android.support.v4.view.PagerAdapter;
import android.support.v4.view.ViewPager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.RadioButton;
import android.widget.RadioGroup;

import java.io.BufferedReader;
import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.List;

public class IntroSlider extends AppCompatActivity {

    private PrefManager prefManager;
    private ViewPager viewPager;//使用viewpager 要先將ViewPager 放在xml中
    private ArrayList<View> viewList;
    private RadioGroup indicator;
    private RadioButton radioButton;
    private Button btnReg,btnLogin;
    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_intro_slider);
        prefManager=new PrefManager(this);
        LayoutInflater lf=getLayoutInflater().from(this);
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

        btnReg=(Button)findViewById(R.id.register);
        btnLogin=(Button)findViewById(R.id.login);

        btnLogin.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                Intent i=new Intent(IntroSlider.this,Login.class);
                startActivity(i);
                finish();

            }
        });

        btnReg.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent i=new Intent(IntroSlider.this,Register.class);
                startActivity(i);
                finish();
            }
        });

    }
    //使用ViewPager進行調控就如同Fragment 需要FragmentPagerAdapter
    public class myPagerAdapter extends PagerAdapter{


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
