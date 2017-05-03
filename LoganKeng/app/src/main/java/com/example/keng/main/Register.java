package com.example.keng.main;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

public class Register extends AppCompatActivity {

    EditText usr,pwd,name,mail;
    Button register;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_register);
        usr=(EditText)findViewById(R.id.usr);
        name=(EditText)findViewById(R.id.usr_name);
        pwd=(EditText)findViewById(R.id.pwd);
        mail=(EditText)findViewById(R.id.mail);
        //設定提示字元的顏色
        usr.setHintTextColor(getResources().getColor(R.color.colorHint));
        name.setHintTextColor(getResources().getColor(R.color.colorHint));
        pwd.setHintTextColor(getResources().getColor(R.color.colorHint));
        mail.setHintTextColor(getResources().getColor(R.color.colorHint));

        register=(Button)findViewById(R.id.registe);

        //可以在這裡放入連線資料庫的內容
        register.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //要有防呆+加入資料庫的code
                //大家覺得要註冊完在登入一次？
                //如果需要下面加這些
                Intent i=new Intent(Register.this,Login.class);
                startActivity(i);
                finish();

            }
        });
    }
}
