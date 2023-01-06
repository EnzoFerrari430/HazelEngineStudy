workspace "Hazel"
    architecture "x86_64"  --64位架构
    startproject "Sandbox"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"  -- debug-windows-x64
-- printf("%s\n", outputdir)  --打印输出路径

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Hazel/vendor/GLFW/include"
IncludeDir["Glad"] = "Hazel/vendor/Glad/include"
IncludeDir["ImGui"] = "Hazel/vendor/imgui"

-- 引入GLFW的premake5文件
include "Hazel/vendor/GLFW"
include "Hazel/vendor/Glad"
include "Hazel/vendor/imgui"

project "Hazel"
    location "Hazel"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    -- 预编译头文件
    pchheader "hzpch.h"
    pchsource "Hazel/src/hzpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs
    {
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}"
    }
    -- Hazel需要链接的库
    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "Off"
        systemversion "latest"

        defines
        {
            "HZ_PLATFORM_WINDOWS",
            "HZ_BUILD_DLL",
            "GLFW_INCLUDE_NONE" --glfw不包含gl头文件，让glad去包含gl头文件
        }

        postbuildcommands --项目属性里的生成后事件
        {
            ("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
        }

    filter "configurations:Debug"
        defines "HZ_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "HZ_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines "HZ_DIST"
        runtime "Release"
        optimize "On"



project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "Hazel/vendor/spdlog/include",
        "Hazel/src"
    }

    links
    {
        "Hazel"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "Off"
        systemversion "latest"

        defines
        {
            "HZ_PLATFORM_WINDOWS",
        }

    filter "configurations:Debug"
        defines "HZ_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "HZ_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines "HZ_DIST"
        runtime "Release"
        optimize "On"