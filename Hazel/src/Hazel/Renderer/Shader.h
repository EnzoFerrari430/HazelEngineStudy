#pragma once

#include <string>

namespace Hazel {

    // ���ﲻ�ó��� ��Ϊ���Զ�shader���з��룬 ���������API��shader����
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