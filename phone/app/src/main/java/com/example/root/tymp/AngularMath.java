package com.example.root.tymp;

public class AngularMath {
    public static double Normalize(double ang) {
        double angle = ang;
        while (angle < 0) {
            angle += 2 * Math.PI;
        }
        while (angle >= 2 * Math.PI) {
            angle -= 2 * Math.PI;
        }
        return angle;
    }
}
