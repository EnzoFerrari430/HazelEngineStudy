#include "hzpch.h"
#include "TetrisLayer.h"
#include "Random.h"
#include "TetrisAudio.h"

#include <ImGui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Utility.h"

std::string_view g_SpecialThanksSentence = "Creativity usually comes from small-budget studios who fear failure less.";

std::vector<std::string> TetrisLayer::m_MainMenus;

TetrisLayer::TetrisLayer()
    : Hazel::Layer("TetrisLayer")
    , m_Camera(-1.0f, 1.0f, -1.5f, 1.5f, -1.0f, 1.0f)
{
    Random::Init();
}

void TetrisLayer::OnAttach()
{
    ImGuiIO io = ImGui::GetIO();
    m_Font = io.Fonts->AddFontFromFileTTF("assets/fonts/OpenSans-Regular.ttf", 120.0f);
    m_SettingFont = io.Fonts->AddFontFromFileTTF("assets/fonts/OpenSans-Regular.ttf", 20.0f);

    m_MenuIconTexture = Hazel::Texture2D::Create("assets/textures/MyLogo.png");

    unsigned int value = 0xff00e5cc;
    unsigned int value2 = Vec4toImU32(m_ActiveMenuItemColor);

    InitMenus();

    TetrisAudio::Init();
    m_Level.Init();
}

void TetrisLayer::OnDetach()
{
    DestoryMenus(m_MenuRoot);
}


void TetrisLayer::OnUpdate(Hazel::Timestep ts)
{
    PROFILE_SCOPE("TetrisLayer::OnUpdate");
    HZ_PROFILE_FUNCTION();

    // 逻辑控制
    m_Time += ts;
    if ((int)(m_Time * 10.0f) % 8 > 4)
        m_Blink = !m_Blink;

    if (m_Level.IsGameOver())
        m_State = GameState::GameOver;

    int screenMask = 0;

    switch (m_State)
    {
        case GameState::Play:
        {
            m_Level.OnUpdate(ts);
            break;
        }
        case GameState::MainMenu:
        case GameState::GameOver:
        case GameState::Pause:
            screenMask = 1;
            break;

    }



    // 渲染部分
    Hazel::Renderer2D::ResetStats();
    {
        HZ_PROFILE_SCOPE("Renderer Prep");
        Hazel::RenderCommand::Clear();
        Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
    }

    {
        HZ_PROFILE_SCOPE("Renderer Draw");

        // 渲染背景
        Hazel::Renderer2D::BeginScene(m_Camera);
        //Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.5f }, { 2.0f, 3.0f }, m_BackgroundTexture);
        m_Level.OnRendererBackGround();
        Hazel::Renderer2D::EndScene();

        // 渲染方块
        Hazel::Renderer2D::BeginScene(m_Camera);
        //float size = 18.0f * 4.0f / 640.0f;
        //for (size_t i = 0; i < m_Tiles.size(); ++i)
        //{
        //    Hazel::Renderer2D::DrawQuad({ -1.0f + i * size, 0.0f }, { size, size }, m_Tiles[i]);
        //}
        m_Level.OnRenderer();
        Hazel::Renderer2D::EndScene();

        // 渲染前景
        Hazel::Renderer2D::BeginScene(m_Camera);
        //Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.5f }, { 2.0f, 3.0f }, m_FrameTexture);
        m_Level.OnRendererForeGround();
        Hazel::Renderer2D::EndScene();

        //前景遮罩
        if (screenMask)
        {
            Hazel::Renderer2D::BeginScene(m_Camera);
            Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.8f}, { 2.0f, 3.0f }, m_ScreenMaskColor);
            Hazel::Renderer2D::EndScene();
        }
    }

}

