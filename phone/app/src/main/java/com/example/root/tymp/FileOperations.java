package com.example.root.tymp;

import android.app.Activity;
import android.content.Context;
import android.os.AsyncTask;
import android.util.Log;
import android.widget.ImageView;
import android.widget.TextView;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.util.ArrayList;
import java.util.Collections;
import java.util.LinkedList;

public class FileOperations {
    static Activity av;
    static Context cxt;

    private static class WriteToFileTask extends AsyncTask<Void, Void, Void> {

        Activity av;
        Context cxt;
        BufferedWriter buf;
        LinkedList<String> buff;
        String filename;
        String dirname;
        long ts;

        public WriteToFileTask(Activity av, Context cxt, BufferedWriter buf, LinkedList<String> buff,String filename, String dirname) {
            this.av = av;
            this.cxt = cxt;
            this.buf=buf;
            this.buff=buff;
            this.filename=filename;
            this.dirname=dirname;
        }

        protected void onPreExecute() {
            super.onPreExecute();
        }
        protected void onPostExecute(Void result) {
            super.onPostExecute(result);
            Log.e("logme","WRITE TIME "+(System.currentTimeMillis()-ts));
        }

        public Void doInBackground(Void... params) {
            ts = System.currentTimeMillis();
            try {
                Log.e("logme", "writetofile- doinbackground");
                Log.e("logme","WRITE_TO_FILE "+filename);
                try {
//                    Thread.sleep(2000);
//            Log.e("logme",filename);
//            Log.e("logme",buff.length+"");

                    for (String s : buff) {
                        buf.append(s+"");
                        buf.newLine();
                    }
                    buf.flush();
                    buf.close();
                }catch(Exception e) {
                    Log.e("logme","write to file "+e.getMessage());
                }
                Log.e("logme","WRITE FINISHED");
            }
            catch(Exception e) {
                Log.e("logme",e.getMessage());
                if (e.getMessage().contains("Error with ") && !Constants.tympAborted) {
                    ((MainActivity)av).runOnUiThread(new Runnable() {
                        public void run() {
                            Utils.mkSnack(av, ((MainActivity)av).findViewById(R.id.layout), e.getMessage());
                        }});
                }
            }
            return null;
        }
    }

    private static class ProcessDataTask extends AsyncTask<Void, Void, Void> {

        ArrayList<PlotPair> pairs = null;
        int[] buff;
        Activity av;
        long ts;

        public ProcessDataTask(Activity av, int[] buff) {
            this.buff = buff;
            this.av = av;
        }

        protected void onPreExecute() {
            super.onPreExecute();
//            if (Build.VERSION.SDK_INT <= 21) {
//                try {
//                    Thread.sleep(5000);
//                } catch (Exception e) {
//                    Log.e("asdf", e.getMessage());
//                }
//            }
        }

        protected void onPostExecute(Void result) {
            Log.e("logme","processdata- postexecute");
            super.onPostExecute(result);
            if (pairs != null) {
                DataProc.processdatawrapperUI(pairs,Constants.filename);
            }
            Log.e("logme","PROCESS TIME "+(System.currentTimeMillis()-ts));
            Constants.files = FileOperations.getFileList();
            Constants.fileNamePointer = Constants.filename;
            ImageView forwardButton = (ImageView)av.findViewById(R.id.forwardButton);
            forwardButton.setEnabled(false);
            forwardButton.setImageAlpha((int)(1*255));
            TextView counterView = (TextView)av.findViewById(R.id.counterView);
            counterView.setText(Constants.files.size()+"/"+Constants.files.size());
        }

        public Void doInBackground(Void... params) {
            ts = System.currentTimeMillis();
            try {
                Log.e("logme", "processdata- doinbackground");
                pairs = DataProc.processdatawrapper(buff,Constants.filename);
            }
            catch(Exception e) {
                Log.e("logme",e.getMessage());
                if (e.getMessage().contains("Error with ") && !Constants.tympAborted) {
                    ((MainActivity)av).runOnUiThread(new Runnable() {
                        public void run() {
                            Utils.mkSnack(av, ((MainActivity)av).findViewById(R.id.layout), e.getMessage());
                        }});
                }
            }
            return null;
        }
    }

    public static ArrayList<String> getFileList() {
        String dir = av.getExternalFilesDir(null).toString();
        File path = new File(dir);
        ArrayList<String> files = new ArrayList<String>();
        for (File file : path.listFiles()) {
            if (file.getName().contains("mout")) {
                files.add(file.getName().substring(4,file.getName().length()-4));
            }
        }
        Collections.sort(files);
        return files;
    }

