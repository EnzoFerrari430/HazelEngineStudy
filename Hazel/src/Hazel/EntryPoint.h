#pragma once

#ifdef HZ_PLATFORM_WINDOWS

//extern关键字 不是必须的
extern Hazel::Application* Hazel::CreateApplication();

int main(int argc, char** argv)
{
	Hazel::Log::Init();
	//HZ_CORE_CRITICAL("Initialized Log!");
	//int a = 10, b = 20;
	//HZ_ERROR("Hello and a = {0}, b = {1}", a, b);

	auto app = Hazel::CreateApplication();
	app->Run();
	delete app;

	return 0;
}

#endif