package com.example.keng.main;

import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;

/**
 * Created by keng on 5/9/17.
 */

public class PagerAdapter extends FragmentPagerAdapter {

    final static int TAB_NUM=3;
    public PagerAdapter(FragmentManager fm) {
        super(fm);
    }

    @Override
    public Fragment getItem(int position) {
        switch (position){
            case 0:
                HistAllFragment hist_all=new HistAllFragment();
                return hist_all;
            case 1:
                HistCompleteFragment hist_complete=new HistCompleteFragment();
                return hist_complete;
            case 2:
                HistIncompleteFragment hist_incomplete=new HistIncompleteFragment();
                return hist_incomplete;
            default:
                return null;
        }
    }

    @Override
    public int getCount() {
        return TAB_NUM;
    }
}
