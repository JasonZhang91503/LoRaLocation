package com.example.keng.main;


import android.app.DatePickerDialog;
import android.os.Bundle;
import android.support.design.widget.TabLayout;
import android.support.v4.app.Fragment;
import android.support.v4.view.ViewPager;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.DatePicker;
import android.widget.ImageButton;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.Calendar;


/**
 * A simple {@link Fragment} subclass.
 */
public class HistoryMain_RecvFragment extends Fragment {

    ArrayList<Order> dataset;
    ViewPager viewPager;
    View view;
    ImageButton front,back;
    Calendar calendar;
    TextView date;
    int index_DayOfMonth;
    int index_Month;
    int index_Year;
    int type=1;
    public HistoryMain_RecvFragment(ArrayList<Order> orderData) {
        // Required empty public constructor
        dataset=new ArrayList<Order>();
        dataset=orderData;
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        view=inflater.inflate(R.layout.fragment_history_main_recv, container, false);
        PagerAdapter adapter=new PagerAdapter(getActivity().getSupportFragmentManager(),dataset,type);//傳入資料表和看是查看寄件紀錄還是收件紀錄
        Log.d("set adapter",type+" ok");
        TabLayout tabLayout=(TabLayout) view.findViewById(R.id.tabLayout);

        date=(TextView)view.findViewById(R.id.txtDate);
        setTime();
        viewPager=(ViewPager)view.findViewById(R.id.recv_pager);
        viewPager.setAdapter(adapter);
        viewPager.addOnPageChangeListener(new TabLayout.TabLayoutOnPageChangeListener(tabLayout));

        back=(ImageButton)view.findViewById(R.id.back);
        back.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                dateDecrease();
                date.setText(index_Year+"年"+(index_Month)+"月"+index_DayOfMonth+"日");
                //更改資料庫資料
            }
        });
        front=(ImageButton)view.findViewById(R.id.front);
        front.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                dateIncrease();
                date.setText(index_Year+"年"+(index_Month)+"月"+index_DayOfMonth+"日");
                //更改資料庫資料
            }
        });



        date.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        DatePickerDialog datePickerDialog=new DatePickerDialog(getContext()
                                , new DatePickerDialog.OnDateSetListener() {
                            @Override
                            public void onDateSet(DatePicker view, int year, int month, int dayOfMonth) {
                                date.setText(year+"年"+(month+1)+"月"+dayOfMonth+"日");
                                index_DayOfMonth=dayOfMonth;
                                index_Month=month+1;
                                index_Year=year;
                                //更改資料庫資料
                            }
                        },calendar.get(Calendar.YEAR),calendar.get(Calendar.MONTH),calendar.get(Calendar.DAY_OF_MONTH));
                        datePickerDialog.show();
                    }
                });
        tabLayout.addOnTabSelectedListener(new TabLayout.OnTabSelectedListener() {
            @Override
            public void onTabSelected(TabLayout.Tab tab) {
                viewPager.setCurrentItem(tab.getPosition());
            }

            @Override
            public void onTabUnselected(TabLayout.Tab tab) {

            }

            @Override
            public void onTabReselected(TabLayout.Tab tab) {

            }
        });

        return view;
    }
    public void dateDecrease(){
        if(index_DayOfMonth>0){
            index_DayOfMonth--;
        }else{
            index_Month--;
            if(index_Month>0){
                switch (index_Month){
                    case 1:
                        index_DayOfMonth=31;
                        break;
                    case 2:
                        if(index_Year%4!=0) {
                            index_DayOfMonth = 28;
                        }else{
                            index_DayOfMonth=29;
                        }
                        break;
                    case 3:
                        index_DayOfMonth=31;
                        break;
                    case 4:
                        index_DayOfMonth=30;
                        break;
                    case 5:
                        index_DayOfMonth=31;
                        break;
                    case 6:
                        index_DayOfMonth=30;
                        break;
                    case 7:
                        index_DayOfMonth=31;
                        break;
                    case 8:
                        index_DayOfMonth=31;
                        break;
                    case 9:
                        index_DayOfMonth=30;
                        break;
                    case 10:
                        index_DayOfMonth=31;
                        break;
                    case 11:
                        index_DayOfMonth=30;
                        break;
                }
            }else{
                index_Month=12;
                index_DayOfMonth=31;
                index_Year--;
            }
        }
    }
    public void dateIncrease(){
        int MAX_date=0;
        switch (index_Month){
            case 1:
                MAX_date=31;
                break;
            case 2:
                if(index_Year%4!=0) {
                    MAX_date = 28;
                }else{
                    MAX_date=29;
                }
                break;
            case 3:
                MAX_date=31;
                break;
            case 4:
                MAX_date=30;
                break;
            case 5:
                MAX_date=31;
                break;
            case 6:
                MAX_date=30;
                break;
            case 7:
                MAX_date=31;
                break;
            case 8:
                MAX_date=31;
                break;
            case 9:
                MAX_date=30;
                break;
            case 10:
                MAX_date=31;
                break;
            case 11:
                MAX_date=30;
                break;
            case 12:
                MAX_date=31;
                break;

        }
        if(index_DayOfMonth<MAX_date){
            index_DayOfMonth++;
        }else{
            index_Month++;
            if(index_Month<=12){
                index_DayOfMonth=1;
            }else{
                index_Month=1;
                index_DayOfMonth=1;
                index_Year++;
            }
        }
    }
    public void setTime(){
        calendar=Calendar.getInstance();
        index_DayOfMonth=calendar.get(Calendar.DAY_OF_MONTH);
        index_Month=calendar.get(Calendar.MONTH)+1;
        index_Year=calendar.get(Calendar.YEAR);
        date.setText(index_Year+"年"+index_Month+"月"+index_DayOfMonth+"日");
    }

}