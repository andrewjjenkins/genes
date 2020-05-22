include(FetchContent)

# Googletest and Googlemock
FetchContent_Declare(googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.8.0
) 
FetchContent_GetProperties(googletest)
FetchContent_MakeAvailable(googletest)
set(GTEST_INCLUDE_DIR "${googletest_SOURCE_DIR}/googletest/include")
set(GTEST_INCLUDE_DIR "${googletest_SOURCE_DIR}/googletest/include" PARENT_SCOPE)
set(GTEST_LIBRARY "${googletest_BINARY_DIR}/googlemock/gtest/libgtest.a")
set(GTEST_MAIN_LIBRARY "${googletest_BINARY_DIR}/googlemock/gtest/libgtest_main.a")
set(GMOCK_INCLUDE_DIRS "${googletest_SOURCE_DIR}/googlemock/include")

# Google benchmark
FetchContent_Declare(benchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG v1.4.1
) 
FetchContent_GetProperties(benchmark)
FetchContent_MakeAvailable(benchmark)
add_dependencies(benchmark gtest)
