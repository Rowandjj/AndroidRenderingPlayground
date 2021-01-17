//
// Created by jj Rowand on 1/16/21.
//

#ifndef ANDROIDRENDERPLAYGROUND_RENDER_FACTORY_H
#define ANDROIDRENDERPLAYGROUND_RENDER_FACTORY_H

#include <memory>
#include "render_interface.h"

namespace bw {
    class RenderFactory final {
    public:
        RenderFactory() = delete;
        ~RenderFactory() = default;

        static std::unique_ptr<RenderInterface> CreateRenderInterface(bool async = false);
    };
}

#endif //ANDROIDRENDERPLAYGROUND_RENDER_FACTORY_H
