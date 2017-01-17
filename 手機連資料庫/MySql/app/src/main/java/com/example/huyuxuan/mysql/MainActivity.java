package com.example.huyuxuan.mysql;

import android.app.ProgressDialog;
import android.content.Intent;
import android.os.AsyncTask;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.utils.URLEncodedUtils;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;

import static com.example.huyuxuan.mysql.NewUserActivity.convertStreamToString;

public class MainActivity extends AppCompatActivity {
    private ProgressDialog pDialog;


    String Account;
    String Password;
    int id;
    String mail;
    String name;

    Button mAddUserBtn;
    Button mLoginBtn;
    EditText mLoginAcc;
    EditText mLoginPass;
    String url;
    int success=-1;

    private static final String TAG_NAME = "name";
    private static final String TAG_MAIL = "mail";
    private static final String TAG_ID = "id";
    private static final String TAG_SUCCESS = "success";
    private static final String TAG_DATA = "data";
    private static final String TAG_MESSAGE = "message";

    private static String url_get_user;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        url_get_user = "http://"+getString(R.string.ip)+"/get_user.php";
        mAddUserBtn = (Button)findViewById(R.id.btnAddUser);
        mLoginBtn = (Button)findViewById(R.id.btnLogin);
        mLoginAcc = (EditText)findViewById(R.id.editTextLoginAcc);
        mLoginPass = (EditText)findViewById(R.id.editTextLoginPass);


        mAddUserBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(getApplicationContext(),NewUserActivity.class);
                startActivity(intent);
            }
        });

        mLoginBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Account = mLoginAcc.getText().toString();
                Password = mLoginPass.getText().toString();
                new GetUser().execute();
            }
        });
    }

    class GetUser extends AsyncTask<String,String,String> {

        String errormes;

        @Override
        protected void onPreExecute() {
            super.onPreExecute();
            pDialog = new ProgressDialog(MainActivity.this);
            pDialog.setMessage("登入中 ...");
            pDialog.setIndeterminate(false);
            pDialog.setCancelable(true);
            pDialog.show();
        }

        @Override
        protected String doInBackground(String... strings) {
            String json = null;
            // Building Parameters

            List<NameValuePair> params = new ArrayList<NameValuePair>();
            params.add(new BasicNameValuePair("account", Account));
            params.add(new BasicNameValuePair("password",Password));
            // getting JSON string from URL

            String paramString = URLEncodedUtils.format(params, "utf-8");
            Log.d("paramString: ",paramString);
            url= url_get_user+"?" + paramString;
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
                    JSONObject tmpObj = new JSONObject(json);
                    success = tmpObj.getInt(TAG_SUCCESS);
                    if(success == 1){
                        JSONArray user = tmpObj.getJSONArray(TAG_DATA);
                        JSONObject c = user.getJSONObject(0);
                        id=c.getInt(TAG_ID);
                        mail = c.getString(TAG_MAIL);
                        name = c.getString(TAG_NAME);
                        Log.d("Login:","mail= "+mail);
                    }else{
                        errormes = tmpObj.getString(TAG_MESSAGE);
                    }
                }
            } catch (IOException e) {
                e.printStackTrace();
            } catch (JSONException e) {
                e.printStackTrace();
            }

            return null;
        }

        protected void onPostExecute(String file_url) {
            // dismiss the dialog once got all details
            pDialog.dismiss();
            if(success == 1){
                Intent intent = new Intent(getApplicationContext(),FeaturesActivity.class);
                Bundle bundle = new Bundle();
                bundle.putInt(TAG_ID,id);

                intent.putExtras(bundle);
                startActivity(intent);
            }else{
                Toast.makeText(getApplicationContext(),errormes,Toast.LENGTH_SHORT).show();
            }

        }

    }

}
