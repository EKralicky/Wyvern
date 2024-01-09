@echo off

if "%1" == "help" (
    goto :help
)

set glfwVersion="3.3.9"
set vulkanVersion="1.3.268.0"
set glfwUrl="https://github.com/glfw/glfw/releases/download/%glfwVersion%/glfw-%glfwVersion%.bin.WIN64.zip"
set vulkanUrl="https://sdk.lunarg.com/sdk/download/%VulkanVersion%/windows/VulkanSDK-%vulkanVersion%-Installer.exe"

cd ./Wyvern/vendor

REM update git modules
set submoduleUpdateFlag=0
echo Checking for uninitialized submodules
for /f "tokens=*" %%a in ('git submodule status') do (
  call :checkUninitialized "%%a"
)

if %submoduleUpdateFlag%==1 (
    echo updating uninitialized submodules
    git submodule init
    git submodule update
) else (
    echo No uninitialized submodules found.
)

REM install and extract GLFW precompiled binaries for WIN64
if exist "./glfw/bin" (
    echo GLFW binaries already installed!
) else (
    echo Downloading GLFW %glfwVersion%
    powershell -Command "$ProgressPreference = 'SilentlyContinue'; Invoke-WebRequest %glfwUrl% -OutFile glfw.bin.WIN64.zip"

    echo Extracting GLFW to ./Wyvern/vendor/glfw/bin
    powershell -Command "$ProgressPreference = 'SilentlyContinue'; Expand-Archive glfw.bin.WIN64.zip -DestinationPath ./glfw"

    REM GLFW cleanup. 
    REM The version number has to be used here because the zip file includes another folder with the version number in the name.
    ren "./glfw/glfw-%glfwVersion%.bin.WIN64" "bin"
    del /f /q "glfw.bin.WIN64.zip"
)

REM install latest Vulkan SDK using the silent windows installer
mkdir vulkanSDK
echo Downloading Vulkan SDK %vulkanVersion%... (This may take a while)
powershell -Command "$ProgressPreference = 'SilentlyContinue'; Invoke-WebRequest %vulkanUrl% -OutFile vulkanSDK-Installer.exe"
echo Running vulkanSDK installer with root directory ./Wyvern/vendor/vulkanSDK
vulkanSDK-Installer.exe --root %cd%/vulkanSDK --accept-licenses --default-answer --confirm-command install com.lunarg.vulkan.vma com.lunarg.vulkan.debug

REM VULKAN cleanup
del /f /q "vulkanSDK-Installer.exe"

REM run premake
echo Running premake for %1
cd .. && cd ..
echo Building project files

REM program exit
exit /b

:help
echo - To build and run premake with your target, run build.bat [target]
echo - To clean your visual studio project, go to the solution root directory and run .\premake\premake5.exe clean
exit /b

:checkUninitialized
set "line=%~1"
if "%line:~0,1%"=="-" (
    set submoduleUpdateFlag=1
)
exit /b

