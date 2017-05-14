package com.example.keng.main;

/**
 * Created by keng on 5/1/17.
 */
import android.content.Context;
import android.content.SharedPreferences;
public class PrefManager {
    SharedPreferences pref;
    SharedPreferences.Editor editor;
    Context _context;

    private static final String PREF_NAME="com.example.keng.main";
    private static final String Is_First_Time_Launch="IsFirstTimeLaunch";
    private static final String ACCOUNT="Account";
    private static final String PASSWORD="Password";
    private static final String NAME="Name";
    public PrefManager(Context context){
        this._context=context;
        pref=context.getSharedPreferences(PREF_NAME,Context.MODE_PRIVATE);
        editor=pref.edit();
    }
    public void setFirstTimeLaunch(boolean isFirstTime){
        editor.putBoolean(Is_First_Time_Launch,isFirstTime);//進行設定
        editor.commit();//設定完要commit 才算儲存

    }
    public boolean isFirstTimeLaunch(){
        return  pref.getBoolean(Is_First_Time_Launch,true);//如果沒有Is_First_Time_Launch 傳回Ture
    }
    public void setAccount(String name,String password){
        editor.putString(ACCOUNT,name);
        editor.putString(PASSWORD,password);
        editor.putString(NAME,"耿楷寗");
    }
    public String getAccount(){
        return  pref.getString(ACCOUNT,"test");
    }
    public String getPassword(){
        return  pref.getString(PASSWORD,"1234");
    }
    public String getName(){
        return pref.getString(NAME,"測試帳戶");
    }
}
