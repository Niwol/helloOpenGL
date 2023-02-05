project(helloopenGL)

############################################
# Configure CMake and GCC flags
cmake_minimum_required(VERSION 3.22.1) # Minimal version compatible QT5
CMAKE_POLICY(SET CMP0072 NEW) # This will silence the Cmake Warning "Policy CMP0043 is not set"

# Compiler stuff
set(CMAKE_CXX_STANDARD 17 CACHE STRING "C++ standard" FORCE)
#set(CMAKE_CXX_STANDARD_REQUIRED OFF)
#set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_CXX_COMPILER "clang++" CACHE STRING "C++ compiler" FORCE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra"
    CACHE STRING "Compiler flags" FORCE)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE) 
endif()


# External libs
find_package(glfw3 REQUIRED)

find_package(OpenGL REQUIRED) # define OPENGL_LIBRARIES

set(Eigen3_DIR ${CMAKE_SOURCE_DIR}/External/Eigen/share/eigen3/cmake)
find_package(Eigen3 REQUIRED)

set(OpenMesh_DIR ${CMAKE_SOURCE_DIR}/External/OpenMesh/share/OpenMesh/cmake)
find_package(OpenMesh REQUIRED)

#OpenGL deprecated on MacosX : remove compilation warning
add_definitions(-DGL_SILENCE_DEPRECATION -DGLM_FORCE_SILENT_WARNINGS)

################################################################################
# Define project private sources and headers
#
# the variable "folder_source" contains all .cpp files of this project
FILE(GLOB_RECURSE
    folder_source
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/glad/src/glad.c
)

FILE(GLOB_RECURSE
    folder_header
    ${CMAKE_SOURCE_DIR}/src/*.h
)

include_directories(
   ${CMAKE_SOURCE_DIR}/src
   ${CMAKE_SOURCE_DIR}/glm
   ${CMAKE_SOURCE_DIR}/glad/include
   ${CMAKE_SOURCE_DIR}/External/Eigne/include
   ${CMAKE_SOURCE_DIR}/External/OpenMesh/include
)

SOURCE_GROUP("Source Files" FILES ${folder_source})
SOURCE_GROUP("Header Files" FILES ${folder_header})
SOURCE_GROUP("Shader Files" FILES ${folder_shader})

################################################################################
# Configure QT

set( CMAKE_INCLUDE_CURRENT_DIR ON )


################################################################################
# Build target application

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build)
add_executable(helloopenGL
               ${folder_source}
               ${folder_header}
               )
          
set(EXT_LIBS Eigen3::Eigen OpenMeshCore glfw ${OPENGL_LIBRARIES})

target_link_libraries(helloopenGL ${EXT_LIBS} )
