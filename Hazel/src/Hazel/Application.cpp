#include "hzpch.h"
#include "Application.h"

#include "Hazel/Renderer/Renderer.h"

#include "Hazel/Input.h" 

#include "Hazel/Log.h"

namespace Hazel {

    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        HZ_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(HZ_BIND_EVENT_FN(Application::OnEvent));

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
    }
    
    Application::~Application()
    {
    }

    void Application::PushLayer(Layer* layer)
    {
        m_Layerstack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* layer)
    {
        m_Layerstack.PushOverlay(layer);
        layer->OnAttach();
    }

    //事件集中调度
    void Application::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowCloseEvent>(HZ_BIND_EVENT_FN(Application::OnWindowClose));

        //HZ_CORE_INFO("{0}", event);

        //事件处理，从后往前
        for (auto it = m_Layerstack.end(); it != m_Layerstack.begin();)
        {
            auto itor = --it;
            HZ_CORE_INFO("{0} Layer Name: {1}", __FUNCTION__, (*itor)->GetName());
            (*itor)->OnEvent(event);
            if (event.Handled)
                break;
        }
    }

    /*
    TODO:
    这里要对逻辑和渲染进行区分
    OnLogical
    OnRender
    两个步骤
    */
    void Application::Run()
    {
        while (m_Running)
        {
            //图像渲染 从前往后
            for (Layer* layer : m_Layerstack)
                layer->OnUpdate();

            m_ImGuiLayer->Begin();
            for (Layer* layer : m_Layerstack)
                layer->OnImGuiRender();
            m_ImGuiLayer->End();
            
            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }

}
