#include "hzpch.h"
#include "Renderer.h"

#include "Renderer2D.h"

namespace Hazel {

    Scope<Renderer::SceneData> Renderer::m_SceneData = CreateScope<Renderer::SceneData>();

    void Renderer::Init()
    {
        HZ_PROFILE_FUNCTION();

        RenderCommand::Init();
        Renderer2D::Init();
    }

    void Renderer::OnWindowResize(uint32_t width, uint32_t height)
    {
        RenderCommand::SetViewport(0, 0, width, height);
    }

    void Renderer::BeginScene(OrthographicCamera& camera)
    {
        m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene()
    {
    }

    void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
    {
        shader->Bind();

        shader->SetMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);  //这个每一帧每一个shader只用设置一次 应该放在全局变量中
        shader->SetMat4("u_Transform", transform);  //每个模型都要设置一次

        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }

}