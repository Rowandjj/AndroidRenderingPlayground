//
// Created by jj Rowand on 1/16/21.
//

#include "async_gl_render.h"

namespace bw {

    AsyncGLRender::AsyncGLRender(std::unique_ptr<RenderInterface> interface):
        render_interface_(std::move(interface)){
    }

    AsyncGLRender::~AsyncGLRender() = default;

    void AsyncGLRender::AttachSurface(ANativeWindow* window, int width, int height) {

    }

    void AsyncGLRender::ChangeSurfaceSize(int width, int height) {

    }

    void AsyncGLRender::DetachSurface() {

    }

    void AsyncGLRender::DrawFrame(long frameInNanos) {

    }

}