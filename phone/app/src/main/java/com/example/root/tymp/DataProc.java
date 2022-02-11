package com.example.root.tymp;

import android.app.Activity;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;
import android.util.Log;
import android.widget.TextView;

import com.jjoe64.graphview.GraphView;
import com.jjoe64.graphview.series.DataPoint;
import com.jjoe64.graphview.series.LineGraphSeries;
import com.jjoe64.graphview.series.PointsGraphSeries;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.LinkedList;

public class    DataProc {
    private static GraphView graph;
    private static int fs;
    static Activity av;
    public DataProc(GraphView graph, int fs, Activity av) {
        this.graph=graph;
        this.fs=fs;
        this.av=av;
    }

    public static ArrayList<PlotPair> processdatawrapper(int[] buff, String filename) throws Exception {
        Log.e("ble","processdatawrapper");
        if (filename == null) {
            filename = Constants.filename;
        }

        if (Constants.filename == null && series != null) {
//            Log.e("logme","we have already processed debug data");
            return Constants.pairs;
        }

        ArrayList<PlotPair>pairs = null;
        if (filename!=null&&filename.length()>0) {
            Log.e("ble",">>> process data wrapper "+filename);
//            Log.e("logme","UPDATING UI");
            double[] pvals = FileOperations.readfromfile("pout" + filename + ".txt");
            if (pvals!=null&&pvals.length>0) {
                double ref = pvals[0];
                pvals = Arrays.copyOfRange(pvals,1,pvals.length);
                for (int i = 0; i < pvals.length; i++) {
                    pvals[i] = pvals[i] - ref;
                }
                fixBitFlip(pvals);
            }

            int[] mvals = null;
            if (buff != null) {
//                Log.e("logme","FROM BUFFER");
                mvals = buff;
            }
            else {
//                Log.e("logme","FROM FILE");
                mvals = FileOperations.readfromfile_int("mout"+filename + ".txt");
            }

            long[] svals = FileOperations.readfromsealfile("sout"+filename + ".txt");
            Log.e("ble", "LENGTHS "+mvals.length+","+pvals.length+","+svals.length);
            Log.e("ble", "LENGTHS "+getMax(pvals)+","+getMin(pvals)+","+pvals[pvals.length-1]);
            if (mvals.length > 0 && pvals.length > 0 && svals.length > 0) {
                try {
                    pairs = processdata(filename, pvals, mvals, svals);
                }
                catch (Exception e) {
                    if (!Constants.tympAborted) {
//                    Utils.mkSnack(av, av.findViewById(R.id.layout), "Error processing signals.");
                        Log.e("ble", Utils.e2s(e));
                        throw new Exception("Error with processing signals");
                    }
                }
            }
            else {
//                Utils.mkSnack(av, av.findViewById(R.id.layout), "Error with  "+filename);
                throw new Exception("Error with "+filename);
            }
        }
        return pairs;
    }

    public static void processdatawrapperUI(ArrayList<PlotPair> pairs, String filename) {
        if (filename == null) {
            filename = Constants.filename;
        }

        if (Constants.filename == null && pairs == null) {

        }
        else {
            if (graph != null) {
                graph.removeAllSeries();
            }
            series = new LineGraphSeries<>();
            series2 = new PointsGraphSeries<>();
            dataProcessUIHelper(pairs, filename);
        }
    }

    public static void fixBitFlip(double[] pvals) {
        int counter = 0;
        int zindex=-1;
        for (Double d : pvals) {
            if (d==0&&counter!=0) {
                zindex=counter;
                break;
            }
            counter += 1;
        }

        if (zindex!=-1) {
            for (int i = zindex; i < pvals.length; i++) {
                pvals[i]+=pvals[zindex-1];
            }
        }
    }

