package com.example.huyuxuan.lora2.Fragment;

/**
 * Created by huyuxuan on 2017/5/10.
 */

import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.support.v4.app.DialogFragment;
import android.support.v7.app.AlertDialog;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ListView;

import com.example.huyuxuan.lora2.MyBoundedService;
import com.example.huyuxuan.lora2.R;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.HashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;

import static android.content.Context.MODE_PRIVATE;

public class BasicDialogFragment extends DialogFragment {
    View v;
    ListView lv;
    private String[] list={"09:00","09:30","10:00","10:30","11:00","11:30","12:00","12:30","13:00","13:30"
            ,"14:00","14:30","15:00","15:30","16:00","16:30","17:00","17:30"};
    String tmp;
    private String dialogMessage;
    String formattedDate;
    static java.text.SimpleDateFormat dayDateFormat = new java.text.SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault());

    Calendar c;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        v = inflater.inflate(R.layout.fragment_available_time, container, false);
        lv = (ListView) v.findViewById(R.id.available_listview);
        tmp = getResources().getString(R.string.message);
        c = Calendar.getInstance();
        formattedDate =dayDateFormat.format(c.getTime());
        Log.d("BasicDialogFragment","onCreateView");
        MyBoundedService.fragmentID = 7;
        MyBoundedService.curFragment=this;


        Bundle bundle = getArguments();
        if(bundle != null) {
            dialogMessage = bundle.getString("message");
        }

        List<Map<String, String>> items = new ArrayList<Map<String,String>>();
        for(int i=0;i<dialogMessage.length();i++){
            Map<String, String> item = new HashMap<String, String>();
            item.put("time",list[i]);

            if(dialogMessage.charAt(i)=='1' &&  checkTime(list[i])){
                item.put("text","可以預約");
            }else{
                item.put("text","無法預約");
            }
            items.add(item);
        }


        MyTimeListAdapter myTimeListAdapter = new MyTimeListAdapter(getContext(),items);
        lv.setAdapter(myTimeListAdapter);



        lv.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, final int position, long id) {
                if(dialogMessage.charAt(position) == '1' && checkTime(list[position])){
                    new AlertDialog.Builder(getContext())
                            .setTitle("選擇時段")
                            .setMessage("確定要預約"+list[position]+"嗎？")
                            .setPositiveButton("是", new DialogInterface.OnClickListener() {
                                @Override
                                public void onClick(DialogInterface dialog, int which) {
                                    Intent tmp = new Intent();
                                    tmp.putExtra("time",list[position]);
                                    getActivity().onBackPressed();
                                    getTargetFragment().onActivityResult(getTargetRequestCode(),1,tmp);

                                }
                            })
                            .setNegativeButton("取消", new DialogInterface.OnClickListener() {
                                @Override
                                public void onClick(DialogInterface dialog, int which) {
                                }
                            })
                            .show();
                }
            }
        });
        return v;
    }


    static BasicDialogFragment newInstance(String dialogMessage){
        BasicDialogFragment fragment = new BasicDialogFragment();
        Bundle args = new Bundle();
        args.putString("message", dialogMessage);
        fragment.setArguments(args);
        return fragment;
    }

    public BasicDialogFragment(){
        Log.d("BasicDialogFragment","constructor");
    }


    public boolean checkTime(String time){
        int hour = Integer.parseInt(formattedDate.substring(formattedDate.indexOf(' ')+1,formattedDate.indexOf(':')));
        int minute =  Integer.parseInt(formattedDate.substring(formattedDate.indexOf(':')+1,formattedDate.lastIndexOf(':')));

        int time_hour = Integer.parseInt(time.substring(0,time.indexOf(':')));
        int time_minute = Integer.parseInt(time.substring(time.indexOf(':')+1));
        if(hour > time_hour || ((hour == time_hour) && (minute > time_minute))){
            //time這個時段不能登記
            return false;
        }
        else{
            return true;
        }

    }
}