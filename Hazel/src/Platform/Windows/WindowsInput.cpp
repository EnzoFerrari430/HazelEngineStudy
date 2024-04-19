#include "hzpch.h"
#include "Hazel/Core/Input.h"
#include "Hazel/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Hazel {

    // glfwGetKey�޷���ȡGLFW_REPEAT״̬, ��Ҫ��clent���Լ�����GLFW_REPEAT״̬
    bool Input::IsKeyPressed(KeyCode key)
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetKey(window, static_cast<int32_t>(key));
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::IsMouseButtonPressed(MouseCode button)
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));
        return state == GLFW_PRESS;
    }

    std::pair<float, float> Input::GetMousePosition()
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        
        return { (float)xpos, (float)ypos };
    }

    float Input::GetMouseX()
    {
        // C++17
        auto[x, y] = GetMousePosition();
        return x;
    }

    float Input::GetMouseY()
    {
        // C++17
        auto[x, y] = GetMousePosition();
        return y;
    }

}