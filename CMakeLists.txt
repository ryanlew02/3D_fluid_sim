
cmake_minimum_required(VERSION 4.3)
project(3D_fluid_sim LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# GLFW
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw)

# Dear ImGui
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG master
)

FetchContent_MakeAvailable(imgui)

# OpenGL
find_package(OpenGL REQUIRED)

# GLAD 2
add_library(glad STATIC
        external/glad/src/gl.c
)

target_include_directories(glad PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include
)

# ImGui Library
add_library(imgui_lib STATIC
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui_lib PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(imgui_lib PUBLIC
        glfw
        OpenGL::GL
)

# Application
add_executable(3D_fluid_sim
        src/main.cpp
        src/Window.cpp
        src/App.cpp
        include/Window.h
        include/App.h
        include/Vec3.h
)

target_include_directories(3D_fluid_sim PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(3D_fluid_sim PRIVATE
        imgui_lib
        glad
)