    public static void writetofile(BufferedWriter buf, short[] buff,String filename, String dirname) {
        int[] ii = new int[buff.length];
        for(int i = 0; i < buff.length; i++) {
            ii[i] = buff[i];
        }

        try {
            new ProcessDataTask(av, ii).execute().get();
            LinkedList<String> lst = new LinkedList<String>();
            for (Short s : buff) {
                lst.add((int) s + "");
            }
            new WriteToFileTask(av, cxt, buf, lst, filename, dirname).execute();
        }
        catch(Exception e) {
            Log.e("asdf",e.getMessage());
        }
        Utils.screenshot(av);
    }

//    public static void writetofileInt(BufferedWriter buf, LinkedList<Integer> buff,String filename, String dirname) {
//        new WriteToFileTask(av, cxt, buf, buff, filename, dirname).execute();
//    }

    public static void writetofileStr(BufferedWriter buf, LinkedList<String> buff,String filename, String dirname) {
        new WriteToFileTask(av, cxt, buf, buff, filename, dirname).execute();
    }

    public static void writetofile(LinkedList<String> buff, String filename, String dirname) {
        try {
            String dir = av.getExternalFilesDir(null).toString();
            Log.e("ble",filename);
            File path = new File(dir);
            if (!path.exists()) {
                path.mkdir();
            }

            File file = new File(dir+File.separator+filename);
            if(!file.exists()) {
                file.createNewFile();
            }
            BufferedWriter buf = new BufferedWriter(new FileWriter(file,false));
            for (int i = 0; i < buff.size(); i++) {
                buf.append(""+buff.get(i));
                buf.newLine();
            }
            buf.flush();
            buf.close();
        } catch(Exception e) {
            Log.e("test",e.getMessage());
        }
    }

    public static void writetofile(String filename,LinkedList<Double>scaled_results) {
        try {
            String dir = av.getExternalFilesDir(null).toString();
            Log.e("ble",filename);
            File path = new File(dir);
            if (!path.exists()) {
                path.mkdir();
            }

            File file = new File(dir+File.separator+filename);
            if(!file.exists()) {
                file.createNewFile();
            }
            BufferedWriter buf = new BufferedWriter(new FileWriter(file,false));
//            for (int i = 0; i < buff.size(); i++) {
//                buf.append(""+buff.get(i));
//                buf.newLine();
//            }
            for (int i = 0; i < scaled_results.size(); i++) {
                buf.append(Constants.pairs.get(i).x+","+scaled_results.get(i));
                buf.newLine();
            }
            buf.flush();
            buf.close();
        } catch(Exception e) {
            Log.e("test",e.getMessage());
        }
    }

    public static void appendtofile(LinkedList<String> buff, String filename, String dirname) {
        try {
            String dir = av.getExternalFilesDir(null).toString();
            Log.e("ble",filename);
            File path = new File(dir);
            if (!path.exists()) {
                path.mkdir();
            }

            File file = new File(dir+File.separator+filename);
            if(!file.exists()) {
                file.createNewFile();
            }
            BufferedWriter buf = new BufferedWriter(new FileWriter(file,true));
            for (int i = 0; i < buff.size(); i++) {
                buf.append(""+buff.get(i));
                buf.newLine();
            }
            buf.flush();
            buf.close();
        } catch(Exception e) {
            Log.e("test",e.getMessage());
        }
    }

    public static void appendtofile(String buff, String filename, String dirname) {
        try {
            String dir = av.getExternalFilesDir(null).toString();
            Log.e("ble",filename);
            File path = new File(dir);
            if (!path.exists()) {
                path.mkdir();
            }

            File file = new File(dir+File.separator+filename);
            if(!file.exists()) {
                file.createNewFile();
            }
            BufferedWriter buf = new BufferedWriter(new FileWriter(file,true));
            buf.append(buff);
            buf.newLine();
            buf.flush();
            buf.close();
        } catch(Exception e) {
            Log.e("test",e.getMessage());
        }
    }

    public static double[] readfromfile(String filename) {
        LinkedList<Double> ll = new LinkedList<Double>();

        try {
            String dir = av.getExternalFilesDir(null).toString();
            File file = new File(dir + File.separator + filename);
            Log.e("ble","file "+filename+","+file.exists()+"");
            BufferedReader buf = new BufferedReader(new FileReader(file));

            String line;
            while ((line = buf.readLine()) != null && line.length() != 0) {
                ll.add(Double.parseDouble(line));
            }

            buf.close();
        } catch (Exception e) {
            Log.e("ble","readfromfile failed "+filename);
            Log.e("ble",e.getMessage());
        }

        double[] ar = new double[ll.size()];
        int counter = 0;
        for (Double d : ll) {
            ar[counter++] = d;
        }
        ll.clear();
        return ar;
    }

    public static short[] readfromfile_short(String filename) {
        LinkedList<Short> ll = new LinkedList<Short>();

        try {
            String dir = av.getExternalFilesDir(null).toString();
            File file = new File(dir + File.separator + filename);
            Log.e("ble","file "+filename+","+file.exists()+"");
            BufferedReader buf = new BufferedReader(new FileReader(file));

            String line;
            while ((line = buf.readLine()) != null && line.length() != 0) {
                ll.add(Short.parseShort(line));
            }

            buf.close();
        } catch (Exception e) {
            Log.e("ble",e.getMessage());
        }

        short[] ar = new short[ll.size()];
        int counter = 0;
        for (Short d : ll) {
            ar[counter++] = d;
        }
        ll.clear();
        return ar;
    }

