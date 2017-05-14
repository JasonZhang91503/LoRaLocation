package com.example.keng.main;

import android.app.Activity;
import android.content.res.Resources;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import java.util.List;

/**
 * Created by keng on 5/7/17.
 */

//參考網址http://xnfood.com.tw/android-recyclerview-list/
//建立一個Adapter 控管每個item的顯示設定，自訂一個Adapter類別
public class  MyAdapter extends RecyclerView.Adapter<MyAdapter.ViewHolder>{

    //利用建構子將資料清單傳進來
    private List<Order> mDataset;
    private String[] location;

    public  MyAdapter(List<Order> data,String[] resource){
        mDataset=data;
        location=resource;
    }
    //指定item所使用的view佈局
    @Override
    public MyAdapter.ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View view= LayoutInflater.from(parent.getContext()).inflate(R.layout.cardview,parent,false);
        ViewHolder viewHolder=new ViewHolder(view);
        return viewHolder;
    }
    //透過position指定每個item所用到的資料
    @Override
    public void onBindViewHolder(ViewHolder holder, int position) {

        String time,month,date;
        time=mDataset.get(position).getSend_time();
        month=mDataset.get(position).getMonth();
        date=mDataset.get(position).getDate();
        holder.Date.setText(String.valueOf(date));
        holder.Start.setText(location[(mDataset.get(position).getStartPlace())]);
        holder.Destination.setText(location[(mDataset.get(position).getFinal_place())]);
        holder.Cost.setText("NT$"+String.valueOf(mDataset.get(position).getOrder_cost()));
        holder.Time.setText(time);
        switch (mDataset.get(position).getOrder_state()) {
            case 1:
                holder.status.setImageResource(R.color.orderBar_unprocessed);
                break;
            case 2:
                holder.status.setImageResource(R.color.orderBar_waiting);
                break;
            case 3:
                holder.status.setImageResource(R.color.orderBar_waiting);
                break;
            case 4:
                holder.status.setImageResource(R.color.orderBar_complete);
                break;

        }

        switch (Integer.parseInt(month)){
            case 1:
                holder.Month.setText(R.string.Jan);
                break;
            case 2:
                holder.Month.setText(R.string.Feb);
                break;
            case 3:
                holder.Month.setText(R.string.Mar);
                break;
            case 4:
                holder.Month.setText(R.string.Apr);
                break;
            case 5:
                holder.Month.setText(R.string.May);
                break;
            case 6:
                holder.Month.setText(R.string.Jun);
                break;
            case 7:
                holder.Month.setText(R.string.Jul);
                break;
            case 8:
                holder.Month.setText(R.string.Aug);
                break;
            case 9:
                holder.Month.setText(R.string.Sep);
                break;
            case 10:
                holder.Date.setText(R.string.Oct);
                break;
            case 11:
                holder.Date.setText(R.string.Nov);
                break;
            case 12:
                holder.Date.setText(R.string.Dec);
                break;
        }

       // 2017-05-31 17:55:00
    }
    //傳回清單的數量
    @Override
    public int getItemCount() {
        return mDataset.size();
    }
    //透過View Holder定義介面和邏輯
    public class ViewHolder extends RecyclerView.ViewHolder{

        public TextView Start,Destination,Time,Cost,Date,Month;
        public ImageView status;
        public ViewHolder(View itemView) {
            super(itemView);
            Start=(TextView)itemView.findViewById(R.id.start);
            Destination=(TextView)itemView.findViewById(R.id.Destination);
            Time=(TextView)itemView.findViewById(R.id.time);
            Cost=(TextView)itemView.findViewById(R.id.cost);
            status=(ImageView)itemView.findViewById(R.id.status);
            Date=(TextView)itemView.findViewById(R.id.date);
            Month=(TextView)itemView.findViewById(R.id.month);
           // Note=(TextView)itemView.findViewById(R.id.note);

        }
    }
}
