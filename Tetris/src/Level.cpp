#include <Hazel.h>
#include "Level.h"

// texture information
constexpr float squareWidth = 18.0f * 2;
constexpr float windowWidth = 640.0f;
constexpr float windowHeight = 960.0f;
constexpr float leftPadding = 28.0f * 2;
constexpr float topPadding = 48.0f * 2;

constexpr int M = 20;
constexpr int N = 10;

int field[M][N] = { 0 };

int colorNum = 1;

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

bool Check()
{
    for (int i = 0; i < 4; i++)
        if (a[i].x < 0 || a[i].x >= N || a[i].y >= M) return false;
        else if (field[a[i].y][a[i].x]) return false;
    return true;
}

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
    m_DeltaTime += ts;
    //if (m_DeltaTime > m_Delay)
    {
        bool rotate = false;
        int dx = 0;
        if (Hazel::Input::IsKeyPressed(HZ_KEY_UP))
            rotate = true;
        if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT))
            dx = -1;
        else if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT))
            dx = 1;

        // move
        for (int i = 0; i < 4; ++i)
        {
            b[i] = a[i];
            a[i].x += dx;
        }
        if (!Check())
        {
            for (int i = 0; i < 4; ++i)
                a[i] = b[i];
        }

        // rotate
        if (rotate)
        {
            Point p = a[1];  //center of rotation
            for (int i = 0; i < 4; ++i)
            {
                int x = a[i].y - p.y;
                int y = a[i].x - p.x;
                a[i].x = p.x - x;
                a[i].y = p.y + y;
            }
        }

        // tick
        if (m_DeltaTime > m_Delay)
        {
            for (int i = 0; i < 4; ++i)
            {
                b[i] = a[i];
                a[i].y += 1;
            }

            if (!Check())
            {
                for (int i = 0; i < 4; ++i)
                {
                    field[b[i].y][b[i].x] = colorNum;
                }
            }

            m_DeltaTime = 0.0f;
        }

        int n = 4;
        if (a[0].x == 0)
            for (int i = 0; i < 4; ++i)
            {
                a[i].x = figures[n][i] % 2;
                a[i].y = figures[n][i] / 2;
            }

    }


}

void Level::OnRenderer()
{
    float size = squareWidth * 2.0f / windowWidth;
    float xOffset = leftPadding / (windowWidth / 2.0f) - 1.0f + size / 2.0f;
    float yOffset = 1.0f - topPadding / (windowHeight / 2.0f);
    for (int i = 0; i < M; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            if (field[i][j] == 0)
                continue;
            Hazel::Renderer2D::DrawQuad({i * size, j * size}, { size, size }, m_Tiles[0]);
        }
    }

    for (int i = 0; i < 4; ++i)
    {
        Hazel::Renderer2D::DrawQuad({ a[i].x * size + xOffset, a[i].y * size + yOffset}, { size, size }, m_Tiles[0]);
        //Hazel::Renderer2D::DrawQuad({ -0.825f + size / 2.0f, a[i].y * size }, { size, size }, m_Tiles[0]);
        HZ_INFO("int point: ({0}:{1})", a[i].x, a[i].y);
        HZ_INFO("float point: ({0}:{1})", a[i].x * size + xOffset, a[i].y * size);
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