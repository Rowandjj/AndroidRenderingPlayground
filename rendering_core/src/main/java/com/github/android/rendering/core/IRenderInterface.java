package com.github.android.rendering.core;

import android.view.Surface;

import androidx.annotation.NonNull;

public interface IRenderInterface {
    void create();
    void destroy();

    void notifySurfaceCreated(@NonNull Surface surface, int width, int height);
    void notifySurfaceSizeChanged(int width, int height);
    void notifySurfaceDestroy();

    void onDrawFrame(long frameInNanos);

    void onRenderPause();
    void onRenderResume();
}
