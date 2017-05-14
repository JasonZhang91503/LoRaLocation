package com.example.huyuxuan.lora2.Fragment;

import android.content.Context;
import android.graphics.Color;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ListView;
import android.widget.TextView;

import com.example.huyuxuan.lora2.R;

import java.util.List;
import java.util.Map;

/**
 * Created by huyuxuan on 2017/5/11.
 */

public class MyTimeListAdapter extends BaseAdapter {
    private LayoutInflater mLayInf;
    private List<Map<String, String>> mList;


    public  MyTimeListAdapter(Context context, List<Map<String, String>> datalist){
        mLayInf = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        mList = datalist;
    }
    @Override
    public int getCount() {
        return mList.size();
    }

    @Override
    public Object getItem(int position) {
        return mList.get(position);
    }

    @Override
    public long getItemId(int position) {
        return position;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        View v = mLayInf.inflate(R.layout.available_time_list_item, parent, false);
        TextView tvTime = (TextView) v.findViewById(R.id.LI_time);
        TextView tvChoose = (TextView)v.findViewById(R.id.LI_Choose);
        tvTime.setText(mList.get(position).get("time"));
        tvChoose.setText(mList.get(position).get("text"));
        if(mList.get(position).get("text").equals("無法預約")){
            tvChoose.setTextColor(Color.WHITE);
            tvChoose.setBackgroundResource(R.color.busy);
            tvChoose.setClickable(false);
            tvTime.setClickable(false);
        }
        return v;
    }

}
