workspace "VampTest"
    platforms "x64"
    configurations {"debug", "dist", "release"}
    startproject "Sandbox"

    include "projects/VampTest"
    include "projects/Sandbox"