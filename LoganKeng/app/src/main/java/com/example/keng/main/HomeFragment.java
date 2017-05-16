package com.example.keng.main;


import android.app.Activity;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentTransaction;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;

import java.util.ArrayList;


public class HomeFragment extends Fragment implements View.OnClickListener{

    HFinterface hfinterface;
    public interface HFinterface{
        ArrayList<Order>getDataset();
    }
    @Override
    public void onAttach(Activity activity) {
        super.onAttach(activity);
        try {
            hfinterface= (HomeFragment.HFinterface) activity;
        } catch (ClassCastException e) {
            throw new ClassCastException(activity.toString() + " must implement OnItemClickedListener");
        }
    }
    // TODO: Rename parameter arguments, choose names that match
    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    private static final String ARG_PARAM1 = "param1";
    private static final String ARG_PARAM2 = "param2";

    // TODO: Rename and change types of parameters
    private String mParam1;
    private String mParam2;

    public ArrayList<Order> dataset;
    String[] resource;
    View view;
    public HomeFragment(ArrayList<Order> orderData) {
        // Required empty public constructor
        dataset=new ArrayList<Order>();
        dataset=orderData;
    }
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        view=inflater.inflate(R.layout.fragment_home, container, false);
        setView();
        return view;
    }
    public void setView(){
        PrefManager pref=new PrefManager(getContext());
        TextView txtName=(TextView) view.findViewById(R.id.txtName);
        String name=pref.getName();
        txtName.setText(name);
        ImageButton addEvent=(ImageButton)view.findViewById(R.id.addEvent);
        addEvent.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                NewOrderFragment nf=new NewOrderFragment();
                FragmentManager fm=getActivity().getSupportFragmentManager();
                FragmentTransaction trans=fm.beginTransaction();
                trans.addToBackStack("null");
                trans.replace(R.id.frame,nf,nf.getTag());
                trans.commit();
            }
        });

        //設定RecycleView的金額
        final RecyclerView recyclerView;
        resource=getResources().getStringArray(R.array.Location);
        MyAdapter myAdapter=new MyAdapter(dataset,resource);
        LinearLayoutManager layoutManager=new LinearLayoutManager(getActivity());
        layoutManager.setOrientation(LinearLayoutManager.VERTICAL);

        recyclerView=(RecyclerView)view.findViewById(R.id.recycleView);
        //recyclerView.addItemDecoration(new DividerItemDecoration(this,DividerItemDecoration.VERTICAL));
        recyclerView.setLayoutManager(layoutManager);
        recyclerView.setAdapter(myAdapter);
    }
    @Override
    public void onResume(){
        super.onResume();
        Log.d("On resume","ok");
        dataset=hfinterface.getDataset();
        setView();

    }
    @Override
    public void onClick(View v) {

    }
}
