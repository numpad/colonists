cmake_minimum_required (VERSION 3.1)

project (colony LANGUAGES CXX)

set (CMAKE_BUILD_TYPE Debug)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set (PROJECT_BASE_SRC "src")
set (PROJECT_SOURCE_DIR "${PROJECT_BASE_SRC}/source")
set (PROJECT_BINARY_DIR "bin")
set (PROJECT_INCLUDE_DIR "${PROJECT_BASE_SRC}/include")
set (PROJECT_LIBRARY_DIR "lib")

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_LIBRARY_DIR})
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_LIBRARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${PROJECT_BINARY_DIR}")

set (CFG_VERSION_MAJOR 0)
set (CFG_VERSION_MINOR 1)
set (CFG_DEBUG true)
configure_file (
	"${PROJECT_BASE_SRC}/config.hpp.in"
	"${PROJECT_BINARY_DIR}/config.hpp"
)
include_directories ("${PROJECT_BINARY_DIR}")
include_directories ("${PROJECT_INCLUDE_DIR}")
include_directories ("${PROJECT_LIBRARY_DIR}")

file (GLOB source_files
	"${PROJECT_SOURCE_DIR}/*.cpp"
	"${PROJECT_SOURCE_DIR}/*/*.cpp"
	"${PROJECT_LIBRARY_DIR}/*/*.cpp"
)

set (OpenGL_GL_PREFERENCE GLVND)
find_package (OpenGL REQUIRED)
find_package (glfw3 REQUIRED)
find_package (glm REQUIRED)

add_executable (colony "${PROJECT_BASE_SRC}/main.cpp" ${source_files})
target_link_libraries (colony
                       GL
                       glfw
                       dl
)

if (MSVC)
	target_compile_options (${TARGET_NAME} PRIVATE /W4 /WX)
else (MSVC)
	target_compile_options (colony PRIVATE -Wall -Wextra -pedantic)
endif (MSVC)

install (TARGETS colony DESTINATION bin)
install (FILES "${PROJECT_BINARY_DIR}/config.hpp"
         DESTINATION include)