package com.example.root.tymp;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattService;
import android.bluetooth.BluetoothManager;
import android.bluetooth.BluetoothProfile;
import android.bluetooth.le.ScanCallback;
import android.bluetooth.le.ScanResult;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.media.MediaRecorder;
import android.preference.PreferenceManager;
import android.provider.MediaStore;
import android.text.Html;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.RadioGroup;
import android.widget.TextView;
import android.widget.Toast;

import androidx.core.app.ActivityCompat;

import com.google.android.material.bottomnavigation.BottomNavigationView;
import com.google.android.material.snackbar.Snackbar;
import com.jjoe64.graphview.GraphView;
import com.jjoe64.graphview.GridLabelRenderer;
import com.jjoe64.graphview.series.DataPoint;
import com.jjoe64.graphview.series.LineGraphSeries;
import com.jjoe64.graphview.series.PointsGraphSeries;

import java.io.PrintWriter;
import java.io.StringWriter;

public class Utils {

    public static String e2s(Exception e) {
        StringWriter sw = new StringWriter();
        e.printStackTrace(new PrintWriter(sw));
        String exceptionAsString = sw.toString();
        return exceptionAsString;
    }

    public static void screenshot(Activity av) {
        View v = (av.getWindow().getDecorView().getRootView());
        v.setDrawingCacheEnabled(true);
        v.buildDrawingCache(true);

        Bitmap bb = v.getDrawingCache();
        Bitmap bitmap = Bitmap.createBitmap(bb);
        v.setDrawingCacheEnabled(false);

        MediaStore.Images.Media.insertImage(
                av.getContentResolver(), bitmap, Constants.filename, "");  // Saves the image.

        Toast.makeText(av, "Screenshot captured", Toast.LENGTH_LONG).show();
    }

    public static void graphme(GraphView graph, LineGraphSeries<DataPoint>series, PointsGraphSeries<DataPoint>series2, double xbegin, double xend, double ybegin, double yend) {
        graph.removeAllSeries();

        series.setColor(Color.BLUE);
        graph.addSeries(series);

        ViewGroup.LayoutParams p=graph.getLayoutParams();
        p.height=750;
        p.width= 1000;
        graph.setLayoutParams(p);

        Log.e("ble",ybegin+","+yend+","+xbegin+","+xend);
        graph.getViewport().setYAxisBoundsManual(true);
        graph.getViewport().setMinY(ybegin);
        graph.getViewport().setMaxY(yend);
        graph.getViewport().setXAxisBoundsManual(true);
        graph.getViewport().setMinX(xbegin);
        graph.getViewport().setMaxX(xend);

        GridLabelRenderer r = graph.getGridLabelRenderer();
        r.setVerticalLabelsVisible(false);
        graph.getGridLabelRenderer().setHorizontalLabelsVisible(true);
        graph.getGridLabelRenderer().setVerticalLabelsVisible(true);
        graph.invalidate();
    }

    public static byte[] packageUpdateData(Activity av) {
        Log.e("ble","packageupdatedata");
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(av);

        int minp=Integer.parseInt(prefs.getString("minpressure",Constants.minp));
        int maxp = Integer.parseInt(prefs.getString("maxpressure",Constants.maxp));
        byte[] minPressure = toByteArray(-minp);
        byte[] maxPressure = toByteArray(maxp);
        Log.e("ble","SENDING UPDATES "+minp+","+maxp);
        byte speed = convert(prefs.getString("pspeed",Constants.speed));
        byte sealcheck = Byte.parseByte(prefs.getString("sealcheck",Constants.sealCheck));
        boolean downsweep = prefs.getBoolean("downsweep",true);
        int settlingTime = Integer.parseInt(prefs.getString("settlingTime",Constants.settlingTime));
        int microsteps = Integer.parseInt(prefs.getString("microsteps",Constants.microsteps));
        int movetoinit = 0;
        if (prefs.getBoolean("movetoinit",Constants.moveToInit)) {
            movetoinit = 1;
        }
        int motorFastSpeed = convert(prefs.getInt("motorFastSpeed",Constants.motorFastSpeed)+"");

        Log.e("ble","SWEEP DIR "+downsweep);
        RadioGroup rg = av.findViewById(R.id.radioGroup);
        int sweep = downsweep?0:1;
        return new byte[]{1,
                maxPressure[0],maxPressure[1],maxPressure[2],
                minPressure[0],minPressure[1],minPressure[2],
                speed,sealcheck,(byte)sweep,(byte)settlingTime,(byte)microsteps,(byte)movetoinit,
                (byte)motorFastSpeed};
    }

