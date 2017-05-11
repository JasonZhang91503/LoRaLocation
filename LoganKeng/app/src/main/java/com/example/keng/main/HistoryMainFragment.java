package com.example.keng.main;


import android.os.Bundle;
import android.support.design.widget.TabLayout;
import android.support.v4.app.Fragment;
import android.support.v4.view.ViewPager;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import java.util.ArrayList;


/**
 * A simple {@link Fragment} subclass.
 */
public class HistoryMainFragment extends Fragment {

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
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        view=inflater.inflate(R.layout.fragment_history_main, container, false);
        PagerAdapter adapter=new PagerAdapter(getActivity().getSupportFragmentManager(),dataset,type);//傳入資料表和看是查看寄件紀錄還是收件紀錄
        Log.d("set adapter",type+" ok");
        TabLayout tabLayout=(TabLayout) view.findViewById(R.id.tabLayout);
        viewPager=(ViewPager)view.findViewById(R.id.pager);
        viewPager.setAdapter(adapter);
        viewPager.addOnPageChangeListener(new TabLayout.TabLayoutOnPageChangeListener(tabLayout));
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

}