void TetrisLayer::OnImGuiRender()
{
    HZ_PROFILE_FUNCTION();
    ImGui::PushFont(m_Font);
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
#if 0
            // ImU32 col顺序: ABGR
            auto pos = ImGui::GetWindowPos();
            auto width = Hazel::Application::Get().GetWindow().GetWidth();
            auto height = Hazel::Application::Get().GetWindow().GetHeight();
            pos.x += width * 0.5f - 300.0f;
            pos.y += 50.0f;
            if (m_Blink)
                ImGui::GetForegroundDrawList()->AddText(m_Font, 120.0f, pos, 0xff000000, "Click to Play!");

            pos = ImGui::GetWindowPos();
            pos.x += width * 0.5f - 300.0f;
            pos.y -= 90.0f;
            uint32_t playerScore = m_Level.GetScore();
            std::string scoreStr = std::string("Score: ") + std::to_string(playerScore);
            ImGui::GetForegroundDrawList()->AddText(m_Font, 48.0f, pos, 0xffff00ff, scoreStr.c_str());
            break;
#endif
            ShowModeMenu(m_CurrentMenu);
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
        case GameState::Pause:
        {
            auto pos = ImGui::GetWindowPos();
            auto width = Hazel::Application::Get().GetWindow().GetWidth();
            auto height = Hazel::Application::Get().GetWindow().GetHeight();
            pos.x += width * 0.5f - 300.0f;
            pos.y += 50.0f;
            //if (m_Blink)
            {
                ImGui::GetForegroundDrawList()->AddText(m_Font, 120.0f, pos, 0xff000000, "Press Esc");
                pos.y += 120.0f;
                ImGui::GetForegroundDrawList()->AddText(m_Font, 120.0f, pos, 0xff000000, "to Resume");
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
    ImGui::PopFont();

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

    ImGui::Begin("Color Debug");
    ImGui::ColorPicker4("ScreenMaskColor", &m_ScreenMaskColor[0], ImGuiColorEditFlags_Float | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_DisplayRGB);
    ImGui::ColorPicker4("LabelHoverColor", &m_ActiveMenuItemColor[0], ImGuiColorEditFlags_Float | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_DisplayRGB);
    ImGui::ColorPicker4("LabelFontColor", &m_MenuFontColor[0], ImGuiColorEditFlags_Float | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_DisplayRGB);
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

void TetrisLayer::ShowMenu(GameState state)
{
    switch (state)
    {
    case GameState::MainMenu:
    {
        // ImU32 col顺序: ABGR
        auto pos = ImGui::GetWindowPos();
        auto width = Hazel::Application::Get().GetWindow().GetWidth();
        auto height = Hazel::Application::Get().GetWindow().GetHeight();
        pos.x += width * 0.5f - 300.0f;
        pos.y += 50.0f;
        if (m_Blink)
            ImGui::GetForegroundDrawList()->AddText(m_Font, 120.0f, pos, 0xff000000, "Click to Play!");

        pos = ImGui::GetWindowPos();
        pos.x += width * 0.5f - 300.0f;
        pos.y -= 90.0f;
        uint32_t playerScore = m_Level.GetScore();
        std::string scoreStr = std::string("Score: ") + std::to_string(playerScore);
        ImGui::GetForegroundDrawList()->AddText(m_Font, 48.0f, pos, 0xffff00ff, scoreStr.c_str());
        break;
    }
    case GameState::Pause:
        break;
    case GameState::GameOver:
        break;
    case GameState::Play:
    default:
        break;
    }
}

void TetrisLayer::InitMenus()
{
    m_MenuRoot = new MenuConfig;
    m_MenuRoot->subMenuCount = 3;
    m_MenuRoot->childs = new MenuConfig[3];

    auto& startMenu = m_MenuRoot->childs[0];
    startMenu.parent = m_MenuRoot;
    startMenu.ID = "Start";

    auto& modeMenu = m_MenuRoot->childs[1];
    modeMenu.parent = m_MenuRoot;
    modeMenu.ID = "Mode Choose";
    modeMenu.subMenuCount = 4;
    modeMenu.childs = new MenuConfig[4];

    auto& normalMode = modeMenu.childs[0];
    normalMode.parent = &modeMenu;
    normalMode.ID = "Normal";

    auto& darkLightMode = modeMenu.childs[1];
    darkLightMode.parent = &modeMenu;
    darkLightMode.ID = "Dark Light";

    auto& cooperationMode = modeMenu.childs[2];
    cooperationMode.parent = &modeMenu;
    cooperationMode.ID = "Cooperation";

    auto& versusMode = modeMenu.childs[3];
    versusMode.parent = &modeMenu;
    versusMode.ID = "Versus";


    auto& specialThanks = m_MenuRoot->childs[2];
    specialThanks.parent = m_MenuRoot;
    specialThanks.ID = "Special Thanks";


    //初始化
    m_CurrentMenu = m_MenuRoot;
}

void TetrisLayer::DestoryMenus(MenuConfig* menu)
{
    if (menu)
    {
        if (menu->subMenuCount)
        {
            for (int i = 0; i < menu->subMenuCount; ++i)
            {
                DestoryMenus(&menu->childs[i]);
            }
            delete[] menu->childs;
        }
        delete menu;
    }
}

void TetrisLayer::ShowModeMenu(MenuConfig* menu)
{
    constexpr float fontSize = 48.0f;
    constexpr float iconSize = 48.0f;
    constexpr float leftMargin = 120.0f;
    constexpr float spacing = 2.0f;
    if (menu)
    {
        m_CurrentMenu = menu;
        //计算居中位置
        int menuCount = menu->subMenuCount;
        float fullSize = menuCount * fontSize + (menuCount - 1) * spacing;
        auto width = Hazel::Application::Get().GetWindow().GetWidth();
        auto height = Hazel::Application::Get().GetWindow().GetHeight();
        auto pos = ImGui::GetWindowPos();
        pos.x += width * 0.5f - 300.0f;
        //pos.y -= 90.0f;

        //float startY = (height - fullSize) / 2.0f;
        for (int i = 0; i < menuCount; ++i)
        {
            auto subItem = menu->childs[i];
            auto itemPos = pos;
            itemPos.y += (fontSize * i + spacing * i);
            if (i == menu->activeIndex)
            {
                ImGui::GetForegroundDrawList()->AddRectFilled({ pos.x, itemPos.y }, { pos.x + (float)width, itemPos.y + fontSize }, Vec4toImU32(m_ActiveMenuItemColor));
                unsigned int textureID = m_MenuIconTexture->GetTextureID();
                ImGui::GetForegroundDrawList()->AddImage((void*)textureID, { itemPos.x - fontSize, itemPos.y },
                    { itemPos.x, itemPos.y + fontSize });
            }
            ImGui::GetForegroundDrawList()->AddText(m_Font, fontSize, itemPos, Vec4toImU32(m_MenuFontColor), subItem.ID.data());
        }
    }
}

bool TetrisLayer::OnKeyPressed(Hazel::KeyPressedEvent& e)
{
    //if ((m_State == GameState::GameOver || m_State == GameState::MainMenu) && e.GetKeyCode() == HZ_KEY_SPACE)
    //{
    //    m_Level.Reset();
    //    m_State = GameState::Play;
    //}

    //if (m_State == GameState::Play && e.GetKeyCode() == HZ_KEY_ESCAPE)
    //{
    //    //Pause the game
    //    m_State = GameState::Pause;
    //}
    //else if (m_State == GameState::Pause && e.GetKeyCode() == HZ_KEY_ESCAPE)
    //{
    //    //Resume the game
    //    m_State = GameState::Play;
    //}
    switch (m_State)
    {
    case GameState::MainMenu:
    case GameState::Pause:
    case GameState::GameOver:
        if (e.GetKeyCode() == HZ_KEY_DOWN && (m_CurrentMenu->activeIndex + 1 < m_CurrentMenu->subMenuCount))
        {
            ++m_CurrentMenu->activeIndex;
        }
        else if (e.GetKeyCode() == HZ_KEY_UP && (m_CurrentMenu->activeIndex > 0))
        {
            --m_CurrentMenu->activeIndex;
        }

        if (e.GetKeyCode() == HZ_KEY_ENTER)
        {
            //进入下一级菜单项
            m_CurrentMenu = &m_CurrentMenu->childs[m_CurrentMenu->activeIndex];
        }
        else if (e.GetKeyCode() == HZ_KEY_BACKSPACE)
        {
            //返回上一级
            if(m_CurrentMenu->parent)
                m_CurrentMenu = m_CurrentMenu->parent;
            
        }
        break;
    case GameState::Play:
        if (e.GetKeyCode() == HZ_KEY_ESCAPE)
            m_State = GameState::Pause;
        break;
    }

    return true;
}

bool TetrisLayer::OnKeyReleased(Hazel::KeyReleasedEvent& e)
{
    if (m_State == GameState::Play)
    {
        //m_Level.OnKeyReleased(e);
    }
    return false;
}
