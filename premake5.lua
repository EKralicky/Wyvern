workspace "Workspace"
    architecture "x64"
    configurations {
        "Debug",
        "Release"
    }
    startproject "Wyvern"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Wyvern"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir("bin" .. outputdir .. "/%{prj.name}")
    objdir("bin-int" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.hpp"
    }

    filter { "configurations:Debug" }
        defines { "DEBUG" }
        buildoptions "/MTd"
        runtime "Debug"
        symbols "On"

    filter { "configurations:Release" }
        defines { "NDEBUG" }
        buildoptions "/MT"
        runtime "Release"
        optimize "On"