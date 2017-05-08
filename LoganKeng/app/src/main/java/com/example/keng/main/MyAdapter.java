package com.example.keng.main;

import android.app.Activity;
import android.content.res.Resources;
import android.support.v7.widget.RecyclerView;
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


        holder.Start.setText(location[(mDataset.get(position).getStartPlace())]);
        holder.Destination.setText(location[(mDataset.get(position).getFinal_place())]);
        holder.Time.setText(mDataset.get(position).getSend_time());
        holder.Cost.setText("NT$"+String.valueOf(mDataset.get(position).getOrder_cost()));
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
       // holder.Note.setText(mDataset.get(position).getNote());
    }
    //傳回清單的數量
    @Override
    public int getItemCount() {
        return mDataset.size();
    }
    //透過View Holder定義介面和邏輯
    public class ViewHolder extends RecyclerView.ViewHolder{

        public TextView Start,Destination,Time,Cost;
        public ImageView status;
        public ViewHolder(View itemView) {
            super(itemView);
            Start=(TextView)itemView.findViewById(R.id.start);
            Destination=(TextView)itemView.findViewById(R.id.Destination);
            Time=(TextView)itemView.findViewById(R.id.time);
            Cost=(TextView)itemView.findViewById(R.id.cost);
            status=(ImageView)itemView.findViewById(R.id.status);
           // Note=(TextView)itemView.findViewById(R.id.note);

        }
    }
}
