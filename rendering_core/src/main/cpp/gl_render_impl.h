//
// Created by jj Rowand on 1/16/21.
//

#ifndef ANDROIDRENDERPLAYGROUND_GL_RENDER_IMPL_H
#define ANDROIDRENDERPLAYGROUND_GL_RENDER_IMPL_H

#include "render_interface.h"
#include "android_surface.h"

namespace bw {
    class GLRenderImpl: public RenderInterface {
    public:
        explicit GLRenderImpl();
        virtual ~GLRenderImpl();

        void AttachSurface(ANativeWindow* window, int width, int height) override;
        void ChangeSurfaceSize(int width, int height) override;
        void DetachSurface() override;
        void DrawFrame(long frameInNanos) override;

        virtual void OnDrawFrame(long frameInNanos);
    private:
        std::unique_ptr<Surface> surface_;
    };
}


#endif //ANDROIDRENDERPLAYGROUND_GL_RENDER_IMPL_H
