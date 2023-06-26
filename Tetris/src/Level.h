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

enum MoveDirection
{
    Left,
    Right,
    None
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
    int GetScore() const { return m_Score; }

    bool OnKeyPressed(Hazel::KeyPressedEvent& e);
    bool OnKeyReleased(Hazel::KeyReleasedEvent& e);
private:
    bool CollisionTest();

    void GameOver();

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
    int m_Score = 0;
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
    //TODO: 增加一个容器 记录每一帧键盘是按了左键还是右键
    //      重复按键就记录到容器中, 并进行逻辑判断
    int m_DX = 0;
    float m_DXTime = 0.0f;
    float m_DXDelay = 1.5f;
    MoveDirection m_CurrentMove = MoveDirection::None;
    MoveDirection m_LastMove = MoveDirection::None;

    PlayMode m_PlayMode = PlayMode::Normal;
    std::vector<std::shared_ptr<Player>> m_Players;
};