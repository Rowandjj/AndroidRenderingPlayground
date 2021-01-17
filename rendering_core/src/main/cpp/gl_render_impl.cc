//
// Created by jj Rowand on 1/16/21.
//

#include "gl_render_impl.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "log.h"
#define LOG_TAG "GLRenderImpl"

namespace bw {
    GLRenderImpl::GLRenderImpl():surface_(nullptr) {
    }

    GLRenderImpl::~GLRenderImpl() = default;

    void GLRenderImpl::AttachSurface(ANativeWindow* window, int width, int height) {
        // 创建surface实例
        surface_ = std::make_unique<Surface>(window);
        // 建立EGL环境
        surface_->Setup();
    }

    void GLRenderImpl::ChangeSurfaceSize(int width, int height) {
        //noop
    }

    void GLRenderImpl::DetachSurface() {
        surface_->Terminate();
    }

    void GLRenderImpl::DrawFrame(long frameInNanos) {
        if(!surface_) {
            ALOGE("draw frame failed: surface not initialized!");
            return;
        }
        surface_->MakeCurrent();
        this->OnDrawFrame(frameInNanos);
        surface_->SwapBuffer();
    }

    void GLRenderImpl::OnDrawFrame(long frameInNanos) {
        // 真正绘制
        glClearColor(1.0,0.0,0.0,1.0);
        glClear(GL_COLOR_BUFFER_BIT);
    }

}