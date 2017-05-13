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
import android.widget.EditText;
import android.widget.Toast;


/**
 * A simple {@link Fragment} subclass.
 * Use the {@link NewOrderFragment2#newInstance} factory method to
 * create an instance of this fragment.
 */
public class NewOrderFragment2 extends Fragment implements View.OnClickListener{

    private final String TAG=this.getClass().getSimpleName();
    // TODO: Rename parameter arguments, choose names that match
    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    private static final String ARG_PARAM1 = "param1";
    private static final String ARG_PARAM2 = "param2";

    // TODO: Rename and change types of parameters
    private int[] mParam1;
    private String mParam2;
    EditText name,note;
    /*---------------------------------
     * 欄位 0 時間
     * 欄位 1 姓名
     * 欄位 2 備註
     */


    String[] packet=new String[3];

    public NewOrderFragment2() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param param1 Parameter 1.
     * @param param2 Parameter 2.
     * @return A new instance of fragment NewOrderFragment2.
     */
    // TODO: Rename and change types and number of parameters
    public static NewOrderFragment2 newInstance(int[] param1, String param2) {
        NewOrderFragment2 fragment = new NewOrderFragment2();
        Bundle args = new Bundle();
        args.putIntArray(ARG_PARAM1, param1);
        args.putString(ARG_PARAM2, param2);
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (getArguments() != null) {
            mParam1 = getArguments().getIntArray(ARG_PARAM1);
            mParam2 = getArguments().getString(ARG_PARAM2);
        }
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        Log.d(TAG, String.valueOf(mParam1[0])+" "+String.valueOf(mParam1[1]));
        Log.d(TAG,mParam2);
        // Inflate the layout for this fragment
        View view=inflater.inflate(R.layout.fragment_new_order_fragment2, container, false);

        name=(EditText)view.findViewById(R.id.Name);
        note=(EditText)view.findViewById(R.id.Note);
        Button btn_next=(Button)view.findViewById(R.id.btn_next);
        btn_next.setOnClickListener(this);
        return view;
    }
    public void setPacket(){
        packet[0]=mParam2;
        packet[1]=String.valueOf(name.getText());
        packet[2]=String.valueOf(note.getText());
    }
    @Override
    public void onClick(View v) {
        //前半部再做防呆
        if(name.getText().length()==0){
            Toast.makeText(getContext(),"請填寫姓名",Toast.LENGTH_SHORT).show();
        }else {
            setPacket();
            NewOrderFragment3 fragment3 = NewOrderFragment3.newInstance(mParam1, packet);
            FragmentManager fm = getActivity().getSupportFragmentManager();
            FragmentTransaction trans = fm.beginTransaction();
            trans.addToBackStack(null);
            trans.replace(R.id.frame, fragment3, fragment3.getTag());
            trans.commit();
        }
    }
}
