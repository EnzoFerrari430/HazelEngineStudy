#include "hzpch.h"
#include "Application.h"

#include "Hazel/Renderer/Renderer.h"

#include "Hazel/Core/Input.h" 

#include "Hazel/Core/Log.h"

#include <GLFW/glfw3.h>

namespace Hazel {

    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        HZ_PROFILE_FUNCTION();

        HZ_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        m_Window = Window::Create();
        m_Window->SetEventCallback(HZ_BIND_EVENT_FN(Application::OnEvent));

        Renderer::Init();

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
    }

    Application::~Application()
    {
    }

    void Application::PushLayer(Layer* layer)
    {
        HZ_PROFILE_FUNCTION();

        m_Layerstack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* layer)
    {
        HZ_PROFILE_FUNCTION();

        m_Layerstack.PushOverlay(layer);
        layer->OnAttach();
    }

    //事件集中调度
    void Application::OnEvent(Event& event)
    {
        HZ_PROFILE_FUNCTION();

        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowCloseEvent>(HZ_BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(Application::OnWindowResize));

        //事件处理，从后往前
        for (auto it = m_Layerstack.rbegin(); it != m_Layerstack.rend(); ++it)
        {
            (*it)->OnEvent(event);
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
        HZ_PROFILE_FUNCTION();

        while (m_Running)
        {
            HZ_PROFILE_SCOPE("Run Loop");

            float time = (float)glfwGetTime(); // TODO: Platform::GetTime()
            Timestep timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;

            //图像渲染 从前往后
            if (!m_Minimized)
            {
                {
                    HZ_PROFILE_SCOPE("LayerStack OnUpdate");

                    for (Layer* layer : m_Layerstack)
                    layer->OnUpdate(timestep);
                }

                m_ImGuiLayer->Begin();
                {
                    HZ_PROFILE_SCOPE("LayerStack OnImGuiRenderer");

                    for (Layer* layer : m_Layerstack)
                        layer->OnImGuiRender();
                }
                m_ImGuiLayer->End();
            }

            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        HZ_PROFILE_FUNCTION();

        if (e.GetWidth() == 0 || e.GetHeight() == 0)
        {
            m_Minimized = true;
            return false;
        }

        m_Minimized = false;
        Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

        return false;
    }

}
