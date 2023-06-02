#pragma once
#include "Player.h"
#include "Tile.h"

enum class PlayMode
{
    Normal,         //普通模式
    Cooperation,    //合作模式
    Versus,         //对抗模式
    DarkLight       //暗光模式
};

class Level
{
public:
    void Init();
    void SetPlayMode(PlayMode mode);
    void Reset();

    void OnUpdate(Hazel::Timestep ts);
    void OnRenderer();
    void OnRendererForeGround();
    void OnRendererBackGround();


    bool IsGameOver() const { return m_GameOver; }

    bool OnKeyPressed(Hazel::KeyPressedEvent& e);
    bool OnKeyReleased(Hazel::KeyReleasedEvent& e);
private:


    void GameOver() { m_GameOver = true; }

    void InitNormal();
    void InitDrakLight();
    void InitCooperation();
    void InitVersus();
private:
    Hazel::Ref<Hazel::Shader> m_TetrisShader;
    Hazel::Ref<Hazel::Texture2D> m_TilesTexture;
    Hazel::Ref<Hazel::Texture2D> m_FrameTexture;
    Hazel::Ref<Hazel::Texture2D> m_BackgroundTexture;
    std::array<Hazel::Ref<Hazel::SubTexture2D>, (int)Tile::NumTiles> m_Tiles;
private:
    bool m_GameOver = false;
    int m_InitColorNum = 4;

    //下落的间隔
    float m_FallTime = 0.0f;
    float m_FallDelay = 0.3f;

    //旋转间隔
    bool m_Rotate = false;
    bool m_Rotated = true;
    float m_RotateTime = 0.0f;
    float m_RotateDelay = 0.1f;

    //左右位移间隔
    int m_DX = 0;
    float m_DXTime = 0.0f;
    float m_DXDelay = 0.05f;

    PlayMode m_PlayMode = PlayMode::Normal;
    std::vector<std::shared_ptr<Player>> m_Players;
};