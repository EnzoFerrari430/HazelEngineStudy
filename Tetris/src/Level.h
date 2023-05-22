#pragma once

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
    void init(PlayMode mode);

    void OnUpdate(Hazel::Timestep ts);
    void OnRenderer();

    bool IsGameOver() const { return m_GameOver; }

private:
    void GameOver() { m_GameOver = true; }
private:
    bool m_GameOver = false;
};