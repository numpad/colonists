cmake_minimum_required (VERSION 3.1)

project (colony LANGUAGES CXX)

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
	"${PROJECT_LIBRARY_DIR}/FastNoise/*.cpp"
)

find_package (GLEW)
find_package (glm)
find_package (SFML 2.5 COMPONENTS graphics audio REQUIRED)
add_executable (colony "${PROJECT_BASE_SRC}/main.cpp" ${source_files})
target_link_libraries (colony sfml-graphics sfml-audio)

install (TARGETS colony DESTINATION bin)
install (FILES "${PROJECT_BINARY_DIR}/config.hpp"
         DESTINATION include)