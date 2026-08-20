cmake_minimum_required(VERSION 3.14)
project(Fractales LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# ---------------------------------------------------------------------------
# GLFW (fenêtrage + contexte OpenGL)
# ---------------------------------------------------------------------------
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(glfw)

# ---------------------------------------------------------------------------
# glad2 (loader OpenGL)
# NOTE: glad_add_library lance un script Python au moment du build pour
# générer le loader. Il faut avoir Python3 + le module "jinja2" installés
# (pip install jinja2) sur la machine qui build.
# ---------------------------------------------------------------------------
FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG        glad2
    GIT_SHALLOW    TRUE
    SOURCE_SUBDIR  cmake
)
FetchContent_MakeAvailable(glad)

glad_add_library(glad_gl_core_33 STATIC API gl:core=3.3)

# ---------------------------------------------------------------------------
# Dear ImGui (n'a pas de CMakeLists officiel -> on construit la lib nous-même)
# ---------------------------------------------------------------------------
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.91.0
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)
target_link_libraries(imgui PUBLIC glfw glad_gl_core_33)

# ---------------------------------------------------------------------------
# Exécutable principal
# ---------------------------------------------------------------------------
add_executable(fractales
    src/main.cpp
    src/core/Shader.cpp
    src/core/Fractale.cpp
    src/core/Palette.cpp
    src/core/Application.cpp
    src/fractals/Mandelbrot.cpp
    src/fractals/Julia.cpp
    src/fractals/BurningShip.cpp
    src/fractals/Tricorn.cpp
    src/fractals/Multibrot.cpp
)

target_include_directories(fractales PRIVATE src)

target_link_libraries(fractales PRIVATE
    glfw
    glad_gl_core_33
    imgui
)

# Copie le dossier shaders/ à côté de l'exécutable pour que
# les chemins relatifs (ex: "shaders/vertex.vs") fonctionnent au runtime.
add_custom_command(TARGET fractales POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        $<TARGET_FILE_DIR:fractales>/shaders
)
