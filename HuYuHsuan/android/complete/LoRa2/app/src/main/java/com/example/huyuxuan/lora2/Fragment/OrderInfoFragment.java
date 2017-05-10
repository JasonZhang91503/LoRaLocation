package com.example.huyuxuan.lora2.Fragment;

import android.app.FragmentManager;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;

import com.example.huyuxuan.lora2.Order;
import com.example.huyuxuan.lora2.R;

/**
 * Created by huyuxuan on 2017/5/10.
 */

public class OrderInfoFragment extends Fragment {

    private View myview;
    private TextView tvRequireTime;
    private TextView tvArriveTime;
    private TextView tvSender;
    private TextView tvReceiver;
    private TextView tvStart;
    private TextView tvDest;
    private TextView tvKey;
    private TextView tvState;
    private TextView tvNote;
    private Button btnOk;
    private Order curOrder;

    private static final String ARG_PARAM = "param";

    public OrderInfoFragment() {
        // Required empty public constructor
    }
    // TODO: Rename and change types and number of parameters
    public static OrderInfoFragment newInstance(Order param) {
        OrderInfoFragment fragment = new OrderInfoFragment();
        Bundle args = new Bundle();
        args.putSerializable(ARG_PARAM,param);
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.d("OrderInfoFragment","onCreate");
        if(getArguments()!=null){
            curOrder=(Order)getArguments().getSerializable(ARG_PARAM);
        }

    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        super.onCreateView(inflater, container, savedInstanceState);
        myview = inflater.inflate(R.layout.order_information,container,false);
        tvRequireTime = (TextView)myview.findViewById(R.id.requireTime);
        tvArriveTime = (TextView)myview.findViewById(R.id.arriveTime);
        tvSender = (TextView)myview.findViewById(R.id.sender);
        tvReceiver = (TextView)myview.findViewById(R.id.receiver);
        tvStart = (TextView)myview.findViewById(R.id.start_id);
        tvDest = (TextView)myview.findViewById(R.id.des_id);
        tvKey=(TextView)myview.findViewById(R.id.key);
        tvState = (TextView)myview.findViewById(R.id.state);
        tvNote = (TextView)myview.findViewById(R.id.note);
        btnOk = (Button)myview.findViewById(R.id.btnOk);


        tvRequireTime.setText(curOrder.getSend_time());
        tvArriveTime.setText(curOrder.getStr_arrive_time());
        tvSender.setText(curOrder.getStr_sender());
        tvReceiver.setText(curOrder.getRecv_name());
        tvStart.setText(curOrder.getStr_start());
        tvDest.setText(curOrder.getStr_dest());
        tvKey.setText(curOrder.getStr_key());
        tvState.setText(curOrder.getStr_state());
        tvNote.setText(curOrder.getNote());
        btnOk.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Log.d("OrderInfoFragment","btn onclick");
                FragmentManager fm = getActivity().getFragmentManager();
                if (fm.getBackStackEntryCount() != 0) {
                    fm.popBackStack();
                }
            }
        });
        return myview;
    }

}
