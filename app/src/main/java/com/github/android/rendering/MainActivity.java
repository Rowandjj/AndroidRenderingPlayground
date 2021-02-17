package com.github.android.rendering;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.ViewGroup;

import com.github.android.rendering.core.RenderInstance;

public class MainActivity extends AppCompatActivity {

    private ViewGroup mContainer;
    private RenderInstance mRenderInstance;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mContainer = findViewById(R.id.container);

        mRenderInstance = new RenderInstance.Builder(this)
                .withRenderType(RenderInstance.RenderType.TEXTURE)
                .enableAsyncRendering()
                .build();
        mRenderInstance.attachTo(mContainer);
    }

    @Override
    protected void onPause() {
        super.onPause();
        mRenderInstance.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        mRenderInstance.onResume();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        mRenderInstance.onDestroy();
    }
}