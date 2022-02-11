package com.example.root.tymp;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

import com.google.android.material.chip.Chip;
import com.jjoe64.graphview.GraphView;
import com.jjoe64.graphview.series.DataPoint;
import com.jjoe64.graphview.series.LineGraphSeries;
import com.jjoe64.graphview.series.PointsGraphSeries;

import java.util.LinkedList;

public class MeasureFragment extends Fragment {
    TextView fnameView;
    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_measure, container, false);
        if (((MainActivity) getActivity()).getSupportActionBar()!=null) {
            ((MainActivity) getActivity()).getSupportActionBar().setTitle("Measure");
        }

        fnameView = (TextView) view.findViewById(R.id.fnameView);
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(getActivity());
        SharedPreferences.Editor editor = PreferenceManager.getDefaultSharedPreferences(getActivity()).edit();

        EditText pidVal = view.findViewById(R.id.pidVal);
        pidVal.addTextChangedListener(new TextWatcher() {

            @Override
            public void afterTextChanged(Editable s) {
            }

            @Override
            public void beforeTextChanged(CharSequence s, int start,
                                          int count, int after) {
            }

            @Override
            public void onTextChanged(CharSequence s, int start,
                                      int before, int count) {
                String ss =pidVal.getText().toString();;
                if (ss.length()>0) {
                    editor.putInt("pid", Integer.parseInt(ss));
                    editor.commit();
                }
            }
        });

        int pid = prefs.getInt("pid",0);
        pidVal.setText(pid+"");

        pidVal.setSelectAllOnFocus(true);

        ImageView upButton = (ImageView)view.findViewById(R.id.imageView1);
        upButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String ss = pidVal.getText().toString();
                if (ss.length() == 0) {
                    pidVal.setText(prefs.getInt("pid",0)+"");
                }
                else {
                    int pid = Integer.parseInt(ss);
                    pidVal.setText((pid + 1) + "");
                    editor.putInt("pid", (pid + 1));
                    editor.commit();
                }
            }
        });
        ImageView downButton = (ImageView)view.findViewById(R.id.imageView2);
        downButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String ss = pidVal.getText().toString();
                int pid = Integer.parseInt(ss);
                if (ss.length() == 0) {
                    pidVal.setText(prefs.getInt("pid",0)+"");
                }
                else {
                    if (pid > 0) {
                        pidVal.setText((pid - 1) + "");
                        editor.putInt("pid",(pid-1));
                        editor.commit();
                    }
                }
            }
        });

        GraphView graph = (GraphView)view.findViewById(R.id.graph);
        ImageView backwardButton = (ImageView)view.findViewById(R.id.backwardButton);
        ImageView forwardButton = (ImageView)view.findViewById(R.id.forwardButton);
        TextView counterView = (TextView)view.findViewById(R.id.counterView);

        backwardButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                getActivity().runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        graph.removeAllSeries();
                    }
                });

                DataProc.series = null;
                String fname="";
                if (Constants.files == null) {
                    Constants.files = FileOperations.getFileList();
                    if (Constants.files.size()>0) {
                        Constants.fileNamePointer = Constants.files.get(Constants.files.size() - 1);
                        fname = Constants.fileNamePointer;
                    }
                    Log.e("asdf","BACKWARD "+Constants.fileNamePointer+"-"+Constants.files.size());
                    counterView.setText(Constants.files.size()+"/"+Constants.files.size());
                    forwardButton.setEnabled(false);
                    forwardButton.setImageAlpha((int)(.5*255));
                }
                else if (Constants.files.size() > 0){
                    int idx = Constants.files.indexOf(Constants.fileNamePointer);
                    forwardButton.setEnabled(true);
                    forwardButton.setImageAlpha((int)(1*255));
                    if (idx > 0) {
                        fname = Constants.files.get(idx - 1);
                        Constants.fileNamePointer = fname;
                        idx -= 1;
                    }
                    if (idx == 0) {
                        backwardButton.setEnabled(false);
                        backwardButton.setImageAlpha((int)(.5*255));
                    }
                    Log.e("asdf","BACKWARD "+Constants.fileNamePointer+"-"+idx);
                    counterView.setText((idx+1)+"/"+Constants.files.size());
                }
                if (fname.length() > 0) {
                    process(view, fname);
                }
            }
        });

        forwardButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                getActivity().runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        graph.removeAllSeries();
                    }
                });

                DataProc.series = null;
                String fname="";
                if (Constants.files == null) {
                    Constants.files = FileOperations.getFileList();
                    if (Constants.files.size()>0) {
                        Constants.fileNamePointer = Constants.files.get(Constants.files.size() - 1);
                        fname = Constants.fileNamePointer;
                    }
                    Log.e("asdf","FORWARD "+Constants.fileNamePointer+"-"+Constants.files.size());
                    counterView.setText(Constants.files.size()+"/"+Constants.files.size());
                    forwardButton.setEnabled(false);
                    forwardButton.setImageAlpha((int)(.5*255));
                }
                else if (Constants.files.size() > 0){
                    backwardButton.setEnabled(true);
                    backwardButton.setImageAlpha((int)(1*255));
                    int idx = Constants.files.indexOf(Constants.fileNamePointer);
                    if (idx < Constants.files.size()-1) {
                        fname = Constants.files.get(idx + 1);
                        Constants.fileNamePointer = fname;
                        idx += 1;
                    }
                    if (idx == Constants.files.size()-1) {
                        forwardButton.setEnabled(false);
                        forwardButton.setImageAlpha((int)(.5*255));
                    }
                    Log.e("asdf","FORWARD "+Constants.fileNamePointer+"-"+idx);
                    counterView.setText((idx+1)+"/"+Constants.files.size());
                }

                if (fname.length() > 0) {
                    process(view, fname);
                }
            }
        });
        return view;
    }

    public void process(View view, String fname) {
        TextView fnameView = (TextView) view.findViewById(R.id.fnameView);
        String[] elts = Constants.fileNamePointer.split("-");
        String ts = elts[elts.length - 1];
        String tsout = ts.substring(0, ts.length() - 4) + "-" + ts.substring(ts.length() - 4, ts.length());
        fnameView.setText(tsout);
        EditText pidVal = (EditText) view.findViewById(R.id.pidVal);
        String pid = elts[0];
        pidVal.setText(pid);
        TextView statsview = (TextView) view.findViewById(R.id.statsView);
        statsview.setText(String.format("daPa: err\nml: err\nV_ea: err"));

        Chip leftChip = view.findViewById(R.id.leftChip);
        Chip rightChip = view.findViewById(R.id.rightChip);

        if (Constants.fileNamePointer.contains("left")) {
            leftChip.setSelected(true);
            leftChip.setChecked(true);
            rightChip.setSelected(false);
            rightChip.setChecked(false);
        }
        else {
            leftChip.setSelected(false);
            leftChip.setChecked(false);
            rightChip.setSelected(true);
            rightChip.setChecked(true);
        }
        Constants.pairs = null;
        try {
            Constants.pairs = DataProc.processdatawrapper(null, fname);
            Log.e("logme","pairs generated for "+fname+" "+Constants.pairs.get(0).x+","+Constants.pairs.get(0).y);
        } catch (Exception e) {
            if (e.getMessage().contains("Error with ") && !Constants.tympAborted) {
                if (Constants.notifType.equals("toast")) {
                    Utils.mkSnack(getActivity(), getActivity().findViewById(R.id.layout), e.getMessage());
                }
                else {
                    Utils.mkToast(getActivity(), e.getMessage());
                }
            }
        }

        if (Constants.pairs != null) {
            Log.e("logme", "DISPLAY " + fname);
            DataProc.processdatawrapperUI(Constants.pairs, fname);
        }
    }

    @Override
    public void onResume() {
        super.onResume();
        Log.e("ble","MeasureFragment - onresume");
        Constants.CurrentFragment = Constants.MeasureFragment;
        Constants.MeasureFragment = this;
    }

    @Override
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
        if (Constants.filename!=null) {
            TextView et = (TextView) getActivity().findViewById(R.id.fnameView);
            String tss=Constants.filename.split("-")[2];
            String ss = tss.substring(0, tss.length() - 4) + "-" + tss.substring(tss.length() - 4, tss.length());
            et.setText(ss);
        }

        Log.e("ble","MeasureFragment - onactivitycreated");
        Constants.CurrentFragment = Constants.MeasureFragment;
        Constants.MeasureFragment = this;

        final Button mb = (Button)getActivity().findViewById(R.id.measureButton);

        GraphView graph = (GraphView)getActivity().findViewById(R.id.graph);
        if (graph!=null) {
            new DataProc(graph, Constants.samplingFreq, getActivity());
        }

        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(getActivity());

        Utils.graphme(graph,new LineGraphSeries<DataPoint>(),new PointsGraphSeries< DataPoint >(),
                Integer.parseInt(prefs.getString("minpressure",Constants.minp)),
                Integer.parseInt(prefs.getString("maxpressure",Constants.maxp))+50,-1,1.5);

        long ts = System.currentTimeMillis();
        String fname=null;

        try {
            LinkedList<String> names = new LinkedList<>();
            LinkedList<Long>times=new LinkedList<>();
            for (String s : names) {
                fname=s;
                Constants.pairs = DataProc.processdatawrapper(null, s);

                double unscaled_vea = Constants.pairs.get(Constants.pairs.size()-1).y;
                LinkedList<Double>scaled_results=new LinkedList<>();
                for(int j = 0; j < Constants.pairs.size(); j++) {
                    double y = Constants.pairs.get(j).y;
                    String[] elts = s.split("-");
                    int pid = Integer.parseInt(elts[0]);
                    scaled_results.add(y);
                }
                Log.e("logme",s);
                FileOperations.writetofile("curve-"+s+".txt",scaled_results);
            }
        }
        catch(Exception e) {
            if (e.getMessage().contains("Error with ") && !Constants.tympAborted) {
                if (Constants.notifType.equals("toast")) {
                    Utils.mkToast(getActivity(), e.getMessage());
                }
                else {
                    Utils.mkSnack(getActivity(), getActivity().findViewById(R.id.layout), e.getMessage());
                }
            }
        }
        if (Constants.pairs != null) {
            Log.e("logme","DISPLAY");
            DataProc.processdatawrapperUI(Constants.pairs, fname);
        }

        if (Constants.timerVal.length()>0) {
            TextView tv9=getActivity().findViewById(R.id.textView9);
            if (tv9!=null) {
                tv9.setText(Constants.timerVal);
            }
        }

        ImageView vv = (ImageView) getActivity().findViewById(R.id.connectedView);
        Log.e("ble","statuses "+Constants.connectionStatus+","+Constants.sealStatus);
        if (vv!=null) {
            if (Constants.connectionStatus.equals("red")) {
                vv.setImageDrawable(getActivity().getDrawable(R.drawable.circle_red));
            } else if (Constants.connectionStatus.equals("yellow")) {
                vv.setImageDrawable(getActivity().getDrawable(R.drawable.circle_yellow));
            } else if (Constants.connectionStatus.equals("green")) {
                vv.setImageDrawable(getActivity().getDrawable(R.drawable.circle_green));
            }
        }

        ImageView vv2 = (ImageView) getActivity().findViewById(R.id.sealView);
        if (vv2!=null) {
            if (Constants.sealStatus.equals("red")) {
                vv2.setImageDrawable(getActivity().getDrawable(R.drawable.circle_red));
            } else if (Constants.sealStatus.equals("yellow")) {
                vv2.setImageDrawable(getActivity().getDrawable(R.drawable.circle_yellow));
            } else if (Constants.sealStatus.equals("green")) {
                vv2.setImageDrawable(getActivity().getDrawable(R.drawable.circle_green));
            }
        }

        ImageView vv3 = (ImageView) getActivity().findViewById(R.id.measuringView);
        if (vv3!=null) {
            if (Constants.measurementStatus.equals("red")) {
                vv3.setImageDrawable(getActivity().getDrawable(R.drawable.circle_red));
            } else if (Constants.measurementStatus.equals("yellow")) {
                vv3.setImageDrawable(getActivity().getDrawable(R.drawable.circle_yellow));
            } else if (Constants.measurementStatus.equals("green")) {
                vv3.setImageDrawable(getActivity().getDrawable(R.drawable.circle_green));
            }
        }

        ImageView vv4 = (ImageView) getActivity().findViewById(R.id.resetView);
        if (vv4!=null) {
            if (Constants.resetStatus.equals("red")) {
                vv4.setImageDrawable(getActivity().getDrawable(R.drawable.circle_red));
            } else if (Constants.resetStatus.equals("yellow")) {
                vv4.setImageDrawable(getActivity().getDrawable(R.drawable.circle_yellow));
            } else if (Constants.resetStatus.equals("green")) {
                vv4.setImageDrawable(getActivity().getDrawable(R.drawable.circle_green));
            }
        }

        if (mb!=null) {
            mb.setEnabled(Constants.measureButtonStatus);

            mb.setOnTouchListener(new View.OnTouchListener() {
                @Override
                public boolean onTouch(View v, MotionEvent event) {
                    Log.e("ble", "ONTOUCH " + Constants.testInProgress);
                    if (event.getAction() == MotionEvent.ACTION_UP && !Constants.testInProgress && Constants.CurrentFragment.toString().toLowerCase().contains("measure")) {
                        Constants.firstfoundseal =false;
                        ((MainActivity) getActivity()).startit(null);
                        mb.setEnabled(false);
                    }
//                else if (event.getAction() == MotionEvent.ACTION_DOWN &&) {
//                    mb.setPressed(false);
//                }
                    return true;
                }
            });
        }

        final Button sb = getActivity().findViewById(R.id.stopButton);
        if (sb!=null) {
            sb.setEnabled(Constants.stopButtonStatus);

            sb.setOnTouchListener(new View.OnTouchListener() {
                @Override
                public boolean onTouch(View v, MotionEvent event) {
                    if (event.getAction() == MotionEvent.ACTION_UP) {
                        if (Constants.testInProgress && !mb.isEnabled()) {
                            mb.setEnabled(true);
                        }
                        sb.setPressed(false);
                        ((MainActivity) getActivity()).stopit(null);
                    } else if (event.getAction() == MotionEvent.ACTION_DOWN) {
                        sb.setPressed(true);
                    }
                    return true;
                }
            });
        }
    }
}
