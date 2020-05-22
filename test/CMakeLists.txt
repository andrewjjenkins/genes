cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

# Benchmark tests
file(GLOB benchmark_sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/benchmark/source/*.cpp)
add_executable(GenesBenchmark ${benchmark_sources})
set_target_properties(GenesBenchmark PROPERTIES CXX_STANDARD 17)
target_link_libraries(GenesBenchmark Genes benchmark)
target_include_directories(GenesBenchmark
  PUBLIC
    $<BUILD_INTERFACE:${benchmark_SOURCE_DIR}/include>
)
set_target_properties(GenesBenchmark PROPERTIES CXX_STANDARD 17)

# Unit tests
file(GLOB unittest_sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/unit/common/*.cpp)
add_executable(GenesUnitTest ${unittest_sources})
set_target_properties(GenesUnitTest PROPERTIES CXX_STANDARD 17)
target_link_libraries(GenesUnitTest Genes gtest gtest_main)
target_include_directories(GenesUnitTest
  PUBLIC
  ${GTEST_INCLUDE_DIR}
)
