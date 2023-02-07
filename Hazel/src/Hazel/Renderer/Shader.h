#pragma once

#include <string>

namespace Hazel {

    // 这里不用抽象， 因为可以对shader进行翻译， 翻译成其他API的shader语言
    class Shader
    {
    public:
        Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
        ~Shader();

        void Bind() const;
        void Unbind() const;

    private:
        uint32_t m_RendererID;
    };

}