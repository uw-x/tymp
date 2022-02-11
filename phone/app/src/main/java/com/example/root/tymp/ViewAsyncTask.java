package com.example.root.tymp;

import android.app.Activity;
import android.content.Context;
import android.os.AsyncTask;

public class ViewAsyncTask extends AsyncTask<Void, Void, Void> {
    String rep = "";
    private Context context;

    public ViewAsyncTask(Activity activity) {
        this.context = activity;
    }

    public void onPostExecute(Void result) {
        Constants.HistoryFragment.historyValue = rep;
        Constants.HistoryFragment.setText();
    }

    @Override
    protected Void doInBackground(Void... params) {

        RecordRepository repo = new RecordRepository(context);
        ((Activity) this.context).runOnUiThread(new Runnable() {
            @Override
            public void run() {
                for (Record record : repo.getAllRecords()) {
                    rep += record.toString(context) + "\n";
                }
            }
        });

        return null;
    }
}