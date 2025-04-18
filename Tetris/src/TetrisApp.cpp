#include <Hazel.h>
#include <Hazel/Core/EntryPoint.h>

#include "TetrisLayer.h"
#include "HazelAudio.h"

class SandBox : public Hazel::Application
{
public:
    SandBox(Hazel::ApplicationCommandLineArgs args)
        : Application("Tetris", args)
    {
        Hazel::Audio::Init();

        PushLayer(new TetrisLayer());
        Get().GetWindow().SetVSync(true);
        Get().GetWindow().SetWindowTitle("Tetris");
        Get().GetWindow().SetWindowSize(640, 960);
    }

    ~SandBox()
    {

    }
};

Hazel::Application* Hazel::CreateApplication(ApplicationCommandLineArgs args)
{
    return new SandBox(args);
}