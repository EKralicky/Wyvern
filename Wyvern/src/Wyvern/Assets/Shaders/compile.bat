@echo off
setlocal
for /r %%i in (*.frag, *.vert) do (
    echo Processing %%i
    "%VULKAN_SDK%/Bin/glslc.exe" "%%i" -o "%%~dpni.spv"
)
endlocal