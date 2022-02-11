package com.example.root.tymp;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.RadioButton;
import android.widget.RadioGroup;

import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

public class SettingsFragment extends Fragment {
    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_settings, container, false);
        if(((MainActivity) getActivity()).getSupportActionBar()!=null) {
            ((MainActivity) getActivity()).getSupportActionBar().setTitle("Settings");
        }
        return view;
    }

    @Override
    public void onResume() {
        super.onResume();
        Constants.CurrentFragment = Constants.SettingsFragment;
        Constants.SettingsFragment = this;
    }

    public void init() {
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(getActivity());
        SharedPreferences.Editor editor = prefs.edit();

        if (!prefs.contains("tone")) {
            editor.putString("tone",Constants.defaultTone);
        }if (!prefs.contains("vol")) {
            editor.putString("vol",Constants.volume+"");
        }if (!prefs.contains("minpressure")) {
            editor.putString("minpressure",Constants.minp);
        }if (!prefs.contains("maxpressure")) {
            editor.putString("maxpressure",Constants.maxp);
        }if (!prefs.contains("pspeed")) {
            editor.putString("pspeed",Constants.speed);
        }if (!prefs.contains("sealcheck")) {
            editor.putString("sealcheck",Constants.sealCheck);
        }if (!prefs.contains("resetSteps")) {
            editor.putString("resetSteps",Constants.resetSteps);
        }if (!prefs.contains("settlingTime")) {
            editor.putString("settlingTime",Constants.settlingTime);
        }if (!prefs.contains("microsteps")) {
            editor.putString("microsteps",Constants.microsteps);
        }if (!prefs.contains("measurements")) {
            editor.putString("measurements",Constants.numMeasurements);
        }if (!prefs.contains("micthresh")) {
            editor.putInt("micthresh",Constants.SEAL_CHECK_THRESH);
        }if (!prefs.contains("occlusionthresh")) {
            editor.putInt("occlusionthresh",Constants.SEAL_OCCLUSION_THRESH);
        }if (!prefs.contains("recordDuration")) {
            editor.putInt("recordDuration",Constants.recordDurationInSeconds);
        }if (!prefs.contains("mic")) {
            editor.putBoolean("mic",Constants.mic);
        }if (!prefs.contains("speaker")) {
            editor.putBoolean("speaker",Constants.speaker);
        }if (!prefs.contains("movetoinit")) {
            editor.putBoolean("movetoinit",Constants.moveToInit);
        }if (!prefs.contains("motorFastSpeed")) {
            editor.putInt("motorFastSpeed",Constants.motorFastSpeed);
        }
        editor.commit();
    }

    @Override
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
        init();
        Constants.CurrentFragment = Constants.SettingsFragment;
        Constants.SettingsFragment = this;

        final EditText et2=getActivity().findViewById(R.id.toneFrequencyVal);
        final EditText et3=getActivity().findViewById(R.id.toneVolumeVal);
        final EditText et4=getActivity().findViewById(R.id.minPressureVal);
        final EditText et5=getActivity().findViewById(R.id.maxPressureVal);
        final EditText et6=getActivity().findViewById(R.id.pressureSpeedVal);
        final EditText et8=getActivity().findViewById(R.id.sealThresholdVal);
        final EditText et9=getActivity().findViewById(R.id.resetStepsVal);
        final EditText et10=getActivity().findViewById(R.id.settlingTimeVal);
        final EditText et11=getActivity().findViewById(R.id.microstepsVal);
        final EditText et12=getActivity().findViewById(R.id.measurementVal);
        final EditText et13=getActivity().findViewById(R.id.micThreshVal);
        final EditText et14=getActivity().findViewById(R.id.occlusionThreshVal);
        final EditText et15=getActivity().findViewById(R.id.recordDurationVal);
        final EditText et16=getActivity().findViewById(R.id.motorFastSpeedVal);

        final CheckBox cb1=getActivity().findViewById(R.id.micCheckbox);
        final CheckBox cb2=getActivity().findViewById(R.id.speakerCheckbox);
        final CheckBox cb3=getActivity().findViewById(R.id.moveToInitBox);

        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(getActivity());

        et2.setText(prefs.getString("tone",Constants.defaultTone));
        et3.setText(prefs.getString("vol",Constants.volume+""));
        et4.setText(prefs.getString("minpressure",Constants.minp));
        et5.setText(prefs.getString("maxpressure",Constants.maxp));
        et6.setText(prefs.getString("pspeed",Constants.speed));
        et8.setText(prefs.getString("sealcheck",Constants.sealCheck));
        et9.setText(prefs.getString("resetSteps",Constants.resetSteps));
        et10.setText(prefs.getString("settlingTime",Constants.settlingTime));
        et11.setText(prefs.getString("microsteps",Constants.microsteps));
        et12.setText(prefs.getString("measurements",Constants.numMeasurements));
        et13.setText(prefs.getInt("micthresh",Constants.SEAL_CHECK_THRESH)+"");
        et14.setText(prefs.getInt("occlusionthresh",Constants.SEAL_OCCLUSION_THRESH)+"");
        et15.setText(prefs.getInt("recordDuration",Constants.recordDurationInSeconds)+"");
        et16.setText(prefs.getInt("motorFastSpeed",Constants.motorFastSpeed)+"");

        cb1.setChecked(prefs.getBoolean("mic",Constants.mic));
        cb2.setChecked(prefs.getBoolean("speaker",Constants.speaker));
        cb3.setChecked(prefs.getBoolean("movetoinit",Constants.moveToInit));

        RadioButton bup = getActivity().findViewById(R.id.upsweep);
        RadioButton bdown = getActivity().findViewById(R.id.downsweep);
        if (prefs.getBoolean("downsweep",true)) {
            bup.setChecked(false);
            bdown.setChecked(true);
        }
        else {
            bup.setChecked(true);
            bdown.setChecked(false);
        }

        RadioButton bback = getActivity().findViewById(R.id.backward);
        RadioButton bfor = getActivity().findViewById(R.id.forward);
        if (prefs.getBoolean("forwarddir",true)) {
            bback.setChecked(false);
            bfor.setChecked(true);
        }
        else {
            bback.setChecked(true);
            bfor.setChecked(false);
        }

        et2.addTextChangedListener(new TextWatcher() {

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
                String ss =et2.getText().toString();;
                SharedPreferences.Editor editor = PreferenceManager.getDefaultSharedPreferences(getActivity()).edit();
                editor.putString("tone", ss);
                editor.commit();
                if (ss.length()>0) {
                    Constants.toneFreq = Integer.parseInt(ss);
                }
            }
        });
        et3.addTextChangedListener(new TextWatcher() {

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
                String ss = et3.getText().toString();
                SharedPreferences.Editor editor = PreferenceManager.getDefaultSharedPreferences(getActivity()).edit();
                editor.putString("vol", ss);
                editor.commit();
                if (ss.length()>1) {
                    Constants.volume = Double.parseDouble(ss);
                }
            }
        });
        et4.addTextChangedListener(new TextWatcher() {

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
                SharedPreferences.Editor editor = PreferenceManager.getDefaultSharedPreferences(getActivity()).edit();
                editor.putString("minpressure", et4.getText().toString());
                editor.commit();
                Constants.minp=et4.getText().toString();
            }
        });
        et5.addTextChangedListener(new TextWatcher() {

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
                SharedPreferences.Editor editor = PreferenceManager.getDefaultSharedPreferences(getActivity()).edit();
                editor.putString("maxpressure", et5.getText().toString());
                editor.commit();
                Constants.maxp=et5.getText().toString();
            }
        });
        et6.addTextChangedListener(new TextWatcher() {

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
                SharedPreferences.Editor editor = PreferenceManager.getDefaultSharedPreferences(getActivity()).edit();
                editor.putString("pspeed", et6.getText().toString());
                editor.commit();
                Constants.speed=et6.getText().toString();
            }
        });
        et8.addTextChangedListener(new TextWatcher() {

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
                SharedPreferences.Editor editor = PreferenceManager.getDefaultSharedPreferences(getActivity()).edit();
                editor.putString("sealcheck", et8.getText().toString());
                editor.commit();
                Constants.sealCheck=et8.getText().toString();
            }
        });
        et9.addTextChangedListener(new TextWatcher() {

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
                SharedPreferences.Editor editor = PreferenceManager.getDefaultSharedPreferences(getActivity()).edit();
                editor.putString("resetSteps", et9.getText().toString());
                editor.commit();
                Constants.resetSteps=et9.getText().toString();
            }
        });
        et10.addTextChangedListener(new TextWatcher() {

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
                SharedPreferences.Editor editor = PreferenceManager.getDefaultSharedPreferences(getActivity()).edit();
                editor.putString("settlingTime", et10.getText().toString());
                editor.commit();
                Constants.settlingTime=et10.getText().toString();
            }
        });
        et11.addTextChangedListener(new TextWatcher() {

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
                SharedPreferences.Editor editor = PreferenceManager.getDefaultSharedPreferences(getActivity()).edit();
                editor.putString("microsteps", et11.getText().toString());
                editor.commit();
                Constants.microsteps=et11.getText().toString();
            }
        });
        et12.addTextChangedListener(new TextWatcher() {

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
                SharedPreferences.Editor editor = PreferenceManager.getDefaultSharedPreferences(getActivity()).edit();
                editor.putString("measurements", et12.getText().toString());
                editor.commit();
                Constants.numMeasurements=et12.getText().toString();
            }
        });

        et13.addTextChangedListener(new TextWatcher() {

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
                SharedPreferences.Editor editor = PreferenceManager.getDefaultSharedPreferences(getActivity()).edit();
                String ss = et13.getText().toString();
                if (ss.length()>0) {
                    editor.putInt("micthresh", Integer.parseInt(ss));
                    editor.commit();
                    Constants.SEAL_CHECK_THRESH=Integer.parseInt(ss);
                }
            }
        });

        et14.addTextChangedListener(new TextWatcher() {

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
                SharedPreferences.Editor editor = PreferenceManager.getDefaultSharedPreferences(getActivity()).edit();
                String ss = et14.getText().toString();
                if (ss.length()>0) {
                    editor.putInt("occlusionthresh", Integer.parseInt(ss));
                    editor.commit();
                    Constants.SEAL_OCCLUSION_THRESH=Integer.parseInt(ss);
                }
            }
        });
        et15.addTextChangedListener(new TextWatcher() {

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
                SharedPreferences.Editor editor = PreferenceManager.getDefaultSharedPreferences(getActivity()).edit();
                String ss = et15.getText().toString();
                if (ss.length()>0) {
                    editor.putInt("recordDuration", Integer.parseInt(ss));
                    editor.commit();
                    Constants.recordDurationInSeconds=Integer.parseInt(ss);
                }
            }
        });

        et16.addTextChangedListener(new TextWatcher() {

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
                SharedPreferences.Editor editor = PreferenceManager.getDefaultSharedPreferences(getActivity()).edit();
                String ss = et16.getText().toString();
                if (ss.length()>0) {
                    editor.putInt("motorFastSpeed", Integer.parseInt(ss));
                    editor.commit();
                    Constants.motorFastSpeed=Integer.parseInt(ss);
                }
            }
        });

        cb1.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
                                           @Override
                                           public void onCheckedChanged(CompoundButton buttonView,boolean isChecked) {
                                               SharedPreferences.Editor editor = PreferenceManager.getDefaultSharedPreferences(getActivity()).edit();
                                               editor.putBoolean("mic", isChecked);
                                               Constants.mic=isChecked;
                                               editor.commit();
                                           }
                                       }
        );
        cb2.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
                                           @Override
                                           public void onCheckedChanged(CompoundButton buttonView,boolean isChecked) {
                                               SharedPreferences.Editor editor = PreferenceManager.getDefaultSharedPreferences(getActivity()).edit();
                                               editor.putBoolean("speaker", isChecked);
                                               Constants.speaker=isChecked;
                                               editor.commit();
                                           }
                                       }
        );
        cb3.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
                                           @Override
                                           public void onCheckedChanged(CompoundButton buttonView,boolean isChecked) {
                                               SharedPreferences.Editor editor = PreferenceManager.getDefaultSharedPreferences(getActivity()).edit();
                                               editor.putBoolean("movetoinit", isChecked);
                                               Constants.moveToInit=isChecked;
                                               editor.commit();
                                           }
                                       }
        );

        final RadioGroup rg=getActivity().findViewById(R.id.radioGroup3);
        rg.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(RadioGroup group, int checkedId) {
                View radioButton = getActivity().findViewById(checkedId);
                int index = rg.indexOfChild(radioButton);
                RadioButton b2 = getActivity().findViewById(R.id.forward);
                SharedPreferences.Editor editor = PreferenceManager.getDefaultSharedPreferences(getActivity()).edit();
                editor.putBoolean("forwarddir", b2.isChecked());
                editor.commit();
            }
        });
        final RadioGroup rg2=getActivity().findViewById(R.id.radioGroup2);
        rg2.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(RadioGroup group, int checkedId) {
                View radioButton = getActivity().findViewById(checkedId);
                int index = rg.indexOfChild(radioButton);
                RadioButton b2 = getActivity().findViewById(R.id.downsweep);
                SharedPreferences.Editor editor = PreferenceManager.getDefaultSharedPreferences(getActivity()).edit();
                editor.putBoolean("downsweep", b2.isChecked());
                editor.commit();
            }
        });
    }
}
