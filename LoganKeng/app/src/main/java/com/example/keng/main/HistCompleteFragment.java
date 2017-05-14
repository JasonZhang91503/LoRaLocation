package com.example.keng.main;


import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import java.util.ArrayList;


/**
 * A simple {@link Fragment} subclass.
 */
public class HistCompleteFragment extends Fragment {

    ArrayList<Order> dataset;
    public HistCompleteFragment(ArrayList<Order> completeDataset) {
        // Required empty public constructor
        dataset=completeDataset;
        Log.d("new Fragment",String.valueOf(dataset.size()));
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View view=inflater.inflate(R.layout.fragment_hist_complete, container, false);
        // Inflate the layout for this fragment
        String[] resource=getActivity().getResources().getStringArray(R.array.Location);

        RecyclerView recyclerView;


        MyAdapter myAdapter = new MyAdapter(dataset, resource);
        LinearLayoutManager layoutManager = new LinearLayoutManager(getActivity());
        layoutManager.setOrientation(LinearLayoutManager.VERTICAL);

        recyclerView = (RecyclerView) view.findViewById(R.id.complete_recycle);
        //recyclerView.addItemDecoration(new DividerItemDecoration(this,DividerItemDecoration.VERTICAL));
        recyclerView.setLayoutManager(layoutManager);
        recyclerView.setAdapter(myAdapter);

        return view;
    }

}
