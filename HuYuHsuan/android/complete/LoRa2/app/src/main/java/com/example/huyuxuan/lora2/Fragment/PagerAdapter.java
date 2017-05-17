package com.example.huyuxuan.lora2.Fragment;

import android.content.SharedPreferences;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import android.support.v4.app.FragmentTransaction;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;

import com.example.huyuxuan.lora2.Order;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by keng on 5/9/17.
 */

public class PagerAdapter extends FragmentPagerAdapter {

    final static int TAB_NUM=3;
    boolean is_State4=true;
    ArrayList<Order> Dataset;
    ArrayList<Order> Complete_Dataset;
    ArrayList<Order> Incomplete_Dataset;
    private FragmentManager mFragmentManager;
    HistAllFragment hist_all;
    HistCompleteFragment hist_complete;
    HistIncompleteFragment hist_incomplete;


    public PagerAdapter(FragmentManager fm, ArrayList<Order> orderData, int type) {
        super(fm);

        Dataset=new ArrayList<Order>();
        this.mFragmentManager=fm;
        Log.d("PagerAdapter","constructor");
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
                hist_all=new HistAllFragment(Dataset);
                Log.d("PagerAdapter","getitem hist_all");
                return hist_all;
            case 1:
                Complete_Dataset=getSpecifieyStateData(is_State4);
                hist_complete=new HistCompleteFragment(Complete_Dataset);
                Log.d("PagerAdapter","getitem hist_compl");
                return hist_complete;
            case 2:
                Incomplete_Dataset=getSpecifieyStateData(!is_State4);
                hist_incomplete=new HistIncompleteFragment(Incomplete_Dataset);
                Log.d("PagerAdapter","getitem hist_incomple");
                return hist_incomplete;
            default:
                return null;
        }
    }
    public ArrayList<Order> getSpecifieyStateData(boolean is_State4){
        Complete_Dataset=new ArrayList<Order>();
        Incomplete_Dataset=new ArrayList<Order>();
        if(Dataset != null){
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
        else{
            return null;
        }
    }
    public ArrayList<Order> getSenderData(ArrayList<Order> temp){

        ArrayList<Order> Sender=new ArrayList<Order>();
        Log.d("enter","ok");
        if(temp!=null){
            for(int i=0;i<temp.size();i++){
                Log.d("requireTime"+i,temp.get(i).getSend_time());
                Log.d("name"+i+" ",temp.get(i).getStr_sender());

                Sender.add(temp.get(i));
                Log.d("tag :",String.valueOf(i));

            }
        }
        return Sender;
    }
    private ArrayList<Order> getReceiverData(ArrayList<Order> temp) {

        ArrayList<Order> Receiver=new ArrayList<Order>();
        if(temp!=null){
            for(int i=0;i<temp.size();i++){
                Log.d("requireTime"+i,temp.get(i).getSend_time());
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



    public void update(ArrayList<Order> data){
        Dataset = data;
        setFragments();
    }


    @Override
    public int getItemPosition(Object object) {
        return POSITION_NONE;
    }

    public void setFragments(){
        FragmentTransaction fragmentTransaction = mFragmentManager.beginTransaction();
        fragmentTransaction.remove(hist_all).remove(hist_complete).remove(hist_incomplete).commit();
        mFragmentManager.executePendingTransactions();
        notifyDataSetChanged();
    }
}
