#include "hzpch.h"
#include "Sandbox2D.h"

#include <ImGui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Sandbox2D::Sandbox2D()
    : Hazel::Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
{

}

void Sandbox2D::OnAttach()
{
    m_BoxTexture = Hazel::Texture2D::Create("assets/textures/container.jpg");
}

void Sandbox2D::OnDetach()
{
}


void Sandbox2D::OnUpdate(Hazel::Timestep ts)
{
    PROFILE_SCOPE("Sandbox2D::OnUpdate");
    HZ_PROFILE_FUNCTION();

    m_CameraController.OnUpdate(ts);

    Hazel::Renderer2D::ResetStats();

    {
        HZ_PROFILE_SCOPE("Renderer Prep");
        Hazel::RenderCommand::Clear();
        Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
    }

    {
        static float rotation = 0.0f;
        rotation += ts * 20.0f;
        HZ_PROFILE_SCOPE("Renderer Draw");

        Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
        Hazel::Renderer2D::DrawRotatedQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, -30.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
        Hazel::Renderer2D::DrawQuad({ -0.5f, -0.5f }, { 0.5f, 0.5f }, { 0.2f, 0.3f, 0.8f, 1.0f });
        Hazel::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 1.5f, 0.75f }, { 0.8f, 0.2f, 0.3f, 1.0f });
        Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.5f }, { 10.0f, 10.0f }, m_BoxTexture, 20.0f, glm::vec4(1.0f, 0.8f, 0.2f, 1.0f));
        Hazel::Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, -0.1f }, { 1.0f, 1.0f }, rotation, m_BoxTexture, 20.0f, glm::vec4(1.0f, 0.8f, 0.2f, 1.0f));
        Hazel::Renderer2D::EndScene();
        
        Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());

        for (float y = -5.0f; y <= 5.0f; y += 0.1f)
        {
            for (float x = -5.0f; x <= 5.0f; x += 0.1f)
            {
                glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.4f };
                Hazel::Renderer2D::DrawQuad({ x, y }, { 0.05f, 0.05f }, color);
            }
        }

        Hazel::Renderer2D::EndScene();
    }
}

void Sandbox2D::OnImGuiRender()
{
    HZ_PROFILE_FUNCTION();
    ImGui::Begin("Settings");

    auto stats = Hazel::Renderer2D::GetStats();
    ImGui::Text("Renderer2D Stats:");
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quads: %d", stats.QuadCount);
    ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

    for (auto& result : m_ProfileResults)
    {
        char label[50];
        strcpy(label, "%.01f FPS ");
        strcat(label, result.Name);
        ImGui::Text(label, 1000/result.Time);
    }
    m_ProfileResults.clear();

    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event & e)
{
    m_CameraController.OnEvent(e);
}
