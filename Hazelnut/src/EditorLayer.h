#pragma once

#include <Hazel.h>

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"

#include "Hazel/Renderer/EditorCamera.h"

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

#define PROFILE_SCOPE(name) Timer scopeTimer##__LINE__(name, [&](ProfileResult profileResult) {m_ProfileResults.push_back(profileResult); })

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
        bool OnKeyPressed(KeyPressedEvent& e);
        bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

        void NewScene();
        void OpenScene();
        void SaveSceneAs();
    private:
        OrthographicCameraController m_CameraController;

        Ref<Texture2D> m_SpriteSheet;
        Ref<SubTexture2D> m_TextureStair;
        Ref<SubTexture2D> m_TextureBarrel;
        Ref<SubTexture2D> m_TextureTree;
        Ref<Framebuffer> m_Framebuffer;

        Ref<Scene> m_ActiveScene;
        Entity m_HoveredEntity;

        bool m_PrimaryCamera = true;

        EditorCamera m_EditorCamera;

        bool m_ViewportFocused = false, m_ViewportHovered = false;
        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
        glm::vec2 m_ViewportBounds[2]; //基于屏幕坐标系的viewport区域

        int m_GizmoType = -1;

        // Panels
        SceneHierarchyPanel m_SceneHierarchyPanel;
        ContentBrowserPanel m_ContentBrowserPanel;

        // benckmark
        struct ProfileResult
        {
            const char* Name;
            float Time;
        };
        std::vector<ProfileResult> m_ProfileResults;
    };

}

