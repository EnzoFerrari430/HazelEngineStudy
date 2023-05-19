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
    m_SpriteSheet = Hazel::Texture2D::Create("assets/game/textures/spritesheet_tilesPink.png");

    m_TexturePuzzle = Hazel::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 662.0f, 0.0f }, {148.0f, 148.0f});

    // Init here
    m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
    m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
    m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
    m_Particle.LifeTime = 1.0f;
    m_Particle.Velocity = { 0.0f, 0.0f };
    m_Particle.VelocityVariation = { 3.0f, 1.0f };
    m_Particle.Position = { 0.0f, 0.0f };
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

    //{
    //    static float rotation = 0.0f;
    //    rotation += ts * 20.0f;
    //    HZ_PROFILE_SCOPE("Renderer Draw");

    //    Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
    //    Hazel::Renderer2D::DrawRotatedQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, glm::radians(-30.0f), { 0.8f, 0.2f, 0.3f, 1.0f });
    //    Hazel::Renderer2D::DrawQuad({ -0.5f, -0.5f }, { 0.5f, 0.5f }, m_SquareColor);
    //    Hazel::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 1.5f, 0.75f }, { 0.8f, 0.2f, 0.3f, 1.0f });
    //    Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.5f }, { 20.0f, 20.0f }, m_BoxTexture, 20.0f, glm::vec4(1.0f, 0.8f, 0.2f, 1.0f));
    //    Hazel::Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, -0.1f }, { 1.0f, 1.0f }, glm::radians(rotation), m_BoxTexture, 20.0f, glm::vec4(1.0f, 0.8f, 0.2f, 1.0f));
    //    Hazel::Renderer2D::EndScene();
    //    
    //    Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());

    //    for (float y = -5.0f; y <= 5.0f; y += 0.5f)
    //    {
    //        for (float x = -5.0f; x <= 5.0f; x += 0.5f)
    //        {
    //            glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.4f };
    //            Hazel::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
    //        }
    //    }

    //    Hazel::Renderer2D::EndScene();
    //}

    if (Hazel::Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_LEFT))
    {
        auto[x, y] = Hazel::Input::GetMousePosition();
        auto width = Hazel::Application::Get().GetWindow().GetWidth();
        auto height = Hazel::Application::Get().GetWindow().GetHeight();

        auto bounds = m_CameraController.GetBounds();
        auto pos = m_CameraController.GetCamera().GetPosition();
        x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
        y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
        m_Particle.Position = { x + pos.x, y + pos.y };
        for (int i = 0; i < 50; i++)
            m_ParticleSystem.Emit(m_Particle);
    }

    static float rotation = 0.0f;
    rotation += ts * 20.0f;
    Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
    float spriteSheetWidth = m_SpriteSheet->GetWidth();
    float spriteSheetHeight = m_SpriteSheet->GetHeight();
    glm::vec2 aspect = { 1.0f, spriteSheetWidth / spriteSheetHeight };
    Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.0f }, glm::vec2(1.0f, 1.0f) * aspect, m_SpriteSheet);
    Hazel::Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f, 0.4f }, glm::vec2(1.0f, 1.0f), glm::radians(rotation), m_TexturePuzzle);
    Hazel::Renderer2D::EndScene();

    m_ParticleSystem.OnUpdate(ts);
    m_ParticleSystem.OnRender(m_CameraController.GetCamera());
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
