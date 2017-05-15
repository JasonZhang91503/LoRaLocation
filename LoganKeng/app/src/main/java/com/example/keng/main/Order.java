package com.example.keng.main;

import android.util.Log;

/**
 * Created by keng on 5/7/17.
 */

public class Order {

    private int start_place;
    private int final_place;
    private int order_state;
    private int order_cost;
    private String month,date,minute,hour,year;
    private String recv_name;
    private String send_time;
    private String note;
    private String key_num;
    private String send_name;
    //起始地 終點 花費 寄送者 接收者 電話 時間 備註 密碼 狀態
    public Order(int start,int destination,int cost,String sender,String receiver,String time,String str,String key,int state){
        start_place=start;
        final_place=destination;
        send_name=sender;
        recv_name=receiver;
        note=str;
        order_state=state;
        order_cost=cost;
        key_num=key;
        setSetTime(time);
    }

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

    public String getKey_num(){
        return key_num;
    }
    public String getSend_name(){
        return send_name;
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
