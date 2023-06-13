#pragma once
#include "Player.h"
#include "Tile.h"

enum class PlayMode
{
    Normal,         //��ͨģʽ
    Cooperation,    //����ģʽ
    Versus,         //�Կ�ģʽ
    DarkLight       //����ģʽ
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

    //����ļ��
    float m_FallTime = 0.0f;
    float m_FallDelay = 0.3f;

    //��ת���
    bool m_Rotate = false;
    bool m_Rotated = true;
    float m_RotateTime = 0.0f;
    float m_RotateDelay = 0.1f;

    //����λ�Ƽ��
    //TODO: ����һ������ ��¼ÿһ֡�����ǰ�����������Ҽ�
    //      �ظ������ͼ�¼��������, �������߼��ж�
    int m_DX = 0;
    bool m_DXRepeat = false;
    bool m_DXRepeated = false;
    float m_DXRepeatDelayTime = 0.2f;
    float m_DXTime = 0.0f;
    float m_DXDelay = 0.1f;

    PlayMode m_PlayMode = PlayMode::Normal;
    std::vector<std::shared_ptr<Player>> m_Players;
};