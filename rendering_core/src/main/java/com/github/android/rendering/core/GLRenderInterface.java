package com.github.android.rendering.core;

import android.view.Surface;

import androidx.annotation.NonNull;

class GLRenderInterface implements IRenderInterface{

    private long mNativeHandle;
    private boolean isAsyncMode;


    private GLRenderInterface(boolean isAsyncMode) {
        this.isAsyncMode = isAsyncMode;
    }

    static {
        System.loadLibrary("render_core");
    }

    static IRenderInterface make(boolean async) {
        return new GLRenderInterface(async);
    }

    @Override
    public void create() {
        mNativeHandle = nativeCreate(isAsyncMode);
    }

    @Override
    public void destroy() {
        if(mNativeHandle > 0) {
            nativeDestroy(mNativeHandle);
        }
    }

    @Override
    public void notifySurfaceCreated(@NonNull Surface surface, int width, int height) {
        if(mNativeHandle > 0) {
            nativeSurfaceCreated(mNativeHandle, surface, width, height);
        }
    }

    @Override
    public void notifySurfaceSizeChanged(int width, int height) {
        if(mNativeHandle > 0) {
            nativeSurfaceSizeChanged(mNativeHandle, width, height);
        }
    }

    @Override
    public void notifySurfaceDestroy() {
        if(mNativeHandle > 0) {
            nativeSurfaceDestroyed(mNativeHandle);
        }
    }


    @Override
    public void onDrawFrame(long frameInNanos) {
        if(mNativeHandle > 0) {
            nativeDrawFrame(mNativeHandle, frameInNanos);
        }
    }

    @Override
    public void onRenderPause() {
        if(mNativeHandle > 0) {
            nativeRenderPause(mNativeHandle);
        }
    }

    @Override
    public void onRenderResume() {
        if(mNativeHandle > 0) {
            nativeRenderResume(mNativeHandle);
        }
    }

    private native long nativeCreate(boolean isAsyncMode);
    private native void nativeDestroy(long handle);

    private native void nativeSurfaceCreated(long handle, Surface surface, int width, int height);
    private native void nativeSurfaceSizeChanged(long handle, int width, int height);
    private native void nativeSurfaceDestroyed(long handle);

    private native void nativeRenderPause(long handle);
    private native void nativeRenderResume(long handle);
    private native void nativeDrawFrame(long handle, long frameInNanos);
}
