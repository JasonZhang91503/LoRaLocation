package com.sillywei.alarmdemo.scheduler;

import android.app.IntentService;
import android.content.Intent;
import android.util.Log;

/**
 * Created by peter on 2017/1/17.
 */

public class MySchedulingService extends IntentService {

    private final String NAME = "SCHEDULING_SERVICE";
    private static int number = 0;

    public MySchedulingService() {
        super("NAME");
    }

    @Override
    protected void onHandleIntent(Intent workIntent) {
        // Gets data from the incoming Intent
        Log.i(NAME, "Service work: " + ++number);
    }
}
