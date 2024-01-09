
![wyvern](https://github.com/EKralicky/Wyvern/assets/88636893/7de0ffc6-de6f-400e-8f31-ef31989b56ad)

Wyvern aims to be a real-time voxel raytracing engine (Currently only supports rasterization). The main goal of this engine is to raytrace hundreds of millions of voxels to the screen in real-time, eventually to become the driver of an open world voxel game. 

# Installation & Setup (Windows Only)
1. **Download & Install Visual Studio Community Edition**
    - Visit [the Visual Studio download page](https://visualstudio.microsoft.com/vs/community/) and download the latest installer
    - Run the installer. When installing, make sure to configure Visual Studio for "Desktop development for C++"
    - If you already have Visual Studio, but it is not configured for C++ development, you can modify your installation within the Visual Studio Installer application.
2. **Clone the Repository**
    - Clone this repository to any desired location on your machine
    - You can use this command in the command prompt to do so: `git clone --recurse-submodules -j5 https://github.com/EKralicky/Wyvern.git`
4. **Setup Wyvern for Visual Studio Development**
    - Open a new terminal or command prompt as administrator
    - Navigate to the root directory of the cloned project using the `cd` command
    - Build the project files using `.\build.bat <Visual Studio version>`
        - Replace `<Visual Studio version>` with the desired version in the format of `vs<year>`. For example, `vs2022`.
        - The batch script will handle submodule setup, all dependency installations, and will run premake for you     
5. **Help**
    - For further build help, you can run `.\build.bat help` in the terminal at Wyverns root directory
    - If you need to clean the Visual Studio solution and project files, run `.\premake\premake5.exe clean`
    - The VulkanSDK installation requires administrator privileges so make sure to use an elevated command prompt when running the batch script
    - If you suspect that the VulkanSDK files did not install properly, delete the `vulkanSDK` folder under `/Wyvern/vendor/` and re-run the batch script

# Features & Objectives
- [x] Vulkan pipelines & API abstraction
  - [x] Rasterization Pipeline
  - [ ] Raytracing Pipeline
- [x] Runtime shader compilation (shaderc)
- [x] ImGui debugging
- [x] Polling input system using GLFW
- [x] Player movement
  - [x]  Player & Player controller
  - [x]  3D Perspective Camera
- [ ] Phong lighting
- [ ] Efficient voxel acceleration structures for raytracing
- [ ] Raytraced rendering with shadows
- [ ] Efficient voxel storage & compression -- Able to store and render a very large scene
- [ ] Fast voxel streaming from disk
- [ ] Chunking systems to improve load speed & voxel update times
- [ ] Physics integration - preferrably Nvidia PHYSX or Bullet

# Challenges
Raytracing 100's of millions of voxels won't be very fast even with the latest hardware. To achieve this, fast and resourceful algorithms need to be implemented to improve ray traversal, speed, and sparse voxel storage. One of the main issues under reasearch is sparse voxel storage. There are various solutions to this problem such as octrees, aabb trees, signed distance fields, and hashmaps, but the best solution may be to use them all, where their strengths are most needed

# Dependencies Used
- Vulkan
  - shaderc glsl compiler
- GLFW
- GLM
- ImGui
- ImPlot
- spdlog
