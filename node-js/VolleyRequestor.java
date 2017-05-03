package com.example.hduser.beacon;

import android.util.Log;

import org.json.JSONObject;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.RequestFuture;

import java.util.concurrent.ExecutionException;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

/**
 * Created by hduser on 2/5/17.
 */

public class VolleyRequestor {
    public static JSONObject requestDataFromUrl(RequestQueue requestQueue, String url) {
        JSONObject response = null;
        RequestFuture<JSONObject> requestFuture = RequestFuture.newFuture();

        JsonObjectRequest request = new JsonObjectRequest(Request.Method.GET,url,
                null, requestFuture, requestFuture);
        requestQueue.add(request);
        try {
            response = requestFuture.get(30000, TimeUnit.MILLISECONDS);
        } catch (InterruptedException e) {
            e.printStackTrace();
            Log.e("beacon",  "cannot get data from " + url );
        } catch (ExecutionException e) {
            e.printStackTrace();
            Log.e("beacon",  "cannot get data from " + url );
        } catch (TimeoutException e) {
            e.printStackTrace();
            Log.e("beacon",  "cannot get data from " + url );
        }
        return response;
    }
}
