#include "hzpch.h"
#include "Input.h"

#ifdef HZ_PLATFORM_WINDOWS
    #include "Platform/Windows/WindowsInput.h"
#endif

namespace Hazel {

    Scope<Input> Input::s_Instance = Input::Create();

    Scope<Input> Input::Create()
    {
    #ifdef HZ_PLATFORM_WINDOWS
        return CreateScope<WindowsInput>();
    #elif
        HZ_CORE_ASSERT(false, "Unknown platform!");
        return nullptr;
    #endif
    }
}