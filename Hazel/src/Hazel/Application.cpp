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

	//�¼����е���
	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		HZ_CORE_INFO("{0}", event);

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
