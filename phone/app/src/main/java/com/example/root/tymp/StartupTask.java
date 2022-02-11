package com.example.root.tymp;

import android.app.Activity;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattService;
import android.content.SharedPreferences;
import android.media.MediaRecorder;
import android.os.AsyncTask;
import android.preference.PreferenceManager;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;

import com.google.android.material.chip.Chip;
import com.google.android.material.chip.ChipGroup;

public class StartupTask extends AsyncTask<Void, Void, Void> {

    Activity av;
    View v;

    public StartupTask(Activity av,View v) {
        this.av = av;
        this.v = v;
    }

    protected void onPreExecute() {
        super.onPreExecute();
        Log.e("ble","preexecute");

        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(av);

        Constants.toneFreq = Integer.parseInt(prefs.getString("tone",Constants.defaultTone));
        Constants.mic = prefs.getBoolean("mic",true);
        Constants.speaker = prefs.getBoolean("speaker",true);

        av.runOnUiThread(new Runnable() {
            public void run() {
                Constants.timer = new CountUpTimer(Constants.recordDurationInSeconds * 1000) {
                    public void onTick(int second) {
                        TextView tv9 = av.findViewById(R.id.textView9);
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
                            tv9.setText(tval);
                        }
                        Constants.timerVal=tval;
                    }
                };
                Constants.timer.start();
            }});
    }

    protected void onPostExecute(Void result) {
        Log.e("ble","postexecute");
        super.onPostExecute(result);
    }

    public Void doInBackground(Void... params) {
        try {
            Log.e("ble", "doinbackground");
            short[] tone = SignalGenerator.continuoussinepulse(Constants.samplingFreq, Constants.toneFreq, Constants.samplingFreq, 0, 1);

            SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(av);
            Constants.volume = Double.parseDouble(prefs.getString("vol", Constants.volume + ""));
            Constants.speaker = prefs.getBoolean("speaker", true);
            Constants.mic = prefs.getBoolean("mic", true);
            Constants.moveToInit = prefs.getBoolean("movetoinit", true);

            Constants.sp1 = new AudioSpeaker(av, tone, Constants.samplingFreq, Constants.bottomspeaker);
            Log.e("ble", "SPEAKER " + Constants.speaker + "," + Constants.volume);
            if (Constants.speaker) {
                Constants.sp1.play(Constants.volume, (int) -1);
            }

            int micType = MediaRecorder.AudioSource.DEFAULT;

            boolean stereo = false;
            long ts = System.currentTimeMillis();

            av.runOnUiThread(new Runnable() {
                public void run() {
                    EditText pidVal = av.findViewById(R.id.pidVal);
                    ChipGroup cg = av.findViewById(R.id.chipGroup);
                    if (cg != null) {
                        Chip cc = av.findViewById(cg.getCheckedChipId());
                        if (cc != null) {
                            String ear = cc.getText().toString().toLowerCase();

                            String tss = ts + "";
                            if (pidVal != null) {
                                Constants.filename = pidVal.getText().toString() + "-" + ear + "-" + ts;
                            } else {
                                Constants.filename = "00-left-" + ts;
                            }

                            String ss = tss.substring(0, tss.length() - 4) + "-" + tss.substring(tss.length() - 4, tss.length());

                            TextView tv2 = av.findViewById(R.id.fnameView);
                            if (tv2 != null) {
                                tv2.setText(ss);
                            }
                        }
                    }
                }
            });

            if (Constants.mic && Constants.rec1 == null) {
                Constants.rec1 = new OfflineRecorder(Constants.samplingFreq, (int) (Constants.recordDurationInSeconds), micType, Constants.dirname, stereo, av, true, 1);

                Constants.rec1.filename = "mout"+Constants.filename+ ".txt";
                Constants.rec1.filename2 = "miccheck-1"+Constants.filename+ ".txt";

                Constants.rec1.start();
            }

            Thread.sleep(Constants.initSleepTime * 1000);

            Log.e("ble","sending ack request");
            if (Constants.gatt != null) {
                BluetoothGattService mSVC = Constants.gatt.getService(Constants.service_uuid);
                if (mSVC != null) {
                    BluetoothGattCharacteristic charac = mSVC.getCharacteristic(Constants.write_charac_uuid);
                    if (charac != null) {
                        charac.setValue(new byte[]{2});
                        boolean b = Constants.gatt.writeCharacteristic(charac);
                    }
                }
            }

            Thread.sleep((int) (Constants.AckWaitTime * 1000));

            Log.e("ble","ACK STATUS "+Constants.ackReceived);
            if(!Constants.ackReceived) {
                Log.e("ble","abort3");
                FileOperations.appendtofile("unresponsive-abort "+System.currentTimeMillis(), "sout" + Constants.filename + ".txt", Constants.dirname);
                Utils.abortMeasurement(av,"Device is unresponsive. ");
            }
        }
        catch(Exception e) {
            Log.e("ble","#### "+e.getMessage());
        }

        return null;
    }
}
