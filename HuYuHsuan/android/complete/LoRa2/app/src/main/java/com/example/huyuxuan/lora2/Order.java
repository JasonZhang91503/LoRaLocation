package com.example.huyuxuan.lora2;

import android.util.Log;

import java.io.Serializable;

/**
 * Created by huyuxuan on 2017/5/9.
 */

public class Order implements Serializable {

    private int start_place;
    private int final_place;
    private int order_state;
    private int order_cost;
    private String recv_name;
    private String recv_phone;
    private String send_time;
    private String note;

    private String str_start;
    private String str_dest;
    private String str_state;
    private String str_key;
    private String str_require_time;
    private String str_arrive_time;
    private String str_sender;

    private String year,month,date,minute,hour;

    public Order(int start,int destination,int cost,String name,String phone,String time,String str,int state){
        start_place=start;
        final_place=destination;
        recv_name=name;
        recv_phone=phone;
        send_time=time;
        note=str;
        order_state=state;
        order_cost=cost;
    }

    public Order(String start,String dest,String receiver,String sender,String time,String arriveTime,String state,String key,String not){
        str_start=start;
        str_dest=dest;
        recv_name=receiver;
        str_sender=sender;
        str_require_time=time;
        setSetTime(str_require_time);
        str_arrive_time=arriveTime;
        str_state=state;
        order_state=Integer.valueOf(state);
        //預設每筆訂單30元
        order_cost=30;
        str_key = key;
        note=not;

    }
    public String getStr_state(){return str_state;}
    public String getStr_start(){return str_start;}
    public String getStr_dest(){return str_dest;}
    public String getStr_key(){return str_key;}
    public String getStr_arrive_time(){return str_arrive_time;}
    public String getStr_sender(){return str_sender;}
    public String getStr_require_time(){return str_require_time;}

    public int getOrder_cost() {
        return order_cost;
    }

    public int getStartPlace(){
        return start_place;
    }
    public int getFinal_place(){
        return final_place;
    }
    public int getOrder_state(){
        return order_state;
    }

    public String getRecv_name() {
        return recv_name;
    }

    public String getSend_time() {
        return send_time;
    }

    public String getNote() {
        return note;
    }

    public String getMonth(){ return month; }
    public String getDate(){ return date; }
    public String getMinute(){return minute;}
    public String getHour(){return  hour;}
    public String getYear(){return  year;}

    public void setSetTime(String time){
        String[] temp;
        temp=time.split("-| |:");//進行字串分解 ：0:年 1:月 2:日 3:時 4:分 5:秒
        Log.d("tag",temp[0]+" "+temp[1]+" "+temp[2]+" "+temp[3]+" "+temp[4]);
        year=temp[0];
        month=temp[1];
        date=temp[2];
        hour=temp[3];
        minute=temp[4];
        send_time=temp[3]+"時"+temp[4]+"分";

    }
}