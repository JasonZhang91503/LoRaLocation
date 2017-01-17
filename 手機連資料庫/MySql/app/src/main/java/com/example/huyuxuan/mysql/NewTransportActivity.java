package com.example.huyuxuan.mysql;

import android.app.Activity;
import android.app.DatePickerDialog;
import android.app.ProgressDialog;
import android.app.TimePickerDialog;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.DatePicker;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.TimePicker;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.client.utils.URLEncodedUtils;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.protocol.HTTP;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.Reader;
import java.io.StringWriter;
import java.io.UnsupportedEncodingException;
import java.io.Writer;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.HashMap;
import java.util.List;
import java.util.Random;

/**
 * Created by huyuxuan on 2016/12/25.
 */

public class NewTransportActivity extends Activity{
    private ProgressDialog pDialog;

    String json = "";
    int sender;
    EditText inputCar;
    Button btnChooseDate;
    Button btnChooseTime;
    Spinner spnSendLocation;
    Spinner spnRcvLocation;
    Spinner spnReceiver;
    String time;
    String requireTime;
    String date;
    String receiver;
    String destination;
    String start;
    String car;
    String key;
    String[] spinnerArray;  //存要放進spinner的location
    String[] spinnerArray2; //存要放進spinner的user


    private static String url_create_transport;
    private static String url_get_all_location;

    // JSON Node names
    private static final String TAG_SUCCESS = "success";
    private static final String TAG_ID = "id";
    private static final String TAG_NAME = "name";
    private static final String TAG_DATA = "data";
    private static final String TAG_USR = "usr";

