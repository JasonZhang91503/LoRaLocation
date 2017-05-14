package com.example.keng.main;

import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import android.util.Log;
import android.widget.ArrayAdapter;

import java.util.ArrayList;

/**
 * Created by keng on 5/9/17.
 */

public class PagerAdapter extends FragmentPagerAdapter {

    final static int TAB_NUM=3;
    boolean is_State4=true;
    ArrayList<Order> Dataset;
    ArrayList<Order> Complete_Dataset;
    ArrayList<Order> Incomplete_Dataset;
    boolean is_send;

    String name="耿楷寗";
    public PagerAdapter(FragmentManager fm,ArrayList<Order> orderData,int type) {
        super(fm);

        Dataset=new ArrayList<Order>();
        if(type==0) {
            Dataset = getSenderData(orderData);
        }else{
            Dataset = getReceiverData(orderData);
            Log.d("set receiver data","ok");
        }
    }



    @Override
    public Fragment getItem(int position) {

        switch (position){
            case 0:
                HistAllFragment hist_all=new HistAllFragment(Dataset);
                return hist_all;
            case 1:
                Complete_Dataset=getSpecifieyStateData(is_State4);
                HistCompleteFragment hist_complete=new HistCompleteFragment(Complete_Dataset);
                return hist_complete;
            case 2:
                Incomplete_Dataset=getSpecifieyStateData(!is_State4);
                HistIncompleteFragment hist_incomplete=new HistIncompleteFragment(Incomplete_Dataset);
                return hist_incomplete;
            default:
                return null;
        }
    }
    public ArrayList<Order> getSpecifieyStateData(boolean is_State4){
        Complete_Dataset=new ArrayList<Order>();
        Incomplete_Dataset=new ArrayList<Order>();
        if(is_State4) {
            for (int i = 0; i < Dataset.size(); i++) {
                if (Dataset.get(i).getOrder_state() == 4) {
                    Log.d("state 4","ok");
                    Complete_Dataset.add(Dataset.get(i));
                }
            }
            return Complete_Dataset;
        }else{
            for (int i = 0; i < Dataset.size(); i++) {
                if (Dataset.get(i).getOrder_state() != 4) {
                    Log.d("not state 4","ok");
                    Incomplete_Dataset.add(Dataset.get(i));
                }
            }
            return Incomplete_Dataset;
        }

    }
    public ArrayList<Order> getSenderData(ArrayList<Order> temp){

        ArrayList<Order> Sender=new ArrayList<Order>();
        Log.d("enter","ok");
        for(int i=0;i<temp.size();i++){
            Log.d("name"+i+" ",temp.get(i).getSend_name());
            if(name==temp.get(i).getSend_name()){
                Sender.add(temp.get(i));
                Log.d("tag :",String.valueOf(i));
            }
        }
        return Sender;
    }
    private ArrayList<Order> getReceiverData(ArrayList<Order> temp) {

        ArrayList<Order> Receiver=new ArrayList<Order>();
        for(int i=0;i<temp.size();i++){
            if(name==temp.get(i).getRecv_name()){
                Receiver.add(temp.get(i));
                Log.d("tag :",String.valueOf(i));
            }
        }
        return Receiver;
    }
    @Override
    public int getCount() {
        return TAB_NUM;
    }
}
