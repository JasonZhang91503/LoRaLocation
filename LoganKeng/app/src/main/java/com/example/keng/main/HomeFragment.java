package com.example.keng.main;


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
import android.widget.Toast;

import java.util.ArrayList;


/**
 * A simple {@link Fragment} subclass.
 * Use the {@link HomeFragment#newInstance} factory method to
 * create an instance of this fragment.
 */
public class HomeFragment extends Fragment implements View.OnClickListener{
    // TODO: Rename parameter arguments, choose names that match
    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    private static final String ARG_PARAM1 = "param1";
    private static final String ARG_PARAM2 = "param2";

    // TODO: Rename and change types of parameters
    private String mParam1;
    private String mParam2;


    public HomeFragment() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param param1 Parameter 1.
     * @param param2 Parameter 2.
     * @return A new instance of fragment HomeFragment.
     */
    // TODO: Rename and change types and number of parameters
    public static HomeFragment newInstance(String param1, String param2) {
        HomeFragment fragment = new HomeFragment();
        Bundle args = new Bundle();
        args.putString(ARG_PARAM1, param1);
        args.putString(ARG_PARAM2, param2);
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (getArguments() != null) {
            mParam1 = getArguments().getString(ARG_PARAM1);
            mParam2 = getArguments().getString(ARG_PARAM2);
        }
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View view=inflater.inflate(R.layout.fragment_home, container, false);
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
        RecyclerView recyclerView;

        ArrayList<Order> dataset = new ArrayList<Order>();
        String[] resource=getActivity().getResources().getStringArray(R.array.Location);

        Order[] order=new Order[4];

        order[0]=new Order(3,4,50,"耿楷寗","0933232456","2017年5月30日17點05分","",1);
        order[1]=new Order(0,2,30,"張紘綸","0921357849","2017年4月28日11點25分","記得要做考古題",2);
        order[2]=new Order(1,5,40,"張主任","0932654378","2017年4月20日14點30分","內有公文記得簽收",3);
        order[3]=new Order(3,4,20,"電資院","0988960459","2017年4月17日09點25分","最新講座資訊敬請公告",4);

        for(int i=0;i<order.length;i++){
            dataset.add(order[i]);
        }

        MyAdapter myAdapter=new MyAdapter(dataset,resource);
        LinearLayoutManager layoutManager=new LinearLayoutManager(getActivity());
        layoutManager.setOrientation(LinearLayoutManager.VERTICAL);

        recyclerView=(RecyclerView)view.findViewById(R.id.recyclerView);
        //recyclerView.addItemDecoration(new DividerItemDecoration(this,DividerItemDecoration.VERTICAL));
        recyclerView.setLayoutManager(layoutManager);
        recyclerView.setAdapter(myAdapter);
        return view;
    }


    @Override
    public void onClick(View v) {

    }
}