    public static LinkedList<Integer> findpks(double[] pvals) {
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(av);
        int minp=Integer.parseInt(prefs.getString("minpressure",Constants.minp));
        int maxp=Integer.parseInt(prefs.getString("maxpressure",Constants.maxp));

        LinkedList<Integer> ppks = new LinkedList<Integer>();

        LinkedList<Integer>maxIdx=new LinkedList<Integer>();
        LinkedList<Integer>minIdx=new LinkedList<Integer>();
        for (int i = 1; i < pvals.length-1; i++) {
            if (pvals[i] > (maxp-Math.abs(pvals[0])-20) && pvals[i] >= pvals[i+1] && pvals[i] >= pvals[i-1]) {
                maxIdx.add(i);
            }
            if (pvals[i] < (minp+Math.abs(pvals[0])+20) && pvals[i] <= pvals[i+1] && pvals[i] <= pvals[i-1]) {
                minIdx.add(i);
            }
        }
        if (maxIdx.size()>0) {
            double maxdiff=0;
            int bestIdx=0;
            for (int i = 0; i < maxIdx.size(); i++) {
                double diff=Math.abs(pvals[maxIdx.get(i)-1]-pvals[maxIdx.get(i)])+Math.abs(pvals[maxIdx.get(i)+1]-pvals[maxIdx.get(i)]);
                if (diff>maxdiff) {
                    maxdiff = diff;
                    bestIdx = i;
                }
            }
            ppks.add(maxIdx.get(bestIdx));
        }
        if (minIdx.size()>0) {
            int xIndex=0;
            double xVal=0;
            for (int i = 0; i < minIdx.size(); i++) {
                if (minIdx.get(i) > ppks.getFirst() && pvals[minIdx.get(i)] < xVal) {
                    xIndex = minIdx.get(i);
                    xVal = pvals[xIndex];
                }
            }
            ppks.add(xIndex);
        }
        Log.e("ble","PKS "+ppks.toString());
        return ppks;
    }

    public static LinkedList<Integer> findpks2(double[] pvals) {
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(av);
        int minp=Integer.parseInt(prefs.getString("minpressure",Constants.minp));
        int maxp=Integer.parseInt(prefs.getString("maxpressure",Constants.maxp));

        LinkedList<Integer> ppks = new LinkedList<Integer>();

        LinkedList<Integer>maxIdx=new LinkedList<Integer>();
        LinkedList<Integer>minIdx=new LinkedList<Integer>();
        for (int i = 1; i < pvals.length-1; i++) {
            if (pvals[i] > (maxp-20) && pvals[i] >= pvals[i+1] && pvals[i] >= pvals[i-1]) {
                maxIdx.add(i);
            }
            if (pvals[i] < (minp+20) && pvals[i] <= pvals[i+1] && pvals[i] <= pvals[i-1]) {
                minIdx.add(i);
            }
        }
        if (maxIdx.size()>0) {
//            double maxdiff=0;
//            int bestIdx=0;
//            for (int i = 0; i < maxIdx.size(); i++) {
//                double diff=Math.abs(pvals[maxIdx.get(i)-1]-pvals[maxIdx.get(i)])+Math.abs(pvals[maxIdx.get(i)+1]-pvals[maxIdx.get(i)]);
//                if (diff>maxdiff) {
//                    maxdiff = diff;
//                    bestIdx = i;
//                }
//            }
//            ppks.add(maxIdx.get(bestIdx));
            ppks.add(maxIdx.getLast());
        }
        if (minIdx.size()>0) {
//            int xIndex=0;
//            double xVal=0;
//            for (int i = 0; i < minIdx.size(); i++) {
//                if (minIdx.get(i) > ppks.getFirst() && pvals[minIdx.get(i)] < xVal) {
//                    xIndex = minIdx.get(i);
//                    xVal = pvals[xIndex];
//                }
//            }
//            ppks.add(xIndex);
            ppks.add(minIdx.getFirst());
        }
        Log.e("ble","PKS "+ppks.toString());
        return ppks;
    }

    public static int zeroCross(double[] pvals) {
        double minval=pvals[0];
        int minidx=0;
        for (int i = 0; i < pvals.length; i++) {
            if (Math.abs(pvals[i]) < minval) {
                minval=Math.abs(pvals[i]);
                minidx=i;
            }
        }
        return minidx;
    }

