#include "hzpch.h"
#include "Window.h"

#ifdef HZ_PLATFORM_WINDOWS
    #include "Platform/Windows/WindowsWindow.h"
#endif

namespace Hazel {

    Scope<Window> Window::Create(const WindowProps& props)
    {
    #ifdef HZ_PLATFORM_WINDOWS
        return CreateScope<WindowsWindow>(props);
    #elif
        HZ_CORE_ASSERT(false, "Unknown platform!");
        return nullptr;
    #endif
    }

}