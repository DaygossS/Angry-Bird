-- Export Compile Commands module for clangd support
require "ecc/ecc"

newoption
{
    trigger = "graphics",
    value = "OPENGL_VERSION",
    description = "version of OpenGL to build raylib against",
    allowed = {
        { "opengl11", "OpenGL 1.1"},
        { "opengl21", "OpenGL 2.1"},
        { "opengl33", "OpenGL 3.3"},
        { "opengl43", "OpenGL 4.3"},
        { "openges2", "OpenGL ES2"},
        { "openges3", "OpenGL ES3"},
        { "software", "OpenGL 1.1 Software Render"}
    },
    default = "opengl33"
}

newoption
{
    trigger = "backend",
    value = "BACKEND_TYPE",
    description = "Backend Platform to use",
    allowed = {
        { "glfw", "GLFW"},
        { "rgfw", "RGFW"},
        { "win32", "WIN32"},
    },
    default = "glfw"
}

newoption
{
    trigger = "wayland",
    value = "WAYLAND",
    description = "build for wayland",
    allowed = {
        { "off", "Off"},
        { "on", "On"}
    },
    default = "off"
}

function download_progress(total, current)
    local ratio = current / total;
    ratio = math.min(math.max(ratio, 0), 1);
    local percent = math.floor(ratio * 100);
    print("Download progress (" .. percent .. "%/100%)")
end

-- Pinned dependency versions: everyone in the team builds exactly the same code.
raylib_version = "6.2"
googletest_version = "1.17.0"

raylib_dir = "external/raylib-" .. raylib_version
googletest_dir = "external/googletest-" .. googletest_version

function fetch_archive(url, folder)
    if (os.isdir(folder) == false) then
        local zipName = folder .. ".zip"
        if (not os.isfile(zipName)) then
            print(folder .. " not found, downloading " .. url)
            local result_str, response_code = http.download(url, zipName, {
                progress = download_progress,
                headers = { "From: Premake", "Referer: Premake" }
            })
            if (response_code ~= 200) then
                error("Download failed (" .. tostring(response_code) .. "): " .. url)
            end
        end
        print("Unzipping " .. zipName .. " to " .. os.getcwd())
        zip.extract(zipName, os.getcwd())
        os.remove(zipName)
    end
end

function build_externals()
    print("calling externals")
    os.chdir("external")
    fetch_archive("https://github.com/raysan5/raylib/archive/refs/tags/" .. raylib_version .. ".zip", "raylib-" .. raylib_version)
    fetch_archive("https://github.com/google/googletest/archive/refs/tags/v" .. googletest_version .. ".zip", "googletest-" .. googletest_version)
    os.chdir("../")
end

function link_raylib()
    links {"raylib"}
    dependson {"raylib"}

    -- Explicit raylib.lib before the default Windows libs: avoids the CloseWindow clash with user32.lib.
    filter "action:vs*"
        links {"raylib.lib"}
        libdirs {"../bin/%{cfg.buildcfg}"}

    filter "system:windows"
        links {"winmm", "gdi32", "opengl32"}

    filter "system:linux"
        links {"pthread", "m", "dl", "rt"}

    filter {"system:linux", "options:wayland=off"}
        links {"X11"}

    filter {"system:linux", "options:wayland=on"}
        links {"wayland-client", "wayland-cursor", "wayland-egl", "xkbcommon"}

    filter "system:macosx"
        links {"OpenGL.framework", "Cocoa.framework", "IOKit.framework", "CoreFoundation.framework", "CoreAudio.framework", "CoreVideo.framework", "AudioToolbox.framework", "QuartzCore.framework"}

    filter {}
end

function platform_defines()
     filter {"options:backend=glfw"}
        defines{"PLATFORM_DESKTOP"}

    filter {"options:backend=rgfw"}
        defines{"PLATFORM_DESKTOP_RGFW"}

    filter {"options:backend=win32"}
        defines{"PLATFORM_DESKTOP_WIN32"}

    filter {"options:graphics=opengl43"}
        defines{"GRAPHICS_API_OPENGL_43"}

    filter {"options:graphics=opengl33"}
        defines{"GRAPHICS_API_OPENGL_33"}

    filter {"options:graphics=opengl21"}
        defines{"GRAPHICS_API_OPENGL_21"}

    filter {"options:graphics=opengl11"}
        defines{"GRAPHICS_API_OPENGL_11"}

    filter {"options:graphics=openges3"}
        defines{"GRAPHICS_API_OPENGL_ES3"}

    filter {"options:graphics=openges2"}
        defines{"GRAPHICS_API_OPENGL_ES2"}

    filter {"options:graphics=software"}
        defines{"GRAPHICS_API_OPENGL_SOFTWARE"}

    filter {"system:macosx"}
        disablewarnings {"deprecated-declarations"}

    filter {"system:linux", "options:wayland=off"}
        defines {"_GLFW_X11"}

    filter {"system:linux", "options:wayland=on"}
        defines {"_GLFW_WAYLAND"}

    filter {}
end

if (os.isdir('build_files') == false) then
    os.mkdir('build_files')
end

