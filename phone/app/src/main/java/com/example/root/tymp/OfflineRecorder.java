package com.example.root.tymp;

import android.app.Activity;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattService;
import android.content.SharedPreferences;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioRecord;
import android.preference.PreferenceManager;
import android.util.Log;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.util.Arrays;
import java.util.LinkedList;


public class OfflineRecorder extends Thread {
    public boolean recording;
    int samplingfrequency;
    public short[] samples;
    public short[] sub = null;
    public short[] temp;
    int count;
    AudioRecord rec;
    int minbuffersize;
    int duration;
    boolean writtenToFile = false;
    String filename;
    String filename2;
    String dirname;
    BufferedWriter buf;
    BufferedWriter buf2;
    double attackTime=1;
    double releaseTime=1;
    double gainAttack;
    double gainRelease;
    Activity av;
    boolean writeToFile;
    int check;
    boolean suppressSending = false;
    SharedPreferences prefs;
    long ts;

    public OfflineRecorder(int samplingfrequency,int duration,int microphone,String dirname,boolean stereo,Activity av, boolean writeToFile,int check) {
        this.check = check;
        Log.e("ble","OFFLINE duration "+duration);
        this.prefs = PreferenceManager.getDefaultSharedPreferences(av);
        count = 0;
        this.samplingfrequency = samplingfrequency;
        this.duration = duration;
        this.av = av;
        sealResult = false;
        int channels=AudioFormat.CHANNEL_IN_MONO;
        if (stereo) {
            channels=AudioFormat.CHANNEL_IN_STEREO;
        }
        envs= new LinkedList<Integer>();
        envsStr= new LinkedList<String>();
        gainAttack = Math.exp(-1.0/(samplingfrequency*attackTime));
        gainRelease = Math.exp(-1.0/(samplingfrequency*releaseTime));

        minbuffersize = AudioRecord.getMinBufferSize(samplingfrequency, channels, AudioFormat.ENCODING_PCM_16BIT);

        Log.e("logme", "minbuffersize " + minbuffersize);

        int counter = 0;
        do {
            rec = new AudioRecord(microphone, samplingfrequency, channels, AudioFormat.ENCODING_PCM_16BIT, minbuffersize);
            Log.e("ble", "audiorecord state 1" + rec.getState());
            try {
                Thread.sleep(100);
            }
            catch(Exception e) {
                Log.e("ble","sleep issue "+e.getMessage());
            }
            counter++;
        }
        while (rec.getState() != 1 && counter < 5);

        temp = new short[minbuffersize];
        samples = new short[duration*samplingfrequency];
        this.dirname = dirname;
        this.writeToFile = writeToFile;
    }

    public static boolean works(int rate, int channelConfig, int audioFormat) {
        int bufferSize = AudioRecord.getMinBufferSize(rate, channelConfig, audioFormat);
        if (bufferSize != AudioRecord.ERROR_BAD_VALUE) {
            AudioRecord recorder = new AudioRecord(AudioManager.STREAM_VOICE_CALL, rate, channelConfig, audioFormat, bufferSize);
            if (recorder.getState() == AudioRecord.STATE_INITIALIZED) {
                Log.e("hello",">>> works true");
                return true;
            }
        }
        Log.e("hello",">>> works false");
        return false;
    }

    private static int[] mSampleRates = new int[] { 8000, 11025, 22050, 44100 };
    public AudioRecord findAudioRecord() {
        for (int rate : mSampleRates) {
            for (short audioFormat : new short[] { AudioFormat.ENCODING_PCM_8BIT, AudioFormat.ENCODING_PCM_16BIT }) {
                for (short channelConfig : new short[] { AudioFormat.CHANNEL_IN_MONO, AudioFormat.CHANNEL_IN_STEREO }) {
                    try {
                        Log.d("hello", "Attempting rate " + rate + "Hz, bits: " + audioFormat + ", channel: "
                                + channelConfig);
                        int bufferSize = AudioRecord.getMinBufferSize(rate, channelConfig, audioFormat);
                        Log.e("hello",bufferSize+","+AudioRecord.ERROR_BAD_VALUE);
                        if (bufferSize != AudioRecord.ERROR_BAD_VALUE) {
                            // check if we can instantiate and have a success
                            AudioRecord recorder = new AudioRecord(AudioManager.STREAM_VOICE_CALL, rate, channelConfig, audioFormat, bufferSize);
                            Log.e("hello",recorder.getState()+","+AudioRecord.STATE_INITIALIZED);
                            if (recorder.getState() == AudioRecord.STATE_INITIALIZED) {
                                Log.e("hello", "worked");
                                return recorder;
                            }
                        }
                    } catch (Exception e) {
//                        Log.e(C.TAG, rate + "Exception, keep trying.",e);
                        Log.e("hello",e.getMessage());
                    }
                }
            }
        }
        return null;
    }

