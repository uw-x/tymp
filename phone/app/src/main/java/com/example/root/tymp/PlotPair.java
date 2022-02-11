package com.example.root.tymp;

public class PlotPair implements Comparable<PlotPair> {
    double x;
    double y;

    public PlotPair(double x, double y) {
        this.x = x;
        this.y = y;
    }

    public int compareTo(PlotPair a)
    {
        if (x - a.x < 0) {
            return -1;
        }
        else if (x-a.x > 0) {
            return 1;
        }
        return 0;
    }
}
