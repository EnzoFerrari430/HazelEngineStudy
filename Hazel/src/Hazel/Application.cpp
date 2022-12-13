#include "hzpch.h"
#include "Application.h"

#include "Events/ApplicationEvent.h"
#include "Log.h"

#include <GLFW/glfw3.h>

namespace Hazel {

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}
	
	Application::~Application()
	{
	
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
			m_Window->OnUpdate();
		}
	}

}
