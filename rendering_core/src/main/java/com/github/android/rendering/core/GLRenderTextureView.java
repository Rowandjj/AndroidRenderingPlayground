package com.github.android.rendering.core;

import android.content.Context;
import android.graphics.SurfaceTexture;
import android.util.AttributeSet;
import android.util.Log;
import android.view.Surface;
import android.view.TextureView;
import android.view.View;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

public class GLRenderTextureView extends TextureView implements IRenderHost {

    private static final String TAG = "GLRenderTextureView";

    private IRenderInterface mRenderInterface;
    private VSyncReceiver mVSyncReceiver;

    public GLRenderTextureView(@NonNull Context context) {
        super(context);
    }

    public GLRenderTextureView(@NonNull Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
    }

    public GLRenderTextureView(@NonNull Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }

    void registerSurfaceTextureListener() {
        this.setSurfaceTextureListener(new SurfaceTextureListener() {
            @Override
            public void onSurfaceTextureAvailable(@NonNull SurfaceTexture surface, int width, int height) {
                if(mRenderInterface != null) {
                    mRenderInterface.notifySurfaceCreated(new Surface(surface), width, height);
                }
            }

            @Override
            public void onSurfaceTextureSizeChanged(@NonNull SurfaceTexture surface, int width, int height) {
                if(mRenderInterface != null) {
                    mRenderInterface.notifySurfaceSizeChanged(width, height);
                }
            }

            @Override
            public boolean onSurfaceTextureDestroyed(@NonNull SurfaceTexture surface) {
                if(mRenderInterface != null) {
                    mRenderInterface.notifySurfaceDestroy();
                }
                return true;
            }

            @Override
            public void onSurfaceTextureUpdated(@NonNull SurfaceTexture surface) {
                // noop
            }
        });
    }

    @Override
    public void attach(IRenderInterface render, VSyncReceiver receiver) {
        mRenderInterface = render;
        mVSyncReceiver = receiver;

        // 初始化渲染接口
        mRenderInterface.create();
        // 注册Surface生命周期回调
        registerSurfaceTextureListener();
        // 注册VSync回调
        receiver.setOnReceiveVSyncListener(new VSyncReceiver.OnReceiveVSyncListener() {
            @Override
            public void onReceiveVSync(long frameInNanos) {
                if(mRenderInterface != null) {
                    try {
                        mRenderInterface.onDrawFrame(frameInNanos);
                    } catch (Throwable e) {
                        Log.e(TAG, "draw frame failed...", e);
                    }
                }
            }
        });
        // 开始监听VSync
        receiver.start();
    }

    @Override
    public void detach() {
        mVSyncReceiver.stop();
        mVSyncReceiver.setOnReceiveVSyncListener(null);
        mRenderInterface.destroy();
        mRenderInterface = null;
        mVSyncReceiver = null;
    }

    @Override
    public void onPagePause() {
        if(mVSyncReceiver != null) {
            mVSyncReceiver.stop();
        }
        if(mRenderInterface != null) {
            mRenderInterface.onRenderPause();
        }
    }

    @Override
    public void onPageResume() {
        if(mVSyncReceiver != null) {
            mVSyncReceiver.start();
        }
        if(mRenderInterface != null) {
            mRenderInterface.onRenderResume();
        }
    }

    @Override
    public View getHostView() {
        return this;
    }
}
