#include <Hazel.h>
#include "Level.h"
#include "Random.h"

#include "TetrisAudio.h"

// texture information
constexpr float squareWidth = 18.0f * 2;
constexpr float windowWidth = 640.0f;
constexpr float windowHeight = 960.0f;
constexpr float leftPadding = 28.0f * 2;
constexpr float topPadding = 31.0f * 2;

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
    m_GameOver = false;
    m_Score = 0;
    memset(field, 0x00, sizeof(field));
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
    m_FallTime += ts;
    m_RotateTime += ts;
    m_DXTime += ts;
    //if (m_DeltaTime > m_Delay)
    {
        if (Hazel::Input::IsKeyPressed(Hazel::Key::Up))
        {
            m_Rotate = true;
        }
        else
        {
            m_Rotate = false;
            m_Rotated = false;
        }
        m_DX = 0;
        if (Hazel::Input::IsKeyPressed(Hazel::Key::Left))
        {
            m_CurrentMove = MoveDirection::Left;
            m_DX = -1;
        }
        else if (Hazel::Input::IsKeyPressed(Hazel::Key::Right))
        {
            m_CurrentMove = MoveDirection::Right;
            m_DX = 1;
        }
        else
        {
            m_CurrentMove = MoveDirection::None;
            m_DXDelay = 0.15f;
        }

        if (Hazel::Input::IsKeyPressed(Hazel::Key::Down))
            m_FallDelay = 0.05f;
        else
            m_FallDelay = 0.3f;

        // move
        if (m_CurrentMove != m_LastMove)
        {
            for (int i = 0; i < 4; ++i)
            {
                b[i] = a[i];
                a[i].x += m_DX;
            }
            if (!Check())
            {
                for (int i = 0; i < 4; ++i)
                    a[i] = b[i];
            }

            TetrisAudio::PlaySound(SoundEffect::Movement);
            m_LastMove = m_CurrentMove;
            m_DXTime = 0.0f;
            m_DXDelay = 0.15f;
        }
        else
        {
            // 连续移动
            if (m_DXTime > m_DXDelay)
            {
                for (int i = 0; i < 4; ++i)
                {
                    b[i] = a[i];
                    a[i].x += m_DX;
                }
                if (!Check())
                {
                    for (int i = 0; i < 4; ++i)
                        a[i] = b[i];
                }
                else if(m_DX != 0)
                {
                    TetrisAudio::PlaySound(SoundEffect::Movement);
                }

                m_LastMove = m_CurrentMove;
                m_DXTime = 0.0f;
                m_DXDelay = 0.05f;
            }
        }


        // rotate
        //if (m_RotateTime > m_RotateDelay)
        {
            if (m_Rotate && !m_Rotated)
            {
                Point p = a[1];  //center of rotation
                for (int i = 0; i < 4; ++i)
                {
                    int x = a[i].y - p.y;
                    int y = a[i].x - p.x;
                    a[i].x = p.x - x;
                    a[i].y = p.y + y;
                }
                if (!Check()) for (int i = 0; i < 4; i++) a[i] = b[i];
                else TetrisAudio::PlaySound(SoundEffect::Movement);
                m_Rotated = true;
            }
            m_RotateTime = 0.0f;
        }


        // tick
        if (m_FallTime > m_FallDelay)
        {
            for (int i = 0; i < 4; ++i)
            {
                b[i] = a[i];
                a[i].y += 1;
            }
            TetrisAudio::PlaySound(SoundEffect::Movement);

            if (!Check())
            {
                for (int i = 0; i < 4; ++i)
                {
                    field[b[i].y][b[i].x] = m_InitColorNum + 1;
                }
                m_InitColorNum = Random::Int(0, 6);
                for (int i = 0; i < 4; ++i)
                {
                    a[i].x = figures[m_InitColorNum][i] % 2 + 4;
                    a[i].y = figures[m_InitColorNum][i] / 2 - 1;
                }
                m_LastMove = MoveDirection::None;

                if (CollisionTest())
                {
                    GameOver();
                }
            }

            m_FallTime = 0.0f;
        }

        //check lines
        int k = M - 1;
        for (int i = M - 1; i > 0; --i)
        {
            int count = 0;
            for (int j = 0; j < N; ++j)
            {
                if (field[i][j])
                    count++;
                field[k][j] = field[i][j];
            }
            if (count < N)
            {
                --k;
            }
            else
            {
                ++m_Score;
            }
        }
    }


}

void Level::OnRenderer()
{
    float size = squareWidth * 2.0f / windowWidth;
    float size2 = squareWidth * 2.0f / windowHeight;
    float xOffset = leftPadding / (windowWidth / 2.0f) - 1.0f + size / 2.0f;
    float yOffset = 1.5f - size / 2.0f - topPadding / (windowWidth / 2.0f);  // WTF!
    for (int i = 0; i < M; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            if (field[i][j] == 0)
                continue;
            Hazel::Renderer2D::DrawQuad({j * size + xOffset, yOffset - i * size}, { size, size }, m_Tiles[field[i][j] - 1]);
        }
    }

    for (int i = 0; i < 4; ++i)
    {
        Hazel::Renderer2D::DrawQuad({ a[i].x * size + xOffset, yOffset - a[i].y * size }, { size, size }, m_Tiles[m_InitColorNum]);
        //HZ_INFO("int point: ({0}:{1})", a[i].x, a[i].y);
        //HZ_INFO("float point: ({0}:{1})", a[i].x * size + xOffset, a[i].y * size);
    }
}

bool Level::CollisionTest()
{
    for (int i = 0; i < 4; i++)
        if (field[a[i].y][a[i].x]) return true;
    return false;
}

void Level::GameOver()
{
    m_GameOver = true;
}

bool Level::OnKeyPressed(Hazel::KeyPressedEvent& e)
{
    if (e.GetKeyCode() == Hazel::Key::Up)
    {
        m_Rotate = true;
    }
    if (e.GetKeyCode() == Hazel::Key::Left)
    {
        m_DX = -1;
    }
    else if (e.GetKeyCode() == Hazel::Key::Right)
    {
        m_DX = 1;
    }
    return false;
}

bool Level::OnKeyReleased(Hazel::KeyReleasedEvent& e)
{
    if (e.GetKeyCode() == Hazel::Key::Up)
    {
        m_Rotate = false;
    }
    if (e.GetKeyCode() == Hazel::Key::Left)
    {
        m_DX = 0;
    }
    else if (e.GetKeyCode() == Hazel::Key::Right)
    {
        m_DX = 0;
    }
    return false;
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
    // 为了居中 x + 4, y - 1
    for (int i = 0; i < 4; ++i)
    {
        a[i].x = figures[m_InitColorNum][i] % 2 + 4;
        a[i].y = figures[m_InitColorNum][i] / 2 - 1;
    }
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