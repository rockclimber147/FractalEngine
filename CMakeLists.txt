cmake_minimum_required(VERSION 3.10)
set(OpenGL_GL_PREFERENCE GLVND)
project(FractalRenderer)

# 1. Use C++17 or 20
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2. Find Dependencies (WSL-specific)
# This looks for the GLFW and OpenGL headers/libraries you installed via apt
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

# 3. Define where the ImGui files live
# Adjust these paths if you named your folders differently!
set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/imgui")
set(IMGUI_BACKEND_DIR "${IMGUI_DIR}/backends")

# 4. Collect ALL Source Files
# We include your logic AND the ImGui implementation files
set(SOURCES
    src/main.cpp
    src/App.cpp
    src/fractals/FractalComponent.cpp
    src/fractals/GPUFractalComponent.cpp
    src/fractals/CPUFractalComponent.cpp
    src/fractals/Checkerboard/Checkerboard.cpp
    src/fractals/Mandelbrot/Mandelbrot.cpp
    src/fractals/Mandelbrot/GPUMandelbrot.cpp
    
    # Dear ImGui Core
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    
    # Dear ImGui Backends (GLFW + OpenGL3)
    ${IMGUI_BACKEND_DIR}/imgui_impl_glfw.cpp
    ${IMGUI_BACKEND_DIR}/imgui_impl_opengl3.cpp
)

# 5. Create the Executable
add_executable(${PROJECT_NAME} ${SOURCES})

# 6. Tell the compiler where the Header (.h) files are
target_include_directories(${PROJECT_NAME} PRIVATE 
    include
    ${IMGUI_DIR}
    ${IMGUI_BACKEND_DIR}
    ${GLFW_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
)

# 7. Link the actual library binaries
target_link_libraries(${PROJECT_NAME} 
    ${GLFW_LIBRARIES}
    ${GLEW_LIBRARIES} 
    OpenGL::GL 
    dl # Required for Linux/WSL graphics
)