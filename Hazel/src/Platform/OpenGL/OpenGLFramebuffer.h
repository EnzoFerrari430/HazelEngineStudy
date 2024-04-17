#pragma once

#include "Hazel/Renderer/Framebuffer.h"

namespace Hazel {

    class OpenGLFramebuffer : public Framebuffer
    {
    public:
        OpenGLFramebuffer(const FramebufferSpecification& spec);
        virtual ~OpenGLFramebuffer();

        void Bind() const override;
        void Unbind() const override;

        void Resize(uint32_t width, uint32_t height) override;

        uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }

        const FramebufferSpecification& GetSpecification() const override { return m_Specification; }

        void Invalidate();
    private:
        uint32_t m_RendererID = 0;
        uint32_t m_ColorAttachment = 0, m_DepthAttachment = 0;
        FramebufferSpecification m_Specification;
    };

}