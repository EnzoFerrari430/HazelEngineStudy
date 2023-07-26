#pragma once

#include <Hazel.h>

#include <ImGui/imgui.h>

#include "Tile.h"
#include "Level.h"

#include <string_view>

template<typename Fn>
class Timer
{
public:
    Timer(const char* name, Fn&& func)
        : m_Name(name), m_Func(func), m_Stopped(false)
    {
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
    }

    ~Timer()
    {
        if (!m_Stopped)
            Stop();
    }

    void Stop()
    {
        auto endTimepoint = std::chrono::high_resolution_clock::now();

        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
        auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

        m_Stopped = true;

        float duration = (end - start) * 0.001f;
        m_Func({ m_Name, duration });
    }
private:
    const char* m_Name;
    std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
    bool m_Stopped;
    Fn m_Func;
};

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) {m_ProfileResults.push_back(profileResult); })

class TetrisLayer : public Hazel::Layer
{
    enum class GameState
    {
        Play = 0, MainMenu = 1, GameOver = 2, Pause = 3
    };

    struct MenuConfig
    {
        std::string ID;
        float fontSize = 48.0f;
        int activeIndex = 0;
        int subMenuCount = 0;;
        MenuConfig* childs = nullptr;
        MenuConfig* parent = nullptr;
    };
public:
    TetrisLayer();
    virtual ~TetrisLayer() = default;

    void OnAttach() override;
    void OnDetach() override;

    void OnUpdate(Hazel::Timestep ts) override;
    void OnImGuiRender() override;
    void OnEvent(Hazel::Event& e) override;
private:
    /*
    Play: None
    MainMenu: ShowMainMenu
    GameOver: ShowGameOverMenu
    Pause: ShowPauseMenu
    */
    void ShowMenu(GameState state);
    
    /*
    选择模式的菜单
    普通模式，黑夜模式，合作模式，对抗模式等
    */
    void ShowModeMenu();
    void SpecialThanks();

    void InitMenus();
    void DestoryMenus(MenuConfig* menu);
    void ShowModeMenu(MenuConfig* menu);
private:
    bool OnKeyPressed(Hazel::KeyPressedEvent& e);
    bool OnKeyReleased(Hazel::KeyReleasedEvent& e);
private:
    Hazel::OrthographicCamera m_Camera;
    Hazel::Ref<Hazel::Shader> m_TetrisShader;
    Hazel::Ref<Hazel::Texture2D> m_TilesTexture;
    Hazel::Ref<Hazel::Texture2D> m_FrameTexture;
    Hazel::Ref<Hazel::Texture2D> m_BackgroundTexture;
    Hazel::Ref<Hazel::Texture2D> m_MenuIconTexture;
    std::array<Hazel::Ref<Hazel::SubTexture2D>, (int)Tile::NumTiles> m_Tiles;

    Level m_Level;

    ImFont* m_Font;
    ImFont* m_SettingFont;

    float m_Time = 0.0f;
    bool m_Blink = false;

    static std::vector<std::string> m_MainMenus;

    GameState m_State = GameState::MainMenu;
    //int m_Index = 0;//菜单当前激活的索引

    MenuConfig* m_MenuRoot = nullptr;
    MenuConfig* m_CurrentMenu = nullptr;

    // benckmark
    struct ProfileResult
    {
        const char* Name;
        float Time;
    };
    std::vector<ProfileResult> m_ProfileResults;

    //Debug Tool
    glm::vec4 m_ScreenMaskColor{ 0.2f, 0.2f, 0.2f, 0.2f };
    glm::vec4 m_ActiveMenuItemColor{ 0.0f, 0.9f, 0.8f, 1.0f };
    glm::vec4 m_MenuFontColor{ 1.0f, 1.0f, 1.0f, 1.0f };
};