    public void stopit() {
        suppressSending = true;
        try {
            Log.e("file","stop it");
            Log.e("ble","audiorecord state 2"+rec.getState());
            recording = false;

            Log.e("file","write buf2 "+check+","+filename2);
            FileOperations.writetofileStr(buf2, envsStr, filename2, dirname);
            if (!writtenToFile && writeToFile) {
                Log.e("file","write micvals "+filename);
                sub=Arrays.copyOfRange(samples, 0, totalAudioVals);
                Log.e("ble","OFFLINE RECORDER STOP IT");
                FileOperations.writetofile(buf, sub, filename, dirname);
            }

            rec.stop();
            rec.release();
        }
        catch(Exception e){
//            Log.e("logme",e.getMessage());
        }
    }

    public double envelope(short[] sig) {
        double envOut = 0;
        double accum=0;
        for (int i = 0; i < sig.length; i++) {
            double envIn = Math.abs(sig[i]);
            if (envOut < envIn) {
                envOut = envIn + gainAttack*(envOut-envIn);
            }
            else {
                envOut = envIn + gainRelease*(envOut-envIn);
            }
            accum += envOut;
        }
        return accum/sig.length;
    }

    public double spec(int[] sig) {
        int fac = 10;
        double[] pows=DataProc.fftnative(sig,(int)(Constants.samplingFreq/fac));
        double bin = pows[(int)(Constants.toneFreq/fac)];
        if (bin == 0) {
            return -20;
        }
        else {
            return 20 * Math.log10(bin);
        }
    }

    LinkedList<Integer>envs;
    LinkedList<String>envsStr;
    boolean sealResult;

    public boolean threshCheck(int num, int recnumber, int lowbound, int upbound) {
        if (envs.size() < num) {
            return false;
        }
        for(int i = 1; i<= num; i++) {
            if (envs.get(envs.size()-i) < lowbound) {
                Log.e("sealinfo",recnumber+" NO SEAL "+envs.get(envs.size()-i));

                if (Constants.snackBar!=null && Constants.snackBarMsg.equals("Tip is occluded")) {
                    Constants.snackBar.dismiss();
                    Constants.snackBarMsg="";
                }

                return false;
            }
            else if(envs.get(envs.size()-i) > upbound) {
                Log.e("sealinfo", recnumber+" OCCLUSION "+envs.get(envs.size()-i)+","+upbound);

                ((MainActivity)av).runOnUiThread(new Runnable() {
                    public void run() {
                        if (recnumber==1&&!Constants.snackBarMsg.equals("Tip is occluded")) {
                            Log.e("logme",Constants.snackBarMsg);

                            if (Constants.notifType.equals("toast")) {
                                Utils.mkToast(((MainActivity) av),
                                        "Tip is occluded");
                            }
                            else {
                                Utils.mkSnack(((MainActivity) av), av.findViewById(R.id.layout),
                                        "Tip is occluded");
                            }
                        }
                    }});

                return false;
            }
        }

        if (Constants.snackBar!=null && Constants.snackBarMsg.equals("Tip is occluded")) {
            Constants.snackBar.dismiss();
            Constants.snackBarMsg="";
        }
        Log.e("sealinfo",recnumber+" SEAL "+envs.get(envs.size()-1));
        return true;
    }

    public void envCheck(short[] sig) {
        double eval = envelope(sig);
        envs.add((int)eval);
        envsStr.add(System.currentTimeMillis()+","+(int)eval);

//        Log.e("logme","EVAL "+eval);

        boolean tcheck = threshCheck(4, 1, prefs.getInt("micthresh", Constants.SEAL_CHECK_THRESH),
                prefs.getInt("occlusionthresh", Constants.SEAL_OCCLUSION_THRESH));
        if (tcheck) {
            sendCheck(1);
        } else if (!tcheck) {
            sendCheck(0);
        }
    }