    public static byte convert(String s) {
        int i = Integer.parseInt(s);
        if (i > 127) {
            int ii = -((i ^ 0xff)+1);
            return (byte)ii;
        }
        byte bb = (byte)Integer.parseInt(s);
        return bb;
    }

    public static byte[] toByteArray(int value) {
        long omin = (long)(0.1*Math.pow(2,24));
        long omax = (long)(0.9*Math.pow(2, 24));
        long target = value;
        target *= 10;
        double temp = (omax-omin) / (25*6895);
        target *= temp;
        String ss = "0"+Long.toHexString(target);

        byte b1 = hexconvert(ss.substring(0,2));
        byte b2 = hexconvert(ss.substring(2,4));
        byte b3 = hexconvert(ss.substring(4,6));
        return new byte[]{b1,b2,b3};
    }

    public static byte hexconvert(String s) {
        int i = Integer.parseInt(s,16);
        if (i > 127) {
            return (byte)-((i ^ 0xff)+1);
        }
//        return (byte)Integer.parseInt(s);
        try {
            return (byte)Integer.parseInt(s);
        }
        catch(Exception e) {
            return (byte)i;
        }
    }

    public static void mkSnack(Activity av, View v, String msg) {
        if (Constants.notifType.equals("toast")) {
            Utils.mkToast(av,msg);
        }
        else {
            if (Constants.snackBar != null) {
                Constants.snackBar.dismiss();
            }
            if (msg.contains("Seal is broken")) {
                Constants.snackBar = Snackbar.make(v, msg, Snackbar.LENGTH_INDEFINITE);
                Constants.snackBar.getView().setBackgroundColor(av.getColor(R.color.colorDarkRed));
            } else if (msg.contains("Seal is found")) {
                Constants.snackBar = Snackbar.make(v, msg, Snackbar.LENGTH_INDEFINITE);
                Constants.snackBar.getView().setBackgroundColor(av.getColor(R.color.colorDarkGreen));
            } else if (msg.contains("Measurement done") || msg.contains("Reset complete")) {
                Constants.snackBar = Snackbar.make(v, Html.fromHtml("<b>" + msg + "</b>", 0), Snackbar.LENGTH_INDEFINITE);
                Constants.snackBar.getView().setBackgroundColor(av.getColor(R.color.colorDarkGreen));
            } else if (msg.contains("Measurement aborted.")) {
                Constants.snackBar = Snackbar.make(v, Html.fromHtml("<b>" + msg + "</b>", 0), Snackbar.LENGTH_INDEFINITE);
                Constants.snackBar.getView().setBackgroundColor(av.getColor(R.color.colorDarkRed));
            } else if (msg.contains("occluded")) {
                Constants.snackBar = Snackbar.make(v, Html.fromHtml(msg, 0), Snackbar.LENGTH_INDEFINITE);
                Constants.snackBar.getView().setBackgroundColor(av.getColor(R.color.colorDarkRed));
            } else {
                Constants.snackBar = Snackbar.make(v, msg, Snackbar.LENGTH_LONG);
            }

            Constants.snackBar.setAction("Dismiss", new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    Constants.snackBar.dismiss();
                }
            });

            View snackbarView = Constants.snackBar.getView();
            TextView textView = (TextView) snackbarView.findViewById(com.google.android.material.R.id.snackbar_text);
            textView.setMaxLines(5);