    public static long parseFname(String ss) {
        String[] elts=ss.split("-");
        String elt=elts[2];
        return Long.parseLong(elt);
    }

    public static HashMap<Integer,Double> genEdges() {
        HashMap<Integer,Double> edges = new HashMap<Integer,Double>();
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(av);
        int maxp=Integer.parseInt(prefs.getString("maxpressure",Constants.maxp));
        int minp=Integer.parseInt(prefs.getString("minpressure",Constants.minp));

        int num = minp-50;
        int range = maxp-num;
        int steps = range/5;

//        Log.e("ble","GEN EDGES "+range+","+steps+","+num);
        for (int i = 0; i <= steps; i++) {
            edges.put(num,0.0);
            num+=5;
//            Log.e("ble","edges "+num);
        }
        return edges;
    }

    public static double[] movingaverage(double[] spectrum, int windowsize) {
        int counter = 0;
        double[] movmean=new double[spectrum.length];
        try {
            double accum=0;
            int ws=2;
            for (int j = 0; j < ws; j++) {
                accum+=spectrum[j];
            }

            for (int i = 0; i < windowsize-2; i++) {
                movmean[counter++]=accum/ws;
                accum+=spectrum[ws];
                ws+=1;
            }

            int toDecrement=0;
            movmean[counter++]=accum/windowsize;
            for (int i = 1; i < spectrum.length-windowsize+1; i++) {
                accum-=spectrum[toDecrement];
                toDecrement+=1;
                accum+=spectrum[toDecrement+windowsize-1];
                movmean[counter++]=accum/windowsize;
            }
            accum-=spectrum[toDecrement];
            movmean[counter]=accum/(windowsize-1);
        }
        catch (Exception e) {
            e.printStackTrace();
        }

        return movmean;
    }

    public static ArrayList<PlotPair> discretize(double[] pvals,double[] mags) {
        HashMap<Integer,Double> edges = genEdges();

        ArrayList<Integer>keys=new ArrayList<Integer>(edges.keySet());
        Collections.sort(keys);
        
        for(int i = 0; i < pvals.length; i++) {
            pvals[i] = Math.round((double) pvals[i] / 5) * 5;
        }

        for (int i = 0; i < pvals.length; i++) {
            edges.put((int)pvals[i],mags[i]);
        }

        for (int i = 1; i < keys.size()-1; i++) {
            if (edges.get(keys.get(i)) == 0) {
                if (edges.get(keys.get(i+1)) != 0) {
                    edges.put(keys.get(i), edges.get(keys.get(i+1)));
                }
                else {
                    edges.put(keys.get(i), edges.get(keys.get(i-1)));
                }
            }
        }

//        int bound1 = 14;
//        int bound2 = keys.size()-7;
        int bound1 = 10;
        int bound2 = keys.size();
        int[] nkeys = new int[bound2-bound1];
        int counter=0;
        for(int i = bound1; i < bound2; i++) {
            nkeys[counter++]=keys.get(i);
        }

        double[] nvalues = new double[nkeys.length];
        for (int i = 0; i < nkeys.length; i++) {
            nvalues[i] = edges.get(nkeys[i]);
        }

        nvalues = movingaverage(nvalues,Constants.movefac);
//        nvalues = downsamp(nvalues,Constants.downfac);
//        nkeys = downsamp(nkeys,Constants.downfac);

        ArrayList<PlotPair> pairs = new ArrayList<>();
        for (int i = 0; i < nkeys.length; i++) {
            pairs.add(new PlotPair(nkeys[i],nvalues[i]));
        }
        return pairs;
    }

    public static ArrayList<PlotPair> collate(double[] pvals,double[] mags) {
        ArrayList<PlotPair> pairs = new ArrayList<>();
        for (int i = 0; i < pvals.length; i++) {
            pairs.add(new PlotPair(pvals[i],mags[i]));
        }
        return pairs;
    }

