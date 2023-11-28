project "Sandbox"
    targetname "sandbox"
    kind "ConsoleApp"
    language "C"
    targetdir "%{_WORKING_DIR}/builds/%{cfg.shortname}"
    objdir "%{_WORKING_DIR}/obj/%{prj.name}-%{cfg.shortname}"

    files {
        "src/**.h",
        "src/**.c"
    }

    includedirs {
        "src",
        "../VampTest/src/include"
    }

    links "VampTest"

    filter "configurations:debug"
        symbols "on"
        optimize "off"

    filter "configurations:dist"
        symbols "off"
        optimize "off"

    filter "configurations:release"
        symbols "off"
        optimize "on"