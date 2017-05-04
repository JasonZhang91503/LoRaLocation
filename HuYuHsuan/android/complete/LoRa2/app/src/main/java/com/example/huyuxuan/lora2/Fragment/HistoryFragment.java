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

import com.example.huyuxuan.lora2.NavigationActivity;
import com.example.huyuxuan.lora2.R;

import java.util.Calendar;

/**
 * Created by huyuxuan on 2017/4/28.
 */

public class HistoryFragment extends Fragment {
    private View myview;
    ViewPager pager;
    PagerTabStrip tab_strp;
    PagerAdapter myPagerAdapter;

    Calendar c;
    int myYear,myMonth,myDay;
    @Override
    public void onCreate(Bundle savedInstances){
        super.onCreate(savedInstances);

    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState){
        myview = inflater.inflate(R.layout.fragment_history,container,false);
        pager = (ViewPager)myview.findViewById(R.id.pager);
        myPagerAdapter = new NavigationActivity.PaperAdapter(getFragmentManager());
        pager.setAdapter(myPagerAdapter);
        tab_strp = (PagerTabStrip)myview.findViewById(R.id.tab_strip);
        tab_strp.setTextColor(Color.WHITE);

        NavigationActivity myParent = (NavigationActivity)getActivity();
        c = Calendar.getInstance();
        myYear = c.get(Calendar.YEAR);
        myMonth = c.get(Calendar.MONTH) + 1;
        myDay = c.get(Calendar.DAY_OF_MONTH);
        myParent.passOnDateSet(myYear,myMonth,myDay);
        return myview;
    }

}

