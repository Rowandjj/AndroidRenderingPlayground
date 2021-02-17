//
// Created by jj Rowand on 1/16/21.
//

#include "async_gl_render.h"
#include "log.h"

#include <pthread.h>

#define LOG_TAG "AsyncGLRender"

namespace bw {

    AsyncGLRender::AsyncGLRender(std::unique_ptr<RenderInterface> interface):
        render_interface_(std::move(interface)),
        render_thread_(std::make_unique<Thread>("render")) {

    }

    AsyncGLRender::~AsyncGLRender() {
        ALOGE("++++++ async gl render destroyed ++++++");
    }

    void AsyncGLRender::AttachSurface(ANativeWindow* window, int width, int height) {
        render_thread_->GetTaskRunner()->PostTask([window, width, height, this](){
            this->render_interface_->AttachSurface(window, width, height);
        });
    }

    void AsyncGLRender::ChangeSurfaceSize(int width, int height) {
        render_thread_->GetTaskRunner()->PostTask([width, height, this](){
            this->render_interface_->ChangeSurfaceSize(width, height);
        });
    }

    void AsyncGLRender::DetachSurface() {
        render_thread_->GetTaskRunner()->PostTask([this](){
            this->render_interface_->DetachSurface();
        });
    }

    void AsyncGLRender::DrawFrame(long frameInNanos) {
        ALOGE("[%ld]will draw frame...", pthread_self());
        render_thread_->GetTaskRunner()->PostTask([this, frameInNanos]() {
            ALOGE("[%ld]did draw frame...", pthread_self());
            this->render_interface_->DrawFrame(frameInNanos);
        });
    }

}