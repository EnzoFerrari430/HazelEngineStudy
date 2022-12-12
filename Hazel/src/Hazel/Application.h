#pragma once

#include "Core.h"
#include "Events/Event.h"

namespace Hazel {
	
	/*
		Application�����ռ��¼�(event)����ת����layer
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


