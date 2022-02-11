package com.example.root.tymp;

import android.app.Activity;
import android.app.Service;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattDescriptor;
import android.bluetooth.BluetoothGattService;
import android.bluetooth.BluetoothProfile;
import android.content.Intent;
import android.media.MediaRecorder;
import android.os.Build;
import android.os.Handler;
import android.os.IBinder;
import android.util.Log;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.Nullable;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.Arrays;
import java.util.LinkedList;

// A service that interacts with the BLE device via the Android BLE API.
public class BluetoothLeService extends Service {
    private final static String TAG = "ble";
    Activity context;
    boolean readble=false;
    LinkedList<String>pdata=new LinkedList<String>();
    boolean read=false;

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    public void finishMeasurement() {
        Log.e("test","finish measurement "+Constants.tympAborted);

        ((MainActivity)context).runOnUiThread(new Runnable() {
            public void run() {

                if (Constants.moveToInit) {
                    Constants.resetStatus = "red";
                    ImageView vv3 = (ImageView) context.findViewById(R.id.resetView);
                    if (vv3 != null) {
                        vv3.setImageDrawable(context.getDrawable(R.drawable.circle_red));
                    }
                    if (Constants.snackBar!=null) {
                        Constants.snackBar.dismiss();
                    }

                    Utils.mkSnack(((MainActivity) context), context.findViewById(R.id.layout),
                            "Reset complete. Ready to measure.");
                    Constants.movingToInit = false;
                }

                Constants.measureButtonStatus = true;
                Button mb = (Button)context.findViewById(R.id.measureButton);
                if (mb!=null) {
                    mb.setEnabled(Constants.measureButtonStatus);
                }

                Utils.toggleBottomNav(context,true);
            }});
//        }

        if (Constants.sp1 != null) {
            Constants.sp1.stopit();
            Constants.sp1 = null;
        }
    }

