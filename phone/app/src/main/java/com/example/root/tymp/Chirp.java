package com.example.root.tymp;

public class Chirp {
    public static double[] generateChirp(double startFreq, double endFreq, int N, double SamplingRate, double initialPhase)
    {
        double[] ans = new double[N];
        double f = startFreq;
        double time = (double)N / SamplingRate;
        double k = (endFreq - startFreq) / (double)time;
        for (int i = 0; i < N; i++)
        {
            double t = (double)i / SamplingRate;
            double phase = initialPhase + 2*Math.PI*(startFreq * t + 0.5 * k * t * t);
            phase = AngularMath.Normalize(phase);
            ans[i] = Math.sin(phase);
        }
        return ans;
    }

    public static short[] generatetriangularChirpSpeaker(double startFreq, double endFreq, int N, double SamplingRate, double initialPhase)
    {
        short[] ans = new short[N];
        double f = startFreq;
        double time = (double)N/(2*SamplingRate);
        double k = (endFreq - startFreq) / (double)time;
        double phase=0;
        double t = 0;
        for (int i = 0; i < N/2; i++)
        {
            t = (double)i/SamplingRate;
            phase = initialPhase + 2*Math.PI*(startFreq*t + 0.5*k*t*t);
            phase = AngularMath.Normalize(phase);
            ans[i] = (short)(Math.sin(phase)*20000);
        }
        t = (double)N /(2* SamplingRate);
        //initialPhase = phase;
        //initialPhase = initialPhase + 2 * Math.PI * (startFreq * t + 0.5 * k * t * t);
        initialPhase = phase + 2 * Math.PI * (startFreq * t + 0.5 * k * t * t);
        for (int i = N/2; i < N ; i++)
        {
            t = (double)(t + 1 / SamplingRate);
            phase = initialPhase - 2 * Math.PI * (endFreq * t - 0.5 * k * t * t);
            phase = AngularMath.Normalize(phase);
            ans[i] = (short)(Math.sin(phase)*20000);
        }

        return ans;
    }
    public static short[] addpreamble(double startFreq, double endFreq, int N, double SamplingRate, double initialPhase,short[] signalsamples,int gap)
    {
        short[] samples = new short[N+gap+signalsamples.length];
        short[] preamble = generateChirpSpeaker(startFreq, endFreq,N,SamplingRate,initialPhase);
        int index = 0;
        for(int i=0;i<N;i++)
            samples[index++] = preamble[i];
        for(int j=0;j<gap;j++)
            samples[index++] = 0;
        for(int i=0;i<signalsamples.length;i++)
            samples[index++]=signalsamples[i];

        return samples;
    }

    public static short[] endpreamble(double startFreq, double endFreq, int N, double SamplingRate, double initialPhase,short[] signalsamples,int gap)
    {
        short[] samples = new short[N+gap+signalsamples.length];
        short[] preamble = generateChirpSpeaker(startFreq, endFreq,N,SamplingRate,initialPhase);
        int index = 0;
        for(int i=0;i<N;i++)
            samples[index++] = preamble[i];
        for(int j=0;j<gap;j++)
            samples[index++] = 0;
        for(int i=0;i<signalsamples.length;i++)
            samples[index++]=signalsamples[i];

        return samples;
    }

    //static double t = 0;
    public static short[] generateChirpSpeaker(double startFreq, double endFreq, int N, double SamplingRate, double initialPhase)
    {
        short[] ans = new short[N];
        double f = startFreq;
        double time = (double)N / SamplingRate;
        double k = (endFreq - startFreq) / (double)time;
        for (int i = 0; i < N; i++)
        {
            double t = (double)i / SamplingRate;
            //t= t+1/SamplingRate;
            double phase = initialPhase + 2*Math.PI*(startFreq * t + 0.5 * k * t * t);
            phase = AngularMath.Normalize(phase);
            ans[i] = (short)(Math.sin(phase)*24000);
        }
        return ans;
    }
}
