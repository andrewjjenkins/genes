cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

project(GenesCmds
  LANGUAGES CXX
)

FILE(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_executable(GenesCmds ${sources})
set_target_properties(GenesCmds PROPERTIES
  CXX_STANDARD 17
  OUTPUT_NAME "Genes"
)
target_link_libraries(GenesCmds Genes)
