#include "hzpch.h"
#include "Application.h"

#include "Log.h"

#include <glad/glad.h>

#include "Hazel/Input.h" 

namespace Hazel {

//#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        HZ_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(HZ_BIND_EVENT_FN(Application::OnEvent));
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

        //HZ_CORE_INFO("{0}", event);

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
            glClearColor(1.0f, 0.0f, 1.0f, 0.33f);
            glClear(GL_COLOR_BUFFER_BIT);
            
            //ͼ����Ⱦ ��ǰ����
            for (Layer* layer : m_Layerstack)
                layer->OnUpdate();
            
            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }

}
