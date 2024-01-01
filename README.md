
![wyvern](https://github.com/EKralicky/Wyvern/assets/88636893/7de0ffc6-de6f-400e-8f31-ef31989b56ad)

Wyvern aims to be a real-time voxel raytracing engine (Currently only supports rasterization). The main goal of this engine is to raytrace hundreds of millions of voxels to the screen in real-time, eventually to become the driver of an open world voxel game. 

### Features & Objectives
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
