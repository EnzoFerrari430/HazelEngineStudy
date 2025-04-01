#include <Hazel.h>
#include <Hazel/Core/EntryPoint.h>

#include "Sandbox2D.h"
#include "ExampleLayer.h"

class SandBox : public Hazel::Application
{
public:
    SandBox(Hazel::ApplicationCommandLineArgs args)
    {
        //PushLayer(new ExampleLayer());
        PushLayer(new Sandbox2D());
    }

    ~SandBox()
    {

    }
};

Hazel::Application* Hazel::CreateApplication(Hazel::ApplicationCommandLineArgs args)
{
    return new SandBox(args);
}