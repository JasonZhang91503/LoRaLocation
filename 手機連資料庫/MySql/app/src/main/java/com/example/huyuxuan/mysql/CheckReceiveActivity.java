package com.example.huyuxuan.mysql;

import android.app.ListActivity;
import android.app.ProgressDialog;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.widget.ListAdapter;
import android.widget.SimpleAdapter;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.utils.URLEncodedUtils;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.Reader;
import java.io.StringWriter;
import java.io.Writer;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

/**
 * Created by huyuxuan on 2017/1/13.
 */

public class CheckReceiveActivity extends ListActivity{
    // Progress Dialog
    private ProgressDialog pDialog;

    ArrayList<HashMap<String, String>> transportList;

    // url to get all products list
    private static String url_check_receive;

    // JSON Node names
    private static final String TAG_SUCCESS = "success";
    private static final String TAG_DATA = "data";
    private static final String TAG_ID = "id";
    private static final String TAG_REQUIRETIME = "requireTime";
    private static final String TAG_KEY = "key";
    private static final String TAG_SENDER = "sender";
    private static final String TAG_RECEIVER = "receiver";
    private static final String TAG_DES = "des";
    private static final String TAG_CARID = "car_id";
    private static final String TAG_STATE = "state";
    private static final String TAG_TYPE = "type";

    // products JSONArray
    JSONArray transports = null;
    String url;
    int sender;
    int type;   //1是檢查是否有信要寄給自己 0是檢查是否已登記寄信

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.all_rows);
        url_check_receive = "http://"+getString(R.string.ip)+"/checkReceive.php";

        Bundle bundle = this.getIntent().getExtras();
        sender = bundle.getInt(TAG_ID);
        type = bundle.getInt(TAG_TYPE);
        Log.d("check:","get type:"+String.valueOf(type));

        // Hashmap for ListView
        transportList = new ArrayList<HashMap<String, String>>();

        // Loading products in Background Thread
        new LoadAllTrans().execute();

        /*
        // Get listview
        ListView lv = getListView();

        // on seleting single product
        // launching Edit Product Screen
        lv.setOnItemClickListener(new AdapterView.OnItemClickListener() {

            @Override
            public void onItemClick(AdapterView<?> parent, View view,
                                    int position, long id) {
                // getting values from selected ListItem
                String pid = ((TextView) view.findViewById(R.id.id)).getText()
                        .toString();

                // Starting new intent
                Intent in = new Intent(getApplicationContext(),
                        EditProductActivity.class);
                // sending pid to next activity
                in.putExtra(TAG_ID, pid);
                Log.d("all rows","change to editActivity");

                // starting new activity and expecting some response back
                startActivityForResult(in, 100);

            }
        });
        */

    }

    /*
    // Response from Edit Product Activity
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        // if result code 100
        if (resultCode == 100) {
            // if result code 100 is received
            // means user edited/deleted product
            // reload this screen again
            Intent intent = getIntent();
            finish();
            startActivity(intent);
        }

    }
    */

    /**
     * Background Async Task to Load all product by making HTTP Request
     * */
    class LoadAllTrans extends AsyncTask<String, String, String> {

        /**
         * Before starting background thread Show Progress Dialog
         * */
        @Override
        protected void onPreExecute() {
            super.onPreExecute();
            pDialog = new ProgressDialog(CheckReceiveActivity.this);
            pDialog.setMessage("Loading transports. Please wait...");
            pDialog.setIndeterminate(false);
            pDialog.setCancelable(false);
            pDialog.show();
        }

        /**
         * getting All transports from url
         * */
        protected String doInBackground(String... args) {
            // Building Parameters
            List<NameValuePair> params = new ArrayList<NameValuePair>();
            params.add(new BasicNameValuePair("id",String.valueOf(sender)));
            params.add(new BasicNameValuePair("type",String.valueOf(type)));
            // getting JSON string from URL
            String json = null;

            StringEntity entity = null;
            String paramString = URLEncodedUtils.format(params, "utf-8");
            url =url_check_receive+ "?" + paramString;
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
                    // products found
                    // Getting Array of DATA
                    transports = tmpObj.getJSONArray(TAG_DATA);

                    // looping through All transports
                    for (int i = 0; i < transports.length(); i++) {
                        JSONObject c = transports.getJSONObject(i);

                        // Storing each json item in variable
                        String id = c.getString(TAG_ID);
                        String requireTime = c.getString(TAG_REQUIRETIME);
                        String sender = c.getString(TAG_SENDER);
                        String receiver = c.getString(TAG_RECEIVER);
                        String des = c.getString(TAG_DES);
                        int car_id = c.getInt(TAG_CARID);
                        int state = c.getInt(TAG_STATE);
                        String key = c.getString(TAG_KEY);

                        // creating new HashMap
                        HashMap<String, String> map = new HashMap<String, String>();

                        // adding each child node to HashMap key => value
                        map.put(TAG_ID, id);
                        map.put(TAG_REQUIRETIME, requireTime);
                        map.put(TAG_SENDER,sender);
                        map.put(TAG_RECEIVER,receiver);
                        map.put(TAG_DES,des);
                        map.put(TAG_CARID,String.valueOf(car_id));
                        map.put(TAG_STATE,String.valueOf(state));
                        map.put(TAG_KEY,key);

                        // adding HashList to ArrayList
                        transportList.add(map);
                    }
                } else {
                    // no transport found
                    Intent i = new Intent(getApplicationContext(), FeaturesActivity.class);
                    /*
                    Bundle bundle = new Bundle();
                    bundle.putInt("type",type);
                    Log.d("check:","type:"+String.valueOf(type));
                    i.putExtras(bundle);
                    */
                    setResult(200,i);
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
            // dismiss the dialog after getting all products
            pDialog.dismiss();
            // updating UI from Background Thread
            runOnUiThread(new Runnable() {
                public void run() {
                    /**
                     * Updating parsed JSON data into ListView
                     * */
                    ListAdapter adapter = new SimpleAdapter(
                            CheckReceiveActivity.this, transportList,
                            R.layout.list_item, new String[] { TAG_ID,
                            TAG_REQUIRETIME,TAG_STATE,TAG_SENDER,TAG_RECEIVER,TAG_DES,TAG_CARID,TAG_KEY},
                            new int[] { R.id.id, R.id.requireTime,R.id.state,R.id.sender,R.id.receiver,R.id.des_id,R.id.car_id,R.id.key});
                    // updating listview
                    setListAdapter(adapter);
                }
            });
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
}
