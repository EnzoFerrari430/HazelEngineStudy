#pragma once

#include "Hazel/Core.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Window.h"

namespace Hazel {
	
	/*
		Application用于收集事件(event)，并转发给layer
	*/
	class HAZEL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	//To be defined in CLIENT
	Application* CreateApplication();

}


