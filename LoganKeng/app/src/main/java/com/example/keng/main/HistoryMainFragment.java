package com.example.keng.main;


import android.app.Activity;
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
import android.widget.TextView;

import java.util.ArrayList;
import java.util.Calendar;


/**
 * A simple {@link Fragment} subclass.
 */
public class HistoryMainFragment extends Fragment {

    HistMaininterface histMaininterface;
    public interface HistMaininterface{
        ArrayList<Order>getDataset();
    }
    @Override
    public void onAttach(Activity activity) {
        super.onAttach(activity);
        try {
            histMaininterface= (HistMaininterface) activity;
        } catch (ClassCastException e) {
            throw new ClassCastException(activity.toString() + " must implement OnItemClickedListener");
        }
    }
    ArrayList<Order> dataset;
    ViewPager viewPager;
    View view;
    int type=0;
    public HistoryMainFragment(ArrayList<Order> orderData) {
        // Required empty public constructor
        dataset=new ArrayList<Order>();
        dataset=orderData;
    }
    @Override
    public void onResume(){
        super.onResume();
        Log.d("histmain On resume","ok");
        dataset=histMaininterface.getDataset();
        setView();

    }
    public void setView(){
        PagerAdapter adapter=new PagerAdapter(getActivity().getSupportFragmentManager(),dataset,type);//傳入資料表和看是查看寄件紀錄還是收件紀錄
        Log.d("set adapter",type+" ok");
        TabLayout tabLayout=(TabLayout) view.findViewById(R.id.tabLayout);
        viewPager=(ViewPager)view.findViewById(R.id.pager);
        viewPager.setAdapter(adapter);
        viewPager.addOnPageChangeListener(new TabLayout.TabLayoutOnPageChangeListener(tabLayout));
        final TextView date=(TextView)view.findViewById(R.id.txtDate);
        final Calendar calendar=Calendar.getInstance();
        date.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                DatePickerDialog datePickerDialog=new DatePickerDialog(getContext()
                        , new DatePickerDialog.OnDateSetListener() {
                    @Override
                    public void onDateSet(DatePicker view, int year, int month, int dayOfMonth) {
                        date.setText(year+"年"+(month+1)+"月"+dayOfMonth+"日");
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
    }
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        view=inflater.inflate(R.layout.fragment_history_main, container, false);
        setView();

        return view;
    }

}
