package com.example.keng.main;


import android.app.AlertDialog;
import android.app.DatePickerDialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.Display;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.DatePicker;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import java.util.Calendar;


/**
 * A simple {@link Fragment} subclass.
 */
public class AccountFragment extends Fragment {

    ImageView editMail,editPwd;
    TextView pwd,mail;
    public AccountFragment() {
        // Required empty public constructor
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View view=inflater.inflate(R.layout.fragment_account, container, false);
        editMail=(ImageView)view.findViewById(R.id.editmail);
        editPwd=(ImageView)view.findViewById(R.id.editpwd);
        pwd=(TextView)view.findViewById(R.id.password);
        mail=(TextView)view.findViewById(R.id.mail);
        editMail.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Toast.makeText(getContext(),"click 1",Toast.LENGTH_SHORT).show();
            }
        });
        editMail.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                View mView =getActivity().getLayoutInflater().inflate(R.layout.change_mail_dialog,null);
                final EditText editmail=(EditText)mView.findViewById(R.id.edit_mail);
                final AlertDialog mailbuilder=new AlertDialog.Builder(getContext()).setView(mView).setPositiveButton("確認",null).setNegativeButton("取消",null).create();
                mailbuilder.setOnShowListener(new DialogInterface.OnShowListener() {
                    @Override
                    public void onShow(final DialogInterface dialog) {
                        Button btn_confirm=mailbuilder.getButton(AlertDialog.BUTTON_POSITIVE);
                        btn_confirm.setOnClickListener(new View.OnClickListener() {
                            @Override
                            public void onClick(View v) {
                                if(editmail.getText().length()==0){
                                    Toast.makeText(getContext(),"請輸入新信箱",Toast.LENGTH_SHORT).show();
                                }else{
                                    //傳入更新信箱至資料庫
                                    mail.setText(editmail.getText());
                                    dialog.dismiss();
                                }
                            }
                        });
                        Button btn_cancel=mailbuilder.getButton(AlertDialog.BUTTON_NEGATIVE);
                        btn_cancel.setOnClickListener(new View.OnClickListener() {
                            @Override
                            public void onClick(View v) {
                                dialog.dismiss();
                            }
                        });
                    }
                });
                mailbuilder.show();
            }
        });
        editPwd.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                View mView =getActivity().getLayoutInflater().inflate(R.layout.change_pwd_dialog,null);
                final EditText editPwd=(EditText)mView.findViewById(R.id.edit_pwd);
                final EditText editConfirmPwd=(EditText)mView.findViewById(R.id.edit_confirm_pwd);
                final AlertDialog pwdbuilder=new AlertDialog.Builder(getContext()).setView(mView).setPositiveButton("確認",null).setNegativeButton("取消",null).create();
                pwdbuilder.setOnShowListener(new DialogInterface.OnShowListener() {
                    @Override
                    public void onShow(final DialogInterface dialog) {
                        Button btn_confirm=pwdbuilder.getButton(AlertDialog.BUTTON_POSITIVE);
                        btn_confirm.setOnClickListener(new View.OnClickListener() {
                            @Override
                            public void onClick(View v) {
                                if(editConfirmPwd.getText().length()==0||editPwd.getText().length()==0){
                                    Toast.makeText(getContext(),"請檢查是否有空白欄位",Toast.LENGTH_SHORT).show();
                                }else if(!editPwd.getText().toString().equals(editConfirmPwd.getText().toString())){
                                    Toast.makeText(getContext(),"請檢查密碼是否相同",Toast.LENGTH_SHORT).show();
                                }else{
                                    //傳入更新密碼至資料庫
                                    pwd.setText(editPwd.getText());
                                    dialog.dismiss();
                                }
                            }
                        });
                        Button btn_cancel=pwdbuilder.getButton(AlertDialog.BUTTON_NEGATIVE);
                        btn_cancel.setOnClickListener(new View.OnClickListener() {
                            @Override
                            public void onClick(View v) {
                                dialog.dismiss();
                            }
                        });
                    }
                });
                pwdbuilder.show();
            }


        });
        return view;
    }

}
