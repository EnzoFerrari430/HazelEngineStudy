#pragma once

#include "Hazel/Core.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Window.h"
#include "Hazel/LayerStack.h"

#include "Hazel/ImGui/ImGuiLayer.h"

#include "Hazel/Core/Timestep.h"


namespace Hazel {
    
    /*
        Application用于收集事件(event)，并转发给layer
    */
    class HAZEL_API Application
    {
    public:
        Application();
        virtual ~Application();

        void OnEvent(Event& event);
        void Run();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);

        inline static Application& Get() { return *s_Instance; }
        inline Window& GetWindow() { return *m_Window; }
    private:
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);
    private:
        std::unique_ptr<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        bool m_Running = true;
        bool m_Minimized = false;
        LayerStack m_Layerstack;
        float m_LastFrameTime = 0.0f;
    private:
        static Application* s_Instance;
    };

    //To be defined in CLIENT
    Application* CreateApplication();

}


