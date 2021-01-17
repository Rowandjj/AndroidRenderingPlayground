//
// Created by jj Rowand on 1/16/21.
//

#include "android_surface.h"
#include "log.h"

#define LOG_TAG "Surface"

namespace bw {
    Surface::Surface(ANativeWindow *window)
            :window_(window),
            has_released_(false),
            egl_surface_(nullptr),
            egl_context_(nullptr),
            egl_config_(nullptr) {
    }

    Surface::~Surface() {
        // TODO:
        this->Terminate();
        if(window_) {
            ANativeWindow_release(window_);
            ALOGW("Release ANativeWindow* .....");
        }
    }

    void Surface::Setup() {
        bool status = false;
        status |= InitEGLSurface();
        status |= InitEGLContext();
        if(status) {
            ALOGW("EGL setup success!");
        } else {
            ALOGE("EGL setup failed!");
        }
    }

    bool Surface::InitEGLSurface() {
        egl_display_ = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        eglInitialize(egl_display_,0,0);
        const EGLint attribs[] = {EGL_RENDERABLE_TYPE,
                                  EGL_OPENGL_ES2_BIT,  // Request opengl ES2.0
                                  EGL_SURFACE_TYPE,
                                  EGL_WINDOW_BIT,
                                  EGL_BLUE_SIZE,
                                  8,
                                  EGL_GREEN_SIZE,
                                  8,
                                  EGL_RED_SIZE,
                                  8,
                                  EGL_DEPTH_SIZE,
                                  24,
                                  EGL_NONE};

        color_size_ = 8;
        depth_size_ = 24;

        EGLint num_configs;
        eglChooseConfig(egl_display_, attribs, &egl_config_, 1, &num_configs);
        if (!num_configs) {
            // Fall back to 16bit depth buffer
            const EGLint attribs_fallback[] = {EGL_RENDERABLE_TYPE,
                                      EGL_OPENGL_ES2_BIT,  // Request opengl ES2.0
                                      EGL_SURFACE_TYPE,
                                      EGL_WINDOW_BIT,
                                      EGL_BLUE_SIZE,
                                      8,
                                      EGL_GREEN_SIZE,
                                      8,
                                      EGL_RED_SIZE,
                                      8,
                                      EGL_DEPTH_SIZE,
                                      16,
                                      EGL_NONE};
            eglChooseConfig(egl_display_, attribs_fallback, &egl_config_, 1, &num_configs);
            depth_size_ = 16;
        }

        if (!num_configs) {
            ALOGW("Unable to retrieve EGL config");
            return false;
        }

        egl_surface_ = eglCreateWindowSurface(egl_display_, egl_config_, window_, nullptr);
        eglQuerySurface(egl_display_, egl_surface_, EGL_WIDTH, &screen_width_);
        eglQuerySurface(egl_display_, egl_surface_, EGL_HEIGHT, &screen_height_);

        return true;
    }

    bool Surface::InitEGLContext() {
        const EGLint context_attribs[] = {EGL_CONTEXT_CLIENT_VERSION,
                                          2,  // Request opengl ES2.0
                                          EGL_NONE};
        egl_context_ = eglCreateContext(egl_display_, egl_config_, nullptr, context_attribs);

        if (eglMakeCurrent(egl_display_, egl_surface_, egl_surface_, egl_context_) == EGL_FALSE) {
            ALOGW("Unable to eglMakeCurrent");
            return false;
        }
        ALOGD("Init EGL Context success!");
        return true;
    }

    bool Surface::MakeCurrent() {
        if(eglGetCurrentContext() == egl_context_) {
            ALOGW("EGL already make current...");
            return true;
        }
        if (eglMakeCurrent(egl_display_, egl_surface_, egl_surface_, egl_context_) == EGL_FALSE) {
            ALOGW("Unable to eglMakeCurrent");
            return false;
        }

        return true;
    }

    bool Surface::SwapBuffer() {
        bool b = eglSwapBuffers(egl_display_, egl_surface_);
        if (!b) {
            EGLint err = eglGetError();
            if (err == EGL_BAD_SURFACE) {
                // Recreate surface
                InitEGLSurface();
                return EGL_SUCCESS;  // Still consider glContext is valid
            } else if (err == EGL_CONTEXT_LOST || err == EGL_BAD_CONTEXT) {
                // Context has been lost!!
                Terminate();
                InitEGLContext();
            }
            return err;
        }
        return EGL_SUCCESS;
    }

    void Surface::Terminate() {
        if (egl_display_ != EGL_NO_DISPLAY) {
            eglMakeCurrent(egl_display_, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
            if (egl_context_ != EGL_NO_CONTEXT) {
                eglDestroyContext(egl_display_, egl_context_);
            }

            if (egl_surface_ != EGL_NO_SURFACE) {
                eglDestroySurface(egl_display_, egl_surface_);
            }
            eglTerminate(egl_display_);
        }

        egl_display_ = EGL_NO_DISPLAY;
        egl_context_ = EGL_NO_CONTEXT;
        egl_surface_ = EGL_NO_SURFACE;
        window_ = nullptr;
    }
}