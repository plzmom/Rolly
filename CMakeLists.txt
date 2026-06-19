cmake_minimum_required(VERSION 3.10)
project(lib_app LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_APP "Build example app executable" ON)
option(GLFW_FROM_SOURCE "Download and build GLFW from source instead of using a system package" OFF)
set(APP_ENTRY_FILE "src/main.c" CACHE FILEPATH "Primary C source file to compile for app executable")

if(NOT IS_ABSOLUTE "${APP_ENTRY_FILE}")
    set(APP_ENTRY_FILE "${CMAKE_SOURCE_DIR}/${APP_ENTRY_FILE}")
endif()

if(BUILD_APP)
    if(NOT EXISTS "${APP_ENTRY_FILE}")
        message(FATAL_ERROR "APP_ENTRY_FILE does not exist: ${APP_ENTRY_FILE}")
    endif()
endif()

file(GLOB_RECURSE LIB_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.c"
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

if(BUILD_APP)
    list(REMOVE_ITEM LIB_SOURCES "${APP_ENTRY_FILE}")
endif()

add_library(libapp STATIC ${LIB_SOURCES})
add_library(game::libapp ALIAS libapp)

target_include_directories(libapp PUBLIC
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/glad/include
    ${CMAKE_SOURCE_DIR}/src/lib/UI/imgui
)

target_compile_features(libapp PUBLIC c_std_11 cxx_std_17)

find_package(OpenGL REQUIRED)
if(NOT TARGET OpenGL::GL)
    message(FATAL_ERROR "OpenGL target not found")
endif()

target_link_libraries(libapp PUBLIC OpenGL::GL)

if(GLFW_FROM_SOURCE)
    include(FetchContent)
    FetchContent_Declare(
        glfw
        URL https://github.com/glfw/glfw/archive/refs/tags/3.3.8.zip
    )
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(glfw)
    target_link_libraries(libapp PUBLIC glfw)
else()
    find_package(glfw3 QUIET)
    if(glfw3_FOUND)
        target_link_libraries(libapp PUBLIC glfw)
    else()
        include(FetchContent)
        FetchContent_Declare(
            glfw
            URL https://github.com/glfw/glfw/archive/refs/tags/3.3.8.zip
        )
        set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
        set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
        set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(glfw)
        target_link_libraries(libapp PUBLIC glfw)
    endif()
endif()

if(UNIX)
    target_link_libraries(libapp PUBLIC m)
    find_library(DL_LIB dl)
    if(DL_LIB)
        target_link_libraries(libapp PUBLIC ${DL_LIB})
    endif()
endif()

if(BUILD_APP)
    add_executable(app "${APP_ENTRY_FILE}")
    target_link_libraries(app PRIVATE libapp)
endif()

message(STATUS "Configured for ${CMAKE_SYSTEM_NAME}")
message(STATUS "Build app executable: ${BUILD_APP}")
message(STATUS "Entry source: ${APP_ENTRY_FILE}")
message(STATUS "GLFW from source: ${GLFW_FROM_SOURCE}")
