#include "hzpch.h"
#include "Application.h"

#include "Hazel/Renderer/Renderer.h"

#include "Hazel/Input.h" 

#include "Hazel/Log.h"

#include <GLFW/glfw3.h>

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

    //�¼����е���
    void Application::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowCloseEvent>(HZ_BIND_EVENT_FN(Application::OnWindowClose));

        //�¼������Ӻ���ǰ
        for (auto it = m_Layerstack.end(); it != m_Layerstack.begin();)
        {
            (*--it)->OnEvent(event);
            if (event.Handled)
                break;
        }
    }

    /*
    TODO:
    ����Ҫ���߼�����Ⱦ��������
    OnLogical
    OnRender
    ��������
    */
    void Application::Run()
    {
        while (m_Running)
        {
            float time = (float)glfwGetTime(); // TODO: Platform::GetTime()
            Timestep timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;

            //ͼ����Ⱦ ��ǰ����
            for (Layer* layer : m_Layerstack)
                layer->OnUpdate(timestep);

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
