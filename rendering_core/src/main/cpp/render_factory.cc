//
// Created by jj Rowand on 1/16/21.
//

#include "render_factory.h"
#include "gl_render_impl.h"
#include "async_gl_render.h"

namespace bw {
    std::unique_ptr<RenderInterface> RenderFactory::CreateRenderInterface(bool async) {
        auto gl_render = std::make_unique<GLRenderImpl>();
        if(!async) {
            return gl_render;
        }
        // async rendering
        return std::make_unique<AsyncGLRender>(std::move(gl_render));
    }
}