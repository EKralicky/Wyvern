
![wyvern](https://github.com/EKralicky/Wyvern/assets/88636893/8bfa0da2-af9e-41f9-bc6c-6f057010b187)


Wyvern aims to be a real-time voxel raytracing engine (Currently only supports rasterization). The main goal of this engine is to raytrace hundreds of millions of voxels to the screen in real-time, eventually to become the driver of an open world voxel game. 

### Objectives
- Custom event based keybinding system to handle all user input
- Raytraced rendering with shadows in real-time
- Efficient voxel storage & compression -- Able to store and render a very large scene
- Level of Detail
- Fast voxel streaming from disk
- Chunking systems to improve load speed & voxel update times
- Simple physics integration - preferrably Nvidia PHYSX or Bullet

### Challenges
Raytracing 100's of millions of voxels won't be very fast even with the latest hardware. To achieve this, fast and resourceful algorithms need to be implemented to improve ray traversal, speed, and sparse voxel storage. One of the main issues under reasearch is sparse voxel storage. There are various solutions to this problem such as octrees, aabb trees, signed distance fields, and hashmaps, but the best solution may be to use them all, where their strengths are most needed


### Dependencies Used
- Vulkan
  - shaderc glsl compiler
- GLFW
- GLM
- ImGui
- ImPlot
- spdlog
