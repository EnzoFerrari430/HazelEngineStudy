#pragma once

#include <Hazel.h>

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

namespace Hazel {

    class EditorLayer : public Layer
    {
    public:
        EditorLayer();
        virtual ~EditorLayer() = default;

        void OnAttach() override;
        void OnDetach() override;

        void OnUpdate(Timestep ts) override;
        void OnImGuiRender() override;
        void OnEvent(Event& e) override;
    private:
        OrthographicCameraController m_CameraController;

        Ref<Texture2D> m_BoxTexture;
        Ref<Texture2D> m_SpriteSheet;
        Ref<SubTexture2D> m_TextureStair;
        Ref<SubTexture2D> m_TextureBarrel;
        Ref<SubTexture2D> m_TextureTree;
        Ref<Framebuffer> m_Framebuffer;

        int m_MapWidth, m_MapHeight;
        std::unordered_map<char, Ref<SubTexture2D>> m_TextureMap;

        glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };


        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

        // benckmark
        struct ProfileResult
        {
            const char* Name;
            float Time;
        };
        std::vector<ProfileResult> m_ProfileResults;
    };

}

