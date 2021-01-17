package com.github.android.rendering.core;

import android.util.Log;
import android.view.Choreographer;

import java.util.Locale;
import java.util.concurrent.atomic.AtomicBoolean;

class VSyncReceiver {
    private final Choreographer mChoreographer;
    private OnReceiveVSyncListener mOnReceiveVSyncListener;
    private final AtomicBoolean shouldStop = new AtomicBoolean(false);
    private final AtomicBoolean hasStarted = new AtomicBoolean(false);

    private static final String TAG = "VSyncReceiver";

    public interface OnReceiveVSyncListener {
        void onReceiveVSync(long frameInNanos);
    }

    VSyncReceiver() {
        mChoreographer = Choreographer.getInstance();
    }

    void setOnReceiveVSyncListener(OnReceiveVSyncListener listener) {
        this.mOnReceiveVSyncListener = listener;
    }

    void start() {
        if(hasStarted.get()) {
            return;
        }

        hasStarted.set(true);
        shouldStop.set(false);
        mChoreographer.postFrameCallback(new Choreographer.FrameCallback() {
            @Override
            public void doFrame(long frameTimeNanos) {
                if(mOnReceiveVSyncListener != null) {
                    mOnReceiveVSyncListener.onReceiveVSync(frameTimeNanos);
                    Log.d(TAG, String.format(Locale.CHINA,"[%d]receive VSync signal...", frameTimeNanos));
                }
                if(!shouldStop.get()) {
                    mChoreographer.postFrameCallback(this);
                }
            }
        });
    }

    void stop() {
        shouldStop.set(true);
        hasStarted.set(false);
    }

}