    public void envCheck2(short[] sig) {
        if ((System.currentTimeMillis()-ts) >= (Constants.SEAL_LOST_TIMEOUT*1000)){
//            Log.e("sealinfo","TIMEOUT "+(System.currentTimeMillis()-ts));
            sendCheck(2);
        }
        else {
            double eval = envelope(sig);
            envs.add((int)eval);
            envsStr.add(System.currentTimeMillis()+","+(int)eval);

            boolean tcheck = threshCheck(4, 2, prefs.getInt("micthresh", Constants.SEAL_CHECK_THRESH),
                    prefs.getInt("occlusionthresh", Constants.SEAL_OCCLUSION_THRESH));
//            Log.e("sealinfo","tcheck "+tcheck);
            if (tcheck) {
                sendCheck(0);
            } else if (!tcheck) {
                sendCheck(2);
            }
        }
    }

    public void sendCheck(int val) {
        if (!suppressSending) {
//            Log.e("sealinfo","sendcheck "+val);
//            Log.e("logme", check + " sendcheck " + val);
            if (Constants.gatt != null) {
                BluetoothGattService mSVC = Constants.gatt.getService(Constants.service_uuid);
                if (mSVC != null) {
//                Log.e("ble", (mSVC.getUuid().toString()) + "");
                    BluetoothGattCharacteristic charac = mSVC.getCharacteristic(Constants.write_charac_uuid);
                    if (charac != null) {
//                    Log.e("ble", (charac.getUuid().toString()) + "");
                        charac.setValue(new byte[]{5, (byte)val});
                        boolean b = Constants.gatt.writeCharacteristic(charac);
                        // Log.e("ble", (charac.getProperties() == BluetoothGattCharacteristic.PROPERTY_WRITE) + "," + (b) + "");
                    }
                }
            }
        }
    }

    int totalAudioVals=0;
    public void run() {
        String dir = av.getExternalFilesDir(null).toString();
        try {
            File path = new File(dir);
            if (!path.exists()) {
                path.mkdir();
            }

            File file2 = new File(dir + File.separator + filename2);
            if (!file2.exists()) {
                file2.createNewFile();
            }
            buf2 = new BufferedWriter(new FileWriter(file2, false));
        }
        catch(Exception e ){
            Log.e("ble",e.getMessage());
        }

        if (writeToFile) {
            try {
                Log.e("file","writetofile");
                Log.e("ble", "audiorecord state 3" + rec.getState());

                File file = new File(dir + File.separator + filename);
                if (!file.exists()) {
                    file.createNewFile();
                }

                buf = new BufferedWriter(new FileWriter(file, false));

                int bytesread;
                rec.startRecording();
                recording = true;
                while (recording) {
                    //                Log.e("ble","audiorecord state 4"+rec.getState());
                    bytesread = rec.read(temp, 0, minbuffersize);
//                    Log.e("ble","bytes read "+bytesread);
                    totalAudioVals += bytesread;
                    if (check==1) {
                        envCheck(temp);
                    }
                    else {
                        envCheck2(temp);
                    }
                    for (int i = 0; i < bytesread; i++) {
//                        Log.e("file","write");
                        if (count >= samples.length) {
                            rec.stop();
                            rec.release();
                            recording = false;
                            Log.e("ble","OFFLINE RECORDER DONE");
                            FileOperations.writetofile(buf, samples, filename, dirname);
                            writtenToFile=true;
                            // this is when the recorder times out
                            Log.e("ble","abort1");
                            FileOperations.appendtofile("toolong-abort "+System.currentTimeMillis(), "sout" + Constants.filename + ".txt", Constants.dirname);
                            Utils.abortMeasurement(av,"Measurement took too long. ");
                            break;
                        } else {
                            if (check==1) {
                                samples[count] = temp[i];
                            }
                            count++;
                        }
                    }
                }
            } catch (Exception e) {
                Log.e("logme","offlinerecorder run "+Utils.e2s(e));
            }
        }
        else {
            try {
                Log.e("ble", "audiorecord state 3" + rec.getState());

                int bytesread;
                rec.startRecording();
                recording = true;
                this.ts = System.currentTimeMillis();
                while (recording) {
                    bytesread = rec.read(temp, 0, minbuffersize);
                    if (check==1) {
                        envCheck(temp);
                    }
                    else {
                        envCheck2(temp);
                    }
                }
            } catch (Exception e) {
                Log.e("logme","offlinerecorder run "+Utils.e2s(e));
            }
        }
    }
}
