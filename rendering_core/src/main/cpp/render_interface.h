//
// Created by jj Rowand on 1/16/21.
//

#ifndef ANDROIDRENDERPLAYGROUND_RENDER_INTERFACE_H
#define ANDROIDRENDERPLAYGROUND_RENDER_INTERFACE_H

#include <memory>
#include <android/native_window.h>

namespace bw {
    class RenderInterface {
    public:
        RenderInterface() = default;
        virtual ~RenderInterface() = default;

        virtual void AttachSurface(ANativeWindow* window, int width, int height) = 0;
        virtual void ChangeSurfaceSize(int width, int height) = 0;
        virtual void DetachSurface() = 0;
        virtual void DrawFrame(long frameInNanos) = 0;
    };
}


#endif //ANDROIDRENDERPLAYGROUND_RENDER_INTERFACE_H
