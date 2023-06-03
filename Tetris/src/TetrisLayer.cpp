#include "hzpch.h"
#include "TetrisLayer.h"

#include <ImGui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


TetrisLayer::TetrisLayer()
    : Hazel::Layer("TetrisLayer")
    , m_Camera(-1.0f, 1.0f, -1.5f, 1.5f, -1.0f, 1.0f)
{

}

void TetrisLayer::OnAttach()
{
    ImGuiIO io = ImGui::GetIO();
    m_Font = io.Fonts->AddFontFromFileTTF("assets/fonts/OpenSans-Regular.ttf", 120.0f);
    m_SettingFont = io.Fonts->AddFontFromFileTTF("assets/fonts/OpenSans-Regular.ttf", 20.0f);

    m_Level.Init();
}

void TetrisLayer::OnDetach()
{
}


void TetrisLayer::OnUpdate(Hazel::Timestep ts)
{
    PROFILE_SCOPE("TetrisLayer::OnUpdate");
    HZ_PROFILE_FUNCTION();

    // ¬ﬂº≠øÿ÷∆
    m_Time += ts;
    if ((int)(m_Time * 10.0f) % 8 > 4)
        m_Blink = !m_Blink;

    if (m_Level.IsGameOver())
        m_State = GameState::GameOver;

    switch (m_State)
    {
        case GameState::Play:
        {
            m_Level.OnUpdate(ts);
            break;
        }
    }

    // ‰÷»æ≤ø∑÷
    Hazel::Renderer2D::ResetStats();
    {
        HZ_PROFILE_SCOPE("Renderer Prep");
        Hazel::RenderCommand::Clear();
        Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
    }

    {
        HZ_PROFILE_SCOPE("Renderer Draw");

        // ‰÷»æ±≥æ∞
        Hazel::Renderer2D::BeginScene(m_Camera);
        //Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.5f }, { 2.0f, 3.0f }, m_BackgroundTexture);
        m_Level.OnRendererBackGround();
        Hazel::Renderer2D::EndScene();

        // ‰÷»æ∑ΩøÈ
        Hazel::Renderer2D::BeginScene(m_Camera);
        //float size = 18.0f * 4.0f / 640.0f;
        //for (size_t i = 0; i < m_Tiles.size(); ++i)
        //{
        //    Hazel::Renderer2D::DrawQuad({ -1.0f + i * size, 0.0f }, { size, size }, m_Tiles[i]);
        //}
        m_Level.OnRenderer();
        Hazel::Renderer2D::EndScene();

        // ‰÷»æ«∞æ∞
        Hazel::Renderer2D::BeginScene(m_Camera);
        //Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.5f }, { 2.0f, 3.0f }, m_FrameTexture);
        m_Level.OnRendererForeGround();
        Hazel::Renderer2D::EndScene();
    }

}

void TetrisLayer::OnImGuiRender()
{
    HZ_PROFILE_FUNCTION();
    switch (m_State)
    {
        case GameState::Play:
        {
            uint32_t playerScore = m_Level.GetScore();
            std::string scoreStr = std::string("Score: ") + std::to_string(playerScore);
            auto pos = ImGui::GetWindowPos();
            auto width = Hazel::Application::Get().GetWindow().GetWidth();
            auto height = Hazel::Application::Get().GetWindow().GetHeight();
            pos.x += width * 0.5f - 300.0f;
            pos.y -= 90.0f;
            ImGui::GetForegroundDrawList()->AddText(m_Font, 48.0f, pos, 0xffff00ff, scoreStr.c_str());
            break;
        }
        case GameState::MainMenu:
        {
            // ImU32 colÀ≥–Ú: ARGB
            auto pos = ImGui::GetWindowPos();
            auto width = Hazel::Application::Get().GetWindow().GetWidth();
            auto height = Hazel::Application::Get().GetWindow().GetHeight();
            pos.x += width * 0.5f - 300.0f;
            pos.y += 50.0f;
            if (m_Blink)
                ImGui::GetForegroundDrawList()->AddText(m_Font, 120.0f, pos, 0xff000000, "Click to Play!");
            break;
        }
        case GameState::GameOver:
        {
            auto pos = ImGui::GetWindowPos();
            auto width = Hazel::Application::Get().GetWindow().GetWidth();
            auto height = Hazel::Application::Get().GetWindow().GetHeight();
            pos.x += width * 0.5f - 300.0f;
            pos.y += 50.0f;
            if (m_Blink)
            {
                ImGui::GetForegroundDrawList()->AddText(m_Font, 120.0f, pos, 0xff000000, "GameOver");
                pos.y += 120.0f;
                ImGui::GetForegroundDrawList()->AddText(m_Font, 120.0f, pos, 0xff000000, "Play Again!");
            }

            pos = ImGui::GetWindowPos();
            pos.x += width * 0.5f - 300.0f;
            pos.y -= 90.0f;
            uint32_t playerScore = m_Level.GetScore();
            std::string scoreStr = std::string("Score: ") + std::to_string(playerScore);
            ImGui::GetForegroundDrawList()->AddText(m_Font, 48.0f, pos, 0xffff00ff, scoreStr.c_str());
            break;
        }
    }

#if 1
    ImGui::PushFont(m_SettingFont);
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

    ImGui::End();
    ImGui::PopFont();
#endif
}


void TetrisLayer::OnEvent(Hazel::Event & e)
{
    Hazel::EventDispatcher dispatcher(e);    
    dispatcher.Dispatch<Hazel::KeyPressedEvent>(HZ_BIND_EVENT_FN(TetrisLayer::OnKeyPressed));
    dispatcher.Dispatch<Hazel::KeyReleasedEvent>(HZ_BIND_EVENT_FN(TetrisLayer::OnKeyReleased));
}

bool TetrisLayer::OnKeyPressed(Hazel::KeyPressedEvent& e)
{
    if ((m_State == GameState::GameOver || m_State == GameState::MainMenu) && e.GetKeyCode() == HZ_KEY_SPACE)
    {
        m_Level.Reset();
        m_State = GameState::Play;
    }
    if (m_State == GameState::Play)
    {
        //m_Level.OnKeyPressed(e);
    }
    return false;
}

bool TetrisLayer::OnKeyReleased(Hazel::KeyReleasedEvent& e)
{
    if (m_State == GameState::Play)
    {
        //m_Level.OnKeyReleased(e);
    }
    return false;
}
