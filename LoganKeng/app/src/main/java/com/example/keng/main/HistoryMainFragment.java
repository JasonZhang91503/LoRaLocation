package com.example.keng.main;


import android.app.Activity;
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
