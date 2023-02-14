#pragma once

#include "RenderCommand.h"

/*
Renderer调用RenderCommand
RenderCommand调用具体的RendererAPI
*/
namespace Hazel {

    class Renderer
    {
    public:
        static void BeginScene();
        static void EndScene();

        static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

        inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
    };

}