            Constants.snackBarMsg = msg;
            Constants.snackBar.show();
        }
    }

    public static void mkToast(Activity av,String msg) {
        Toast.makeText(av,msg,Toast.LENGTH_LONG).show();
    }

    public static void abortMeasurement(Activity av, String msg) {
        Constants.tympAborted = true;
        if (Constants.gatt != null) {
            BluetoothGattService mSVC = Constants.gatt.getService(Constants.service_uuid);
            if (mSVC != null) {
                BluetoothGattCharacteristic charac = mSVC.getCharacteristic(Constants.write_charac_uuid);

                if (charac != null) {
                    charac.setValue(new byte[]{3});
                    boolean b = Constants.gatt.writeCharacteristic(charac);
                }
            }
        }

        av.runOnUiThread(new Runnable() {
            public void run() {
                Constants.stopButtonStatus = false;
                Button sb = (Button) av.findViewById(R.id.stopButton);
                if (sb != null) {
                    sb.setEnabled(false);
                }

                Utils.toggleBottomNav(av, true);

                Constants.measurementStatus = "red";
                Constants.testInProgress = false;

                ImageView vv = (ImageView) av.findViewById(R.id.measuringView);
                if (vv != null) {
                    vv.setImageDrawable(av.getDrawable(R.drawable.circle_red));
                }

                Constants.sealStatus = "red";
                ImageView vv2 = (ImageView) av.findViewById(R.id.sealView);
                if (vv2 != null) {
                    vv2.setImageDrawable(av.getDrawable(R.drawable.circle_red));
                }

                if (Constants.moveToInit) {
                    Constants.resetStatus = "green";
                    ImageView vv4 = (ImageView) av.findViewById(R.id.resetView);
                    if (vv4 != null) {
                        vv4.setImageDrawable(av.getDrawable(R.drawable.circle_green));
                    }
                } else {
                    Constants.resetStatus = "red";
                    ImageView vv4 = (ImageView) av.findViewById(R.id.resetView);
                    if (vv4 != null) {
                        vv4.setImageDrawable(av.getDrawable(R.drawable.circle_red));
                    }
                }

                if (!Constants.moveToInit) {
                    Constants.measureButtonStatus = true;
                    Button mb = (Button) av.findViewById(R.id.measureButton);
                    if (mb != null) {
                        mb.setEnabled(true);
                    }
                } else {
                    Constants.measureButtonStatus = false;
                    Button mb = (Button) av.findViewById(R.id.measureButton);
                    if (mb != null) {
                        mb.setEnabled(false);
                    }
                }

                Log.e("test","TEST3");
                if (Constants.snackBar!=null) {
                    Log.e("test","TEST3- dismissing "+Constants.snackBarMsg);
                    Constants.snackBar.dismiss();
                }
                if (Constants.moveToInit) {
                    if (Constants.notifType.equals("toast")) {
                        Utils.mkToast((MainActivity)av,msg + "Measurement aborted. Remove tip from ear for device to reset. This can take up to " + Constants.SEAL_LOST_TIMEOUT + " seconds. Please wait.");
                    }
                    else {
                        Utils.mkSnack(((MainActivity) av), av.findViewById(R.id.layout),
                                msg + "Measurement aborted. Remove tip from ear for device to reset. This can take up to " + Constants.SEAL_LOST_TIMEOUT + " seconds. Please wait.");
                    }
                }
                else {
                    if (Constants.notifType.equals("toast")) {
                        Utils.mkToast((MainActivity)av,msg+"Measurement aborted.");
                    }
                    else {
                        Utils.mkSnack(((MainActivity) av), av.findViewById(R.id.layout),
                                msg + "Measurement aborted.");
                    }
                }
            }});

        if (!Constants.moveToInit) {
            if (Constants.sp1 != null) {
                Constants.sp1.stopit();
                Constants.sp1 = null;
            }
        }

        if (Constants.rec1 != null) {
            Constants.rec1.stopit();
            Constants.rec1 = null;
        }

        if (Constants.timer != null) {
            Constants.timer.cancel();
            Constants.timer = null;
        }
        if (Constants.startupTask != null) {
            Constants.startupTask.cancel(true);
            Constants.startupTask = null;
        }

        if (Constants.moveToInit) {
            Log.e("ble","unsealing");
            if (Constants.sp1 == null) {
                short[] tone = SignalGenerator.continuoussinepulse(Constants.samplingFreq, Constants.toneFreq, Constants.samplingFreq, 0, 1);
                Constants.sp1 = new AudioSpeaker(av, tone, Constants.samplingFreq, Constants.bottomspeaker);
                if (Constants.speaker) {
                    Constants.sp1.play(Constants.volume, (int) -1);
                }
            }

            if (Constants.rec2 == null) {
                Constants.rec2 = new OfflineRecorder(Constants.samplingFreq, (int) 0, MediaRecorder.AudioSource.DEFAULT, Constants.dirname, false, av, false, 2);
                Constants.rec2.filename2 = "miccheck-2"+Constants.filename+ ".txt";
                Constants.rec2.start();
            }
        }
    }

    public static void toggleBottomNav(Activity av, boolean toggle) {
        av.runOnUiThread(new Runnable() {
            public void run() {
                BottomNavigationView bottomNavigationView = av.findViewById(R.id.bottom_navigation);
                for (int i = 1; i < bottomNavigationView.getMenu().size(); i++) {
                    bottomNavigationView.getMenu().getItem(i).setEnabled(toggle);
                    bottomNavigationView.getMenu().getItem(i).setVisible(toggle);
                }
            }
        });
    }

    public static void bleInit(Activity av,View v) {
        Log.e("ble","bleinit");
        BluetoothManager bluetoothManager =
                (BluetoothManager) av.getSystemService(Context.BLUETOOTH_SERVICE);
        Constants.blueAdapter = bluetoothManager.getAdapter();
        if (Constants.blueAdapter == null || !Constants.blueAdapter.isEnabled()) {
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            av.startActivityForResult(enableBtIntent, 0);
        }
        Constants.service = new BluetoothLeService();
        Constants.service.context = av;

        MyTask mytask = new MyTask(av,av,v);
        mytask.run();
    }

    static BluetoothDevice device;
    static class MyTask implements Runnable {
        Activity av;
        Context cxt;
        View v;
        public MyTask(Activity av, Context cxt, View v) {
            this.av = av;
            this.cxt = cxt;
            this.v = v;
        }

        @Override
        public void run() {
            Log.e("ble","mytask-run "+(Constants.gatt==null));

            BluetoothManager bluetoothManager = (BluetoothManager) cxt.getSystemService(Context.BLUETOOTH_SERVICE);

            if (Constants.gatt == null ||
                    (bluetoothManager != null && device != null &&
                            bluetoothManager.getConnectionState(device, BluetoothProfile.GATT) == BluetoothProfile.STATE_DISCONNECTED)) {

                Log.e("ble","start scan");
                BluetoothAdapter mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
                if (mBluetoothAdapter == null) {
                    Log.e("ble","no support");
                } else if (!mBluetoothAdapter.isEnabled()) {
                    Log.e("ble","not enabled");
                } else {
                    Log.e("ble","enabled");
                }

                s_av = av;
                s_v = v;
                Constants.blueAdapter.getBluetoothLeScanner().flushPendingScanResults(mLeScanCallback);
                Constants.blueAdapter.getBluetoothLeScanner().stopScan(mLeScanCallback);
                Constants.blueAdapter.getBluetoothLeScanner().startScan(mLeScanCallback);
            }

            int state = -1;
            if (Constants.gatt!=null) {
                state = bluetoothManager.getConnectionState(device, BluetoothProfile.GATT);
                Log.e("ble","state "+state);
            }
            else {
                Log.e("ble","gatt is null");
            }

            if (state == BluetoothProfile.STATE_CONNECTED && !Constants.testInProgress) {
                av.runOnUiThread(new Runnable() {
                    public void run() {
                        Constants.connectionStatus="green";
                        Constants.measureButtonStatus=true;

                        ImageView vv = (ImageView) v.findViewById(R.id.connectedView);
                        if (vv!=null) {
                            vv.setImageDrawable(cxt.getDrawable(R.drawable.circle_green));
                        }
                        Button mb = (Button) v.findViewById(R.id.measureButton);
                        if (mb!=null) {
                            mb.setEnabled(true);
                        }
                    }});
            }
            else if (state == BluetoothProfile.STATE_DISCONNECTED) {
                Log.e("ble","test2");

                av.runOnUiThread(new Runnable() {
                    public void run() {
                        Constants.testInProgress = false;
                        Constants.connectionStatus="yellow";
                        Constants.resetStatus="red";
                        Constants.measureButtonStatus=false;

                        ImageView vv = (ImageView) v.findViewById(R.id.connectedView);
                        if (vv!=null) {
                            vv.setImageDrawable(cxt.getDrawable(R.drawable.circle_yellow));
                        }
                        ImageView vv2 = (ImageView) v.findViewById(R.id.resetView);
                        if (vv2!=null) {
                            vv2.setImageDrawable(cxt.getDrawable(R.drawable.circle_red));
                        }
                        Button mb = (Button) v.findViewById(R.id.measureButton);
                        if (mb!=null) {
                            mb.setEnabled(false);
                        }
                    }});

                s_av = av;
                s_v = v;
                Constants.blueAdapter.getBluetoothLeScanner().flushPendingScanResults(mLeScanCallback);
                Constants.blueAdapter.getBluetoothLeScanner().stopScan(mLeScanCallback);
                Log.e("ble","start scan2");
                Constants.blueAdapter.getBluetoothLeScanner().startScan(mLeScanCallback);
            }
            else {
                Log.e("ble","test3 "+state);
            }
        }
    }

    static Activity s_av;
    static View s_v;
    static ScanCallback mLeScanCallback =
            new ScanCallback() {
                @Override
                public void onScanResult(int callbackType, final ScanResult result) {
                    super.onScanResult(callbackType, result);
                    Log.e("ble","myresult "+result.getDevice());
                    if (result.getDevice().getName()!=null) {
                        Log.e("ble","onscanresult "+result.getDevice().getName());
                        if (result.getDevice().getName().equals("Tympanometer")) {
                            if (Constants.gatt == null) {
                                device = result.getDevice();
                                Log.e("ble", "got device " + device.getName().toString());
                                Log.e("ble", "got device address " + device.getAddress());
                                Constants.blueAdapter.getBluetoothLeScanner().stopScan(this);
                                Constants.gatt = device.connectGatt(s_av, true, Constants.service.gattCallback);
                            }

                            ((MainActivity)s_av).runOnUiThread(new Runnable() {
                                public void run() {
                                    if (Constants.notifType.equals("toast")) {
                                        Toast.makeText(s_av, "Connecting to server", Toast.LENGTH_SHORT).show();
                                    }
                                    else {
                                        Utils.mkSnack(s_av, s_v.findViewById(R.id.layout), "Connecting to server");
                                    }
                                }
                            });
                        }
                    }
                }

                @Override
                public void onScanFailed(int errorCode) {
                    super.onScanFailed(errorCode);
                    Log.e("ble", "error onscanfailed "+errorCode);
                }
            };

    public static boolean hasPermissions(Context context) {
        Log.e("results", "check for permission");
        if (context != null && Constants.perms != null) {
            for (String permission : Constants.perms) {
                if (ActivityCompat.checkSelfPermission(context, permission) != PackageManager.PERMISSION_GRANTED) {
                    Log.e("results", "return false on " + permission);
                    return false;
                }
            }
        }
        return true;
    }
}
