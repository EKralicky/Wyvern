project "Wyvern"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    inheritdependencies "On"

    targetdir("../bin/" .. outputdir .. "/%{prj.name}")
    objdir("../bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "src/**.cpp",
        "src/**.h",
        "src/**.hpp",
        "vendor/glm/glm/**.hpp",
        "vendor/glm/glm/**.inl",
        "vendor/imgui/*.cpp",
        "vendor/imgui/*.h",
        "vendor/imgui/backends/imgui_impl_glfw.cpp",
        "vendor/imgui/backends/imgui_impl_glfw.h",
        "vendor/imgui/backends/imgui_impl_vulkan.cpp",
        "vendor/imgui/backends/imgui_impl_vulkan.h",
        "vendor/implot/*.cpp",
        "vendor/implot/*.h",
        "vendor/spdlog/include/**.cpp",
        "vendor/spdlog/include/**.h",
    }

    includedirs {
        "src",
        "vendor/glm",
        "vendor/imgui",
        "vendor/implot",
        "vendor/spdlog/include",
        "vendor/vulkanSDK/Include",
        "vendor/glfw/include"
    }

    filter "action:vs2022"
        libdirs {
            "vendor/vulkanSDK/Lib",
            "vendor/glfw/bin/lib-vc2022"
        }

        links {
            "vulkan-1.lib",
            "glfw3.lib",
            "glslangd.lib",
            "SPIRVd.lib",
            "OGLCompilerd.lib",
            "OSDependentd.lib",
            "HLSLd.lib"
        }

    filter "configurations:Debug"
        defines { "DEBUG" }
        buildoptions "/MDd"
        runtime "Debug"
        symbols "On"

        links {
            "shaderc_combinedd.lib",
        }
    
    filter "configurations:Release"
        defines { "NDEBUG" }
        buildoptions "/MT"
        runtime "Release"
        optimize "On"

        links {
            "shaderc_combined.lib",
        }