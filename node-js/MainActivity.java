package com.example.hduser.beacon;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Log;

import com.estimote.sdk.SystemRequirementsChecker;

import java.util.List;

public class MainActivity extends AppCompatActivity{
    private RecyclerView mRecyclerView;
    private BeaconViewAdapter mAdapter;
    private RecyclerView.LayoutManager mLayoutManager;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        //SystemRequirementsChecker.checkWithDefaultDialogs(this);
        BeaconApplicationContext beaconCtxt = new BeaconApplicationContext();

        mRecyclerView = (RecyclerView) findViewById(R.id.my_recycler_view);

        // use this setting to improve performance if you know that changes
        // in content do not change the layout size of the RecyclerView
        mRecyclerView.setHasFixedSize(true);

        // use a linear layout manager
        mLayoutManager = new LinearLayoutManager(this);
        mRecyclerView.setLayoutManager(mLayoutManager);

        // specify an adapter (see also next example)
        //String[] myDataSet = {"vox", "vad", "milo"};
        mAdapter = new BeaconViewAdapter(null);
        mRecyclerView.setAdapter(mAdapter);

        BeaconCallback beaconCallback = new BeaconCallback(mAdapter);
        Log.e("create main activity", "registering callback");
        beaconCtxt.registerBeaconActivityCallback(beaconCallback);
    }

    @Override
    protected void onResume() {
        super.onResume();
        BeaconCallback beaconCallback = new BeaconCallback(mAdapter);
        Log.e("resume main activity", "registering callback");
        BeaconApplicationContext beaconCtxt = (BeaconApplicationContext) getApplicationContext();
        beaconCtxt.registerBeaconActivityCallback(beaconCallback);
        SystemRequirementsChecker.checkWithDefaultDialogs(this);
    }



    private class BeaconCallback implements BeaconActivityInterface{
        private BeaconViewAdapter mAdapter;
        BeaconCallback(BeaconViewAdapter adapter){
            mAdapter = adapter;
        }
        @Override
        public void beaconDataChanged(List<BeaconData> list) {
            mAdapter.setBeaconDataList(list);
            mAdapter.notifyDataSetChanged();
        }
    }
}
