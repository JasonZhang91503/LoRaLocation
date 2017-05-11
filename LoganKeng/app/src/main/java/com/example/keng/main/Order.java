package com.example.keng.main;

/**
 * Created by keng on 5/7/17.
 */

public class Order {

    private int start_place;
    private int final_place;
    private int order_state;
    private int order_cost;
    private String recv_name;
    private String recv_phone;
    private String send_time;
    private String note;
    private String key_num;
    private String send_name;
    //起始地 終點 花費 寄送者 接收者 電話 時間 備註 密碼 狀態
    public Order(int start,int destination,int cost,String sender,String receiver,String phone,String time,String str,String key,int state){
        start_place=start;
        final_place=destination;
        recv_name=receiver;
        recv_phone=phone;
        send_time=time;
        note=str;
        order_state=state;
        order_cost=cost;
        key_num=key;
        send_name=sender;
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

    public String getRecv_phone() {
        return recv_phone;
    }

    public String getNote() {
        return note;
    }
}
