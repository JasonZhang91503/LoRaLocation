package com.example.huyuxuan.lora2.Fragment;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.example.huyuxuan.lora2.Order;
import com.example.huyuxuan.lora2.R;

import java.util.ArrayList;

@SuppressLint("ValidFragment")
public class HistCompleteFragment extends Fragment {
    ArrayList<Order> dataset;
    public HistCompleteFragment(ArrayList<Order> completeDataset) {
        // Required empty public constructor
        dataset=completeDataset;
        if(dataset!=null){
            Log.d("HistComple",String.valueOf(dataset.size()));
        }
        else{
            Log.d("HistComple","dataset = null");
        }
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View view=inflater.inflate(R.layout.fragment_hist_complete, container, false);
        // Inflate the layout for this fragment


        RecyclerView recyclerView;


        MyAdapter myAdapter = new MyAdapter(dataset);
        LinearLayoutManager layoutManager = new LinearLayoutManager(getActivity());
        layoutManager.setOrientation(LinearLayoutManager.VERTICAL);

        recyclerView = (RecyclerView) view.findViewById(R.id.complete_recycle);
        //recyclerView.addItemDecoration(new DividerItemDecoration(this,DividerItemDecoration.VERTICAL));
        recyclerView.setLayoutManager(layoutManager);
        recyclerView.setAdapter(myAdapter);

        return view;
    }

}
