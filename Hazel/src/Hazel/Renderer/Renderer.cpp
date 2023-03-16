#include "hzpch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Hazel {

    Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

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

        // TODO: 这里暂时只使用OpenGL, 后期抽象要考虑其他图形接口 DirectX Vulcan等
        std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);  //这个每一帧每一个shader只用设置一次 应该放在全局变量中
        std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);  //每个模型都要设置一次

        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }

}