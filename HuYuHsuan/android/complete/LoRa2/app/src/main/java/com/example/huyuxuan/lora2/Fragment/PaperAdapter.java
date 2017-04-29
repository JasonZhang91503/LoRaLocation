package com.example.huyuxuan.lora2.Fragment;

import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentStatePagerAdapter;

import java.util.Locale;

/**
 * Created by huyuxuan on 2017/4/28.
 */

public class PaperAdapter extends FragmentStatePagerAdapter {
    public PaperAdapter(FragmentManager fm) {
        super(fm);
    }

    @Override
    public Fragment getItem(int position) {
        switch (position){
            case 0:
                //創收件歷史的fragment
                RcvHistoryPage rcpPage = new RcvHistoryPage();
                return rcpPage;
            case 1:
                //創寄件歷史的fragment
                break;
        }
        return null;
    }

    @Override
    public int getCount() {
        return 2;
    }

    @Override
    public CharSequence getPageTitle(int position) {
        Locale l = Locale.getDefault();
        switch (position) {
            case 0:
                return "ReceiveHistory";
            case 1:
                return "SendHistory";
        }
        return null;
    }
}