    int success=0;
    JSONArray locations = null;
    JSONArray users = null;
    ArrayList<HashMap<String, String>> locationList;
    ArrayList<HashMap<String, String>> userList;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.add_transport);
        url_create_transport = "http://"+getString(R.string.ip)+"/create_transport.php";
        url_get_all_location = "http://"+getString(R.string.ip)+"/getAllLocation.php";

        Bundle bundle = this.getIntent().getExtras();
        sender = bundle.getInt(TAG_ID);

        // Edit Text
        inputCar = (EditText)findViewById(R.id.editTextCar);

        // Create button
        Button btnCreateProduct = (Button) findViewById(R.id.btnCreate);

        //Button
        btnChooseDate = (Button)findViewById(R.id.btnChooseDate);
        btnChooseTime = (Button)findViewById(R.id.btnChooseTime);

        //Spinner
        spnSendLocation = (Spinner)findViewById(R.id.spnSendLocation);
        spnRcvLocation = (Spinner)findViewById(R.id.spnRcvLocation);
        spnReceiver = (Spinner)findViewById(R.id.spnReceiver);

        locationList = new ArrayList<HashMap<String, String>>();
        userList = new ArrayList<HashMap<String, String>>();

        new NewTransportActivity.GetLocation().execute();

        // button click event
        btnCreateProduct.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View view) {
                // creating new product in background thread
                car = inputCar.getText().toString();
                Random r = new Random();
                char tmp[] = new char[4];
                for(int i=0;i<4;i++){
                    tmp[i]=Character.forDigit(r.nextInt(10-0)+0,10);
                }
                key = String.copyValueOf(tmp);
                requireTime = date + " "+time;
                Log.d("requireTime:",requireTime);
                Log.d("receiver:",receiver);
                Log.d("start:",start);
                Log.d("destination:",destination);
                Log.d("car:",car);
                Log.d("key:",key);
                new NewTransportActivity.CreateNewTransport().execute();
            }
        });

        btnChooseDate.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                showDatePickerDialog();
            }
        });

        btnChooseTime.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                showTimePickerDialog();
            }
        });

        spnSendLocation.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> adapterView, View view, int i, long l) {
                HashMap<String,String> record;
                record = locationList.get(i);
                start = record.get(TAG_ID);
            }

            @Override
            public void onNothingSelected(AdapterView<?> adapterView) {

            }
        });

        spnRcvLocation.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> adapterView, View view, int i, long l) {
                HashMap<String,String> record;
                record = locationList.get(i);
                destination = record.get(TAG_ID);
            }

            @Override
            public void onNothingSelected(AdapterView<?> adapterView) {

            }
        });

        spnReceiver.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> adapterView, View view, int i, long l) {
                HashMap<String,String> record;
                record = userList.get(i);
                receiver = record.get(TAG_ID);
            }

            @Override
            public void onNothingSelected(AdapterView<?> adapterView) {

            }
        });

    }

    /**
     * Background Async Task to Create new row
     * */
    class CreateNewTransport extends AsyncTask<String, String, String> {

        /**
         * Before starting background thread Show Progress Dialog
         * */
        @Override
        protected void onPreExecute() {
            super.onPreExecute();
            pDialog = new ProgressDialog(NewTransportActivity.this);
            pDialog.setMessage("Creating Transport..");
            pDialog.setIndeterminate(false);
            pDialog.setCancelable(true);
            pDialog.show();
        }

        /**
         * Creating product
         * */
        protected String doInBackground(String... args) {
            JSONObject jsonObj = new JSONObject();

            try {
                jsonObj.put("sender",sender);
                jsonObj.put("requireTime",requireTime);
                jsonObj.put("receiver",receiver);
                jsonObj.put("start_id",start);
                jsonObj.put("des_id",destination);
                jsonObj.put("car_id",car);
                jsonObj.put("key",key);
            } catch (JSONException e) {
                e.printStackTrace();
            }
            Log.i("POST", "JsonObj: " + jsonObj.toString());
            HttpPost httpPost = new HttpPost(url_create_transport);
            StringEntity entity = null;
            try {
                entity = new StringEntity(jsonObj.toString(), HTTP.UTF_8);
            } catch (UnsupportedEncodingException e) {
                e.printStackTrace();
            }
            httpPost.setEntity(entity);
            HttpClient client = new DefaultHttpClient();
            HttpResponse response = null;
            try {
                response = client.execute(httpPost);
                HttpEntity httpEntity = response.getEntity();
                if (httpEntity != null) {
                    InputStream is = httpEntity.getContent();
                    json = convertStreamToString(is);

                    Log.i("POST", "Json: " + json);
                }
            } catch (IOException e) {
                e.printStackTrace();
            }


            Log.d("PARSER", "json:"+json);

            // check for success tag
            try {
                JSONObject tmpObj = new JSONObject(json);
                success = tmpObj.getInt(TAG_SUCCESS);

                if (success == 1) {
                    // successfully created transport
                    Intent i = new Intent(getApplicationContext(), FeaturesActivity.class);
                 //   startActivity(i);
                    setResult(100,i);
                    // closing this screen
                    finish();
                }

            } catch (JSONException e) {
                e.printStackTrace();
            }

            return null;
        }

        /**
         * After completing background task Dismiss the progress dialog
         * **/
        protected void onPostExecute(String file_url) {
            // dismiss the dialog once done
            pDialog.dismiss();
        }

    }

    public static String convertStreamToString(InputStream is) throws IOException {
        if (is != null) {
            Writer writer = new StringWriter();
            char[] buffer = new char[1024];
            try {
                Reader reader = new BufferedReader(new InputStreamReader(is, "UTF-8"));
                int n = 0;
                while ((n = reader.read(buffer)) != -1)
                    writer.write(buffer, 0, n);

            } catch (Exception e) {

            } finally {
                is.close();
            }
            return writer.toString();
        } else {
            return "";
        }
    }

    public void showDatePickerDialog() {
        // 設定初始日期
        final Calendar c = Calendar.getInstance();
        final int mYear = c.get(Calendar.YEAR);
        final int mMonth = c.get(Calendar.MONTH);
        int mDay = c.get(Calendar.DAY_OF_MONTH);

        // 跳出日期選擇器
        DatePickerDialog dpd = new DatePickerDialog(this,
                new DatePickerDialog.OnDateSetListener() {
                    public void onDateSet(DatePicker view, int year,
                                          int monthOfYear, int dayOfMonth) {
                        // 完成選擇，顯示日期
                        date = String.valueOf(year)+"-"+String.valueOf(monthOfYear + 1)+"-"+dayOfMonth;
                        Log.d("choose date:",date);
                        btnChooseDate.setText(date);
                    }
                }, mYear, mMonth, mDay);
        dpd.show();
    }

    public void showTimePickerDialog() {
        // 設定初始時間
        final Calendar c = Calendar.getInstance();
        int mHour = c.get(Calendar.HOUR_OF_DAY);
        int mMinute = c.get(Calendar.MINUTE);


        // 跳出時間選擇器
        CustomTimePickerDialog ctpd = new CustomTimePickerDialog(this,new CustomTimePickerDialog.OnTimeSetListener(){
            @Override
            public void onTimeSet(TimePicker timePicker, int hourOfDay, int minute) {
                // 完成選擇，顯示時間
                time = hourOfDay+":"+minute+":00";
                btnChooseTime.setText(hourOfDay + ":" + minute);
            }
        },mHour,mMinute,true);
        ctpd.show();
    }

    class GetLocation extends AsyncTask<String, String, String>{

        HashMap<String, String> map;
        HashMap<String,String>map_usr;

        @Override
        protected String doInBackground(String... strings) {

            List<NameValuePair> params = new ArrayList<NameValuePair>();

            String json = null;

            StringEntity entity = null;
            String paramString = URLEncodedUtils.format(params, "utf-8");
            String url =url_get_all_location+ "?" + paramString;
            HttpGet httpGet = new HttpGet(url);

            HttpClient client = new DefaultHttpClient();
            HttpResponse response = null;
            try {
                response = client.execute(httpGet);
                HttpEntity httpEntity = response.getEntity();
                if (httpEntity != null) {
                    InputStream is = httpEntity.getContent();
                    json = convertStreamToString(is);

                    Log.i("GET", "Json: " + json);
                }
            } catch (IOException e) {
                e.printStackTrace();
            }


            try {
                // Checking for SUCCESS TAG
                JSONObject tmpObj = new JSONObject(json);
                int success = tmpObj.getInt(TAG_SUCCESS);


                if (success == 1) {
                    //抓location的資料
                    locations = tmpObj.getJSONArray(TAG_DATA);
                    //抓user的資料
                    users = tmpObj.getJSONArray(TAG_USR);

                    // looping through All Products
                    for (int i = 0; i < locations.length(); i++) {
                        JSONObject c = locations.getJSONObject(i);

                        // Storing each json item in variable
                        String id = c.getString(TAG_ID);
                        String name = c.getString(TAG_NAME);

                        // creating new HashMap
                        map = new HashMap<String, String>();

                        // adding each child node to HashMap key => value
                        map.put(TAG_ID, id);
                        map.put(TAG_NAME,name);

                        // adding HashList to ArrayList
                        locationList.add(map);
                    }
                    for (int i = 0; i < users.length(); i++) {
                        JSONObject c = users.getJSONObject(i);

                        // Storing each json item in variable
                        String id = c.getString(TAG_ID);
                        String name = c.getString(TAG_NAME);

                        // creating new HashMap
                        map_usr = new HashMap<String, String>();

                        // adding each child node to HashMap key => value
                        map_usr.put(TAG_ID, id);
                        map_usr.put(TAG_NAME,name);

                        // adding HashList to ArrayList
                        userList.add(map_usr);
                    }
                }
            } catch (JSONException e) {
                e.printStackTrace();
            }

            return null;
        }

        protected void onPostExecute(String file_url) {
            // dismiss the dialog after getting all products
            spinnerArray = new String[locationList.size()];
            spinnerArray2 = new String[userList.size()];
            for (int i = 0; i < locationList.size(); i++)
            {
                spinnerArray[i] = locationList.get(i).get(TAG_NAME);
            }
            for (int i = 0; i < userList.size(); i++)
            {
                spinnerArray2[i] = userList.get(i).get(TAG_NAME);
            }
            // updating UI from Background Thread
            runOnUiThread(new Runnable() {
                public void run() {
                    /**
                     * Updating parsed JSON data into ListView
                     * */
                    ArrayAdapter<String> locaList = new ArrayAdapter<>(NewTransportActivity.this,
                            android.R.layout.simple_spinner_dropdown_item,
                            spinnerArray);
                    ArrayAdapter<String> _usrList = new ArrayAdapter<>(NewTransportActivity.this,
                            android.R.layout.simple_spinner_dropdown_item,
                            spinnerArray2);
                    spnSendLocation.setAdapter(locaList);
                    spnRcvLocation.setAdapter(locaList);
                    spnReceiver.setAdapter(_usrList);
                    spnSendLocation.setClickable(true);
                    spnRcvLocation.setClickable(true);
                    spnReceiver.setClickable(true);
                }
            });
        }

    }


}
