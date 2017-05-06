package com.example.keng.main;


import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentTransaction;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;


/**
 * A simple {@link Fragment} subclass.
 * Use the {@link NewOrderFragment3#newInstance} factory method to
 * create an instance of this fragment.
 */
public class NewOrderFragment3 extends Fragment implements View.OnClickListener{

    private final String TAG=getClass().getSimpleName();
    // TODO: Rename parameter arguments, choose names that match
    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    private static final String ARG_PARAM1 = "param1";
    private static final String ARG_PARAM2 = "param2";

    // TODO: Rename and change types of parameters
    private int[] mParam1;
    private String[] mParam2;

    String[] Location;
    TextView start,destination,name,phone,note,time;
    public NewOrderFragment3() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param param1 Parameter 1.
     * @param param2 Parameter 2.
     * @return A new instance of fragment NewOrderFragment3.
     */
    // TODO: Rename and change types and number of parameters
    public static NewOrderFragment3 newInstance(int[] param1, String[] param2) {
        NewOrderFragment3 fragment = new NewOrderFragment3();
        Bundle args = new Bundle();
        args.putIntArray(ARG_PARAM1, param1);
        args.putStringArray(ARG_PARAM2, param2);
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (getArguments() != null) {
            mParam1 = getArguments().getIntArray(ARG_PARAM1);
            mParam2 = getArguments().getStringArray(ARG_PARAM2);
        }
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View view=inflater.inflate(R.layout.fragment_new_order_fragment3, container, false);
        for(int i=0;i<mParam2.length;i++){
            Log.d(TAG,mParam2[i]);
        }
        Log.d(TAG,String.valueOf(mParam1[0]));
        Log.d(TAG,String.valueOf(mParam1[1]));

        Location=getResources().getStringArray(R.array.Location);
        time=(TextView)view.findViewById(R.id.txtTime);
        start=(TextView)view.findViewById(R.id.Start);
        destination=(TextView)view.findViewById(R.id.Destination);
        name=(TextView)view.findViewById(R.id.txtName);
        phone=(TextView)view.findViewById(R.id.Phone);
        note=(TextView)view.findViewById(R.id.txtNote);

        setText();
        //Button btn_next=(Button)view.findViewById(R.id.btn_next);
        //btn_next.setOnClickListener(this);
        return view;
    }
    public void setText(){
        time.setText(mParam2[0]);
        name.setText(mParam2[1]);
        phone.setText(mParam2[2]);
        if(mParam2[3]!=null){
            note.setText(mParam2[3]);
        }else{
            note.setText("");
        }
        start.setText(Location[mParam1[0]]);
        destination.setText(Location[mParam1[1]]);
    }
    @Override
    public void onClick(View v) {
        //回到MainFragment
    }
}
