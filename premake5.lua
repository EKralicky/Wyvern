workspace "Workspace"

    architecture "x64"

    configurations {
        "Debug",
        "Release"
    }

    startproject "Wyvern"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
 
include "Wyvern"

if _ACTION == 'clean' then
    print("Cleaning solution")
    os.rmdir('bin')
    os.rmdir('bin-int')
    os.rmdir('obj')
    print("Removed binaries and obj files")
    os.remove('*.sln')
    print("Removed solution files")
    os.remove('Wyvern/*.vcxproj')
    os.remove('Wyvern/*.vcxproj.filters')
    print("Removed project files (does not remove vcxproj.user)")
 end