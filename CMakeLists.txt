cmake_minimum_required (VERSION 3.1)

project (colony)

set (PROJECT_SOURCE_DIR "src")
set (PROJECT_INCLUDE_DIR "src/include")
set (PROJECT_BINARY_DIR "bin")

set (CFG_VERSION_MAJOR 0)
set (CFG_VERSION_MINOR 1)
configure_file (
	"${PROJECT_SOURCE_DIR}/config.hpp.in"
	"${PROJECT_BINARY_DIR}/config.hpp"
)
include_directories ("${PROJECT_BINARY_DIR}")
include_directories ("${PROJECT_INCLUDE_DIR}")

find_package (SFML 2.5 COMPONENTS graphics audio REQUIRED)
add_executable (colony "${PROJECT_SOURCE_DIR}/main.cpp")
target_link_libraries (colony sfml-graphics sfml-audio)

install (TARGETS colony DESTINATION bin)
install (FILES "${PROJECT_BINARY_DIR}/config.hpp"
         DESTINATION include)