    Runnable runnable;
    final Handler ha=new Handler();
    public BluetoothGattCallback gattCallback =
            new BluetoothGattCallback() {
                @Override
                public void onConnectionStateChange(BluetoothGatt gatt, int status,
                                                    int newState) {
                    String intentAction;
                    if (newState == BluetoothProfile.STATE_CONNECTED) {
                        Constants.mMtuRequested = false;
                        Constants.connectionStatus="green";
                        Constants.measureButtonStatus=false;
                        Constants.stopButtonStatus=false;
                        Log.e(TAG, "Connected to GATT server.");
                        Log.e(TAG, "ADDRESS "+gatt.getDevice().getAddress());
                        try {
                            boolean disc = gatt.discoverServices();
                            Log.e(TAG, "Attempting to start service discovery:" +
                                    disc);
                        }
                        catch(Exception e){
                            Log.e("ble",e.getMessage());
                        }
                        ((MainActivity)context).runOnUiThread(new Runnable() {
                            public void run() {
                                if (Constants.notifType.equals("toast")) {
                                    Toast.makeText(((MainActivity) context), "Bluetooth connected", Toast.LENGTH_SHORT).show();
                                }
                                else {
                                    Utils.mkSnack(((MainActivity) context), context.findViewById(R.id.layout), "Bluetooth connected");
                                }
                            }});
                    } else if (newState == BluetoothProfile.STATE_DISCONNECTED) {
                        Log.e(TAG, "Disconnected from GATT server.");
                        Constants.tympAborted = true;
                        ((MainActivity)context).runOnUiThread(new Runnable() {
                            public void run() {
                                Constants.testInProgress = false;
                                if (Constants.timer!=null) {
                                    Constants.timer.cancel();
                                }

                                if (Constants.notifType.equals("toast")) {
                                    Toast.makeText(((MainActivity) context), "Disconnected", Toast.LENGTH_SHORT).show();
                                }
                                else {
                                    Utils.mkSnack(((MainActivity) context), context.findViewById(R.id.layout), "Disconnected");
                                }

                                if (Constants.f1!=null) {
                                    Constants.f1.cancel(true);
                                }

                                Constants.connectionStatus="yellow";
                                ImageView vv = (ImageView) context.findViewById(R.id.connectedView);
                                if (vv!=null) {
                                    vv.setImageDrawable(context.getDrawable(R.drawable.circle_yellow));
                                }

                                Constants.sealStatus="red";
                                ImageView vv2 = (ImageView) context.findViewById(R.id.sealView);
                                if (vv2!=null) {
                                    vv2.setImageDrawable(context.getDrawable(R.drawable.circle_red));
                                }

                                Constants.measurementStatus="red";
                                ImageView vv3 = (ImageView) context.findViewById(R.id.measuringView);
                                if (vv3!=null) {
                                    vv3.setImageDrawable(context.getDrawable(R.drawable.circle_red));
                                }

                                Constants.resetStatus="red";
                                ImageView vv4 = (ImageView) context.findViewById(R.id.resetView);
                                if (vv4!=null) {
                                    vv4.setImageDrawable(context.getDrawable(R.drawable.circle_red));
                                }

                                Constants.measureButtonStatus=false;
                                Button mb = (Button) context.findViewById(R.id.measureButton);
                                if (mb!=null) {
                                    mb.setEnabled(false);
                                }

                                Constants.stopButtonStatus=false;
                                Button sb = (Button) context.findViewById(R.id.stopButton);
                                if (sb!=null) {
                                    sb.setEnabled(false);
                                }

                                if (Constants.sp1 != null) {
                                    Constants.sp1.stopit();
                                    Constants.sp1 = null;
                                }
                                if (Constants.rec1 != null) {
                                    Constants.rec1.stopit();
                                    Constants.rec1 = null;
                                }
                                if (Constants.rec2 != null) {
                                    Constants.rec2.stopit();
                                    Constants.rec2 = null;
                                }

                                Utils.toggleBottomNav(context,true);

                                FileOperations.appendtofile("bluetoothdisconnect-abort "+System.currentTimeMillis(), "sout" + Constants.filename + ".txt", Constants.dirname);
                                Utils.mkSnack(((MainActivity) context), context.findViewById(R.id.layout),
                                        "Bluetooth disconnected. Measurement aborted.");
                            }});

                    }else if (newState == BluetoothProfile.STATE_DISCONNECTING) {
                        Log.e(TAG, "Disconnecting from GATT server.");
                        ((MainActivity)context).runOnUiThread(new Runnable() {
                            public void run() {
                                if (Constants.notifType.equals("toast")) {
                                    Toast.makeText(((MainActivity) context), "Disconnecting", Toast.LENGTH_SHORT).show();
                                }
                                else {
                                    Utils.mkSnack(((MainActivity) context), context.findViewById(R.id.layout), "Disconnecting");
                                }
                            }});
                    }else if (newState == BluetoothProfile.STATE_CONNECTING) {
                        Log.e(TAG, "Connecting to GATT server.");
                        ((MainActivity)context).runOnUiThread(new Runnable() {
                            public void run() {
                                if (Constants.notifType.equals("toast")) {
                                    Toast.makeText(((MainActivity) context), "Connected", Toast.LENGTH_SHORT).show();
                                }
                                else {
                                    Utils.mkSnack(((MainActivity) context), context.findViewById(R.id.layout), "Connected");
                                }
                            }});
                    }
                }

                @Override
                public void onServicesDiscovered(final BluetoothGatt gatt, int status) {
                    if (status == BluetoothGatt.GATT_SUCCESS) {
                        Log.e(TAG, "sevices discovered!");

                        ((MainActivity)context).runOnUiThread(new Runnable() {
                            public void run() {

                                if (Constants.notifType.equals("toast")) {
                                    Toast.makeText(((MainActivity) context), "Bluetooth services discovered", Toast.LENGTH_SHORT).show();
                                }
                                else {
                                    Utils.mkSnack(((MainActivity) context), context.findViewById(R.id.layout), "Bluetooth services discovered");
                                }

                                Constants.connectionStatus="green";
                                ImageView vv = (ImageView) context.findViewById(R.id.connectedView);
                                if (vv!=null) {
                                    vv.setImageDrawable(context.getDrawable(R.drawable.circle_green));
                                }

                                Constants.sealStatus="red";
                                ImageView vv2 = (ImageView) context.findViewById(R.id.sealView);
                                if (vv2!=null) {
                                    vv2.setImageDrawable(context.getDrawable(R.drawable.circle_red));
                                }

                                Constants.measureButtonStatus=true;
                                Button mb = (Button) context.findViewById(R.id.measureButton);
                                if (mb!=null) {
                                    mb.setEnabled(Constants.measureButtonStatus);
                                }
                            }
                        });

                        enableTXNotification();

                    } else {
                        Log.e(TAG, "onServicesDiscovered received: " + status);
                    }
                }

                public void queryTurns() {
                    Log.e("ble","query turns");
                    if (Constants.gatt != null) {
                        BluetoothGattService mSVC = Constants.gatt.getService(Constants.service_uuid);
                        if (mSVC != null) {
                            BluetoothGattCharacteristic charac = mSVC.getCharacteristic(Constants.write_charac_uuid);
                            if (charac != null) {
                                charac.setValue(new byte[]{7});
                                boolean b = Constants.gatt.writeCharacteristic(charac);
                            }
                        }
                    }
                }

                public void enableTXNotification() {
                    Log.e("ble","enabling tx notification");
                    Log.e("ble","is gatt null? "+(Constants.gatt==null));
                    if (Constants.gatt!=null) {
                        BluetoothGattService mSVC = Constants.gatt.getService(Constants.service_uuid);
                        if (mSVC == null) {
                            Log.e("ble", "error service is null!");
                            return;
                        }

                        BluetoothGattCharacteristic charac_read = mSVC.getCharacteristic(Constants.read_charac_uuid);
                        if (charac_read == null) {
                            Log.e("ble", "char is null!!");
                            return;
                        }

                        Constants.gatt.setCharacteristicNotification(charac_read, true);

                        BluetoothGattDescriptor descriptor = charac_read.getDescriptor(Constants.cccd);
                        descriptor.setValue(BluetoothGattDescriptor.ENABLE_NOTIFICATION_VALUE);

                        boolean b = Constants.gatt.writeDescriptor(descriptor);
                        Log.e("ble", "write descriptor result for tx notif " + descriptor.getUuid().toString() + "," + b);
                        Log.e("ble", "-------------------");
                    }
                }

                @Override
                public void onCharacteristicRead(BluetoothGatt gatt,
                                                 BluetoothGattCharacteristic characteristic,
                                                 int status) {
                    Log.e("ble","on characteristic read "+characteristic.getUuid().toString());
                    if (status == BluetoothGatt.GATT_SUCCESS) {
                        actionDataTransfer(characteristic.getValue());
                    }
                    else {
                        Log.e("ble","read characteristic status "+status);
                    }
                }

                @Override
                public void onDescriptorRead (BluetoothGatt gatt, BluetoothGattDescriptor descriptor, int status) {
                    Log.e("ble","on descriptor read "+descriptor.getUuid().toString());
                    if (status == BluetoothGatt.GATT_SUCCESS && pdata.size()<10) {
                        byte[] data=descriptor.getValue();
                        Log.e("ble","data length "+data.length);
                        for(byte byteChar : data) {
                            Log.e("ble",byteChar+"");
                        }
                    }
                    else {
                        Log.e("ble","read descriptor status "+status);
                    }
                }

                @Override
                public void onCharacteristicChanged(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic) {
                    if (characteristic.getUuid().equals(Constants.img_info_char)) {
                        byte[] bs = characteristic.getValue();
                        infoDataTransfer(bs);
                    }
                    else {
                        byte[] bs = characteristic.getValue();
                        actionDataTransfer(bs);
                    }
                }

                public void infoDataTransfer(byte[] txValue) {
                    switch(txValue[0]) {
                        case 0:
                        case 1:
                            if (txValue[0] == 0) {
                                Constants.tympAborted = true;
                                FileOperations.appendtofile("device-abort "+System.currentTimeMillis(), "sout" + Constants.filename + ".txt", Constants.dirname);
                            }
                            else {
                                Constants.tympAborted = false;
                            }
                            Log.e("ble","start a new file transfer");
                            // Start a new file transfer
                            ByteBuffer byteBuffer = ByteBuffer.wrap(Arrays.copyOfRange(txValue, 1, 5));
                            byteBuffer.order(ByteOrder.LITTLE_ENDIAN);
                            Constants.fileSize = byteBuffer.getInt();
                            Constants.mBytesTotal = Constants.fileSize;
                            Log.e("ble","file size in bytes "+Constants.fileSize);
                            Constants.mBytesTransfered = 0;
                            Constants.mStartTimeImageTransfer = System.currentTimeMillis();
                            pdata = new LinkedList<String>();
                            Constants.wroteSealToFile = false;
                            Constants.pksTransferred = 0;
                            break;
                        case 2:
                            ByteBuffer mtuBB = ByteBuffer.wrap(Arrays.copyOfRange(txValue, 1, 3));
                            mtuBB.order(ByteOrder.LITTLE_ENDIAN);
                            short mtu = mtuBB.getShort();
                            Log.e("ble","mtu is "+mtu);
//                            mTextViewMtu.setText(String.valueOf(mtu) + " bytes");
                            if(!Constants.mMtuRequested && mtu < 64){
                                Log.e("ble", "requesting mtu " + mtu);
                                Constants.gatt.requestMtu(247);
                                Constants.mMtuRequested = true;
                            }
                            ByteBuffer ciBB = ByteBuffer.wrap(Arrays.copyOfRange(txValue, 3, 5));
                            ciBB.order(ByteOrder.LITTLE_ENDIAN);
                            short conInterval = ciBB.getShort();

                            short txPhy = txValue[5];
                            short rxPhy = txValue[6];

                            Log.e("ble", "conInterval " + conInterval);
                            Log.e("ble", "txPhy " + txPhy);
                            Log.e("ble", "rxPhy " + rxPhy);
                            Log.e("ble", "----------------");
                            break;
                        case 3:
                            if (!Constants.movingToInit) {
                                if (Constants.rec1 != null) {
                                    Constants.rec1.suppressSending = true;
                                }

                                if (Constants.rec2 != null) {
                                    Constants.rec2.suppressSending = false;
                                }

                                byte[] motorVal = Arrays.copyOfRange(txValue, 1, 5);
                                int motorValConverted = convertMotorValue(motorVal);
                                FileOperations.appendtofile("gotseal-motor "+motorValConverted, "sout" + Constants.filename + ".txt", Constants.dirname);
                                Log.e("ble","GOT SEAL MOTOR VAL "+motorValConverted);

                                byte[] pressureIndex = Arrays.copyOfRange(txValue, 5, 9);
                                int pressureIndexConverted = convertMotorValue(pressureIndex);
                                FileOperations.appendtofile("seal-pindex "+pressureIndexConverted, "sout" + Constants.filename + ".txt", Constants.dirname);
                                Log.e("ble","GOT SEAL PINDEX "+pressureIndexConverted);

                                //seal is found
                                ((MainActivity)context).runOnUiThread(new Runnable() {
                                    public void run() {
                                        FileOperations.appendtofile("sealtime "+System.currentTimeMillis(), "sout" + Constants.filename + ".txt", Constants.dirname);
                                        Constants.firstfoundseal =true;
                                        Constants.sealStatus="green";
                                        ImageView vv = (ImageView) context.findViewById(R.id.sealView);
                                        if (vv!=null) {
                                            vv.setImageDrawable(context.getDrawable(R.drawable.circle_green));
                                        }

                                        if (Constants.snackBar!=null&&!Constants.snackBarMsg.equals("Seal is found")) {
                                            Constants.snackBar.dismiss();
                                            Utils.mkSnack(((MainActivity) context),
                                                    context.findViewById(R.id.layout), "Seal is found");
                                        }
    //
                                        if (Constants.timer == null) {
                                            TextView tv9 = context.findViewById(R.id.textView9);
                                            if (tv9!=null) {
                                                tv9.setText("0:00");
                                            }

                                            Constants.timer = new CountUpTimer(Constants.recordDurationInSeconds * 1000) {
                                                public void onTick(int second) {
                                                    TextView tv9 = context.findViewById(R.id.textView9);
                                                    int raw = Integer.parseInt(String.valueOf(second));
                                                    int min = raw / 60;
                                                    int sec = raw % 60;
                                                    String tval = "";
                                                    if (sec < 10) {
                                                        tval=(min + ":0" + sec);
                                                    } else {
                                                        tval=(min + ":" + sec);
                                                    }
                                                    if (tv9!=null) {
                                                        tv9.setText(second+"");
                                                    }
                                                    Constants.timerVal = second+"";
                                                }
                                            };
                                            Constants.timer.start();
                                        }
                                    }
                                });
                            }
                            break;
                        case 4:
                            if (!Constants.movingToInit) {
                                //seal is broken
                                if (Constants.rec1 != null) {
                                    Constants.rec1.suppressSending = false;
                                }

                                if (Constants.rec2 != null) {
                                    Constants.rec2.suppressSending = true;
                                }

                                ((MainActivity) context).runOnUiThread(new Runnable() {
                                    public void run() {
                                        FileOperations.appendtofile("broketime "+System.currentTimeMillis(), "sout" + Constants.filename + ".txt", Constants.dirname);

                                        if (Constants.firstfoundseal) {
                                            if (Constants.snackBar != null && !Constants.snackBarMsg.equals("Seal is broken") && Constants.rec1 != null
                                                    && !Constants.snackBarMsg.contains("occluded")) {
                                                if (Build.VERSION.SDK_INT > 21) {
                                                    Utils.mkSnack(((MainActivity) context), context.findViewById(R.id.layout),
                                                            "Seal is broken");
                                                } else {
                                                    Utils.mkToast((MainActivity) context, "Seal is broken");
                                                }
                                            }
                                        }

                                        Constants.sealStatus = "red";
                                        ImageView vv2 = (ImageView) context.findViewById(R.id.sealView);
                                        if (vv2 != null) {
                                            vv2.setImageDrawable(context.getDrawable(R.drawable.circle_red));
                                        }

                                        if (Constants.timer != null) {
                                            Constants.timer.cancel();
                                            Constants.timer = null;
                                        }
                                    }
                                });
                            }
                            break;
                        case 5:
                            FileOperations.appendtofile("maxtime "+System.currentTimeMillis(), "sout" + Constants.filename + ".txt", Constants.dirname);
                            byte[] pressureIndex = Arrays.copyOfRange(txValue, 5, 9);
                            int pressureIndexConverted = convertMotorValue(pressureIndex);
                            FileOperations.appendtofile("max-pindex "+pressureIndexConverted, "sout" + Constants.filename + ".txt", Constants.dirname);
                            Log.e("ble","GOT MAX PINDEX "+pressureIndexConverted);
                            break;
                        case 6:
                            FileOperations.appendtofile("mintime "+System.currentTimeMillis(), "sout" + Constants.filename + ".txt", Constants.dirname);
                            pressureIndex = Arrays.copyOfRange(txValue, 5, 9);
                            pressureIndexConverted = convertMotorValue(pressureIndex);
                            FileOperations.appendtofile("min-pindex "+pressureIndexConverted, "sout" + Constants.filename + ".txt", Constants.dirname);
                            Log.e("ble","GOT MIN PINDEX "+pressureIndexConverted);
                            break;
                        case 7:
                            FileOperations.appendtofile("endtime "+System.currentTimeMillis(), "sout" + Constants.filename + ".txt", Constants.dirname);
                            byte[] motorVal = Arrays.copyOfRange(txValue, 1, 5);
                            int motorValConverted = convertMotorValue(motorVal);
                            FileOperations.appendtofile("end-motor "+motorValConverted, "sout" + Constants.filename + ".txt", Constants.dirname);
                            Log.e("ble","END MOTOR VAL "+motorValConverted);
                            break;
                        case 8:
                            FileOperations.appendtofile("middletime "+System.currentTimeMillis(), "sout" + Constants.filename + ".txt", Constants.dirname);
                            break;
                        case 9:
                            byte[] initPressure = Arrays.copyOfRange(txValue, 1, 5);
                            double initPressureConverted = convertMotorValue(initPressure);
                            FileOperations.appendtofile("initpressure "+initPressureConverted, "sout" + Constants.filename + ".txt", Constants.dirname);
                            Constants.initPressure = initPressureConverted;
                            break;
                        case 10:
                            finishMeasurement();
                            if (Constants.rec2!=null) {
                                Constants.rec2.stopit();
                                Constants.rec2 = null;
                                Log.e("logme","rec2 destroyed");
                            }
                        case 11:
                            Log.e("ble","ACK");
                            Constants.ackReceived = true;
                            break;
                        case 12:
                            motorVal = Arrays.copyOfRange(txValue, 1, 5);
                            motorValConverted = convertMotorValue(motorVal);

                            if (Constants.turnInfo != 0) {
                                Log.e("ble","turn info is bad");
                                // this is when the tymp is running a previous measurement on startup
                                Log.e("ble","abort4");
                                FileOperations.appendtofile("badturn-abort "+System.currentTimeMillis(), "sout" + Constants.filename + ".txt", Constants.dirname);
                                Utils.abortMeasurement(context, "Previous measurement still in session. ");
                            }
                            else {
                                Log.e("ble","turn info is good");
                                ((MainActivity)context).runOnUiThread(new Runnable() {
                                    public void run() {
                                        Constants.measureButtonStatus = true;
                                        Button mb = (Button) context.findViewById(R.id.measureButton);
                                        if (mb != null) {
                                            mb.setEnabled(Constants.measureButtonStatus);
                                        }
                                    }});
                            }

                            break;
                        default:
                            Log.e("ble","unknown command "+txValue[0]);
                            break;
                    }
                }

                public void actionDataTransfer(byte[] txValue) {
                    if (txValue.length==0) {
                        return;
                    }

                    Log.e("ble", "action data transfer");

                    if (Constants.mBytesTransfered == 0) {
                        Log.e("ble", "first packet received: " + String.valueOf(txValue.length) + " bytes");
                    }
                    Constants.mBytesTransfered += txValue.length;
                    Log.e("ble", "we have transferred " + Constants.mBytesTransfered+","+txValue.length + " out of "+Constants.mBytesTotal);

                    LinkedList<String> vals = convertBytes(txValue);
                    FileOperations.appendtofile(vals,"pout"+Constants.filename+".txt",Constants.dirname);

                    Constants.pksTransferred += 1;

                    if (Constants.testInProgress &&
                        ((Constants.mBytesTransfered >= Constants.mBytesTotal && txValue.length > 0) ||
                        Constants.mBytesTotal >= 900 && Constants.mBytesTransfered > 700) ||
                        Constants.pksTransferred >= 4) {

                        Log.e("ble", ">>> we are done transferring");

                        if (Constants.rec1 != null) {
                            Constants.rec1.suppressSending=true;
                        }

                        ((MainActivity)context).runOnUiThread(new Runnable() {
                            public void run() {
                                if (Constants.tympAborted) {
                                    if (Constants.moveToInit) {
                                        Log.e("logme","TEST1");
                                        Utils.mkSnack(((MainActivity) context), context.findViewById(R.id.layout),
                                                "Measurement aborted. Remove tip from ear for device to reset. This can take up to "+ Constants.SEAL_LOST_TIMEOUT + " seconds. Please wait.");
                                    }
                                    else {
                                        Utils.mkSnack(((MainActivity) context), context.findViewById(R.id.layout),
                                                "Measurement aborted.");
                                    }
                                } else {
                                    if (Constants.moveToInit) {
//                                        Utils.mkSnack(((MainActivity) context), context.findViewById(R.id.layout),
//                                                "Measurement done. Remove tip from ear for device to reset. This can take up to "+ Constants.SEAL_LOST_TIMEOUT + " seconds. Please wait.");
                                    } else {
//                                        Utils.mkSnack(((MainActivity) context), context.findViewById(R.id.layout),
//                                                "Measurement done");
                                    }
                                }

                                Constants.sealStatus = "red";
                                ImageView vv = context.findViewById(R.id.sealView);
                                if (vv != null) {
                                    vv.setImageDrawable(context.getDrawable(R.drawable.circle_red));
                                }

                                Constants.measurementStatus = "red";
                                ImageView vv2 = (ImageView) context.findViewById(R.id.measuringView);
                                if (vv2 != null) {
                                    vv2.setImageDrawable(context.getDrawable(R.drawable.circle_red));
                                }

                                if (Constants.timer != null) {
                                    Constants.timer.cancel();
                                }

                                Constants.stopButtonStatus=false;
                                Button sb = (Button)context.findViewById(R.id.stopButton);
                                sb.setEnabled(Constants.stopButtonStatus);

                                if (Constants.moveToInit) {
                                    Constants.resetStatus="red";
                                    ImageView vv3 = (ImageView)context.findViewById(R.id.resetView);
                                    if (vv3!=null) {
                                        vv3.setImageDrawable(context.getDrawable(R.drawable.circle_green));
                                    }
                                }
                            }});

                        try {
                            Thread.sleep(Constants.endSleepTime*1000);
                        }
                        catch(Exception e) {
                            Log.e("ble",e.getMessage());
                        }

                        Log.e("ble","is speaker null? "+(Constants.sp1==null));
                        if (!Constants.moveToInit && Constants.sp1!=null) {
                            Constants.sp1.stopit();
                            Constants.sp1 = null;
                        }

                        Log.e("ble","is rec null? "+(Constants.rec1 ==null));
                        if(Constants.rec1 != null) {
                            Constants.rec1.stopit();
                            Constants.rec1 = null;
                        }
                        if (Constants.startupTask!=null) {
                            Constants.startupTask.cancel(true);
                            Constants.startupTask=null;
                        }

                        Constants.testInProgress=false;

                        if (!Constants.moveToInit) {
                            finishMeasurement();
                        }
                        else {
                            Log.e("logme","offlinerecorder2 - measurement");
                            if (Constants.rec2 == null) {
                                Constants.movingToInit = true;
                                Constants.rec2 = new OfflineRecorder(Constants.samplingFreq, (int) 0, MediaRecorder.AudioSource.DEFAULT, Constants.dirname, false, context, false, 2);
                                Constants.rec2.filename2 = "miccheck-2"+Constants.filename+ ".txt";
                                Constants.rec2.start();
                            }
                        }
                    }
                }

                public int convertMotorValue(byte[] data) {
                    int i1 = data[0]&0x0000ff;
                    int i2 = data[1]&0x0000ff;
                    int i3 = data[2]&0x0000ff;
                    int i4 = data[3]&0x0000ff;
                    double temp = (i4 + (i3*256) + (i2 * Math.pow(256,2)) + (i1 * Math.pow(256,3)));
                    Log.e("ble","CONVERT MOTOR "+temp);
                    return (int)temp;
                }

                public LinkedList<String> convertBytes(byte[] data) {
                    LinkedList<String> vals = new LinkedList<String>();
                    for (int i = 0; i < data.length; i += 4) {
                        int i1 = data[i + 1]&0x0000ff;
                        int i2 = data[i + 2]&0x0000ff;
                        int i3 = data[i + 3]&0x0000ff;
                        double out = i3 + i2 * 256 + i1 * 256 * 256;
                        out = (out - Constants.omin) / (Constants.omax - Constants.omin);
                        out = out * 25 * 6895;
                        out = out / 10;
                        if (Constants.pcounter == 0) {
                            Constants.pinit = out;
                        }
                        vals.add(out+"");
                        Constants.pcounter+=1;
                    }
                    return vals;
                }

                @Override
                public void onCharacteristicWrite (BluetoothGatt gatt,
                                                   BluetoothGattCharacteristic characteristic,
                                                   int status) {
//                    Log.e("ble","on characteristic write "+characteristic.getUuid().toString());
                    if (status == BluetoothGatt.GATT_SUCCESS) {
//                        Log.e("ble","write characteristic success "+characteristic.getUuid().toString());
                    }
                    else {
//                        Log.e("ble","write characteristic status "+status);
                    }
                }

                @Override
                public void onDescriptorWrite(BluetoothGatt gatt, BluetoothGattDescriptor descriptor, int status) {
//                    Log.e("ble","on descriptor write "+descriptor.getUuid().toString()+","+descriptor.getCharacteristic().getUuid().toString());
                    if (descriptor.getCharacteristic().getUuid().equals(Constants.read_charac_uuid)) {
//                        Log.e("ble","found descriptor read char "+descriptor.getUuid().toString()+","+descriptor.getCharacteristic().getUuid().toString());

                        BluetoothGattService mSVC = Constants.gatt.getService(Constants.service_uuid);
                        BluetoothGattCharacteristic charac = mSVC.getCharacteristic(Constants.img_info_char);
                        if(charac==null) {
                            Log.e("ble","img info char not found");
                            return;
                        }
                        Constants.gatt.setCharacteristicNotification(charac,true);

                        BluetoothGattDescriptor descriptor2 = charac.getDescriptor(Constants.cccd);
                        descriptor2.setValue(BluetoothGattDescriptor.ENABLE_NOTIFICATION_VALUE);
                        Constants.gatt.writeDescriptor(descriptor2);
                    }
                    else if (descriptor.getCharacteristic().getUuid().equals(Constants.img_info_char)) {
                        Log.e("ble","img info notification "+status);

                        // now we can send commands
                        queryTurns();
                    }
                    else {
                        Log.e("ble","found descriptor "+descriptor.getUuid().toString()+","+descriptor.getCharacteristic().getUuid().toString());
                    }
                    Log.e("ble","***********");
                }

                @Override
                public void onMtuChanged(BluetoothGatt gatt, int mtu, int status) {
                    Log.e("ble","on mtu changed "+mtu);
                    if (status == BluetoothGatt.GATT_SUCCESS) {
                        Log.e("ble","mtu changed success ");
                    }
                    else {
                        Log.e("ble","mtu changed no success "+status);
                    }
                    Log.e("ble","--------------------");
                }
            };
}