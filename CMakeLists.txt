cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

project(Genes
  VERSION 0.1
  LANGUAGES CXX
)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif()

# Dependencies

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
#find_package(Boost 1.65 REQUIRED COMPONENTS format)
find_package(Boost 1.65)
message(STATUS "Boost version: ${Boost_VERSION}")

add_subdirectory(extern)

# Main Genes library

FILE(GLOB_RECURSE headers CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
FILE(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp")
add_library(Genes ${headers} ${sources})
set_target_properties(Genes PROPERTIES CXX_STANDARD 17)
target_include_directories(Genes
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>
)

# enable compiler warnings
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  target_compile_options(Genes PUBLIC -Wall -pedantic -Wextra -Werror)
elseif(MSVC)
  target_compile_options(Genes PUBLIC /W4 /WX)
endif()

# Executable
add_subdirectory(cmd)

# Tests
add_subdirectory(test)

# Tools
add_custom_target(clang-format
  COMMAND clang-format
  -i
  ${headers} ${sources}
)
