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
	����Ҫ���߼�����Ⱦ��������
	OnLogical
	OnRender
	��������
	*/
	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		HZ_INFO(e);
		while (true);
	}

}
