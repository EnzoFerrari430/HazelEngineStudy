#include <Hazel.h>
#include <Hazel/Core/EntryPoint.h>

#include "TetrisLayer.h"

class SandBox : public Hazel::Application
{
public:
    SandBox()
    {
        PushLayer(new TetrisLayer());
        Get().GetWindow().SetWindowTitle("Tetris");
    }

    ~SandBox()
    {

    }
};

Hazel::Application* Hazel::CreateApplication()
{
    return new SandBox();
}