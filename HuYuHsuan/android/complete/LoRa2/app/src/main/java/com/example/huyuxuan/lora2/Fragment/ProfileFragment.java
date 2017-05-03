package com.example.huyuxuan.lora2.Fragment;

import android.content.ContentResolver;
import android.content.ContentValues;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Bundle;
import android.provider.MediaStore;
import android.support.v4.app.Fragment;
import android.support.v7.app.AlertDialog;
import android.util.DisplayMetrics;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageButton;
import android.widget.TextView;

import com.example.huyuxuan.lora2.R;

import java.io.FileNotFoundException;

import static android.content.Context.MODE_PRIVATE;

/**
 * Created by huyuxuan on 2017/5/1.
 */

public class ProfileFragment extends Fragment{
    private View myview;
    private TextView myNameView;
    private TextView myAccountView;
    private TextView myEmailView;


    private SharedPreferences sharedPreferences;



    @Override
    public void onCreate(Bundle savedInstances){
        super.onCreate(savedInstances);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState){
        myview = inflater.inflate(R.layout.fragment_profile,container,false);
        myNameView = (TextView)myview.findViewById(R.id.tvProfileName);
        myAccountView = (TextView)myview.findViewById(R.id.tvProfileAccount);
        myEmailView = (TextView)myview.findViewById(R.id.tvProfileEmail);

        sharedPreferences = getActivity().getSharedPreferences("data" , MODE_PRIVATE);

        myNameView.setText(sharedPreferences.getString(getString(R.string.name),""));
        myAccountView.setText(sharedPreferences.getString(getString(R.string.account),""));
        myEmailView.setText(sharedPreferences.getString(getString(R.string.email),""));



        return myview;
    }


}
