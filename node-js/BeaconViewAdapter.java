package com.example.hduser.beacon;

import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import java.lang.reflect.Array;

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

        TextView restaurantName;
        TextView cuisine;

        public ViewHolderBeacon(View itemView) {
            super(itemView);
            restaurantName = (TextView) itemView.findViewById(R.id.restaurant_name);
            cuisine = (TextView) itemView.findViewById(R.id.cuisine);
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
                .inflate(R.layout.recycler_item, parent, false);
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
        holder.restaurantName.setText(mBeaconDataList.get(position).restaurant);
        holder.cuisine.setText(mBeaconDataList.get(position).cuisine);
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
        mBeaconDataList = dataList;
    }

}