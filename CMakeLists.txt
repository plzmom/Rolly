cmake_minimum_required(VERSION 3.10)
project(lib_app LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Sources
set(SRCS_C
    src/main.c
    src/glad/src/glad.c
    src/lib/render/render.c
    src/lib/win/window.c
)

set(SRCS_CPP
    src/lib/UI/ui.cpp
    src/lib/UI/imgui/imgui.cpp
    src/lib/UI/imgui/imgui_draw.cpp
    src/lib/UI/imgui/imgui_tables.cpp
    src/lib/UI/imgui/imgui_widgets.cpp
    src/lib/UI/imgui/backends/imgui_impl_glfw.cpp
    src/lib/UI/imgui/backends/imgui_impl_opengl3.cpp
)

add_executable(app ${SRCS_C} ${SRCS_CPP})

target_include_directories(app PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/glad/include
    ${CMAKE_SOURCE_DIR}/src/lib/UI/imgui
)

find_package(OpenGL REQUIRED)
if(NOT TARGET OpenGL::GL)
    message(FATAL_ERROR "OpenGL target not found")
endif()

# Try to find glfw using package config or find_package
find_package(glfw3 QUIET)
if(NOT glfw3_FOUND)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLFW REQUIRED glfw3)
    if(GLFW_FOUND)
        target_include_directories(app PRIVATE ${GLFW_INCLUDE_DIRS})
        target_link_libraries(app PRIVATE ${GLFW_LIBRARIES})
    else()
        message(FATAL_ERROR "GLFW not found. Install glfw3 and try again.")
    endif()
else()
    target_link_libraries(app PRIVATE glfw)
endif()

if(WIN32)
    target_link_libraries(app PRIVATE OpenGL::GL)
    # On Windows, link with user32 and gdi if needed (usually provided by toolchain)
else()
    # Linux / Unix
    find_library(DL_LIB dl)
    find_library(M_LIB m)
    if(DL_LIB)
        target_link_libraries(app PRIVATE ${DL_LIB})
    endif()
    if(M_LIB)
        target_link_libraries(app PRIVATE ${M_LIB})
    endif()
    target_link_libraries(app PRIVATE OpenGL::GL)
endif()

message(STATUS "Configured for ${CMAKE_SYSTEM_NAME}")