    public static double[] getMags(double[] pvals, int[] mvals, int fftwin, int binloc) {
        int micValsPerPressureBucket = (int) (((double) mvals.length / Constants.samplingFreq / pvals.length) * Constants.samplingFreq) - 1;
        Log.e("ble", "micValsPerPressureBucket " + mvals.length + "," + pvals.length + "," + micValsPerPressureBucket);
        double[] mags = new double[pvals.length];
        for (int i = 0; i < pvals.length; i += 1) {
            int[] mval = Arrays.copyOfRange(mvals, (int) micValsPerPressureBucket * i, (int) micValsPerPressureBucket * (i + 1));
            String ss = Arrays.toString(mval);
//            Log.e("ble","mval len "+mval.length);
//            Log.e("ble","fftwin "+fftwin);
            double[] spec = fftnative(mval, fftwin);
//            double mag = 1.0/(20 * Math.log10(spec[binloc]));
            mags[i] = 1.0 / spec[binloc];
        }
        return mags;
    }

    public static double[] joinArray(double[] a1, double[] a2) {
        double[] out = new double[a1.length+a2.length];
        int counter=0;
        for(int i = 0; i < a1.length; i++) {
            out[counter++] = a1[i];
        }
        for(int i = 0; i < a2.length; i++) {
            out[counter++] = a2[i];
        }
        return out;
    }

    public static int[] getMinMaxTimes(int sealtime, int endtime, double[] pvals, int pk1, int pk2) {
        int maxtime=(int)(((endtime-sealtime)*((double)pk1/pvals.length))+sealtime);
        int mintime=(int)(((endtime-sealtime)*((double)pk2/pvals.length))+sealtime);
        Log.e("ble","maxmin "+maxtime+","+mintime);
        return new int[]{maxtime,mintime};
    }

    public static int[] findSyncTimes(int[] mvals, int sealtime, int endtime) {
        int divs = 20;
        int seglen = Constants.samplingFreq/divs;
        int numsegs = mvals.length/seglen;
        int ss=0;

        int sealstart=-1;
        int sealend=-1;

        Log.e("ble","synctime init "+sealtime+","+endtime);

        sealtime=(int)(sealtime-(0.2*Constants.samplingFreq));
        int startbin = sealtime/seglen;

        int startspan = (int)(0.4*Constants.samplingFreq)/seglen;
        Log.e("ble","startspan "+startspan);

//        String rep="";
        LinkedList<Double> pows = new LinkedList<Double>();
        for (int i = 0; i < numsegs; i++) {
            int[] seg = Arrays.copyOfRange(mvals, ss, ss + seglen);
            double[] spec = fftnative(seg, Constants.samplingFreq);
            double pow = spec[27];
//            rep+=pow+",";
            pows.add(pow);
            ss += seglen;
        }

        for (int i = startbin; i < startbin+startspan; i++) {
            if (pows.get(i) > Constants.MotorNoiseThreshold) {
                sealstart = i;
                Log.e("ble", "synctime begin " + i);
                break;
            }
        }

        for (int i = numsegs-1; i >= sealstart; i--) {
            if (pows.get(i) > Constants.MotorNoiseThreshold) {
                sealend = i;
                Log.e("ble","synctime end "+i);
                break;
            }
        }

        if (sealstart != sealtime) {
            sealstart *= seglen;
        }
        else {
            sealstart = sealtime;
        }

        if (sealend != endtime) {
            sealend *= seglen;
        }
        else {
            sealend = endtime;
        }

        Log.e("ble","SYNCTIMES "+sealstart+","+sealend);

        return new int[]{sealstart,sealend};
    }

    public static double getMin(double[] vals) {
        double min = 1000;
        for (Double d : vals) {
            if (d < min) {
                min = d;
            }
        }
        return min;
    }

    public static double getMax(double[] vals) {
        double max = -1000;
        for (Double d : vals) {
            if (d > max) {
                max = d;
            }
        }
        return max;
    }

