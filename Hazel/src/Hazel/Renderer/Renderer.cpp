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

    void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
    {
        shader->Bind();

        // TODO: ������ʱֻʹ��OpenGL, ���ڳ���Ҫ��������ͼ�νӿ� DirectX Vulcan��
        std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);  //���ÿһ֡ÿһ��shaderֻ������һ�� Ӧ�÷���ȫ�ֱ�����
        std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);  //ÿ��ģ�Ͷ�Ҫ����һ��

        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }

}