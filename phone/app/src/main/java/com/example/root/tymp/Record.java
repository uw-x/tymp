package com.example.root.tymp;

import android.content.Context;

import java.util.Calendar;

import androidx.annotation.NonNull;
import androidx.room.ColumnInfo;
import androidx.room.Entity;
import androidx.room.PrimaryKey;

@Entity(tableName = "record_table")
public class Record {

    static Calendar calendar = Calendar.getInstance();

    @PrimaryKey
    @NonNull
    @ColumnInfo(name = "ts")
    private long ts;

    @NonNull
    @ColumnInfo(name = "pid")
    private int pid;

    @NonNull
    @ColumnInfo(name = "ear")
    private String ear;

    @NonNull
    @ColumnInfo(name = "dapa")
    private double dapa;

    @NonNull
    @ColumnInfo(name = "ml")
    private double ml;

    @NonNull
    @ColumnInfo(name = "vea")
    private double vea;

    public Record(@NonNull long ts, int pid, String ear, double dapa, double ml, double vea) {
        this.ts = ts;
        this.pid = pid;
        this.ear = ear;
        this.dapa = dapa;
        this.ml = ml;
        this.vea = vea;
    }

    public long getTs() {
        return this.ts;
    }

    public int getPid() {
        return this.pid;
    }

    public String getEar() {
        return this.ear;
    }

    public double getDapa() {
        return this.dapa;
    }

    public double getMl() {
        return this.ml;
    }

    public double getVea() {
        return this.vea;
    }

    public String toString(Context context) {

        calendar.setTimeInMillis(ts);
        String dd = "";

        String dateFormat;
        if (Constants.SHOW_YEAR) {
            dateFormat = "%02d/%02d/%4d,%02d:%02d";
        } else {
            dateFormat = "%02d/%02d,%02d:%02d";
        }

        int hour = calendar.get(Calendar.HOUR_OF_DAY);
        if (calendar.get(Calendar.HOUR_OF_DAY) > 12) {
            dateFormat += "pm";
            hour -= 12;
        } else if (calendar.get(Calendar.HOUR_OF_DAY) == 12) {
            dateFormat += "pm";
        } else if (calendar.get(Calendar.HOUR_OF_DAY) == 0) {
            dateFormat += "am";
            hour = 12;
        } else {
            dateFormat += "am";
        }

        if (Constants.SHOW_YEAR) {
            dd = String.format(dateFormat,
                    calendar.get(Calendar.MONTH)+1, calendar.get(Calendar.DAY_OF_MONTH),
                    calendar.get(Calendar.YEAR),
                    hour, calendar.get(Calendar.MINUTE));
        } else {
            dd = String.format(dateFormat, calendar.get(Calendar.MONTH)+1,
                    calendar.get(Calendar.DAY_OF_MONTH),
                    hour, calendar.get(Calendar.MINUTE));
        }

        String rep = String.format("%s,%d-%s,%d,%.2f,%.2f",dd,pid,ear,(int)dapa,ml,vea);
//        String resultFormat = "%s, %s, %2d%%";
//        String result = "";
//        if (finding) {
//            result = context.getString(R.string.pos_result);
//        } else {
//            result = context.getString(R.string.neg_result);
//        }
//
//        if (Constants.SHOW_FILENAME) {
//            rep = String.format(resultFormat, dd, Utils.filenameSplit(filename), (int) (index * 100));
//        } else {
//            rep = String.format(resultFormat, dd, result, (int) (index * 100));
//        }

        return rep;
    }
}
