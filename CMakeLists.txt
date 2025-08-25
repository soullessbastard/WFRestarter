cmake_minimum_required(VERSION 3.15.0)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

project(WFRestarter VERSION 1.0.0 LANGUAGES C CXX)

add_executable(WFRestarter 
    src/main.cpp
    src/utils/system_hook/system_hook.cpp
    src/utils/process_kill/process_kill.cpp
    src/utils/config_gen/config_gen.cpp
    src/resources/appicon.rc
)

set_target_properties(WFRestarter PROPERTIES
    LINK_FLAGS "/SUBSYSTEM:WINDOWS"
)