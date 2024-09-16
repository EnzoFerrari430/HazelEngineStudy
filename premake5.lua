include "./vendor/premake/premake_customization/solution_items.lua"

workspace "Hazel"
    architecture "x86_64"  --64位架构
    startproject "Hazelnut"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

    flags
    {
        "MultiProcessorCompile"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"  -- debug-windows-x64
-- printf("%s\n", outputdir)  --打印输出路径

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Hazel/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Hazel/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Hazel/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/Hazel/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/Hazel/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/Hazel/vendor/entt/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/Hazel/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/Hazel/vendor/ImGuizmo"

-- 引入GLFW的premake5文件
group "Dependencies"
    include "vendor/premake"
    include "Hazel/vendor/GLFW"
    include "Hazel/vendor/Glad"
    include "Hazel/vendor/imgui"
    include "Hazel/vendor/yaml-cpp"
    include "HazelAudio/HazelAudio"
    include "HazelAudio/HazelAudio/vendor/OpenAL-Soft"
    include "HazelAudio/HazelAudio/vendor/libogg"
    include "HazelAudio/HazelAudio/vendor/Vorbis"
group ""

include "Hazel"
include "Sandbox"
include "Hazelnut"
include "Tetris"