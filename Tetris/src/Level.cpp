#include <Hazel.h>
#include "Level.h"

constexpr int M = 20;
constexpr int N = 10;

int field[M][N] = { 0 };

struct Point
{
    int x, y;
} a[4], b[4];

int figures[7][4] = 
{
    1, 3, 5, 7, //I
    2, 4, 5, 7, //Z
    3, 5, 4, 6, //S
    3, 5, 4, 7, //T
    2, 3, 5, 7, //L
    3, 5, 7, 6, //J
    2, 3, 4, 5  //O
};

void Level::Init()
{
    m_TilesTexture = Hazel::Texture2D::Create("assets/textures/tiles.png");
    m_FrameTexture = Hazel::Texture2D::Create("assets/textures/frame.png");
    m_BackgroundTexture = Hazel::Texture2D::Create("assets/textures/background.png");

    for (size_t i = 0; i < m_Tiles.size(); ++i)
    {
        m_Tiles[i] = Hazel::SubTexture2D::CreateFromCoords(m_TilesTexture, { i * 18.0f, 0.0f }, { 18.0f, 18.0f });
    }


    m_Players.reserve(2);
    m_Players.push_back(std::make_shared<Player>());
    m_Players.push_back(std::make_shared<Player>());
    m_PlayMode = PlayMode::Normal;
    Reset();
}

void Level::SetPlayMode(PlayMode mode)
{
    m_PlayMode = mode;
    Reset();
}

void Level::Reset()
{
    switch (m_PlayMode)
    {
    case PlayMode::Normal:
        InitNormal();
        break;
    case PlayMode::DarkLight:
        InitDrakLight();
        break;
    case PlayMode::Cooperation:
        InitCooperation();
        break;
    case PlayMode::Versus:
        InitVersus();
        break;
    }
}

void Level::OnUpdate(Hazel::Timestep ts)
{
    int n = 4;
    for (int i = 0; i < 4; ++i)
    {
        a[i].x = figures[n][i] % 2;
        a[i].y = figures[n][i] / 2;
    }
}

void Level::OnRenderer()
{
    float size = 18.0f * 4.0f / 640.0f;
    for (int i = 0; i < 4; ++i)
    {
        Hazel::Renderer2D::DrawQuad({ a[i].x * size, a[i].y * size }, { size, size }, m_Tiles[4]);
    }
}

void Level::OnRendererForeGround()
{
    Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.5f }, { 2.0f, 3.0f }, m_FrameTexture);
}

void Level::OnRendererBackGround()
{
    Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.5f }, { 2.0f, 3.0f }, m_BackgroundTexture);
}

void Level::InitNormal()
{

}

void Level::InitDrakLight()
{

}

void Level::InitCooperation()
{

}

void Level::InitVersus()
{

}