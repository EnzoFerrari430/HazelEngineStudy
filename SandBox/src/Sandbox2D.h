#pragma once

#include <Hazel.h>

#include "ParticleSystem.h"

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

class Sandbox2D : public Hazel::Layer
{
public:
    Sandbox2D();
    virtual ~Sandbox2D() = default;

    void OnAttach() override;
    void OnDetach() override;

    void OnUpdate(Hazel::Timestep ts) override;
    void OnImGuiRender() override;
    void OnEvent(Hazel::Event& e) override;
private:
    Hazel::OrthographicCameraController m_CameraController;

    Hazel::Ref<Hazel::Texture2D> m_BoxTexture;
    Hazel::Ref<Hazel::Texture2D> m_SpriteSheet;
    Hazel::Ref<Hazel::SubTexture2D> m_TextureStair;
    Hazel::Ref<Hazel::SubTexture2D> m_TextureBarrel;
    Hazel::Ref<Hazel::SubTexture2D> m_TextureTree;

    int m_MapWidth, m_MapHeight;
    std::unordered_map<char, Hazel::Ref<Hazel::SubTexture2D>> m_TextureMap;

    glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

    ParticleProps m_Particle;
    ParticleSystem m_ParticleSystem;

    // benckmark
    struct ProfileResult
    {
        const char* Name;
        float Time;
    };
    std::vector<ProfileResult> m_ProfileResults;
};