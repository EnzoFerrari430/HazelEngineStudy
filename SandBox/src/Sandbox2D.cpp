#include "hzpch.h"
#include "Sandbox2D.h"

#include <ImGui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D()
    : Hazel::Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
{

}

void Sandbox2D::OnAttach()
{
    m_FlatColorVertexArray = Hazel::VertexArray::Create();
    float flatColorVertices[3 * 4] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };
    Hazel::Ref<Hazel::VertexBuffer> flatColorVertexBuffer;
    flatColorVertexBuffer = Hazel::VertexBuffer::Create(flatColorVertices, sizeof(flatColorVertices));
    Hazel::BufferLayout flatColorLayout = {
        {Hazel::ShaderDataType::Float3, "a_Position", false}
    };
    flatColorVertexBuffer->SetLayout(flatColorLayout);
    m_FlatColorVertexArray->AddVertexBuffer(flatColorVertexBuffer);

    uint32_t flatColorIndices[6] = { 0, 1, 2, 2, 3, 0 };
    Hazel::Ref<Hazel::IndexBuffer> flatColorIndexBuffer;
    flatColorIndexBuffer = Hazel::IndexBuffer::Create(flatColorIndices, sizeof(flatColorIndices) / sizeof(uint32_t));
    m_FlatColorVertexArray->SetIndexBuffer(flatColorIndexBuffer);

    m_FlatColorShader = Hazel::Shader::Create("assets/shaders/FlatColor.glsl");
}

void Sandbox2D::OnDetach()
{
}


void Sandbox2D::OnUpdate(Hazel::Timestep ts)
{
    m_CameraController.OnUpdate(ts);

    Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
    Hazel::RenderCommand::Clear();

    Hazel::Renderer::BeginScene(m_CameraController.GetCamera());

    std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->Bind();
    std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);
    Hazel::Renderer::Submit(m_FlatColorShader, m_FlatColorVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

    // For Test
    //static int count = 0;
    //if (count < 3)
    //{
    //    char filename[100] = { 0 };
    //    sprintf(filename, "background%d.png", count);
    //    Hazel::RenderCommand::ScreenShot(filename);
    //    count++;
    //}

    Hazel::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event & e)
{
    m_CameraController.OnEvent(e);
}
