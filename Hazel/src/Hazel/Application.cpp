#include "hzpch.h"
#include "Application.h"

#include "Log.h"

#include <GLFW/glfw3.h>

namespace Hazel {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	}
	
	Application::~Application()
	{
	}

	void Application::PushLayer(Layer* layer)
	{
		m_Layerstack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_Layerstack.PushOverlay(layer);
	}

	//事件集中调度
	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		HZ_CORE_INFO("{0}", event);

		//事件处理，从后往前
		for (auto it = m_Layerstack.end(); it != m_Layerstack.begin();)
		{
			(*--it)->OnEvent(event);
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
			glClearColor(1.0f, 0.0f, 1.0f, 0.33f);
			glClear(GL_COLOR_BUFFER_BIT);
			
			//图像渲染 从前往后
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
