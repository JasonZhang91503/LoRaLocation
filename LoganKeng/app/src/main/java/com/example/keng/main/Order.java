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

    public String getRecv_phone() {
        return recv_phone;
    }

    public String getNote() {
        return note;
    }
}
