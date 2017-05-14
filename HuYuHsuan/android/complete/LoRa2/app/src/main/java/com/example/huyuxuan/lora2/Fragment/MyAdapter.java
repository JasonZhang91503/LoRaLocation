package com.example.huyuxuan.lora2.Fragment;

import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import com.example.huyuxuan.lora2.Order;
import com.example.huyuxuan.lora2.R;

import java.util.List;

/**
 * Created by huyuxuan on 2017/5/9.
 */

public class MyAdapter extends RecyclerView.Adapter<MyAdapter.ViewHolder> implements View.OnClickListener {
    //利用建構子將資料清單傳進來
    private List<Order> mDataset;
    private OnItemClickListener mOnItemClickListener = null;

    public  MyAdapter(List<Order> data){
        mDataset=data;
    }
    //指定item所使用的view佈局
    @Override
    public MyAdapter.ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View view= LayoutInflater.from(parent.getContext()).inflate(R.layout.cardview,parent,false);
        ViewHolder viewHolder=new ViewHolder(view);
        view.setOnClickListener(this);
        return viewHolder;
    }
    //透過position指定每個item所用到的資料
    @Override
    public void onBindViewHolder(ViewHolder holder, int position) {


        holder.Start.setText(mDataset.get(position).getStr_start());
        holder.Destination.setText(mDataset.get(position).getStr_dest());
        holder.Time.setText(mDataset.get(position).getSend_time());
        holder.Cost.setText("NT$30");
        holder.itemView.setTag(position);
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
        //holder.Note.setText(mDataset.get(position).getNote());
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
            //Note=(TextView)itemView.findViewById(R.id.note);

        }
    }

    public static interface OnItemClickListener {
        void onItemClick(View view , int position);
    }


    public void onClick(View v) {
        if (mOnItemClickListener != null) {
            //注意这里使用getTag方法获取position
            mOnItemClickListener.onItemClick(v,(int)v.getTag());
        }
    }
    public void setOnItemClickListener(OnItemClickListener listener) {
        this.mOnItemClickListener = listener;
    }
}
