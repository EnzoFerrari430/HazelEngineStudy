#include "hzpch.h"
#include "Application.h"

#include "Events/ApplicationEvent.h"
#include "Log.h"


namespace Hazel {

	Application::Application()
	{
	
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
		WindowResizeEvent e(1280, 720);
		HZ_INFO(e);
		while (true);
	}

}
