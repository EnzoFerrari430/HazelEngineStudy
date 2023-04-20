#include <Hazel.h>
#include <Hazel/Core/EntryPoint.h>

#include "Sandbox2D.h"
#include "ExampleLayer.h"

class SandBox : public Hazel::Application
{
public:
    SandBox()
    {
        PushLayer(new ExampleLayer());
        //PushLayer(new Sandbox2D());
    }

    ~SandBox()
    {

    }
};

Hazel::Application* Hazel::CreateApplication()
{
    return new SandBox();
}