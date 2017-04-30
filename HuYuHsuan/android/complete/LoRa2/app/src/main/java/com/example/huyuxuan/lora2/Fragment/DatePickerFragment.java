package com.example.huyuxuan.lora2.Fragment;

import android.app.Activity;
import android.app.DatePickerDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.support.v4.app.Fragment;
import android.os.Bundle;
import android.util.Log;
import android.widget.DatePicker;

import com.example.huyuxuan.lora2.NavigationActivity;

import java.util.Calendar;

/**
 * Created by huyuxuan on 2017/4/29.
 */

public class DatePickerFragment extends DialogFragment implements DatePickerDialog.OnDateSetListener {

    private PassOnDateSetListener mListener;

    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        // Use the current date as the default date in the picker
        final Calendar c = Calendar.getInstance();
        int year = c.get(Calendar.YEAR);
        int month = c.get(Calendar.MONTH);
        int day = c.get(Calendar.DAY_OF_MONTH);

        // Create a new instance of DatePickerDialog and return it
        return new DatePickerDialog(getActivity(), this, year, month, day);
    }

    public void onDateSet(DatePicker view, int year, int month, int day) {
        if (mListener != null) {
            mListener.passOnDateSet(year, month, day);
        }

        Log.d("DatePickerFragment","curFragment.passOnDateSet");
    }

    public interface PassOnDateSetListener {
        void passOnDateSet(int year, int month, int day);

    }


    @Override
    public void onAttach(Activity activity) {
        super.onAttach(activity);

        // This makes sure that the container activity has implemented
        // the callback interface. If not, it throws an exception
        try {
            NavigationActivity mActivity = (NavigationActivity) activity;
            mListener = (PassOnDateSetListener) (mActivity.myFragment);
        } catch (ClassCastException e) {
            throw new ClassCastException(activity.toString()
                    + " must implement OnHeadlineSelectedListener");
        }
    }
}
