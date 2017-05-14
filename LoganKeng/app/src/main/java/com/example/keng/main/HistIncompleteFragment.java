package com.example.keng.main;


import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import java.util.ArrayList;


/**
 * A simple {@link Fragment} subclass.
 */
public class HistIncompleteFragment extends Fragment {

    ArrayList<Order> dataset;
    public HistIncompleteFragment(ArrayList<Order> incompleteDataset) {
        // Required empty public constructor
        dataset=incompleteDataset;
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View view=inflater.inflate(R.layout.fragment_hist_incomplete, container, false);
        RecyclerView recyclerView=(RecyclerView)view.findViewById(R.id.incomplete_recycle);
        String[] resource=getResources().getStringArray(R.array.Location);
        MyAdapter myAdapter=new MyAdapter(dataset,resource);
        LinearLayoutManager layoutManager=new LinearLayoutManager(getActivity());
        layoutManager.setOrientation(LinearLayoutManager.VERTICAL);
        recyclerView.setLayoutManager(layoutManager);
        recyclerView.setAdapter(myAdapter);
        return view;
    }

}
