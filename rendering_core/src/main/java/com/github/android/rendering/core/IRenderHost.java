package com.github.android.rendering.core;

import android.view.View;

public interface IRenderHost {
    void attach(IRenderInterface render, VSyncReceiver receiver);
    void detach();

    void onPagePause();
    void onPageResume();

    View getHostView();
}
