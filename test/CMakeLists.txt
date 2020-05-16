cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

# Benchmark tests
file(GLOB benchmark_sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/benchmark/source/*.cpp)
add_executable(GenesBenchmark ${benchmark_sources})
target_link_libraries(GenesBenchmark Genes benchmark)
target_include_directories(GenesBenchmark
  PUBLIC
    $<BUILD_INTERFACE:${benchmark_SOURCE_DIR}/include>
)
set_target_properties(GenesBenchmark PROPERTIES CXX_STANDARD 17)
