#include "hzpch.h"
#include "Renderer.h"

namespace Hazel {

    Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

    void Renderer::BeginScene(OrthographicCamera& camera)
    {
        m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene()
    {
    }

    void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
    {
        shader->Bind();
        shader->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);  //这个每一帧每一个shader只用设置一次 应该放在全局变量中
        shader->UploadUniformMat4("u_Transform", transform);  //每个模型都要设置一次

        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }

}