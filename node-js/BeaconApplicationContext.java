package com.example.hduser.beacon;

import android.app.Application;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

import com.android.volley.Request;
import com.android.volley.RequestQueue;

import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;
import com.estimote.sdk.Beacon;
import com.estimote.sdk.BeaconManager;
import com.estimote.sdk.Region;

import org.json.JSONArray;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.UUID;


/**
 * Created by hduser on 8/4/17.
 */

public class BeaconApplicationContext extends Application {
    private BeaconManager beaconManager;
    private RequestQueue requestQueue;
    private ArrayList<BeaconData> beaconDataList = null;
    private ArrayList<BeaconActivityInterface> beaconCallbackList = null;

    public BeaconApplicationContext(){
        beaconDataList = new ArrayList<BeaconData>();
        beaconCallbackList = new ArrayList<BeaconActivityInterface>();
    }

    public ArrayList<BeaconData> getBeaconDataList(){
        return beaconDataList;
    }

    public ArrayList<BeaconActivityInterface> getBeaconCallbackList(){
        return beaconCallbackList;
    }
    @Override
    public void onCreate() {
        super.onCreate();
        Log.d("BeaconApplication", "on create called");

        requestQueue = Volley.newRequestQueue(this);
        beaconManager = new BeaconManager(getApplicationContext());


        beaconManager.setMonitoringListener(new BeaconManager.MonitoringListener() {
            @Override
            public void onEnteredRegion(Region region, List<Beacon> list) {

                String url = "http://192.168.0.10:8080/getjson";
                final List<Beacon> final_list = list;

                StringRequest stringRequest = new StringRequest(Request.Method.POST, url,
                        new Response.Listener<String>() {
                            @Override
                            public void onResponse(String response) {
                                // Display the first 500 characters of the response string.
                                //displayItem.setText("Response is: "+ response.substring(0,500));
                                Log.e("response json " , response.substring(0));
                                ArrayList<BeaconData> myBeaconData = new ArrayList<BeaconData>();
                                try {
                                    JSONObject jsonObj = null;
                                    JSONArray jsonarr = new JSONArray(response.substring(0));
                                    jsonObj = jsonarr.getJSONObject(0);
                                    beaconDataList.clear();
                                    for(int i = 0; i < jsonarr.length(); i++ ){
                                        BeaconData data = new BeaconData();
                                        data.uuid = final_list.get(0).getProximityUUID();
                                        data.minor = final_list.get(0).getMinor();
                                        data.major = final_list.get(0).getMajor();
                                        data.restaurant = jsonarr.getJSONObject(i).getString("name");
                                        data.cuisine = jsonarr.getJSONObject(i).getString("cuisine");
                                        myBeaconData.add(data);
                                    }

                                    BeaconApplicationContext ctxt = (BeaconApplicationContext) getApplicationContext();
                                    ctxt.invokeCallbacks(myBeaconData);

                                    Log.e("response json", jsonObj.getString("name") + "," + jsonObj.getString("cuisine") );
                                    showNotification(jsonObj.getString("name"), jsonObj.getString("cuisine") );
                                }catch(Exception e){
                                    e.printStackTrace();
                                }
                            }
                        }, new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        Log.e("response error ", "that did not work!");
                        //displayItem.setText("That didn't work! network error is " + error.networkResponse.statusCode);
                    }
                }){
                    @Override
                    protected Map<String,String> getParams(){
                        Map<String,String> params = new HashMap<String, String>();
                        params.put("restaurant_id","30075445");
                        return params;
                    }

                };
                requestQueue.add(stringRequest);

                /*JSONObject msgobj = VolleyRequestor.requestDataFromUrl(requestQueue, url);

                try {
                    if( msgobj != null )
                        Log.e("BeaconApplicationContext", "entered region called" + msgobj.getString("title") + msgobj.getString("msg"));
                }catch(Exception e){
                    e.printStackTrace();
                }*/



                /*
                showNotification(
                        "Your gate closes in 47 minutes.",
                        "Current security wait time is 15 minutes, "
                                + "and it's a 5 minute walk from security to the gate. "
                                + "Looks like you've got plenty of time!");
                try {
                    showNotification( msgobj.getString("title"), msgobj.getString("msg"));
                }catch(Exception e){
                    e.printStackTrace();
                }*/
            }
            @Override
            public void onExitedRegion(Region region) {
                // could add an "exit" notification too if you want (-:
            }
        });


        beaconManager.connect(new BeaconManager.ServiceReadyCallback() {
            @Override
            public void onServiceReady() {
                beaconManager.startMonitoring(new Region("monitored region",
                        UUID.fromString("B9407F30-F5F8-466E-AFF9-25556B57FE6D"), 50148, 61997));
            }
        });
    }

    public void showNotification(String title, String message) {
        Intent notifyIntent = new Intent(this, MainActivity.class);
        notifyIntent.setFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP);
        PendingIntent pendingIntent = PendingIntent.getActivities(this, 0,
                new Intent[]{notifyIntent}, PendingIntent.FLAG_UPDATE_CURRENT);
        Notification notification = new Notification.Builder(this)
                .setSmallIcon(android.R.drawable.ic_dialog_info)
                .setContentTitle(title)
                .setContentText(message)
                .setAutoCancel(true)
                .setContentIntent(pendingIntent)
                .build();

        notification.defaults |= Notification.DEFAULT_SOUND;
        NotificationManager notificationManager =
                (NotificationManager) getSystemService(Context.NOTIFICATION_SERVICE);
        notificationManager.notify(1, notification);
    }

    public void registerBeaconActivityCallback( BeaconActivityInterface beaconCallback ){
        beaconCallbackList.add(beaconCallback);
        Log.e("register beacon cb", "size of callback list = " + beaconCallbackList.size() + " ref = " + beaconCallback.toString());
    }

    public void invokeCallbacks( List<BeaconData> list){
        //invoke all callbacks to notify that data has changed
        Log.e("invoke callbacks", "size of callback list = " + beaconCallbackList.size() + " ref = " + beaconCallbackList.toString());
        for (BeaconActivityInterface callback : beaconCallbackList
                ) {
            callback.beaconDataChanged(list);
        }

    }

}