    public static int getMaxInd(double[] vals) {
        double max = -1000000;
        int ind=0;
        for (int i = 0; i < vals.length; i++) {
            if (vals[i] > max) {
                max=vals[i];
                ind=i;
            }
        }
        return ind;
    }

    public static int crossing(double[] pvals, int num) {
        double diff=1000;
        int idx=0;
        for (int i = 0; i < pvals.length; i++) {
            if (Math.abs(pvals[i]-num) < diff) {
                diff=Math.abs(pvals[i]-num);
                idx=i;
            }
        }
        return idx;
    }

    public static ArrayList<PlotPair> processdata(String filename, double[] pvals, int[] mvals, long[] svals) {
        Log.e("ble","processData");

        long fname = parseFname(filename);

        // smartphone, locally timestamped events
        long mintime = (long)(((svals[0]-fname)/1e3)*Constants.samplingFreq);
        long maxtime = (long)(((svals[1]-fname)/1e3)*Constants.samplingFreq);
        long endtime = (long)(((svals[2]-fname)/1e3)*Constants.samplingFreq);
        long sealtime = (long)(((svals[3]-fname)/1e3)*Constants.samplingFreq);
        long middletime = (long)(((svals[4]-fname)/1e3)*Constants.samplingFreq);

        long motortime = svals[5];
        long motor_seal = svals[6];
        long motor_turns = svals[7];
        long end_seal = svals[8];
        long end_turns = svals[9];
        long seal_pindex = svals[10];

        long max_pindex = svals[11]-seal_pindex;
        long min_pindex = svals[12]-seal_pindex;

        maxtime -= Constants.BLUETOOTH_LATENCY;
        mintime -= Constants.BLUETOOTH_LATENCY;

        double dur = (svals[0]-svals[1])/1e3;
        double speed = Math.abs(getMin(pvals)-getMax(pvals))/dur;
        Log.e("DEBUG", "DEBUG SPEED " +(int)speed);

        int fftwin=Constants.samplingFreq;
        double freqbinsize=fs/fftwin;
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(av);
        int toneFreq=Integer.parseInt(prefs.getString("tone",Constants.defaultTone));
        int binloc=(int)(toneFreq/freqbinsize);

        int pk1 = (int)max_pindex;
        int pk2 = (int)min_pindex;

        if (Constants.syncMethod == 1) {
            //MOTOR NOISE METHOD
            int[] syncTimes = findSyncTimes(mvals, (int) sealtime, (int) endtime);
            int[] minMaxTimes = getMinMaxTimes(syncTimes[0], syncTimes[1], pvals, pk1, pk2);
            mvals = Arrays.copyOfRange(mvals, (int) minMaxTimes[0], (int) minMaxTimes[1]);
        }
        else if (Constants.syncMethod == 2) {
            // BLE, max,mintime method
            Log.e("ble","sync2 "+mvals.length+","+maxtime+","+mintime);
            mvals = Arrays.copyOfRange(mvals, (int) maxtime, (int) mintime);
        }
        else if (Constants.syncMethod == 3) {
            //BLE of seal/endtime, calculate max and min
            int repfac = (int) ((endtime - sealtime) / pvals.length);
            int maxpt = (int) (repfac * pk1 + sealtime);
            int minpt = (int) (repfac * pk2 + sealtime);
            mvals = Arrays.copyOfRange(mvals, maxpt, minpt);
        }

        Log.e("debug",pvals.length+","+pk1+","+pk2);
        pvals = Arrays.copyOfRange(pvals, pk1, pk2 + 1);
        Log.e("debug",pvals.length+"");

        int zerocross = zeroCross(pvals);
        Log.e("ble","ZEROCROSS "+zerocross);

        double[] mags = new double[1];
        if (Constants.TWO_DIVISIONS) {
            if (middletime-maxtime < 0) {
                middletime = (mintime-maxtime)/2;
            }
            else {
                middletime -= maxtime;
            }
            int[] mvals1 = Arrays.copyOfRange(mvals, (int) 0, (int) middletime);
            int[] mvals2 = Arrays.copyOfRange(mvals, (int) middletime, mvals.length);
            double[] pvals1 = Arrays.copyOfRange(pvals, 0, zerocross+1);
            double[] pvals2 = Arrays.copyOfRange(pvals, zerocross+1, pvals.length);
            double[] mag1=getMags(pvals1,mvals1,fftwin,binloc);
            double[] mag2=getMags(pvals2,mvals2,fftwin,binloc);
            mags = joinArray(mag1,mag2);
        }
        else {
            Log.e("ble", "PEAKS " + pk1 + "," + pk2);
            Log.e("ble", "VALS " + fname + "," + sealtime + "," + maxtime + "," + mintime + "," + endtime);
            Log.e("ble", "PKS " + pk1 + "," + pk2);
            mags = getMags(pvals, mvals, fftwin, binloc);
        }
        int maxind=getMaxInd(mags);
        double maxp=pvals[maxind];

        Constants.pairs = discretize(pvals,mags);

        Log.e("ble",">>> done adding plot pairs");

        Collections.sort(Constants.pairs);

        String xx="";
        String yy="";
        for (PlotPair p : Constants.pairs) {
            xx+=p.x+",";
            yy+=p.y+",";
        }

        mvals = null;
        pvals = null;

        return Constants.pairs;
    }

