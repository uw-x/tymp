package com.example.root.tymp;

import android.Manifest;
import android.app.Activity;
import android.app.AlertDialog;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattService;
import android.content.DialogInterface;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.util.Log;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.fragment.app.Fragment;

import com.google.android.material.bottomnavigation.BottomNavigationView;

import java.util.LinkedList;
import java.util.concurrent.ScheduledThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

public class  MainActivity extends AppCompatActivity {

    Activity activity;

    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        FileOperations.av = this;
        FileOperations.cxt = this;

        if (!getPackageManager().hasSystemFeature(PackageManager.FEATURE_BLUETOOTH_LE)) {
            Toast.makeText(this, "BLE not supported", Toast.LENGTH_SHORT).show();
            finish();
        }

        activity = this;
        if (Build.VERSION.SDK_INT >= 23) {
            if (this.checkSelfPermission(Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
                final AlertDialog.Builder builder = new AlertDialog.Builder(this);
                builder.setTitle("This app needs location access");
                builder.setMessage("Please grant location access so this app can detect peripherals.");
                builder.setPositiveButton(android.R.string.ok, null);
                builder.setOnDismissListener(new DialogInterface.OnDismissListener() {
                    @RequiresApi(api = Build.VERSION_CODES.M)
                    @Override
                    public void onDismiss(DialogInterface dialog) {
                        requestPermissions(new String[]{Manifest.permission.ACCESS_COARSE_LOCATION}, 1);
                    }
                });
                builder.show();
            }
            else {
                Log.e("asdf","perm granted");
            }
        }
        if (!Utils.hasPermissions(this)) {
            ActivityCompat.requestPermissions(this, Constants.perms, 1);
        }
        else {
            View v=findViewById(android.R.id.content).getRootView();
            Utils.bleInit(this,v);
            Log.e("asdf","perm granted2");
        }

        if (Build.VERSION.SDK_INT<=21) {
            Constants.notifType="toast";
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        Constants.init(this);
        initView();
        initBottomNav();
    }

    public void initView() {
        if (Constants.CurrentFragment != null) {
            Log.e("secret", "current fragment not null");

            Log.e("secret", "transitioning to current fragment");

            if (Constants.CurrentFragment.getClass().getName().toLowerCase().contains("measure")) {
                getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container, Constants.MeasureStateFragment).commit();
            } else {
                getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container, Constants.CurrentFragment).commit();
            }
        } else {
            getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container, Constants.MeasureFragment).commit();
        }
    }

    public void initBottomNav() {
        BottomNavigationView bottomNavigationView = findViewById(R.id.bottom_navigation);

        if (!Constants.DEBUG) {
            bottomNavigationView.getMenu().clear();
            ImageView view = findViewById(R.id.colorBar);
            if (view!=null) {
                view.setImageBitmap(null);
                view.destroyDrawingCache();
            }
        }

        if (bottomNavigationView!=null) {
            bottomNavigationView.setOnNavigationItemSelectedListener(new BottomNavigationView.OnNavigationItemSelectedListener() {
                @Override
                public boolean onNavigationItemSelected(@NonNull MenuItem item) {
                    Fragment selectedFragment = null;
                    switch (item.getItemId()) {
                        case R.id.action_measure:
                            selectedFragment = Constants.MeasureFragment;
                            break;
                        case R.id.action_settings:
                            selectedFragment = Constants.SettingsFragment;
                            break;
                        default:
                            selectedFragment = Constants.CurrentFragment;
                            break;
                    }
                    if (selectedFragment != null && !Constants.testInProgress) {
                        getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container, selectedFragment).commit();
                    }
                    return true;
                }
            });
        }
    }

    @Override // android recommended class to handle permissions
    public void onRequestPermissionsResult(int requestCode,
                                           String permissions[], int[] grantResults) {
        if (!Utils.hasPermissions(this)) {
            Utils.mkSnack(this, findViewById(R.id.layout), "Permissions must be enabled before measurement can take place. Please try again.");
            ActivityCompat.requestPermissions(this, Constants.perms, 1);
        }
        else {
            View v=findViewById(android.R.id.content).getRootView();
            Utils.bleInit(this,v);
        }
    }

    @Override
    public void onBackPressed() {
    }

    public void update(View view) {
        Log.e("ble","UPDATE");
        byte[] data = Utils.packageUpdateData(this);
        if (Constants.gatt != null) {
            BluetoothGattService mSVC = Constants.gatt.getService(Constants.service_uuid);
            if (mSVC!=null) {
                BluetoothGattCharacteristic charac = mSVC.getCharacteristic(Constants.write_charac_uuid);
                if (charac!=null) {
                    charac.setValue(data);
                    boolean b = Constants.gatt.writeCharacteristic(charac);
                }
            }
        }
    }

    public void startit(View v) {
        if (!Constants.testInProgress) {
            Constants.tympAborted = false;
            Constants.ackReceived = false;
            Utils.toggleBottomNav(this, false);
            counter = 0;
            Constants.pcounter = 0;

            ScheduledThreadPoolExecutor exec = new ScheduledThreadPoolExecutor(1);
            Constants.f1 = exec.scheduleWithFixedDelay(new MyTask2(this), 0, 1, TimeUnit.SECONDS);
        }
    }

    public void stopit(View v) {
        Log.e("ble","abort2");
        FileOperations.appendtofile("stopit-abort "+System.currentTimeMillis(), "sout" + Constants.filename + ".txt", Constants.dirname);
        Utils.abortMeasurement(this,"");
    }

    int counter=0;
    class MyTask2 implements Runnable {

        Activity cxt;

        public MyTask2(Activity cxt) {
            this.cxt = cxt;
        }

        @Override
        public void run() {
            SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(cxt);
            int mm=Integer.parseInt(prefs.getString("measurements",Constants.numMeasurements));
            if (!Constants.testInProgress&&counter<mm) {
                Constants.stopButtonStatus=true;

                cxt.runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        Button sb = (Button) findViewById(R.id.stopButton);
                        if (sb != null) {
                            sb.setEnabled(true);
                        }

                        if (Constants.snackBar != null) {
                            Constants.snackBar.dismiss();
                        }
                    }});

                Constants.testInProgress = true;
                Log.e("ble", "Startit "+(Constants.gatt==null));

                update(null);

                if (Constants.gatt == null) {
                    Log.e("logme","we have a problem");
                    if (Constants.notifType.equals("toast")) {
                        Toast.makeText(cxt, "Bluetooth not connected", Toast.LENGTH_SHORT).show();
                    }
                    else {
                        Utils.mkSnack(cxt, findViewById(R.id.layout), "Bluetooth not connected");
                    }
                } else {
                    Constants.sealtimes = new LinkedList<String>();
                    View v=findViewById(android.R.id.content).getRootView();
                    Constants.startupTask = new StartupTask(cxt,v);
                    Constants.startupTask.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
                    Log.e("ble","TOGGLE FALSE main activity");
                    Utils.toggleBottomNav(cxt,false);
                    cxt.runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            Constants.measurementStatus = "green";
                            ImageView vv = (ImageView) findViewById(R.id.measuringView);
                            if (vv!=null) {
                                vv.setImageDrawable(getDrawable(R.drawable.circle_green));
                            }
                        }
                    });
                }
                Log.e("results","measurement "+counter+" started");
                counter+=1;
            }
        }
    }

    public void updateStart(View v) {
        if (Constants.gatt != null) {
            BluetoothGattService mSVC = Constants.gatt.getService(Constants.service_uuid);
            if (mSVC!=null) {
                Log.e("ble", (mSVC.getUuid().toString()) + "");
                BluetoothGattCharacteristic charac = mSVC.getCharacteristic(Constants.write_charac_uuid);
                if (charac!=null) {
                    Log.e("ble", (charac.getUuid().toString()) + "");
                    charac.setValue(new byte[]{6});
                    boolean b = Constants.gatt.writeCharacteristic(charac);
                    Log.e("ble", (charac.getProperties() == BluetoothGattCharacteristic.PROPERTY_WRITE) + "," + (b) + "");

                }
            }
        }
    }

    public void resetPump(View v) {
        if (Constants.gatt != null) {
            BluetoothGattService mSVC = Constants.gatt.getService(Constants.service_uuid);
            if (mSVC!=null) {
                Log.e("ble", (mSVC.getUuid().toString()) + "");
                BluetoothGattCharacteristic charac = mSVC.getCharacteristic(Constants.write_charac_uuid);
                if (charac!=null) {
                    Log.e("ble", (charac.getUuid().toString()) + "");

                    SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(this);

                    int mag = Integer.parseInt(prefs.getString("resetSteps", Constants.resetSteps));
                    boolean checked = prefs.getBoolean("forwarddir", true);
                    int direc = checked ? 0 : 1;
                    int microsteps = Integer.parseInt(prefs.getString("microsteps",Constants.microsteps));

                    charac.setValue(new byte[]{4, (byte) mag, (byte) direc, (byte) microsteps});
                    boolean b = Constants.gatt.writeCharacteristic(charac);
                    Log.e("ble", (charac.getProperties() == BluetoothGattCharacteristic.PROPERTY_WRITE) + "," + (b) + "");

                    Button rb = (Button) findViewById(R.id.resetButton);
                    if (rb != null) {
                        rb.setEnabled(false);
                    }
                    try {
                        Thread.sleep(3000);
                    } catch (Exception e) {
                        Log.e("ble", e.getMessage());
                    }
                    if (rb != null) {
                        rb.setEnabled(true);
                    }
                }
            }
        }
    }

    public native double[] fftnativetrim(double data[], int N, int outcropstart, int outcropend);
    public native double[][] fftcomplexoutnative(double data[], int N);
    public native double[] ifftnative(double data[][]);
    public native void conjnative(double[][] data);
    public native double[][] timesnative(double[][] data1,double[][] data2);
}
