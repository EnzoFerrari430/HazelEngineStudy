#pragma once

#include "Hazel/Core/Core.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Core/Window.h"
#include "Hazel/Core/LayerStack.h"

#include "Hazel/ImGui/ImGuiLayer.h"

#include "Hazel/Core/Timestep.h"

int main(int argc, char** argv);

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

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);

        inline static Application& Get() { return *s_Instance; }
        inline Window& GetWindow() { return *m_Window; }
    private:
        void Run();
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);
    private:
        Scope<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        bool m_Running = true;
        bool m_Minimized = false;
        LayerStack m_Layerstack;
        float m_LastFrameTime = 0.0f;
    private:
        static Application* s_Instance;
        friend int main(int argc, char** argv);
    };

    //To be defined in CLIENT
    Application* CreateApplication();

}