if (os.isdir('external') == false) then
    os.mkdir('external')
end

build_externals()

cyclone_include = "../cyclone/include"
core_include = "../src/Core"
library_include = "../SiegeLab_Library"
googletest_include = googletest_dir .. "/googletest/include"

workspace "Angry-Bird"
    location "../"
    configurations { "Debug", "Release"}
    platforms { "x64" }
    defaultplatform ("x64")
    startproject "SiegeLabGame"

    language "C++"
    cppdialect "C++20"
    cdialect "C17"
    architecture "x86_64"
    targetdir "../bin/%{cfg.buildcfg}"

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

    filter {"configurations:Release", "action:vs*"}
        linktimeoptimization "On"

    filter "action:vs*"
        defines {"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS"}
        characterset ("Unicode")
        buildoptions { "/Zc:__cplusplus" }

    filter "system:windows"
        defines {"_WIN32"}

    filter {}

group "Dependencies"

    -- Supplied teacher library (Project/Cyclone_Library.zip). Do not edit, except where milestones say so.
    project "Cyclone"
        kind "StaticLib"
        location "../"
        files {"../cyclone/include/cyclone/*.h", "../cyclone/src/*.cpp"}
        includedirs {"../cyclone/include/cyclone", cyclone_include}
        warnings "Off"

    project "GoogleTest"
        kind "StaticLib"
        location "../"
        files {googletest_dir .. "/googletest/src/gtest-all.cc", googletest_dir .. "/googletest/src/gtest_main.cc"}
        includedirs {googletest_include, googletest_dir .. "/googletest"}
        warnings "Off"

group ""

    -- Game rules, physics world and entities: everything testable without a window.
    project "SiegeLabCore"
        kind "StaticLib"
        location "../"
        files {"../src/Core/**.h", "../src/Core/**.hpp", "../src/Core/**.cpp"}
        includedirs {core_include, cyclone_include, raylib_dir .. "/src"}
        links {"Cyclone"}
        platform_defines()

    -- Game mechanics (GameController, cameras, ...). Every file in SiegeLab_Library/ is picked up automatically.
    project "SiegeLab_Library"
        kind "StaticLib"
        location "../"
        files {"../SiegeLab_Library/**.h", "../SiegeLab_Library/**.hpp", "../SiegeLab_Library/**.cpp"}
        vpaths
        {
            ["Header Files/*"] = {"../SiegeLab_Library/**.h", "../SiegeLab_Library/**.hpp"},
            ["Source Files/*"] = {"../SiegeLab_Library/**.cpp"},
        }
        pchheader "pch.h"
        pchsource "../SiegeLab_Library/pch.cpp"
        includedirs {library_include, core_include, cyclone_include, raylib_dir .. "/src"}
        links {"SiegeLabCore", "Cyclone"}
        platform_defines()

    -- Window, input and rendering entry point.
    project "SiegeLabGame"
        kind "ConsoleApp"
        location "../"

        filter {"system:windows", "configurations:Release", "action:gmake*"}
            kind "WindowedApp"
            buildoptions { "-Wl,--subsystem,windows" }

        filter {"system:windows", "configurations:Release", "action:vs*"}
            kind "WindowedApp"
            entrypoint "mainCRTStartup"

        filter "action:vs*"
            debugdir "$(SolutionDir)"

        filter {}

        vpaths
        {
            ["Header Files/*"] = {"../src/Game/**.h", "../src/Game/**.hpp"},
            ["Source Files/*"] = {"../src/Game/**.cpp"},
            ["Windows Resource Files/*"] = {"../src/Game/**.rc", "../src/Game/**.ico"},
            ["Game Resource Files/*"] = {"../resources/**"},
        }

        files {"../src/Game/**.cpp", "../src/Game/**.h", "../src/Game/**.hpp"}

        filter {"system:windows", "action:vs*"}
            files {"../src/Game/*.rc", "../src/Game/*.ico", "../resources/**"}

        filter {}

        includedirs {"../src/Game", library_include, core_include, cyclone_include, raylib_dir .. "/src"}
        links {"SiegeLab_Library", "SiegeLabCore", "Cyclone"}
        link_raylib()
        platform_defines()

    -- Headless GoogleTest executable.
    project "SiegeLabTests"
        kind "ConsoleApp"
        location "../"
        files {"../tests/**.cpp", "../tests/**.h"}
        includedirs {"../tests", library_include, core_include, cyclone_include, googletest_include, raylib_dir .. "/src"}
        links {"GoogleTest", "SiegeLab_Library", "SiegeLabCore", "Cyclone"}
        link_raylib()
        platform_defines()

        filter "action:vs*"
            debugdir "$(SolutionDir)"

        filter {}

