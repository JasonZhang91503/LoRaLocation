package com.example.huyuxuan.lora2.Fragment;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.example.huyuxuan.lora2.Order;
import com.example.huyuxuan.lora2.R;

import java.util.ArrayList;


public class HistAllFragment extends Fragment {
    ArrayList<Order> dataset ;
    public HistAllFragment(ArrayList<Order> allData) {
        // Required empty public constructor
        dataset=allData;
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment

        View view=inflater.inflate(R.layout.fragment_hist_all, container, false);
        RecyclerView recyclerView;


        MyAdapter myAdapter = new MyAdapter(dataset);
        LinearLayoutManager layoutManager = new LinearLayoutManager(getActivity());
        layoutManager.setOrientation(LinearLayoutManager.VERTICAL);

        recyclerView = (RecyclerView) view.findViewById(R.id.all_recycle);
        //recyclerView.addItemDecoration(new DividerItemDecoration(this,DividerItemDecoration.VERTICAL));
        recyclerView.setLayoutManager(layoutManager);
        recyclerView.setAdapter(myAdapter);

        return view;
    }
}
