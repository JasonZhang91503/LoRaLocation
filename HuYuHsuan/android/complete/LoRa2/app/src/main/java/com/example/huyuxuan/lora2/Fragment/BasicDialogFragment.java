package com.example.huyuxuan.lora2.Fragment;

/**
 * Created by huyuxuan on 2017/5/10.
 */

import android.app.Dialog;
import android.app.FragmentManager;
import android.content.DialogInterface;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.v4.app.DialogFragment;
import android.support.v7.app.AlertDialog;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ListView;
import android.widget.SimpleAdapter;
import android.widget.TextView;

import com.example.huyuxuan.lora2.R;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
public class BasicDialogFragment extends DialogFragment {
    View v;
    ListView lv;
    private String[] list={"09:00","09:30","10:00","10:30","11:00","11:30","12:00","12:30","13:00","13:30"
            ,"14:00","14:30","15:00","15:30","16:00","16:30","17:00","17:30"};
    String tmp;
    private String dialogMessage;

    private PassOnTimeChooseListener mHost;

    public interface PassOnTimeChooseListener{
        void passTime(String str_time);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        v = inflater.inflate(R.layout.fragment_available_time, container, false);
        lv = (ListView) v.findViewById(R.id.available_listview);
        tmp = getResources().getString(R.string.message);
        Log.d("BasicDialogFragment","onCreateView");
        try {
            mHost = (PassOnTimeChooseListener) getTargetFragment();
        }catch (ClassCastException e){
            e.printStackTrace();
        }


        Bundle bundle = getArguments();
        if(bundle != null) {
            dialogMessage = bundle.getString("message");
        }

        List<Map<String, String>> items = new ArrayList<Map<String,String>>();
        for(int i=0;i<dialogMessage.length();i++){
            Map<String, String> item = new HashMap<String, String>();
            item.put("time",list[i]);
            if(dialogMessage.charAt(i)=='1'){
                item.put("text","可以預約");
            }else{
                item.put("text","無法預約");
            }
            items.add(item);
        }
        SimpleAdapter adapter = new SimpleAdapter(getActivity().getApplicationContext(),items,
                R.layout.available_time_list_item,new String[]{"time","text"}
                ,new int[]{R.id.LI_time,R.id.LI_Choose});
        lv.setAdapter(adapter);
        for(int i=0;i<dialogMessage.length();i++){
            TextView tmpView;
            tmpView = (TextView)getViewByPosition(i,lv);
            if(dialogMessage.charAt(i)=='0'){
                //不能預約，要把該textView的clickable設為false
                tmpView.setClickable(false);
            }
        }
        lv.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, final int position, long id) {
                new AlertDialog.Builder(getContext())
                        .setTitle("選擇時段")
                        .setMessage("確定要預約"+list[position]+"嗎？")
                        .setPositiveButton("是", new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int which) {
                                mHost.passTime(list[position]);
                                dismiss();
                            }
                        })
                        .setNegativeButton("取消", new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int which) {
                                dismiss();
                            }
                        })
                        .show();
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


    public View getViewByPosition(int pos, ListView listView) {
        final int firstListItemPosition = listView.getFirstVisiblePosition();
        final int lastListItemPosition = firstListItemPosition + listView.getChildCount() - 1;

        if (pos < firstListItemPosition || pos > lastListItemPosition ) {
            return listView.getAdapter().getView(pos, null, listView);
        } else {
            final int childIndex = pos - firstListItemPosition;
            return listView.getChildAt(childIndex);
        }
    }


}