    private static void dataProcessUIHelper(ArrayList<PlotPair> pairs, String filename) {
        double min=999999;
        double max=-99999;
        int maxloc=0;

        String xs="";
        String ys="";

        double unscaled_vea = pairs.get(pairs.size()-1).y;

        String[] elts = filename.split("-");
        int pid = Integer.parseInt(elts[0]);

        double vea=0;
        vea=poly_scale(unscaled_vea);

        Log.e("ble","unscaled_vea: "+unscaled_vea);
        Log.e("ble","scaled_vea: "+vea);

        LinkedList<Double>unscaled_results = new LinkedList<Double>();
        LinkedList<Double>scaled_results = new LinkedList<Double>();
        for(int j = 0; j < pairs.size(); j++) {
            double xx = pairs.get(j).x;
            double y = pairs.get(j).y;

            double yy=0;
            yy = poly_scale(y)-vea;
            unscaled_results.add(y);
            scaled_results.add(poly_scale(y));

            xs+=xx+",";
            ys+=yy+",";
            DataPoint dp = new DataPoint(xx,yy);
            series.appendData(dp, true, 200);
            series2.appendData(dp, true, 200);
            if (yy > max && xx > -300) {
                max = yy;
                maxloc = (int)xx;
            }
            if (yy < min) {
                min = yy;
            }
        }
        TextView statsview = (TextView) av.findViewById(R.id.statsView);
        if (statsview!=null) {
            statsview.setText(String.format("Peak admittance: %e ml\nEar canal volume: %e ml\nPeak pressure: %d daPa", max, vea, maxloc));
        }
        Log.e("results",String.format("%s,%d,%.2f,%f",filename,maxloc,max,vea));

        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(av);
        int minp=Integer.parseInt(prefs.getString("minpressure",Constants.minp));
        int maxp=Integer.parseInt(prefs.getString("maxpressure",Constants.maxp));

        Utils.graphme(graph, series, series2, -400, 200, Math.min(min, -.5), Math.max(1.5, max));

        Constants.lastChirpTs = Long.parseLong(elts[2]);

        System.gc();
    }

    public static double poly_scale(double x) {
        double[] coeffs = new double[] {
                0.2077,   -3.0709,    6.0118, 0
        };
        double init = 1.0493e-07;
        double mmax=6.3696e-07;

        double normed = (x-init)/mmax;
        double out = 0;
        for (int i = 0; i < coeffs.length; i++) {
            out += coeffs[i]*Math.pow(normed,(coeffs.length-1-i));
        }

        return out;
    }

    static LineGraphSeries<DataPoint> series;
    static PointsGraphSeries<DataPoint> series2;

    public static native double[] fftnative(int data[], int N);
}
