package com.example.root.tymp;

import android.content.Context;
import android.os.AsyncTask;

public class InsertAsyncTask extends AsyncTask<Void, Void, Void> {
    private Context context;
    private long ts;
    private int pid;
    private String ear;
    private double dapa;
    private double ml;
    private double vea;

    public InsertAsyncTask(Context activity, long ts, int pid, String ear, double dapa, double ml, double vea) {
        this.context = activity;
        this.ts = ts;
        this.pid = pid;
        this.ear = ear;
        this.dapa = dapa;
        this.ml = ml;
        this.vea = vea;
    }

    public void onPostExecute(Void result) {
    }

    @Override
    protected Void doInBackground(Void... params) {

        RecordRepository repo = new RecordRepository(context);
        repo.insert(new Record(this.ts, this.pid, this.ear, this.dapa, this.ml, this.vea));

        return null;
    }
}
