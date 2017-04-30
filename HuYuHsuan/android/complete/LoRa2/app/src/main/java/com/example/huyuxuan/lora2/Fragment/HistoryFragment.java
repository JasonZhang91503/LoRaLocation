package com.example.huyuxuan.lora2.Fragment;

import android.graphics.Color;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.view.PagerAdapter;
import android.support.v4.view.PagerTabStrip;
import android.support.v4.view.ViewPager;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.example.huyuxuan.lora2.ConnectService;
import com.example.huyuxuan.lora2.R;

/**
 * Created by huyuxuan on 2017/4/28.
 */

public class HistoryFragment extends Fragment {
    private View myview;
    ViewPager pager;
    PagerTabStrip tab_strp;


    @Override
    public void onCreate(Bundle savedInstances){
        super.onCreate(savedInstances);

    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState){
        myview = inflater.inflate(R.layout.fragment_history,container,false);
        pager = (ViewPager)myview.findViewById(R.id.pager);
        PagerAdapter myPagerAdapter = new PaperAdapter(getFragmentManager());
        pager.setAdapter(myPagerAdapter);
        tab_strp = (PagerTabStrip)myview.findViewById(R.id.tab_strip);
        tab_strp.setTextColor(Color.WHITE);

        return myview;
    }



}
