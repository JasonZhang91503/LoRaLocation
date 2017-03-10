package com.example.huyuxuan.mysql.alarm;

import android.app.IntentService;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.TaskStackBuilder;
import android.content.Context;
import android.content.Intent;
import android.support.v4.app.NotificationCompat;
import android.util.Log;
import com.example.huyuxuan.mysql.FeaturesActivity;
import com.example.huyuxuan.mysql.MainActivity;
import com.example.huyuxuan.mysql.R;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.utils.URLEncodedUtils;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;

import static com.example.huyuxuan.mysql.CheckReceiveActivity.convertStreamToString;

/**
 * Created by peter on 2017/1/17.
 */

public class MySchedulingService extends IntentService {

    private static final String TAG_SUCCESS = "success";
    private static final String TAG_DATA = "data";
    private static String url_check_notification;
    private int id;
    int notifyId=0;

    public MySchedulingService() {

        super("NAME");
        url_check_notification = "http://192.168.0.103/notification.php";
    }

    @Override
    protected void onHandleIntent(Intent workIntent) {
        id = workIntent.getExtras().getInt("id");
        Log.d("service","work id = "+ String.valueOf(id));

        // Gets data from the incoming Intent
        List<NameValuePair> params = new ArrayList<NameValuePair>();
        params.add(new BasicNameValuePair("id",String.valueOf(id)));
        // getting JSON string from URL
        String json = null;

        StringEntity entity = null;
        String paramString = URLEncodedUtils.format(params, "utf-8");
        String url =url_check_notification+ "?" + paramString;
        HttpGet httpGet = new HttpGet(url);

        HttpClient client = new DefaultHttpClient();
        HttpResponse response = null;
        try {
            response = client.execute(httpGet);
            HttpEntity httpEntity = response.getEntity();
            if (httpEntity != null) {
                InputStream is = httpEntity.getContent();
                json = convertStreamToString(is);

                Log.i("POST", "Json: " + json);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

        try {
            // Checking for SUCCESS TAG
            JSONObject tmpObj = new JSONObject(json);
            int success = tmpObj.getInt(TAG_SUCCESS);


            if (success == 1) {
                /*
                JSONArray transports = tmpObj.getJSONArray(TAG_DATA);

                // looping through All transports
                for (int i = 0; i < transports.length(); i++) {
                    JSONObject c = transports.getJSONObject(i);

                    // Storing each json item in variable
                    String id = c.getString("id");
                    String requireTime = c.getString("requireTime");
                    String sender = c.getString("sender");
                    String receiver = c.getString("receiver");
                    String des = c.getString("des_id");
                    int car_id = c.getInt("car_id");
                    int state = c.getInt("state");
                    String key = c.getString("key");

                    Log.d("service",id+" "+requireTime+" "+sender+" "+receiver+" "+des+" "+String.valueOf(car_id)+" "+String.valueOf(state)+" "+key);
                }
                */
                //跳通知
                createSimpleNotification();

            }else{
                Log.d("service",tmpObj.getString("message"));
            }
        } catch (JSONException e) {
            e.printStackTrace();
        }

    }

    private void createSimpleNotification() {
        Log.d("service","notification");
        NotificationCompat.Builder mBuilder =
                new NotificationCompat.Builder(this)
                        .setSmallIcon(R.mipmap.ic_launcher)
                        .setContentTitle("專題！！！")
                        .setContentText("車子到囉！！！！！")
                        .setAutoCancel(true);
        // Creates an explicit intent for an Activity in your app
        Intent resultIntent = new Intent(this, FeaturesActivity.class);
        resultIntent.putExtra("id",id);

        // The stack builder object will contain an artificial back stack for the
        // started Activity.
        // This ensures that navigating backward from the Activity leads out of
        // your application to the Home screen.
        TaskStackBuilder stackBuilder = TaskStackBuilder.create(this);
        // Adds the back stack for the Intent (but not the Intent itself)
        stackBuilder.addParentStack(MainActivity.class);
        // Adds the Intent that starts the Activity to the top of the stack
        stackBuilder.addNextIntent(resultIntent);
        PendingIntent resultPendingIntent =
                stackBuilder.getPendingIntent(
                        0,
                        PendingIntent.FLAG_UPDATE_CURRENT
                );
        mBuilder.setContentIntent(resultPendingIntent);
        NotificationManager mNotificationManager =
                (NotificationManager) getSystemService(Context.NOTIFICATION_SERVICE);
        // mId allows you to update the notification later on.
        mNotificationManager.notify(notifyId, mBuilder.build());
    }

}