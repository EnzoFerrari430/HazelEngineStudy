#pragma once

namespace Hazel {

    // ‰÷»æAPI≥ÈœÛ¿‡
    class GraphicsContext
    {
    public:
        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
    };

}