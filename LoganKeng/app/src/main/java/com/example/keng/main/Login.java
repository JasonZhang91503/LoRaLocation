package com.example.keng.main;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class Login extends AppCompatActivity {

    EditText usr,pwd;
    Button login;
    PrefManager pref;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);
        usr=(EditText)findViewById(R.id.user);
        pwd=(EditText)findViewById(R.id.pwd);
        login=(Button)findViewById(R.id.button) ;
        usr.setHintTextColor(getResources().getColor(R.color.colorHint));
        pwd.setHintTextColor(getResources().getColor(R.color.colorHint));
        login.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //這邊可以加詢問資料庫是否有這筆帳戶資料
                //如果可以才給登入
                pref=new PrefManager(v.getContext());
                if(usr.getText().length()!=0||pwd.getText().length()!=0){
                    pref.setAccount(String.valueOf(usr.getText()),String.valueOf(pwd.getText()));
                    pref.setFirstTimeLaunch(false);
                    Log.d("preference set up ",usr.getText()+" "+pwd.getText());
                    Log.d(getClass().getSimpleName(),"login successfully");

                    Intent i=new Intent(Login.this,Mainpage.class);
                    startActivity(i);
                    finish();
                }else{
                    Toast.makeText(v.getContext(),"請輸入帳號及密碼",Toast.LENGTH_SHORT).show();
                }
            }
        });


    }
}
