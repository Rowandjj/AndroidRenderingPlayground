//
// Created by jj Rowand on 1/16/21.
//

#ifndef ANDROIDRENDERPLAYGROUND_ANDROID_SURFACE_H
#define ANDROIDRENDERPLAYGROUND_ANDROID_SURFACE_H

#include <android/native_window.h>
#include <EGL/egl.h>

namespace bw {
    class Surface {
    public:
        explicit Surface(ANativeWindow* window);
        ~Surface();

        void Setup();
        bool MakeCurrent();
        bool SwapBuffer();
        void Terminate();

        EGLDisplay GetDisplay() const { return egl_display_; }
        EGLSurface GetSurface() const { return egl_surface_; }
        ANativeWindow* GetANativeWindow(void) const { return window_; };

    private:
        bool InitEGLSurface();
        bool InitEGLContext();

    private:
        ANativeWindow* window_;

        EGLSurface egl_surface_;
        EGLContext egl_context_;
        EGLDisplay egl_display_;
        EGLConfig  egl_config_;

        int32_t color_size_;
        int32_t depth_size_;

        int32_t screen_width_;
        int32_t screen_height_;

        bool has_released_;
    };
}


#endif //ANDROIDRENDERPLAYGROUND_ANDROID_SURFACE_H
