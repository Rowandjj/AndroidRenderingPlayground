//
// Created by jj Rowand on 1/16/21.
//

#ifndef ANDROIDRENDERPLAYGROUND_ASYNC_GL_RENDER_H
#define ANDROIDRENDERPLAYGROUND_ASYNC_GL_RENDER_H

#include "render_interface.h"
#include "thread.h"
#include <memory>

namespace bw {
    class AsyncGLRender: public RenderInterface {
    public:
        explicit AsyncGLRender(std::unique_ptr<RenderInterface> interface);
        ~AsyncGLRender();

        void AttachSurface(ANativeWindow* window, int width, int height) override;
        void ChangeSurfaceSize(int width, int height) override;
        void DetachSurface() override;
        void DrawFrame(long frameInNanos) override;
    private:
       std::unique_ptr<RenderInterface> render_interface_;
       std::unique_ptr<Thread> render_thread_;
    };
}

#endif //ANDROIDRENDERPLAYGROUND_ASYNC_GL_RENDER_H
