package com.example.root.tymp;

import android.content.Context;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.os.Build;

public class AudioSpeaker extends Thread {

    AudioTrack track1;
    int SamplingFreq;
    Context mycontext;
    short[] samples;
    int speakerType;
    AudioManager man;

    int[] streams = new int[]{AudioManager.STREAM_MUSIC,
            AudioManager.STREAM_ACCESSIBILITY, AudioManager.STREAM_ALARM,
            AudioManager.STREAM_DTMF, AudioManager.STREAM_NOTIFICATION,
            AudioManager.STREAM_RING, AudioManager.STREAM_SYSTEM,
            AudioManager.STREAM_VOICE_CALL};

    public AudioSpeaker(Context mycontext, short[] samples, int samplingFreq, int speakerType) {
        this.mycontext = mycontext;
        this.speakerType = speakerType;
        man = (AudioManager)mycontext.getSystemService(Context.AUDIO_SERVICE);

        if (Build.VERSION.SDK_INT > 21) {
            for (Integer i : streams) {
                man.adjustStreamVolume(i, AudioManager.ADJUST_MUTE, 0);
            }
            man.adjustStreamVolume(speakerType, AudioManager.ADJUST_UNMUTE, 0);
        }

        man.setStreamVolume(speakerType,(int)(man.getStreamMaxVolume(speakerType)),0);

        SamplingFreq = samplingFreq;
        this.samples = samples;

        track1 = new AudioTrack(speakerType,SamplingFreq, AudioFormat.CHANNEL_OUT_MONO,
                AudioFormat.ENCODING_PCM_16BIT,samples.length*2,AudioTrack.MODE_STATIC);

        track1.write(samples,0,samples.length);
    }

    public void play(double vol, int time) {
        try {
            track1.setLoopPoints(0,samples.length,time);
            track1.setStereoVolume((float) vol, 0f);
            track1.play();
        }catch(Exception e) {
        }
    }

    public void stopit(){
        track1.stop();
    }

}
