package com.example.huyuxuan.lora;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class MainActivity extends AppCompatActivity {

    private Button btnLogin;
    private Button btnSignUp;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        btnLogin = (Button)findViewById(R.id.btnLogin);
        btnSignUp = (Button)findViewById(R.id.btnSignUp);

        btnLogin.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(MainActivity.this, ConnectService.class);
                intent.putExtra("type","out");
                intent.putExtra("id",3);
                intent.putExtra("account","dfdf");
                intent.putExtra("password","1123");
                startService(intent);
            }
        });
        btnSignUp.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(MainActivity.this, ConnectService.class);
                intent.putExtra("type","out");
                intent.putExtra("id",2);
                intent.putExtra("account","dfdf");
                intent.putExtra("password","1123");
                intent.putExtra("name","huhu");
                intent.putExtra("email","ssdd@gmail.com");
                startService(intent);
            }
        });
    }

}
