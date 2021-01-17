package com.github.android.rendering.core;

import android.content.Context;
import android.util.AttributeSet;
import android.view.SurfaceView;
import android.view.View;

public class GLRenderSurfaceView extends SurfaceView implements IRenderHost{

    public GLRenderSurfaceView(Context context) {
        super(context);
    }

    public GLRenderSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public GLRenderSurfaceView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }

    @Override
    public void attach(IRenderInterface render, VSyncReceiver receiver) {
        // TODO
    }

    @Override
    public void detach() {
        // TODO
    }

    @Override
    public void onPagePause() {

    }

    @Override
    public void onPageResume() {

    }

    @Override
    public View getHostView() {
        return this;
    }
}
