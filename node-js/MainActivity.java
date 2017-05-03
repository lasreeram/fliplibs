package com.example.hduser.beacon;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import com.estimote.sdk.SystemRequirementsChecker;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        //SystemRequirementsChecker.checkWithDefaultDialogs(this);
        MyApplication beaAppl = new MyApplication();
    }

    @Override
    protected void onResume() {
        super.onResume();

        SystemRequirementsChecker.checkWithDefaultDialogs(this);
    }

}
