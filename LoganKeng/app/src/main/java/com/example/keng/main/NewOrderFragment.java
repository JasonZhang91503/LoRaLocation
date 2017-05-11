package com.example.keng.main;


import java.util.ArrayList;
import java.util.Calendar;

import android.app.TimePickerDialog;
import android.icu.util.TimeZone;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentTransaction;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.TimePicker;
import android.widget.Toast;

import java.util.Date;


/**
 * A simple {@link Fragment} subclass.
 */
public class NewOrderFragment extends Fragment implements View.OnClickListener{

    private final String TAG=this.getClass().getSimpleName();
    int[] location={0,0};
    String timeText;
    boolean is_setTime=false;
    public NewOrderFragment() {
        // Required empty public constructor
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment

        View view=inflater.inflate(R.layout.fragment_new_order, container, false);
        //得到日期資訊並且顯示於Text中
        LinearLayout time=(LinearLayout)view.findViewById(R.id.time);
        final Calendar calendar=Calendar.getInstance();//透過Calendar得到現在的日期並進行轉換
        final TextView orderTime=(TextView)view.findViewById(R.id.orderTime);
        //設定下一步Btn的內容
        Button btn=(Button)view.findViewById(R.id.btn_next);
        btn.setOnClickListener(this);
        Spinner start=(Spinner)view.findViewById(R.id.start);
        Spinner dest=(Spinner)view.findViewById(R.id.destination);
        ArrayAdapter<CharSequence> locationList=ArrayAdapter.createFromResource(getActivity(),R.array.Location,R.layout.support_simple_spinner_dropdown_item);
        start.setAdapter(locationList);
        start.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                String[] array=getResources().getStringArray(R.array.Location);
                location[0]=position;
                Log.d(TAG,array[position]);
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });
        dest.setAdapter(locationList);
        dest.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                String[] array=getResources().getStringArray(R.array.Location);
                location[1] =position;
                Log.e(TAG,array[position]);
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });
        //替time Linearlayout 設定ClickListener
        time.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

              new TimePickerDialog(getContext(),2, new TimePickerDialog.OnTimeSetListener() {
                  @Override
                  public void onTimeSet(TimePicker view, int hourOfDay, int minute) {
                      String h,m;
                      //讀取現在的時間資料
                      int year=calendar.get(Calendar.YEAR);
                      int month=calendar.get(Calendar.MONTH)+1;
                      int day=calendar.get(Calendar.DAY_OF_MONTH);
                      //優化文字介面用
                      if(hourOfDay<10){
                          h="0"+String.valueOf(hourOfDay);
                      }else{
                          h=String.valueOf(hourOfDay);
                      }
                      if(minute<10){
                          m="0"+String.valueOf(minute);
                      }else{
                          m=String.valueOf(minute);
                      }
                      timeText=year+"年"+month+"月"+day+"日"+h+"時"+m+"分";
                      orderTime.setText(timeText);
                      is_setTime=true;
                  }
              }, calendar.get(Calendar.HOUR_OF_DAY), calendar.get(Calendar.MINUTE), true).show();

            }
        });

        return view;
    }

    @Override
    public void onClick(View v) {

        //創造一個可以傳遞參數的Fragment，並且將地點和時間的參數傳遞到下一個Fragment讓最後確認時可以有資訊
        //傳遞 Location 編號->可以從 string.xml 找到
        //傳遞 timeText 時間資訊->看看是否要改格式(方便傳遞)
        if(!is_setTime){
            //避免沒有選填時間
            Toast.makeText(getContext(),"請填選時間",Toast.LENGTH_SHORT).show();
        }else if(location[0]==location[1]){
            //避免相同收件地
            Toast.makeText(getContext(),"寄件地收件地不可相同",Toast.LENGTH_SHORT).show();
        }
        else {
            NewOrderFragment2 fragment2 = NewOrderFragment2.newInstance(location, timeText);
            FragmentManager fm = getActivity().getSupportFragmentManager();
            FragmentTransaction trans = fm.beginTransaction();
            trans.addToBackStack(null);
            trans.replace(R.id.frame, fragment2, fragment2.getTag());
            trans.commit();
        }

    }
}