    public static int[] readfromfile_int(String filename) {
        LinkedList<Integer> ll = new LinkedList<Integer>();

        try {
            String dir = av.getExternalFilesDir(null).toString();
            File file = new File(dir + File.separator + filename);
            Log.e("ble","file "+filename+","+file.exists()+"");
            BufferedReader buf = new BufferedReader(new FileReader(file));

            String line;
            while ((line = buf.readLine()) != null && line.length() != 0) {
                ll.add(Integer.parseInt(line));
//                Log.e("asdf",line);
            }

            buf.close();
        } catch (Exception e) {
            Log.e("ble",e.getMessage());
        }

        int[] ar = new int[ll.size()];
        int counter = 0;
        for (Integer d : ll) {
            ar[counter++] = d;
        }
        ll.clear();
        return ar;
    }

    public static long[] readfromsealfile(String filename) {
        LinkedList<Long> ll = new LinkedList<Long>();
        long mintime=0,maxtime=0,endtime=0,sealtime=0,middletime=0,motortime=0;
        long gotseal_motor=0, gotseal_turns=0, end_motor=0, end_turn=0;
        long max_pindex=0,min_pindex=0,seal_pindex=0;
        try {
            String dir = av.getExternalFilesDir(null).toString();
            File file = new File(dir + File.separator + filename);
            Log.e("ble","file "+filename+","+file.exists()+"");
            BufferedReader buf = new BufferedReader(new FileReader(file));

            String line;
            while ((line = buf.readLine()) != null && line.length() != 0) {
                if (line.contains("mintime")) {
                    mintime=Long.parseLong(line.substring(8,line.length()));
                }
                else if(line.contains("maxtime")) {
                    maxtime=Long.parseLong(line.substring(8,line.length()));
                }
                else if (line.contains("endtime")) {
                    endtime=Long.parseLong(line.substring(8,line.length()));
                }
                else if (line.contains("sealtime")) {
                    sealtime=Long.parseLong(line.substring(9,line.length()));
                }
                else if (line.contains("middletime")) {
                    middletime=Long.parseLong(line.substring(11,line.length()));
                }
//                else if (line.contains("motor")) {
//                    motortime=(long)Double.parseDouble(line.substring(6,line.length()));
//                }
                else if (line.contains("gotseal-motor")) {
                    gotseal_motor=Long.parseLong(line.substring(14,line.length()));
                }
                else if (line.contains("gotseal-turns")) {
                    gotseal_turns=Long.parseLong(line.substring(14,line.length()));
                }
                else if (line.contains("end-motor")) {
                    end_motor=Long.parseLong(line.substring(10,line.length()));
                }
                else if (line.contains("end-turns")) {
                    end_turn=Long.parseLong(line.substring(10,line.length()));
                }
                else if (line.contains("max-pindex")) {
                    max_pindex=Long.parseLong(line.substring(11,line.length()));
                }
                else if (line.contains("min-pindex")) {
                    min_pindex=Long.parseLong(line.substring(11,line.length()));
                }
                else if (line.contains("seal-pindex")) {
                    seal_pindex=Long.parseLong(line.substring(12,line.length()));
                }
//                else {
//                    ll.add(Long.parseLong(line));
//                }
            }

            buf.close();
        } catch (Exception e) {
            Log.e("ble","REALFROMSEALFILE "+e.getMessage());
        }

        return new long[]{mintime,maxtime,endtime,sealtime,middletime,motortime, gotseal_motor,gotseal_turns,end_motor,end_turn,seal_pindex,max_pindex,min_pindex};
    }

    public static ArrayList<Double[]> readfromfile2(String filename) {
        LinkedList<Double> ll1 = new LinkedList<Double>();
        LinkedList<Double> ll2 = new LinkedList<Double>();

        try {
            String dir = av.getExternalFilesDir(null).toString();
            File file = new File(dir + File.separator + filename);
            Log.e("ble","filename "+","+file.exists()+"");
            BufferedReader buf = new BufferedReader(new FileReader(file));

            String line;
            while ((line = buf.readLine()) != null && line.length() != 0) {
                String[]elts=line.split(",");
                ll1.add(Double.parseDouble(elts[0]));
                ll2.add(Double.parseDouble(elts[1]));
            }

            buf.close();
        } catch (Exception e) {
            Log.e("ble",e.getMessage());
        }

        ArrayList<Double[]> out=new ArrayList<Double[]>();

        Double[] ar1 = new Double[ll1.size()];
        int counter = 0;
        for (Double d : ll1) {
            ar1[counter++] = d;
        }
        ll1.clear();
        out.add(ar1);

        Double[] ar2 = new Double[ll2.size()];
        counter = 0;
        for (Double d : ll2) {
            ar2[counter++] = d;
        }
        ll2.clear();
        out.add(ar2);

        return out;
    }
}
