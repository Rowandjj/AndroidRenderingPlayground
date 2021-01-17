package com.github.android.rendering.core;

import android.content.Context;
import android.util.Log;
import android.view.ViewGroup;

import androidx.annotation.NonNull;

public class RenderInstance {

    private static final String TAG = "RenderInstance";

    private IRenderHost mRenderHost;

    public enum RenderType {
        SURFACE,
        TEXTURE
    }

    private RenderInstance(@NonNull Builder builder) {
        init(builder.mContext, builder.mRenderType, builder.isAsyncMode);
    }

    private void init(Context context, RenderType type, boolean isAsyncMode) {
        final IRenderInterface renderInterface = GLRenderInterface.make(isAsyncMode);
        final VSyncReceiver vsyncReceiver = new VSyncReceiver();
        if(type == RenderType.TEXTURE) {
            mRenderHost = new GLRenderTextureView(context);
        } else {
            mRenderHost = new GLRenderSurfaceView(context);
        }
        mRenderHost.attach(renderInterface, vsyncReceiver);
    }

    /**
     * 将渲染实例绑定到{@link ViewGroup} 上，并开始渲染
     *
     * */
    public void attachTo(@NonNull ViewGroup container) {
        if(mRenderHost != null) {
            container.addView(mRenderHost.getHostView(),
                    new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT));
        }
    }

    public void onPause() {
        Log.d(TAG, "page pause...");
        if(mRenderHost != null) {
            mRenderHost.onPagePause();
        }
    }

    public void onResume() {
        Log.d(TAG, "page resume...");
        if(mRenderHost != null) {
            mRenderHost.onPageResume();
        }
    }

    public void onDestroy() {
        Log.d(TAG, "page destroy...");
        if(mRenderHost != null) {
            mRenderHost.detach();
        }
    }

    public static class Builder {
        private final Context mContext;
        private RenderType mRenderType;
        private boolean isAsyncMode;

        public Builder(Context context) {
            this.mContext = context;
        }

        public Builder withRenderType(RenderType type) {
            this.mRenderType = type;
            return this;
        }

        public Builder enableAsyncRendering() {
            this.isAsyncMode = true;
            return this;
        }

        public RenderInstance build() {
            return new RenderInstance(this);
        }
    }
}
