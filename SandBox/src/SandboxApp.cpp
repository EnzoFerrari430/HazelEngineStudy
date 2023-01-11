#include <Hazel.h>

class ExampleLayer : public Hazel::Layer
{
public:
    ExampleLayer()
        : Layer("Example")
    {

    }

    void OnUpdate() override
    {
        if (Hazel::Input::IsKeyPressed(HZ_KEY_TAB))
            HZ_TRACE("Tab key is pressed (poll)!");
    }

    void OnEvent(Hazel::Event& event) override
    {
        if (event.GetEventType() == Hazel::EventType::KeyPressed)
        {
            Hazel::KeyPressedEvent& e = dynamic_cast<Hazel::KeyPressedEvent&>(event);
            if (Hazel::Input::IsKeyPressed(HZ_KEY_TAB))
                HZ_TRACE("Tab key is pressed (event)!");
            HZ_TRACE("{0}", (char)e.GetKeyCode());
        }
    }
};

class SandBox : public Hazel::Application
{
public:
    SandBox()
    {
        PushLayer(new ExampleLayer());
        PushOverlay(new Hazel::ImGuiLayer());
    }

    ~SandBox()
    {

    }
};

Hazel::Application* Hazel::CreateApplication()
{
    return new SandBox();
}