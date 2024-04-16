#include "hzpch.h"
#include "Sandbox2D.h"

#include <ImGui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static const uint32_t s_MapWidth = 24;
static const char* s_MapTiles =
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWDDDDDDDDDDDDDDDWWWWWW"
"WWDDDDDDDDDDDDDDDDDDDWWW"
"WWWDDDDDDDDDDDDDDDDDDDWW"
"WWWWDDDDWWDDDDDDDDDDDDWW"
"WWWDDDDWWWWDDDDDDDDDDWWW"
"WWDDDDDWWWDDDDDDDDDDDWWW"
"WWWWDDDDWDDDDDDDDDDWWWWW"
"WWWWWWDDDDDDDDDDDDWWWWWW"
"WWWWWWWDDDDDDDDDDWWWWWWW"
"WWWWWWWWDDDDDDDWWWWWWWWW"
"WWWWWWWWWDDDDDWWWWWWWWWW"
"WWWWWWWWWWDDDWWWWWWWWWWW"
"WWWWWWWWWWDDWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW"
;

Sandbox2D::Sandbox2D()
    : Hazel::Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
{

}

void Sandbox2D::OnAttach()
{
    m_BoxTexture = Hazel::Texture2D::Create("assets/textures/container.jpg");
    m_SpriteSheet = Hazel::Texture2D::Create("assets/game/textures/RPGpack_sheet_2X.png");

    m_TextureStair = Hazel::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 7, 6 }, { 128.0f, 128.0f }, { 1, 1 });
    m_TextureBarrel = Hazel::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 8, 2 }, { 128.0f, 128.0f }, { 1, 1 });
    m_TextureTree = Hazel::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 2, 1 }, { 128.0f, 128.0f }, { 1, 2 });

    m_MapWidth = s_MapWidth;
    m_MapHeight = strlen(s_MapTiles) / s_MapWidth;
    m_TextureMap['D'] = Hazel::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 6, 11 }, { 128.0f, 128.0f }, { 1, 1 });
    m_TextureMap['W'] = Hazel::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 11, 11 }, { 128.0f, 128.0f }, { 1, 1 });

    // Init here
    m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
    m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
    m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
    m_Particle.LifeTime = 1.0f;
    m_Particle.Velocity = { 0.0f, 0.0f };
    m_Particle.VelocityVariation = { 3.0f, 1.0f };
    m_Particle.Position = { 0.0f, 0.0f };

    m_CameraController.SetZoomLevel(5.0f);
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

    m_ParticleSystem.OnUpdate(ts);
    m_ParticleSystem.OnRender(m_CameraController.GetCamera());

    static float rotation = 0.0f;
    rotation += ts * 20.0f;
    Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());

    for (uint32_t y = 0; y < m_MapHeight; ++y)
    {
        for (uint32_t x = 0; x < m_MapWidth; ++x)
        {
            char tileType = s_MapTiles[x + y * m_MapWidth];
            Hazel::Ref<Hazel::SubTexture2D> texture;
            if (m_TextureMap.find(tileType) != m_TextureMap.end())
                texture = m_TextureMap[tileType];
            else
                texture = m_TextureStair;

            Hazel::Renderer2D::DrawQuad({ x - m_MapWidth / 2.0f, y - m_MapHeight / 2.0f, 0.4f }, glm::vec2(1.0f, 1.0f), texture);
        }
    }

    //Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.4f }, glm::vec2(1.0f, 1.0f), m_TextureStair);
    //Hazel::Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f, 0.4f }, glm::vec2(1.0f, 1.0f), glm::radians(rotation), m_TextureBarrel);
    //Hazel::Renderer2D::DrawRotatedQuad({ -1.0f, 0.0f, 0.4f }, glm::vec2(1.0f, 2.0f), glm::radians(rotation), m_TextureTree);
    Hazel::Renderer2D::EndScene();


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
