package com.example.root.tymp;

import android.Manifest;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothGatt;
import android.content.Context;
import android.content.SharedPreferences;
import android.media.AudioManager;
import android.preference.PreferenceManager;

import androidx.appcompat.app.AlertDialog;
import androidx.fragment.app.Fragment;

import com.google.android.material.snackbar.Snackbar;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.UUID;
import java.util.concurrent.ScheduledFuture;

public class Constants {
    public final static UUID service_uuid =
            UUID.fromString("6e400001-b5a3-f393-e0a9-e50e24dcca3e");
    public final static UUID write_charac_uuid =
            UUID.fromString("6e400002-b5a3-f393-e0a9-e50e24dcca3e");
    public final static UUID read_charac_uuid =
            UUID.fromString("6e400003-b5a3-f393-e0a9-e50e24dcca3e");
    public final static UUID img_info_char =
            UUID.fromString("6e400004-b5a3-f393-e0a9-e50e24dcca3e");

    public final static UUID cccd =
            UUID.fromString("00002902-0000-1000-8000-00805f9b34fb");

    public static boolean DEBUG = true;
    public static double BLUETOOTH_LATENCY=3000;
    public static boolean firstfoundseal =false;

    static String defaultTone = "226";
    static double volume = 1.0;
    static String minp = "-400";
    static String maxp = "200";
    static String speed = "200";
    static String sealCheck = "10";
    static String settlingTime = "0";
    static String microsteps = "8";
    static String numMeasurements = "1";

    static int SEAL_CHECK_THRESH = 7;
    static int SEAL_OCCLUSION_THRESH = 240;
    static int recordDurationInSeconds=180;
    static int motorFastSpeed=60;

    static boolean mic = true;
    static boolean speaker = true;
    static boolean moveToInit = true;

    static String resetSteps = "50";
    //////////////////////////////////////////////
    static int SEAL_LOST_TIMEOUT = 5;
    static int turnInfo;

    static boolean ackReceived=false;
    static double MotorNoiseThreshold = 1e5;
    static boolean tympAborted;
    static Snackbar snackBar;
    static String snackBarMsg;
    public static double initPressure=-1;
    public static boolean dialogShown=false;
    public static AlertDialog dialog;
    static boolean TWO_DIVISIONS = false;
    static int movefac=5;
    static int pcounter=0;
    static double pinit = 0;
    static String notifType="snackbar";
    static String timerVal="";
    static long lastRefreshTs = 0;
    static long lastChirpTs = 0;
    static boolean SHOW_YEAR = false;
    static MeasureFragment MeasureFragment;
    static Fragment MeasureStateFragment;
    static ResultsFragment ResultsFragment;
    static HistoryFragment HistoryFragment;
    static SettingsFragment SettingsFragment;
    static Fragment CurrentFragment;
    static CountUpTimer timer;
    static boolean testInProgress=false;
    static String connectionStatus="red";
    static String measurementStatus="red";
    static String sealStatus="red";
    static String resetStatus="red";
    static boolean measureButtonStatus=false;
    static boolean stopButtonStatus=false;
    static ArrayList<PlotPair> pairs = null;
    static AudioSpeaker sp1;
    static OfflineRecorder rec1;
    static OfflineRecorder rec2;
    static int samplingFreq=24000;
    static int toneFreq;
    static ScheduledFuture f1;
    static StartupTask startupTask;
    static BluetoothLeService service;
    static BluetoothGatt gatt;
    static BluetoothAdapter blueAdapter;
    static boolean movingToInit=false;
    static String filename;
    static String fileNamePointer;
    static String[] perms = new String[]{
            Manifest.permission.WRITE_EXTERNAL_STORAGE, Manifest.permission.READ_EXTERNAL_STORAGE,
            Manifest.permission.RECORD_AUDIO, Manifest.permission.BLUETOOTH,
            Manifest.permission.BLUETOOTH_ADMIN, Manifest.permission.ACCESS_COARSE_LOCATION,
            Manifest.permission.ACCESS_FINE_LOCATION};
    static int fileSize;
    static boolean mMtuRequested = false;
    static int mBytesTotal;
    static int syncMethod = 2;
    static int pksTransferred;
    static int mBytesTransfered;
    static long mStartTimeImageTransfer;
    static int bottomspeaker = AudioManager.STREAM_MUSIC;
    static LinkedList<String> sealtimes;
    static double AckWaitTime = 1;
    static int initSleepTime = 1;
    static int endSleepTime = 1;
    static boolean wroteSealToFile = false;
    static String dirname = "data3";
    static ArrayList<String> files;
    static double omin = 0.1*Math.pow(2,24);
    static double omax = 0.9*Math.pow(2, 24);

    public static void init(Context context) {
        MeasureFragment = new MeasureFragment();
        HistoryFragment = new HistoryFragment();
        SettingsFragment = new SettingsFragment();
        MeasureStateFragment = MeasureFragment;

        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(context);
        Constants.moveToInit=prefs.getBoolean("movetoinit",Constants.moveToInit);
        Constants.speaker=prefs.getBoolean("speaker",Constants.moveToInit);
        Constants.mic=prefs.getBoolean("mic",Constants.moveToInit);

        Constants.toneFreq = Integer.parseInt(prefs.getString("tone",Constants.defaultTone));
        Constants.volume = Double.parseDouble(prefs.getString("vol",Constants.volume+""));
        Constants.minp = (prefs.getString("minpressure",Constants.minp));
        Constants.maxp = (prefs.getString("maxpressure",Constants.maxp));
        Constants.speed = (prefs.getString("pspeed",Constants.speed));
        Constants.sealCheck = (prefs.getString("sealcheck",Constants.sealCheck));
        Constants.resetSteps = (prefs.getString("resetSteps",Constants.resetSteps));
        Constants.settlingTime = (prefs.getString("settlingTime",Constants.settlingTime));
        Constants.microsteps = (prefs.getString("microsteps",Constants.microsteps));
        Constants.numMeasurements = (prefs.getString("measurements",Constants.numMeasurements));
        Constants.SEAL_CHECK_THRESH = (prefs.getInt("micthresh",Constants.SEAL_CHECK_THRESH));
        Constants.SEAL_OCCLUSION_THRESH = (prefs.getInt("occlusionthresh",Constants.SEAL_OCCLUSION_THRESH));
        Constants.recordDurationInSeconds = (prefs.getInt("recordDuration",Constants.recordDurationInSeconds));
    }
}
