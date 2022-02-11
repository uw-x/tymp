package com.example.root.tymp;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

public class ResultsFragment extends Fragment {
    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_results, container, false);
        if (((MainActivity) getActivity()).getSupportActionBar()!=null) {
            ((MainActivity) getActivity()).getSupportActionBar().setTitle("Measure");
        }
        return view;
    }

    @Override
    public void onResume() {
        super.onResume();
        Constants.CurrentFragment = Constants.ResultsFragment;
        Constants.ResultsFragment = this;
    }

    @Override
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
        Constants.CurrentFragment = Constants.ResultsFragment;
        Constants.ResultsFragment = this;
    }
}
