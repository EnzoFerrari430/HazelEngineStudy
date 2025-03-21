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

    Hazel::FramebufferSpecification fbSpec;
    fbSpec.Width = 1280.0f;
    fbSpec.Height = 720.f;
    m_Framebuffer = Hazel::Framebuffer::Create(fbSpec);

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
        //m_Framebuffer->Bind();
        Hazel::RenderCommand::Clear();
        Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
    }

    if (Hazel::Input::IsMouseButtonPressed(Hazel::Mouse::ButtonLeft))
    {
        float x, y;
        glm::vec2 position = Hazel::Input::GetMousePosition();
        x = position.x;
        y = position.y;
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
    //m_Framebuffer->Unbind();


}

void Sandbox2D::OnImGuiRender()
{
    HZ_PROFILE_FUNCTION();

    static bool dockingEnable = false;
    if (dockingEnable)
    {

        static bool dockSpaceOpen = true;
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockSpaceOpen, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Close", NULL, false))
                {
                    Hazel::Application::Get().Close();
                    dockSpaceOpen = false;
                }
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }


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
            ImGui::Text(label, 1000 / result.Time);
        }
        m_ProfileResults.clear();

        ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
        uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
        ImGui::Image((void*)textureID, ImVec2(1280.0f, 720.0f));
        ImGui::End();

        ImGui::End();
    }
    else
    {
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
            ImGui::Text(label, 1000 / result.Time);
        }
        m_ProfileResults.clear();

        ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
        uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
        ImGui::Image((void*)textureID, ImVec2(1280.0f, 720.0f));
        ImGui::End();
    }
}

void Sandbox2D::OnEvent(Hazel::Event & e)
{
    m_CameraController.OnEvent(e);
}
