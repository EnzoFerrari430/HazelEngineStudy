include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

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


-- 引入GLFW的premake5文件
group "Dependencies"
    include "vendor/premake"
    include "Hazel/vendor/Box2D"
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