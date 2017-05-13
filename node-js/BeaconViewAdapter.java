package com.example.hduser.beacon;

import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import java.lang.reflect.Array;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by hduser on 4/5/17.
 */

public class BeaconViewAdapter extends RecyclerView.Adapter<BeaconViewAdapter.ViewHolderBeacon> {
    private List<BeaconData> mBeaconDataList;

    // Provide a reference to the views for each data item
    // Complex data items may need more than one view per item, and
    // you provide access to all the views for a data item in a view holder
    static class ViewHolderBeacon extends RecyclerView.ViewHolder {

        TextView title;
        TextView text;

        public ViewHolderBeacon(View itemView) {
            super(itemView);
            title = (TextView) itemView.findViewById(R.id.title);
            text = (TextView) itemView.findViewById(R.id.text);
        }
    }

    // Provide a suitable constructor (depends on the kind of dataset)
    public BeaconViewAdapter(List<BeaconData> dataList ) {
        mBeaconDataList = dataList;
    }

    // Create new views (invoked by the layout manager)
    @Override
    public BeaconViewAdapter.ViewHolderBeacon onCreateViewHolder(ViewGroup parent,
                                                                 int viewType) {
        // create a new view
        View v = LayoutInflater.from(parent.getContext())
                .inflate(R.layout.recycler_item_2, parent, false);
        // set the view's size, margins, paddings and layout parameters
        ViewHolderBeacon vh = new ViewHolderBeacon(v);
        return vh;
    }

    // Replace the contents of a view (invoked by the layout manager)
    @Override
    public void onBindViewHolder(ViewHolderBeacon holder, int position) {
        // - get element from your dataset at this position
        // - replace the contents of the view with that element
        //holder.mTextView.setText(mBeaconDataList[position])
        Log.e("Beacon Adapter", "onBindVieHolder called with position = " + position );
        holder.title.setText(mBeaconDataList.get(position).restaurant);
        holder.text.setText(mBeaconDataList.get(position).cuisine);
    }

    // Return the size of your dataset (invoked by the layout manager)
    @Override
    public int getItemCount() {
        if( mBeaconDataList == null ) {
            Log.e("Beacon adapter", "return item count 0");
            return 0;
        }
        Log.e("Beacon adapter", "return item count 0" + mBeaconDataList.size());
        return mBeaconDataList.size();
    }

    public void setBeaconDataList(List<BeaconData> dataList ){
        Log.e("beacon adapter", "set data list called count = " + dataList.size());

        if( mBeaconDataList == null ) {
            mBeaconDataList = new ArrayList<BeaconData>();

            //for(int i = 0; i < 5; i++ ) {
            //    BeaconData dat = new BeaconData();
            //    dat.cuisine = "cusine " + i;
            //    dat.restaurant = "restaurant " + i;
            //    mBeaconDataList.add(dat);
            //}
            for (int i = 0; i < dataList.size(); i++) {
                mBeaconDataList.add(dataList.get(i));
            }
        }else{
            mBeaconDataList.clear();
            for (int i = 0; i < dataList.size(); i++) {
                mBeaconDataList.add(dataList.get(i));
            }
        }


    }

}