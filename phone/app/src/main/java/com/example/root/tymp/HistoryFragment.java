package com.example.root.tymp;

import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TableLayout;
import android.widget.TableRow;
import android.widget.TextView;

import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

public class HistoryFragment extends Fragment {

    String historyValue;

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_history, container, false);
        if(((MainActivity) getActivity()).getSupportActionBar()!=null) {
            ((MainActivity) getActivity()).getSupportActionBar().setTitle("History");
        }
        return view;
    }

    @Override
    public void onResume() {
        super.onResume();
        Constants.CurrentFragment = Constants.HistoryFragment;
        Constants.HistoryFragment = this;
    }

    @Override
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
        Constants.CurrentFragment = Constants.HistoryFragment;
        Constants.HistoryFragment = this;

        if ((Constants.lastRefreshTs == 0 && Constants.lastChirpTs == 0) ||
                Constants.lastChirpTs > Constants.lastRefreshTs) {
            new ViewAsyncTask(getActivity()).execute();
            Constants.lastRefreshTs = System.currentTimeMillis();
        } else {
            setText();
        }
    }

    public void setText() {

        Log.e("logme", "history settext");
        if (historyValue != null && !historyValue.isEmpty()) {
            TableLayout tl = getActivity().findViewById(R.id.tableLayout);
            for (String line : historyValue.split("\n")) {
                Log.e("ble","RECORD "+line);
                String[] elts = line.split(",");
                TableRow tr = new TableRow(getActivity());
                tr.setLayoutParams(new TableRow.LayoutParams(TableRow.LayoutParams.MATCH_PARENT, TableRow.LayoutParams.MATCH_PARENT));
                for (String elt : elts) {
                    Log.e("ble",">>>"+elt);
                    TextView tv1 = new TextView(getActivity());
                    tv1.setText(elt.trim());
                    tv1.setHeight(100);
                    tv1.setGravity(Gravity.CENTER_VERTICAL | Gravity.LEFT);
                    tr.addView(tv1);
                }
                if(tl!=null) {
                    tl.addView(tr, new TableLayout.LayoutParams(TableLayout.LayoutParams.MATCH_PARENT, TableLayout.LayoutParams.MATCH_PARENT));
                }
            }
        }

    }
}
