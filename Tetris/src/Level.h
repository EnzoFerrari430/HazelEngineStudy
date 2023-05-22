#pragma once

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
    void init(PlayMode mode);

    void OnUpdate(Hazel::Timestep ts);
    void OnRenderer();

    bool IsGameOver() const { return m_GameOver; }

private:
    void GameOver() { m_GameOver = true; }
private:
    bool m_GameOver = false;
};