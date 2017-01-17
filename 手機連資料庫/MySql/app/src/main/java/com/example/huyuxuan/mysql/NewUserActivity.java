package com.example.huyuxuan.mysql;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.protocol.HTTP;
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

/**
 * Created by huyuxuan on 2016/12/8.
 */

public class NewUserActivity extends Activity{
    private ProgressDialog pDialog;

    String json = "";

    EditText inputName;
    EditText inputAccount;
    EditText inputPass;
    EditText inputEmail;
    String name;
    String account;
    String password;
    String email;
    private static String url_create_user;

    // JSON Node names
    private static final String TAG_SUCCESS = "success";
    int success=0;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.add_user);
        url_create_user = "http://"+getString(R.string.ip)+"/create_user.php";

        // Edit Text
        inputName = (EditText) findViewById(R.id.editTextName);
        inputAccount = (EditText) findViewById(R.id.editTextAccount);
        inputPass = (EditText) findViewById(R.id.editTextPass);
        inputEmail = (EditText)findViewById(R.id.editTextEmail);

        // Create button
        Button btnCreateProduct = (Button) findViewById(R.id.btnCreate);

        // button click event
        btnCreateProduct.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View view) {
                // creating new product in background thread
                name = inputName.getText().toString();
                account = inputAccount.getText().toString();
                password = inputPass.getText().toString();
                email = inputEmail.getText().toString();

                Log.d("name:",name);
                Log.d("account:",account);
                Log.d("password:",password);
                Log.d("email:",email);
                new CreateNewUser().execute();
            }
        });


    }

    /**
     * Background Async Task to Create new row
     * */
    class CreateNewUser extends AsyncTask<String, String, String> {

        /**
         * Before starting background thread Show Progress Dialog
         * */
        @Override
        protected void onPreExecute() {
            super.onPreExecute();
            pDialog = new ProgressDialog(NewUserActivity.this);
            pDialog.setMessage("Creating User..");
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

                jsonObj.put("name",name);
                jsonObj.put("account",account);
                jsonObj.put("password",password);
                jsonObj.put("mail",email);
            } catch (JSONException e) {
                e.printStackTrace();
            }
            Log.i("POST", "JsonObj: " + jsonObj.toString());
            HttpPost httpPost = new HttpPost(url_create_user);
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
                    // successfully created user
                     Intent i = new Intent(getApplicationContext(), FeaturesActivity.class);
                       startActivity(i);

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
}
