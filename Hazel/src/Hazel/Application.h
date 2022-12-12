#pragma once

#include "Core.h"
#include "Events/Event.h"

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
	};

	//To be defined in CLIENT
	Application* CreateApplication();

}


