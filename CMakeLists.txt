cmake_minimum_required(VERSION 3.16)

# Sets project name
project(csci-1230-2022-FP LANGUAGES CXX C)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Sets C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specifies required Qt components
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Gui)
find_package(Qt6 REQUIRED COMPONENTS OpenGL)
find_package(Qt6 REQUIRED COMPONENTS OpenGLWidgets)
find_package(Qt6 REQUIRED COMPONENTS Xml)

# Allows you to include files from within those directories, without prefixing their filepaths
include_directories(src)

# Specifies .cpp and .h files to be passed to the compiler
add_executable(${PROJECT_NAME}
    src/main.cpp

    src/realtime.cpp
    src/mainwindow.cpp
    src/settings.cpp
    src/utils/scenefilereader.cpp
    src/utils/sceneparser.cpp
    src/shapes/Cube.cpp
    src/shapes/Sphere.cpp
    src/shapes/Cylinder.cpp
    src/shapes/Cone.cpp
    src/shapes/Building.cpp
    src/camera/camera.cpp
    src/shapes/Ground.cpp

    src/mainwindow.h
    src/realtime.h
    src/settings.h
    src/utils/scenedata.h
    src/utils/scenefilereader.h
    src/utils/sceneparser.h
    src/utils/shaderloader.h
    src/utils/debug.h
    src/shapes/Cube.h
    src/shapes/Sphere.h
    src/shapes/Cylinder.h
    src/shapes/Cone.h
    src/shapes/Building.h
    src/camera/camera.h
    src/floorplan.h
    src/shapes/Ground.h
)

# GLM: this creates its library and allows you to `#include "glm/..."`
add_subdirectory(glm)

# GLEW: this creates its library and allows you to `#include "GL/glew.h"`
add_library(StaticGLEW STATIC glew/src/glew.c)
include_directories(${PROJECT_NAME} PRIVATE glew/include)

# Specifies libraries to be linked (Qt components, glew, etc)
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt::Core
    Qt::Gui
    Qt::OpenGL
    Qt::OpenGLWidgets
    Qt::Xml
    StaticGLEW
)

# Specifies other files
qt6_add_resources(${PROJECT_NAME} "Resources"
    PREFIX
        "/"
    FILES
        resources/shaders/phong.frag
        resources/shaders/phong.vert
        resources/shaders/texture.frag
        resources/shaders/texture.vert
        resources/shaders/skybox.frag
        resources/shaders/skybox.vert
        resources/images/kitten.png
        resources/images/front.png
        resources/images/back.png
        resources/images/left.png
        resources/images/right.png
        resources/images/bottom.png
        resources/images/top.png
        resources/images/front-s.png
        resources/images/back-s.png
        resources/images/left-s.png
        resources/images/right-s.png
        resources/images/bottom-s.png
        resources/images/top-s.png
        resources/images/brick.png
        resources/images/building1.png
        resources/images/building2.png
        resources/images/building3.png
        resources/images/building4.png
        resources/images/building5.png
        resources/images/building6.png
        resources/images/building7.png
        resources/images/building8.png
        resources/images/ground.jpg
)

# GLEW: this provides support for Windows (including 64-bit)
if (WIN32)
  add_compile_definitions(GLEW_STATIC)
  target_link_libraries(${PROJECT_NAME} PRIVATE
    opengl32
    glu32
  )
endif()

# Set this flag to silence warnings on Windows
if (MSVC OR MSYS OR MINGW)
  set(CMAKE_CXX_FLAGS "-Wno-volatile")
endif()
# Set this flag to silence warnings on MacOS
if (APPLE)
  set(CMAKE_CXX_FLAGS "-Wno-deprecated-volatile")
endif()
