
DependencyType = "StaticLib"
OutDir = "../Build/"

workspace "mp5_format"
    location("../Build")
    architecture("x86_64")
    configurations { "Debug", "Release" }

    flags {"MultiProcessorCompile"}

    BuildLocation = "%{wks.location}/%{cfg.buildcfg}/"
    
    filter "configurations:Release"
        defines {"NDEBUG"}
    filter {}

    project("mp5")

    kind "StaticLib"
    language "C++"
    staticruntime("on")

    location(OutDir)

    targetdir("%{BuildLocation}%{prj.name}")
    objdir("%{BuildLocation}Imde/%{prj.name}")

    if workingDir == nil then
        workingDir = "%{BuildLocation}%{prj.name}"
    end

    print("Working Dir: " .. workingDir)
    debugdir(workingDir)

    files
    {
        "./../src/**.cpp",
        "./../include/**.h",
        "./../include/**.hpp"
    }
    
    includedirs {
        "%{wks.location}",
        "./include"
    }
    
    filter "configurations:Debug"
       defines { "DEBUG" }
       symbols "On"

    filter "configurations:Release"
       defines { "NDEBUG" }
       optimize "On"
    filter ""