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
    PlayMode m_PlayMode = PlayMode::Normal;
    std::vector<std::shared_ptr<Player>> m_Players;
};