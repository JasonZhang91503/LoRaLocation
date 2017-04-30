package com.example.huyuxuan.lora2.Fragment;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.example.huyuxuan.lora2.R;


/**
 * Created by huyuxuan on 2017/4/30.
 */

public class SettingFragment extends Fragment {
    private View myview;

    @Override
    public void onCreate(Bundle savedInstances){
        super.onCreate(savedInstances);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState){
        myview = inflater.inflate(R.layout.fragment_setting,container,false);
        return myview;
    }
}