group "Dependencies"

    project "raylib"
        kind "StaticLib"
    
        platform_defines()

        location "../"

        language "C"
        targetdir "../bin/%{cfg.buildcfg}"


        filter {"options:wayland=on"}
            defines {"GLFW_LINUX_ENABLE_WAYLAND=TRUE" }

        filter {"options:wayland=on", "system:linux"}
            prebuildcommands {
                "@echo Generating Wayland protocols...",
                -- Core Wayland & Shell
                "@wayland-scanner client-header build/" .. raylib_dir .. "/src/external/glfw/deps/wayland/wayland.xml build/" .. raylib_dir .. "/src/wayland-client-protocol.h",
                "@wayland-scanner private-code build/" .. raylib_dir .. "/src/external/glfw/deps/wayland/wayland.xml build/" .. raylib_dir .. "/src/wayland-client-protocol-code.h",

                "@wayland-scanner client-header build/" .. raylib_dir .. "/src/external/glfw/deps/wayland/xdg-shell.xml build/" .. raylib_dir .. "/src/xdg-shell-client-protocol.h",
                "@wayland-scanner private-code build/" .. raylib_dir .. "/src/external/glfw/deps/wayland/xdg-shell.xml build/" .. raylib_dir .. "/src/xdg-shell-client-protocol-code.h",

                "@wayland-scanner client-header build/" .. raylib_dir .. "/src/external/glfw/deps/wayland/xdg-decoration-unstable-v1.xml build/" .. raylib_dir .. "/src/xdg-decoration-unstable-v1-client-protocol.h",
                "@wayland-scanner private-code build/" .. raylib_dir .. "/src/external/glfw/deps/wayland/xdg-decoration-unstable-v1.xml build/" .. raylib_dir .. "/src/xdg-decoration-unstable-v1-client-protocol-code.h",

                -- Viewporter
                "@wayland-scanner client-header build/" .. raylib_dir .. "/src/external/glfw/deps/wayland/viewporter.xml build/" .. raylib_dir .. "/src/viewporter-client-protocol.h",
                "@wayland-scanner private-code build/" .. raylib_dir .. "/src/external/glfw/deps/wayland/viewporter.xml build/" .. raylib_dir .. "/src/viewporter-client-protocol-code.h",

                -- Relative Pointer
                "@wayland-scanner client-header build/" .. raylib_dir .. "/src/external/glfw/deps/wayland/relative-pointer-unstable-v1.xml build/" .. raylib_dir .. "/src/relative-pointer-unstable-v1-client-protocol.h",
                "@wayland-scanner private-code build/" .. raylib_dir .. "/src/external/glfw/deps/wayland/relative-pointer-unstable-v1.xml build/" .. raylib_dir .. "/src/relative-pointer-unstable-v1-client-protocol-code.h",

                -- Pointer Constraints
                "@wayland-scanner client-header build/" .. raylib_dir .. "/src/external/glfw/deps/wayland/pointer-constraints-unstable-v1.xml build/" .. raylib_dir .. "/src/pointer-constraints-unstable-v1-client-protocol.h",
                "@wayland-scanner private-code build/" .. raylib_dir .. "/src/external/glfw/deps/wayland/pointer-constraints-unstable-v1.xml build/" .. raylib_dir .. "/src/pointer-constraints-unstable-v1-client-protocol-code.h",

                -- Fractional Scale
                "@wayland-scanner client-header build/" .. raylib_dir .. "/src/external/glfw/deps/wayland/fractional-scale-v1.xml build/" .. raylib_dir .. "/src/fractional-scale-v1-client-protocol.h",
                "@wayland-scanner private-code build/" .. raylib_dir .. "/src/external/glfw/deps/wayland/fractional-scale-v1.xml build/" .. raylib_dir .. "/src/fractional-scale-v1-client-protocol-code.h",

                -- XDG Activation
                "@wayland-scanner client-header build/" .. raylib_dir .. "/src/external/glfw/deps/wayland/xdg-activation-v1.xml build/" .. raylib_dir .. "/src/xdg-activation-v1-client-protocol.h",
                "@wayland-scanner private-code build/" .. raylib_dir .. "/src/external/glfw/deps/wayland/xdg-activation-v1.xml build/" .. raylib_dir .. "/src/xdg-activation-v1-client-protocol-code.h",

                -- Idle Inhibit
                "@wayland-scanner client-header build/" .. raylib_dir .. "/src/external/glfw/deps/wayland/idle-inhibit-unstable-v1.xml build/" .. raylib_dir .. "/src/idle-inhibit-unstable-v1-client-protocol.h",
                "@wayland-scanner private-code build/" .. raylib_dir .. "/src/external/glfw/deps/wayland/idle-inhibit-unstable-v1.xml build/" .. raylib_dir .. "/src/idle-inhibit-unstable-v1-client-protocol-code.h",
            }
        filter {}

        filter "action:vs*"
            defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS"}
            characterset ("Unicode")
            buildoptions { "/Zc:__cplusplus" }
        filter{}

        includedirs {raylib_dir .. "/src", raylib_dir .. "/src/external/glfw/include" }
        vpaths
        {
            ["Header Files"] = { raylib_dir .. "/src/**.h"},
            ["Source Files/*"] = { raylib_dir .. "/src/**.c"},
        }
        files {raylib_dir .. "/src/*.h", raylib_dir .. "/src/*.c"}

        removefiles {raylib_dir .. "/src/rcore_*.c"}

        filter { "system:macosx", "files:" .. raylib_dir .. "/src/rglfw.c" }
            compileas "Objective-C"

        filter{}
