package com.example.keng.main;


import android.app.Activity;
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
import android.widget.Toast;


/**
 * A simple {@link Fragment} subclass.
 * Use the {@link NewOrderFragment3#newInstance} factory method to
 * create an instance of this fragment.
 */
public class NewOrderFragment3 extends Fragment implements View.OnClickListener{

    ListComplete listComplete;

    //建立interface 將重新設定index回主畫面
    public interface ListComplete{
        void setIndex(int i);
        void setNew_order(Order new_order);
    }
    //回傳參數的方法
    @Override
    public void onAttach(Activity activity) {
        super.onAttach(activity);
        try {
            listComplete = (ListComplete) activity;
        } catch (ClassCastException e) {
            throw new ClassCastException(activity.toString() + " must implement OnItemClickedListener");
        }
    }
    private final String TAG=getClass().getSimpleName();
    // TODO: Rename parameter arguments, choose names that match
    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    private static final String ARG_PARAM1 = "param1";
    private static final String ARG_PARAM2 = "param2";

    // TODO: Rename and change types of parameters
    private int[] mParam1;
    private String[] mParam2;

    String[] Location;
    TextView start,destination,name,note,time;
    PrefManager prefManager;
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
        prefManager=new PrefManager(getContext());
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
        note=(TextView)view.findViewById(R.id.txtNote);

        setText();
        Button btn_confirm=(Button)view.findViewById(R.id.btn_confirm);
        btn_confirm.setOnClickListener(this);
        return view;
    }
    public void setText(){
        //Param2 時間 名字 備註
        time.setText(mParam2[0]);
        name.setText(mParam2[1]);
        if(mParam2[2]!=null){
            note.setText(mParam2[2]);
        }else{
            note.setText("");
        }
        start.setText(Location[mParam1[0]]);
        destination.setText(Location[mParam1[1]]);
    }
    @Override
    public void onClick(View v) {
        //起始地 終點 花費 寄送者 接收者 時間 備註 密碼 狀態
        String sendName,recvName,send_time,send_note;
        sendName=prefManager.getName();
        recvName=String.valueOf(name.getText());
        send_time=String.valueOf(time.getText());
        send_note=String.valueOf(note.getText());
        Order order=new Order(mParam1[0],mParam1[1],30,sendName,recvName,send_time,send_note,"1234",0);
        FragmentManager fm=getActivity().getSupportFragmentManager();
        FragmentTransaction trans=fm.beginTransaction();
        //0509更新內容//
        trans.detach(this);
        fm.popBackStack(null,FragmentManager.POP_BACK_STACK_INCLUSIVE);
        Toast.makeText(getActivity(),"完成訂單登記",Toast.LENGTH_SHORT).show();
        listComplete.setIndex(0);
        listComplete.setNew_order(order);
        trans.commit();
        //回到MainFragment
    }
}
