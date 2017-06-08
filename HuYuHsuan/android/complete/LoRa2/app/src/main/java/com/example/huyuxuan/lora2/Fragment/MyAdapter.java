package com.example.huyuxuan.lora2.Fragment;

import android.app.AlertDialog;
import android.content.Context;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
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

public class MyAdapter extends RecyclerView.Adapter<MyAdapter.ViewHolder>{
    //利用建構子將資料清單傳進來
    private List<Order> mDataset;

    public  MyAdapter(List<Order> data){
        mDataset=data;
    }
    Context context;
    //指定item所使用的view佈局
    @Override
    public MyAdapter.ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View view= LayoutInflater.from(parent.getContext()).inflate(R.layout.cardview,parent,false);
        ViewHolder viewHolder=new ViewHolder(view);
        this.context=parent.getContext();
        return viewHolder;
    }
    //透過position指定每個item所用到的資料
    @Override
    public void onBindViewHolder(ViewHolder holder, int position) {

        String month = mDataset.get(position).getMonth();
        holder.Start.setText(mDataset.get(position).getStr_start());
        holder.Destination.setText(mDataset.get(position).getStr_dest());
        holder.Time.setText(mDataset.get(position).getHour()+"時"+mDataset.get(position).getMinute()+"分");
        holder.Cost.setText("NT$30");
        holder.Date.setText(mDataset.get(position).getDate());
        holder.itemView.setTag(position);
        switch (mDataset.get(position).getOrder_state()) {
            case 0:
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
    }
    //傳回清單的數量
    @Override
    public int getItemCount() {
        if(mDataset!=null){
            return mDataset.size();
        }else{
            return 0;
        }
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

            itemView.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    Log.e("jwzhangjie", "当前点击的位置："+getPosition());
                    Order temp=mDataset.get(getPosition());
                    //傳遞Order資料至Dialog，必須先擷取資料內容，再將資料一一設定
                    View mView =((AppCompatActivity)context).getLayoutInflater().inflate(R.layout.ordercard,null);
                    TextView time=(TextView)mView.findViewById(R.id.txtTime);
                    TextView sender=(TextView)mView.findViewById(R.id.txtSendName);
                    TextView receiver=(TextView)mView.findViewById(R.id.txtRecvName);
                    TextView start=(TextView)mView.findViewById(R.id.Start);
                    TextView destination=(TextView)mView.findViewById(R.id.Destination);
                    TextView pwd=(TextView)mView.findViewById(R.id.pwd);
                    TextView cost=(TextView)mView.findViewById(R.id.cost);
                    TextView status=(TextView)mView.findViewById(R.id.status);
                    TextView note=(TextView)mView.findViewById(R.id.txtNote);
                    TextView txtTotal=(TextView)mView.findViewById(R.id.txtTotal);
                    TextView txtPwd=(TextView)mView.findViewById(R.id.txtPwd);
                    String txtTime=temp.getYear()+"年"+temp.getMonth()+"月"+temp.getDate()+"日"+temp.getHour()+"時"+temp.getMinute()+"分";
                    time.setText(txtTime);
                    sender.setText(temp.getStr_sender());
                    receiver.setText(temp.getRecv_name());
                    start.setText(temp.getStr_start());
                    destination.setText(temp.getStr_dest());
                    pwd.setText(temp.getStr_key());

                    if(temp.getNote().length()!=0){
                        note.setText(temp.getNote());
                    }else{
                        note.setText("");
                    }
                    switch (temp.getOrder_state()){
                        case 0:
                            time.setBackgroundColor(context.getResources().getColor(R.color.orderBar_unprocessed));
                            txtTotal.setBackgroundColor(context.getResources().getColor(R.color.orderBar_unprocessed));
                            status.setText(R.string.order_inprocess);
                            break;
                        case 1:
                            time.setBackgroundColor(context.getResources().getColor(R.color.orderBar_unprocessed));
                            txtTotal.setBackgroundColor(context.getResources().getColor(R.color.orderBar_unprocessed));
                            status.setText(R.string.order_processing);
                            break;
                        case 2:
                            time.setBackgroundColor(context.getResources().getColor(R.color.orderBar_waiting));
                            txtTotal.setBackgroundColor(context.getResources().getColor(R.color.orderBar_waiting));
                            status.setText(R.string.order_waiting);
                            break;
                        case 3:
                            time.setBackgroundColor(context.getResources().getColor(R.color.orderBar_waiting));
                            txtTotal.setBackgroundColor(context.getResources().getColor(R.color.orderBar_waiting));
                            status.setText(R.string.order_arrive);
                            break;
                        case 4:
                            time.setBackgroundColor(context.getResources().getColor(R.color.orderBar_complete));
                            txtTotal.setBackgroundColor(context.getResources().getColor(R.color.orderBar_complete));
                            status.setText(R.string.order_complete);
                            break;
                    }


                    AlertDialog.Builder builder=new AlertDialog.Builder(context).setView(mView);
                    builder.create();
                    builder.show();


                }
            });


        }
    }



}
