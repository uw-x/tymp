package com.example.root.tymp;

public class SignalGenerator {

    public static short[] continuoussinepulse(int length,double freq,int Samplingfreq,int gap,int count) {
        short[] signal = new short[(length+gap)*count];
        short[] signal1;

        signal1 = SineWaveSpeaker(length,freq,0,Samplingfreq);

        int index = 0;
        for(int i=0;i<count;i++) {
            for(int j=0;j<signal1.length;j++)
                signal[index++] = signal1[j];
            for(int j=0;j<gap;j++)
                signal[index++] = 0;
        }

        return signal;
    }

    public static short[] SineWaveSpeaker(int len, double freq, double initialPhase, double samplingFreq) {
        short[] sin = new short[len];
        initialPhase = AngularMath.Normalize(initialPhase);
        double dphase = 2 * Math.PI * (double)freq / samplingFreq;
        double phase = initialPhase;

        for (int i = 0; i < len; i++) {
            sin[i] = (short)(Math.sin(phase) * 25000);
            phase += dphase;
            phase = AngularMath.Normalize(phase);
        }
        return sin;
    }
}